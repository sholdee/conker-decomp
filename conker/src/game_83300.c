#include <ultra64.h>

// matching: the shared headers declare these with parameter types that would change the code here
// (extra narrowing in callers) or the variable type; hide those declarations and declare them below.
#define func_1000F91C func_1000F91C_hdr
#include "functions.h"
#undef func_1000F91C
void func_1000F91C(u16, s32, s16, s32, s32, s32, s32, s32, s32, s32);
u16 func_10010E78(u16, s32, u16, s32, s32, s32, s32, s32, s32, s32, s32);
#define D_800D1550 D_800D1550_hdr
#define D_800CC298 D_800CC298_hdr
#include "variables.h"
#undef D_800D1550
#undef D_800CC298
extern f32 D_800D1550; /* frame time step */

/* State of the per-type texture animations in func_15061B4C. */
typedef struct Game83300Anim298 {
    f32 unk0;
    f32 unk4;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    u8 unkB[4];
    u8 unkF;
    u8 unk10;
} Game83300Anim298;
extern Game83300Anim298 D_800CC298;

void func_15058F24(Actor *arg0, f32, f32);
void func_1505D6F0(Actor *arg0, s32 arg1);
u8   func_150599C8(Actor *arg0, u8 arg1, u16 arg2);
void func_1505A250(f32 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4);
void func_150611E8(Actor *arg0, s32 arg1);
void func_1505A770(Actor *actor);
void func_15059C84(Actor *actor);
void func_1505A3A8(f32 targetSpeed, Actor *actor, f32 accel, f32 decel, u8 turnDiff);

s32 func_15080738(s32 arg0);
void func_150642AC(Actor *arg0, s32 arg1, f32 arg2, s32 arg3, s32 arg4);
s32 func_15065A5C(Actor *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

struct Entry1505C1E4;
void func_1505B9C4(Actor *attacker, Actor *victim, struct Entry1505C1E4 *base, struct Entry1505C1E4 *hit, u8 index, s32 victimSlot, u8 damageMask);
extern void (*D_80086694[])(void *, Actor *, u8 *);
s32 func_1506E46C(Actor *, s32 *, s32);
struct Entry1505C1E4;
s32 func_1505C1E4(Actor *arg0, Actor *arg1, struct Entry1505C1E4 *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern struct255 *D_800CC4A4;
void func_1505F188(Actor *arg0);
s32 func_1505C7D8(Actor *arg0, s32 arg1);

/* An actor's animation playback state (Actor.unk2D0); the previous animation is kept for blending. */
typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ u16 anim; /* bit 0x8000: set by func_1505E0C4's last argument */
    /* 0x006 */ u16 prevAnim;
    /* 0x008 */ f32 unk8;
    /* 0x00C */ f32 prevUnk8;
    /* 0x010 */ f32 speed;
    /* 0x014 */ f32 prevSpeed;
    /* 0x018 */ f32 frameCount;
    /* 0x01C */ f32 prevFrameCount;
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 prevUnk20;
    /* 0x028 */ s32 loaded;
    /* 0x02C */ s32 prevLoaded;
    /* 0x030 */ s32 unk30;
    /* 0x034 */ s32 unk34;
    /* 0x038 */ s8 unk38;
    /* 0x039 */ s8 prevUnk38;
    /* 0x03A */ s16 blendStep;
    /* 0x03C */ s16 blend;
    /* 0x03E */ u8 pad3E[2];
    /* 0x040 */ u8 header[0x1D0]; /* the animation's data header: [3] frame count */
    /* 0x210 */ u8 prevHeader[0x1D0];
} AnimPlaybackState;

typedef struct {
    s16 x;
    s16 z;
    s16 y;
    s16 h;
    u16 r;
} Seg15055E50;

typedef struct {
    Seg15055E50 *segs;
    u16 count;
    u16 id;
} Tbl15055E50;

extern Tbl15055E50 *D_800D18B0;

/* Keeps the actor inside its zone set: pushed out of obstacle circles, snapped to the nearest allowed circle. */
void func_15055E50(Actor *actor, s32 zoneSetId) {
    s32 pad[5]; // matching: stack frame size
    f32 bestX;
    f32 bestZ;
    f32 dist;
    f32 invDist;
    f32 dx;
    f32 dz;
    f32 segZ;
    f32 radius;
    f32 bestDist;
    f32 y;
    s32 i;
    s32 outsideAll;
    s32 haveZone;
    u16 radiusFlags;
    s32 count;
    Seg15055E50 *seg;
    Seg15055E50 *segs;

    outsideAll = 1;
    haveZone = 0;
    if (actor->id == 0x3A) {
        if (actor->x_position > 1200.0f) {
            actor->x_position = 1200.0f;
        }
        if (actor->x_position < -1200.0f) {
            actor->x_position = -1200.0f;
        }
        if (actor->z_position > 1200.0f) {
            actor->z_position = 1200.0f;
        }
        if (actor->z_position < -1200.0f) {
            actor->z_position = -1200.0f;
        }
    }
    if ((zoneSetId != 0) && (D_800D18B0 != NULL) && (actor->unk13D < 100)) {
        bestDist = 1000000000.0f;
        i = 0;
        while (zoneSetId != D_800D18B0[i].id) {
            if (D_800D18B0[i].segs == NULL) {
                return;
            }
            i++;
        }
        segs = D_800D18B0[i].segs;
        count = D_800D18B0[i].count;
        for (i = 0; i < count; i++) {
            seg = &segs[i];
            if (outsideAll || (seg->r & 0x8000)) {
                y = actor->y_position;
                if ((seg->y <= y) && (y <= (seg->h + seg->y))) {
                    radiusFlags = seg->r;
                    radius = radiusFlags & 0x7FFF;
                    dx = actor->x_position - seg->x;
                    segZ = seg->z;
                    dz = actor->z_position - segZ;
                    radiusFlags &= 0x8000;
                    if (radiusFlags) {
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (radius * radius)) {
                            invDist = 1.0f / sqrtf(dist);
                            actor->x_position = seg->x + (dx * invDist * radius);
                            actor->z_position = seg->z + (dz * invDist * radius);
                            actor->unkF4 |= 0x2000;
                        }
                    } else {
                        haveZone = 1;
                        dist = (dx * dx) + (dz * dz) + 0.1f;
                        if (dist < (radius * radius)) {
                            outsideAll = 0;
                        } else {
                            if ((sqrtf(dist) - radius) < bestDist) {
                                invDist = 1.0f / sqrtf(dist);
                                bestDist = sqrtf(dist) - radius;
                                bestX = seg->x + (dx * invDist * radius);
                                bestZ = segZ;
                                bestZ += dz * invDist * radius; // matching: split statement orders the z add after x
                            }
                        }
                    }
                }
            }
        }
        if (outsideAll && haveZone) {
            actor->x_position = bestX;
            actor->z_position = bestZ;
            actor->unkF4 |= 0x2000;
        }
    }
}

/* One point of an actor's path (D_800D2104[actor->unk13F]). */
typedef struct PathPoint {
    s16 x;
    s16 y;
    s16 z;
    u16 param; /* point 0: radius / 8 of the actor's area; later points: speed */
} PathPoint;

/* Keeps the actor within unk2E8 * 8 units of its path's second point. */
void func_15056150(Actor *actor) {
    f32 dist;
    f32 centerX;
    f32 centerZ;
    f32 maxDist;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 centerY;
    PathPoint *path;

    if (actor->unk2E8 != 0) {
        path = ((PathPoint **)D_800D2104)[actor->unk13F];
        centerX = path[1].x;
        centerZ = path[1].z;
        centerY = path[1].y;
        maxDist = actor->unk2E8;
        maxDist *= 8.0f; // matching: int->float convert must be its own statement
        dx = actor->x_position - centerX;
        dz = actor->z_position - centerZ;
        dy = actor->y_position - centerY;
        dist = sqrtf((dx * dx) + (dz * dz) + (dy * dy) + 0.1000000015f);
        dx *= 1.0f / dist;
        dz *= 1.0f / dist;
        dy *= 1.0f / dist;
        if (maxDist < dist) {
            actor->x_position = dx * maxDist + centerX;
            actor->z_position = dz * maxDist + centerZ;
            actor->y_position = dy * maxDist + centerY;
        }
    }
}

/* Moves the actor with the change in unk118 since last frame, scaled by its height above it. */
void func_15056258(Actor *actor) {
    f32 delta;
    f32 heightFactor;

    delta = actor->unk118 - actor->unk11C;
    actor->unk11C = actor->unk118;
    heightFactor = 2.0f * (actor->y_position - ((actor->unk118 - 60.0f) - 170.0f));
    if (!(heightFactor < 0.0f) && !(fabsf(delta) > 30.0f)) {
        if (heightFactor > 300.0f) {
            heightFactor = 300.0f;
        }
        actor->y_position += heightFactor * delta * 0.002997000003f;
    }
}
/* One step of an actor's reaction sequence. */
typedef struct {
    u16 anim;
    u8 speed; /* percent */
    u8 flags; /* bit 0: copied to unk10B; & 0xFE: stun time / 100 */
} Game83300AnimSub;

/* An actor type's reaction (0x18 bytes); ((s32 *)D_800D1588[id])[-2] points to the table. */
typedef struct {
    u8 kind; /* matched by func_1505E7CC */
    u8 pad1[3];
    s32 sound;
    u8 unk8;
    u8 pad9;
    Game83300AnimSub sub[3];
    u8 pad16[2];
} Game83300AnimEntry;
void func_150649A0(s32, s32);

/* Moves the actor toward its host D_800CC2D0[unk13D - 100] and picks the carried animation. */
void func_150562FC(Actor *actor) {
    s32 *animTable;
    Game83300AnimEntry *anims;
    s16 *hostData;
    u32 animIdx;
    f32 heightOffset;
    f32 offsetX;
    f32 offsetZ;
    f32 offsetY;
    s32 timer;
    u8 hostIdx;
    f32 lerp;
    s32 variant;
    s32 attached;
    s32 animKind;
    f32 animArg;
    f32 tx;
    f32 tz;

    hostIdx = actor->unk13D - 100;
    attached = 0;
    animArg = 5.0f;
    actor->immune = 30;
    variant = 0;
    animKind = 13;
    actor->unk180 = D_800CC2D0[hostIdx].unk180;
    actor->unk1CC = actor->y_position;
    actor->gravity = 4.5f;
    actor->y_velocity = 0.0;
    timer = actor->unk21C;
    if (timer < 30) {
        actor->unk21C = (u32)timer + D_800BE9A0; // matching: u32 cast keeps addu operand order
    }
    if (actor->id == 0x57) {
        actor->unk5C = 9;
        if (actor->unk21C < 10) {
            return;
        }
        attached = 1;
        if (D_800CC2D0[hostIdx].unk84.uh == 0x114) {
            animKind = 0x24;
        }
        if (D_800CC2D0[hostIdx].unk84.uh == 0x116) {
            variant = 1;
        }
        if (D_800CC2D0[hostIdx].unk84.uh == 0x117) {
            variant = 2;
        }
        func_150649A0(hostIdx, D_800C3E78);
    } else if (actor->id == 0x8C) {
        actor->unk5C = 4;
        if (actor->unk21C < 10) {
            return;
        }
        attached = 1;
        if (D_800CC2D0[hostIdx].unk84.uh == 0x1A6) {
            variant = 1;
        }
        if (D_800CC2D0[hostIdx].unk84.uh == 0x1A7) {
            variant = 2;
        }
        func_150649A0(hostIdx, D_800C3E78);
    } else if ((actor->id == 0xA8) || (actor->id == 0xA9)) {
        actor->unk5C = 9;
        attached = 1;
        if (D_800CC2D0[hostIdx].unk84.uh == 0x1B0) {
            variant = 1;
        }
        if (D_800CC2D0[hostIdx].unk84.uh == 0x1B1) {
            variant = 2;
        }
    } else if (actor->id == 0x5E) {
        actor->unk5C = 9;
        animArg = 16.0f;
        attached = 1;
        if (D_800CC2D0[hostIdx].unk84.uh == 0x286) {
            variant = 2;
        }
        if (D_800CC2D0[hostIdx].unk84.uh == 0x27E) {
            variant = 1;
        }
    }
    animIdx = func_1505E7CC(animKind, actor);
    animTable = (s32 *)D_800D1588[actor->id];
    anims = (Game83300AnimEntry *)animTable[-2];
    if ((D_800CC2D0[hostIdx].interaction_state == 1) && (D_800CC2D0[hostIdx].in_water != 0)) {
        actor->unk5C = 4;
        attached = 1; if (anims[animIdx].sub[1].anim != 0) { // matching: same line (as1 tie-break)
            variant = 1;
        }
    }
    if (actor->unk21C < 20) {
        timer = actor->unk21C;
        lerp = (u32)timer / 50.0f;
        if (D_800BE9B4 != 0) {
            lerp = 1.0f;
        }
    } else {
        lerp = 0.8000000119f;
    }
    offsetX = (u32)actor->unk10F;
    func_150599C8(actor, 0x10, D_800CC2D0[hostIdx].unk7A + (actor->unk13E << 8));
    if (D_800CC2D0[hostIdx].unk1D4 != NULL) {
        heightOffset = actor->unk10C / 100;
        if (D_800CC2D0[hostIdx].interaction_state != 0x15) {
            hostData = (s16 *)((u8 *)D_800CC2D0[hostIdx].unk1D4 + 0x240);
            if (actor->unk21C >= 7) {
                actor->y_position = hostData[0xD];
                if (actor->unk21C < 14) {
                    heightOffset = ((u32)(s32)actor->unk21C / 14.0f) * heightOffset;
                }
                actor->y_position += heightOffset;
            }
            if (actor->id == 0xE) {
                actor->unkC4 = 0.0;
            } else if (actor->id == 0x13) {
                offsetX = 204.0f;
                actor->unkC4 = 0.0;
            }
        }
    }
    if (D_800CC2D0[hostIdx].interaction_state == 0x15) {
        animIdx = func_1505E7CC(0x39, actor);
        actor->unk7A = D_800CC2D0[hostIdx].unk7A;
        actor->y_position = D_800CC2D0[hostIdx].y_position - 10.0f;
        if ((actor->unk28 > 100.0f) && (actor->unk21C >= 21)) {
            variant = 1;
        } else {
            animArg = 0.0;
        }
        offsetX = 0.0f;
        actor->unkC4 += (D_800CC2D0[hostIdx].unkC4 - actor->unkC4) * 0.1199999973f;
        actor->xz_velocity = 0.0f;
    }
    func_1505A184(D_800CC2D0[hostIdx].unk7A, offsetX, 0.0, &offsetX, &offsetZ, &offsetY);
    tx = D_800CC2D0[hostIdx].x_position + offsetX;
    tz = D_800CC2D0[hostIdx].z_position + offsetZ;
    actor->x_position += (tx - actor->x_position) * lerp;
    actor->z_position += (tz - actor->z_position) * lerp;
    {
        f32 rate;

        rate = (u32)anims[animIdx].sub[variant].speed;
    split: // matching: block boundary moves the u32->f32 convert's register (f16 instead of f0)
        rate = rate / 100.0f;
        func_1505E650(actor, anims[animIdx].sub[variant].anim, rate, animArg, 0.0, 0, 0);
    }
    if (attached != 0) {
        actor->unk65 = hostIdx + 1;
        actor->unk101 |= 4;
    } else {
        actor->unk65 = 0;
    }
}

/* Whether actor D_800CC2D0[targetIdx] is inside the circle (radius * 8) around the actor's first path point. */
s32 func_1505693C(Actor *actor, s32 targetIdx) {
    f32 radiusSq;
    f32 dz;
    f32 dx;
    PathPoint *path = ((PathPoint **)D_800D2104)[actor->unk13F];

    if (path[0].param == 0) {
        return TRUE;
    }

    radiusSq = path[0].param * 8;
    radiusSq = radiusSq * radiusSq;
    dx = path[0].x - D_800CC2D0[targetIdx].x_position;
    dz = path[0].z - D_800CC2D0[targetIdx].z_position;

    if ((dx * dx) + (dz * dz) < radiusSq) {
        return TRUE;
    }
    return FALSE;
}

/* D_80099A3C is really u8[][10] (variables.h declares s16[]). Neither caller reads a return value. */
void func_15056A00(Actor *actor, u8 turn, u8 row) {
    u8 col;
    u16 angle;
    u8 *entry;

    col = 0;
    if (((u8 (*)[10])D_80099A3C)[row][7] < turn) {
        col = 5;
    } else if (((u8 (*)[10])D_80099A3C)[row][2] < turn) {
        col = 2;
    }
    angle = actor->unk76;
    if ((actor->unk78 - angle) & 0x8000) {
        col += 3;
    } else {
        col += 4;
    }
    entry = &((u8 (*)[10])D_80099A3C)[row][col];
    if (*entry != 0xFF) {
        actor->unk218--;
        actor->unk21C = 20000;
        actor->unk223 = 0xD;
        actor->unk44 = 0.0f;
        actor->unk244 = *entry;
        actor->unkF4 &= ~0xE;
        actor->unkF4 |= 4;
        actor->unk78 = angle;
        actor->unk7A = angle;
        actor->unk138 = 0;
    }
}
/* Per actor-type info, D_800D1C90[id] (only the fields used here). */
typedef struct ActorInfo15056B08 {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 headHeight;
    /* 0x16 */ u8 pad16;
    /* 0x17 */ u8 viewAngle; /* width of the spotting cone, in 1/256 turns */
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 pad1A[0xD];
    /* 0x27 */ u8 headBone;
} ActorInfo15056B08;

struct216 *func_1507BB28(s32, s32);
void func_1507BC14(Actor *);
s32 func_150229E4(Actor *);
void func_15047390(MtxF mf, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);
void func_1504C9E4(Actor *arg0, s8 arg1, u8 arg2);
u8 func_15059B54(Actor *arg0, u16 arg1);
extern f32 D_800CC2E4;
extern f32 D_800CC2EC;

/*
 * Per-frame AI for a path/chase actor: picks the point to head for from its mode (unk223),
 * looks for targets to switch behaviour script (unk218), then turns and moves toward the point.
 */
void func_15056B08(Actor *actor) {
    f32 tx;
    f32 tz;
    f32 ty;
    s32 state;
    f32 rangeSq;
    f32 dist;
    u8 targetIdx;
    u8 mode;
    f32 accel;
    f32 decel;
    u8 turnDiff;
    u16 cooldown;
    f32 speed;
    u16 moveTurnSpeed;
    u8 faceTurnRate;
    f32 targetDist;
    Actor *target;
    f32 arriveDist;
    u8 scanForTargets;
    ActorInfo15056B08 *info;
    f32 dx;
    f32 dz;
    f32 distSqXZ;

    scanForTargets = 1;
    if (actor->unk23A) {
        actor->unk218 = func_1507BB28(4, actor->unk23A);
        actor->unk21C = 0;
        actor->unk23A = 0;
    }
    if ((D_800C3638 != 0) && (func_150229E4(actor) != 0)) {
        return;
    }
    if ((actor->unkF4 & 0x1000) && (actor->unk28 < 0.200000003f)) {
        actor->unk21C = 0;
    }
    func_1507BC14(actor);
    mode = actor->unk223;
    if ((mode == 0xE) || (mode == 0xF) || (mode == 0x10)) {
        mode = 1;
    }
    if (mode == 0x11) {
        mode = 0;
    }
    if (actor->stunned != 0) {
        return;
    }
    moveTurnSpeed = (((actor->unk1E5 << 8) + actor->unk1E7) * D_800CC264) / 100;
    faceTurnRate = actor->unk1E6;
    if (D_800D2108[actor->unk13F] == 0) {
        actor->xz_velocity = 0.0f;
        func_1505841C(actor, 0.0f);
        return;
    }
    state = mode;
    info = (ActorInfo15056B08 *)D_800D1C90[actor->id];
    if (actor->unk23D == 0xFF) {
        rangeSq = 1000000000.0f;
    } else {
        rangeSq = actor->unk23D * 8;
        rangeSq *= rangeSq;
    }
    if (((PathPoint **)D_800D2104)[actor->unk13F]->param != 0) {
        tx = ((PathPoint **)D_800D2104)[actor->unk13F]->x - actor->x_position;
        tz = ((PathPoint **)D_800D2104)[actor->unk13F]->z - actor->z_position;
    }
    if ((actor->unk25C & 4) && !(actor->unk25C & 1) && ((state == 1) || (state == 3))) {
        actor->unk21C = 0;
    }
    if ((actor->unk25C & 1) && ((state == 0) || (state == 5))) {
        if (func_1505693C(actor, actor->pad259[2]) != 0) {
            actor->unk218 = func_1507BB28(1, actor->unk233);
            actor->unk21C = 0;
            actor->unkA8 = actor->pad259[2] | 0x80;
            actor->unk222 = actor->pad259[2];
        }
    }
    if (actor->pad259[1] != 0) {
        actor->unk44 = actor->pad259[1];
    }
    if ((state == 1) || (state == 0xC) || (state == 2) || (state == 3) || (state == 9) || (state == 0xA) ||
        ((state == 6) && !(actor->unk22C & 1))) {
        targetIdx = actor->unk222;
        target = &D_800CC2D0[targetIdx];
        tx = target->x_position;
        tz = target->z_position;
        ty = target->y_position;
        if (actor->unk223 == 0x10) {
            f32 leadX;
            f32 leadZ;
            f32 leadY;

            leadZ = func_1505A72C(target, actor);
            leadX = target->xz_velocity * (leadZ / actor->unk231);
            func_1505A184(target->unk76, leadX, 0.0f, &leadX, &leadZ, &leadY);
            tx += leadX;
            tz += leadZ;
        }
        dx = tx - actor->x_position;
        dz = tz - actor->z_position;
        dist = sqrtf((dx * dx) + (dz * dz));
        if ((state == 1) && (actor->unk44 > 1.0f) && (sqrtf(rangeSq) < (dist * 0.8000000119f))) {
            actor->unk21C = 0;
        }
        if (actor->id != 0x16) {
            if ((state == 1) && (actor->unk234 != 0) && ((dist < 370.0f) || (actor->id == 0x21)) && (targetIdx < 4) &&
                ((actor->xz_velocity > 15.0f) || (actor->id == 0x53))) {
                if (target->unk31C != NULL) {
                    target->unk31C->chased = 10;
                }
            }
            if ((state == 2) && (dist < 470.0f) && (targetIdx < 4) && (((actor->unk76 - target->unk76) - 0x4000) & 0x8000)) {
                if (target->unk31C != NULL) {
                    target->unk31C->chasing = 6;
                }
            }
        }
        if (state != 9) {
            f32 range;
            s32 ok;
            u16 angle;

            if ((state != 0xA) && (state != 0xC)) {
                scanForTargets = 0;
            }
            if ((actor->unk22C & 0x80) &&
                (fabsf((actor->y_position - actor->unk28) - (target->y_position - target->unk28)) > 30.0f)) {
                actor->unk21C = 0;
            }
            if (actor->unk223 != 0xF) {
                if (func_1505693C(actor, actor->unk222) == 0) {
                    actor->unk21C = 0;
                }
            }
            if ((u16)actor->unk22E != 0) {
                range = 0.0f;
            } else if (!actor->unk236) {
                range = actor->unk23C * 8;
            } else {
                range = (actor->unk236 == 0xFF) ? 3000000.0f : actor->unk236 * 8;
            }
            if (((state == 1) || (state == 6) || (state == 0xA)) && (actor->unk234 != 0) && (dist < range)) {
                ok = 1;
                if ((actor->id == 8) && (range < fabsf(actor->y_position - target->y_position))) {
                    ok = 0;
                }
                if (actor->pad22A != 0) {
                    range = actor->pad22A * 8.0f;
                    if ((D_800C3E78 != 0) && (fabsf(actor->y_position - D_800CC2E8[0]) < range)) {
                        ok = 0;
                    }
                }
                if (ok) {
                    angle = func_1505A630(dx, -dz, 0);
                    if (actor->unk237 &&
                        (((((actor->unk76 - angle) >> 8) + (actor->unk237 / 2)) & 0xFF) < actor->unk237)) {
                        actor->unk218 = func_1507BB28(2, actor->unk234);
                        actor->unk21C = 0;
                    }
                }
            }
        }
    }
    if (scanForTargets != 0) {
        s32 alertKind;

        cooldown = actor->unk22E;
        if (actor->unk235 != 0) {
            alertKind = 0;
        } else {
            alertKind = 1;
        }
        if (cooldown != 0) {
            if (D_800BE9A0 >= cooldown) {
                actor->unk22E = 0;
            } else {
                actor->unk22E = cooldown - D_800BE9A0;
            }
        }
        if (actor->unk241 != 0) {
            s32 i;
            s32 best;
            f32 bestDist;
            f32 ox;
            f32 oz;
            f32 d;
            s32 cone;
            s32 angHi;

            best = 0;
            bestDist = actor->unk23F * 8;
            for (i = 0; i < 25; i++) {
                target = &D_800CC2D0[i];
                if ((target->interaction_state != 0) && (target->id == actor->unk240) && (target->unk2FA != 0)) {
                    ox = target->x_position - actor->x_position;
                    oz = actor->z_position - target->z_position;
                    d = sqrtf((ox * ox) + (oz * oz));
                    if (d < bestDist) {
                        cone = info->viewAngle * 3;
                        cone /= 2;
                        angHi = (u8)(func_1505A630(ox, oz, 0) >> 8);
                        if ((((((actor->unk7A >> 8) - angHi) + (cone / 2)) & 0xFF) < cone)) {
                            best = i + 1;
                            bestDist = d;
                        }
                    }
                }
            }
            if (best != 0) {
                actor->unk218 = func_1507BB28(0, actor->unk241);
                actor->unk21C = 0;
                actor->unk222 = best - 1;
                actor->unkA8 = best + 0x7F;
                actor->unk22E = 5;
                if (actor->unk242 != 0) {
                    D_800D154C = &D_800CC2D0[best - 1];
                    D_800CC2D0[best - 1].unk218 = func_1507BB28(0, actor->unk242);
                    D_800CC2D0[best - 1].unk21C = 0;
                    D_800CC2D0[best - 1].unk222 = D_800C3E78;
                    D_800D154C = actor;
                }
            }
        }
        if (((u16)actor->unk22E == 0) && (state != 4) && (state != 7) &&
            (((alertKind == 0) && (actor->unk235 != 0)) ||
             ((alertKind == 1) && (actor->unk233 != 0) && !(actor->unk25C & 4)))) {
            u8 spotted;
            f32 px;
            f32 pz;
            f32 dsq;
            u8 pad; // matching: stack slot

            if (info->unk18 <= actor->xz_velocity) {
                if (func_1505693C(actor, 0) != 0) {
                    spotted = 0;
                    px = D_800CC2E4 - actor->aim.x;
                    pz = actor->aim.z - D_800CC2EC;
                    if ((actor->id == 0x9E) && (actor->unk2E4 == 0)) {
                        rangeSq = 129600.0f;
                    }
                    dsq = (px * px) + (pz * pz);
                    if ((dsq < rangeSq) && (dsq < 100000000.0f)) {
                        if (func_1505693C(actor, 0) != 0) {
                            if (actor->unk229 == 0xFF) {
                                dsq = sqrtf(rangeSq);
                            } else {
                                dsq = actor->unk229 * 8.0f;
                            }
                            if ((D_800C3E78 != 0) &&
                                (!actor->unk229 || (fabsf(actor->y_position - D_800CC2E8[0]) < dsq))) {
                                if (actor->id == 0x9E) {
                                    MtxF mtx;
                                    f32 lookX;
                                    f32 lookZ;
                                    f32 lookY;

                                    func_1505A184(actor->unk7A, 9000.0f, actor->unkB8, &lookX, &lookZ, &lookY);
                                    lookX += actor->x_position;
                                    lookZ += actor->z_position;
                                    lookY += actor->y_position;
                                    func_15047390(mtx, actor->x_position, actor->y_position, actor->z_position, lookX, lookY,
                                                  lookZ, 0.0f, 1.0f, 0);
                                    if (func_15049440(mtx, D_800CC2E4, D_800CC2E8[0], D_800CC2EC, 80.0f, 80.0f, 0.0f,
                                                      4000.0f, 320.0f, 320.0f) != 0) {
                                        spotted = 1;
                                    }
                                } else {
                                    s32 fov;

                                    fov = info->viewAngle;
                                    if (alertKind == 0) {
                                        fov *= 3;
                                    }
                                    if (((((((actor->unk7A >> 8) + actor->unk2CA) - (u8)(func_1505A630(px, pz, 0) >> 8)) +
                                           (fov / 2)) & 0xFF) < fov)) {
                                        spotted = 1;
                                    }
                                }
                            }
                        }
                    }
                    if (spotted) {
                        if (alertKind == 0) {
                            actor->unk218 = func_1507BB28(3, actor->unk235);
                        } else {
                            actor->unk218 = func_1507BB28(1, actor->unk233);
                        }
                        actor->unk21C = 0;
                        actor->unk222 = spotted - 1;
                        actor->unkA8 = spotted + 0x7F;
                    }
                }
            }
        }
        if (state != 9) {
            if ((state == 6) || (state == 0xC)) {
                tx = D_800CC2D0[actor->unk222].x_position;
                tz = D_800CC2D0[actor->unk222].z_position;
                ty = D_800CC2D0[actor->unk222].y_position;
            } else {
                tx = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].x;
                tz = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].z;
                ty = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].y;
            }
            if ((state == 5) || (state == 6)) {
                f32 orbitX;
                f32 orbitZ;
                f32 orbitY;

                if ((state == 6) && (actor->unk22C & 1)) {
                    actor->unk86 = D_800CC2D0[actor->unk222].unk7A >> 8;
                }
                actor->unk86 += actor->unk257;
                orbitX = actor->unk258 * 16.0f;
                func_1505A184(actor->unk86 << 8, orbitX, 0.0f, &orbitX, &orbitZ, &orbitY);
                tx += orbitX;
                tz += orbitZ;
            }
        }
    }
    speed = actor->unk44;
    if ((state != 7) && (state != 8)) {
        tx = tx - actor->x_position;
        tz = actor->z_position - tz;
        ty = actor->y_position - ty;
        if ((state == 6) && (actor->unk22C & 1) && ((fabsf(tx) + fabsf(tz)) < 100.0f)) {
            speed = 0.0f;
            tx = D_800CC2D0[actor->unk222].x_position - actor->x_position;
            tz = actor->z_position - D_800CC2D0[actor->unk222].z_position;
            actor->unk78 = func_1505A630(tx, tz, 0);
        } else if ((actor->unk44 >= 0.5f) && !(actor->unkF4 & 0x40)) {
            actor->unk78 = func_1505A630(tx, tz, 0);
        }
    }
    accel = actor->unk247 / 10.0f;
    distSqXZ = (tx * tx) + (tz * tz);
    targetDist = distSqXZ;
    if (actor->unk22C & 8) {
        targetDist += ty * ty;
    }
    targetDist = sqrtf(targetDist);
    if ((state != 4) && (state != 7) && (state != 8)) {
        u8 k;
        u8 n;
        u16 prevMoveAngle;
        u16 ang;
        s32 flags;

        prevMoveAngle = actor->unk76;
        if (state == 2) {
            actor->unk78 ^= 0x8000;
            if (!(actor->unkF4 & 0x4000)) {
                actor->unk78 += (((actor->unk21C / 100) & 0x20) - 0x10) << 8;
            }
        }
        if (actor->unk223 == 0x11) {
            actor->unk78 += (((actor->unk21C / 100) & 0x10) - 8) << 9;
        }
        if ((actor->unkF4 & 0x200) && (actor->unk1F4 < 3.0f)) {
            moveTurnSpeed = 0;
            faceTurnRate = 0;
        }
        arriveDist = actor->unk23E * 8;
        turnDiff = func_15059B54(actor, moveTurnSpeed);
        if (state != 0xD) {
            if (((u8)actor->unk24F != 0) && ((state != 0) || (arriveDist < targetDist))) {
                k = (u8)actor->unk24F - 1;
                if (((u8(*)[10])D_80099A3C)[k][0] < turnDiff) {
                    actor->unk76 = prevMoveAngle;
                    func_15056A00(actor, turnDiff, k);
                }
            }
        }
        if (state == 0xD) {
            moveTurnSpeed = 0;
            faceTurnRate = 0;
        }
        if ((info->unk19 < turnDiff) && (state != 2)) {
            speed = 0.0f;
            if (actor->unk24A && (actor->xz_velocity > 15.0f)) {
                actor->unk218 = func_1507BB28(6, actor->unk24A);
                actor->unk21C = 0;
                actor->unk222 = 0;
            }
        }
        if (((state == 0) || (state == 0xA)) && (targetDist < arriveDist)) {
            n = D_800D2108[actor->unk13F] - 1;
            if (actor->unk21F != 0) {
                n = actor->unk21F;
            }
            if (actor->unk22C & 4) {
                actor->x_position = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].x;
                actor->z_position = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].z;
                actor->unk1FC |= 8;
                if (actor->unk22C & 8) {
                    actor->y_position = ((PathPoint **)D_800D2104)[actor->unk13F][actor->unk21E + 1].y;
                }
            }
            if (!(actor->unkF4 & 0x10)) {
                actor->unk21C = 0;
            }
            actor->unk21E += actor->unk221;
            actor->unk21E += n;
            actor->unk21E %= n;
            if (actor->unk21E < actor->unk220) {
                actor->unk21E = actor->unk220;
            }
        }
        ang = actor->unk78;
        if (actor->unkF4 & 0x20000) {
            ang = func_1505A630(D_800CC2D0[actor->unk222].x_position - actor->x_position,
                                actor->z_position - D_800CC2D0[actor->unk222].z_position, 0);
        }
        turnDiff = func_150599C8(actor, faceTurnRate, ang);
        decel = actor->unk248 / 10.0f;
        if ((actor->unk3A != 0) && (actor->unk28 == 0.0f) && !(actor->unk3A & 3 & D_800D1548)) {
            actor->unk84.uh = 0xFF;
            actor->y_velocity = actor->unk3A;
        }
        flags = actor->unkF4;
        if (flags & 1) {
            speed = 0.0f;
        }
        if ((actor->unk22C & 0x10) && (actor->unk28 == 0.0f)) {
            speed = 0.0f;
            accel = 1.0f;
        }
        if (actor->interaction_state == 1) {
            actor->unk44 = speed;
            return;
        }
        if (flags & 0x80000) {
            f32 h;

            h = ty + 70.0f;
            if (D_800BE9F0 == 0x26) {
                h -= 45.0f * actor->y_scale;
            }
            ang = func_1505A630(sqrtf(distSqXZ), h, 0);
            func_1504C9E4(actor, (s16)ang / 200, 0);
        } else if (actor->unk1D1 != 0) {
            func_1504C9E4(actor, 0, 0);
        }
        if (actor->unkF8 & 0x40000) {
            f32 wantVelX;
            f32 wantVelZ;
            f32 wantVelY;
            f32 r;

            speed = actor->unk44;
            func_1505A184(actor->unk78, speed, 0.0f, &wantVelX, &wantVelZ, &wantVelY);
            if ((speed < 1.0f) || (((prevMoveAngle - actor->unk78) + 0x4000) & 0x8000)) {
                r = decel;
            } else {
                r = accel;
            }
            func_1505A250(wantVelX, wantVelZ, r, &actor->unk174, &actor->unk178);
            actor->unk76 = func_1505A630(actor->unk174, -actor->unk178, 0);
            r = sqrtf((actor->unk174 * actor->unk174) + (actor->unk178 * actor->unk178));
            actor->xz_velocity = r + r;
        } else {
            func_1505A3A8(speed, actor, accel, decel, turnDiff);
        }
    } else {
        func_150599C8(actor, actor->unk1E6, actor->unk78);
        actor->unk76 = actor->unk78;
        actor->xz_velocity = speed;
    }
    func_1505841C(actor, accel);
    if ((actor->unk22C & 8) && !(actor->unk1FC & 8)) {
        f32 a;
        f32 lim;

        a = (u32)func_1505A630(sqrtf(distSqXZ), ty, 0) * 0.005493164f;
        if (a > 180.0f) {
            a -= 360.0f;
        }
        a = (a - actor->unkB8) * 0.25f;
        lim = moveTurnSpeed * 0.005493164f;
        if (a > lim) {
            a = lim;
        }
        if (a < -lim) {
            a = -lim;
        }
        actor->unkB8 = actor->unkB8 + a;
    }
}
/* Plays the actor's movement animation (unk244) at a rate derived from its speed, or at `idleRate`. */
void func_1505841C(Actor *actor, f32 idleRate) {
    struct197 *anim;
    f32 speed;
    f32 fade;
    f32 rate;
    f32 scaledSpeed;
    s32 flags;

    flags = actor->unk246;
    rate = (f32)(((flags & 0x1F) << 8) + actor->unk249) * 0.0003906250058f;
    if (!(flags & 0x80)) {
        speed = actor->xz_velocity;
        if ((speed <= 1.0f) || ((actor->unk22C & 0x10) && (actor->unk28 == 0.0f))) {
            rate = idleRate;
        } else {
            scaledSpeed = speed;
            scaledSpeed *= 0.5f / actor->xz_scale;
            rate *= 10.0f;
            rate = scaledSpeed / rate + 0.1000000015f;
            if (!(flags & 0x20)) {
                rate += 0.400000006f;
            }
        }
    }
    if (actor->unk223 == 0xD) {
        rate = (f32)(actor->unk250 & 0x7F) * 0.1000000015f;
    }
    if (flags == 0xFF) {
        rate = 0.0f;
    }
    func_1505E650(actor, actor->unk244, rate, (f32)actor->unk1D0, 0.0f, 0.0f, 0);
    if (actor->unk246 == 0xFF) {
        fade = D_800418B0[D_800419A0][0];
        if (fade >= 0.0f) {
            anim = actor->unk2D0;
            anim->unk8 = (anim->unk18 * (32768.0f - fade)) / 32768.0f;
        }
    }
}

/* Per-frame update while the actor is stunned (stunned is the countdown; 255 = indefinitely). */
void func_150585F0(Actor *actor, f32 decel) {
    ActorCtl *child;
    f32 yVel;
    s32 water;
    s32 stun;
    s32 flags;
    s32 facing;
    u16 angle;
    s32 owner;

    decel = (f32)(u32)actor->unk109 * 0.1000000015f;
    func_1505A3A8(0.0f, actor, 1.0f, decel, 0);
    if (actor->unk1CC < 100000.0f) {
        actor->unk1CC = actor->y_position;
    }
    water = actor->in_water;
    if (water != 0 && water < 10) {
        yVel = actor->y_velocity;
        actor->gravity = 0.0f;
        if (yVel < 60.0f) {
            actor->y_velocity = yVel * 0.8999999762f;
        }
        if ((actor->unk118 - 60.0f) + 40.0f < actor->y_position) {
            actor->unk81 = 0;
            actor->unk83 = 0;
            actor->in_water = 0;
            actor->unkB8 = 0.0f;
            actor->gravity = 4.0f;
        }
    }
    if (D_800BE616 == 0 || actor->interaction_state != 1) {
        actor->unk21C = 0;
    }
    stun = actor->stunned;
    if (stun != 255) {
        if (stun != 254 && (actor->health != 0 || actor->interaction_state != 1)) {
            actor->stunned = stun - 1;
        }
        actor->unk10C -= D_800CC264;
        if (actor->unk28 < 0.200000003f && (actor->unkF4 & 0x100)) {
            actor->unk10C = 0;
        }
        if (actor->unk10C <= 0) {
            child = actor->unk31C;
            if (child != NULL) {
                child->matrix_physics = 0;
            }
            actor->unk10C = 0;
            func_1505E874(D_800C3E78, actor);
        }
        if (actor->stunned == 0) {
            facing = actor->unk7A;
            actor->xz_velocity = 0.0f;
            actor->unk81 = 0;
            actor->unk78 = facing;
            actor->unk76 = facing;
            if (actor->in_water != 0) {
                actor->y_velocity = 0.0f;
                if (actor->interaction_state == 1) {
                    func_1506B078();
                }
            } else if (actor->interaction_state == 1) {
                actor->gravity = 4.0f;
                actor->unkF8 &= ~0x8000;
            }
            owner = actor->unk238;
            if (owner != 0) {
                actor->unk23A = owner;
            }
        } else {
            flags = actor->unk10B;
            if (!(flags & 2)) {
                angle = actor->unk76;
                if (flags & 4) {
                    angle ^= 0x8000;
                }
                actor->unk80 = 10;
                func_150599C8(actor, 12, angle);
            }
        }
    }
}

void func_1502178C(Actor *arg0, s32 arg1, s32 arg2);
s32 func_150A3FC4(f32, f32, void *, s16 (*)[3], f32 *);
void func_1510E8BC(void *a0, void *a1, void *a2, void *a3, void *arg4, void *arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, s32 arg10, Actor *arg11, f32 arg12, f32 arg13, s32 arg14);
f32 func_1510F648(f32, f32, f32);
s32 func_15044380(f32 arg0, f32 arg1, f32 arg2, void *arg3, s32 arg4, s32 arg5);
void func_150B9ED0(Actor *arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
struct216 *func_1507BB28(s32, s32);
u8 func_1505DADC(Actor *arg0, u16 *arg1, u8 arg2, u8 arg3, u8 arg4);
extern u8 D_800CBDD3;

/* Updates the actor's floor height (unk180) and pushes it out of walls, reacting to a wall hit. */
void func_15058898(Actor *actor, f32 prevY) {
    f32 groundY;
    s32 hostIdx;
    s32 fastFloor;
    s32 flags;
    f32 *floorOut;

    actor->unk118 = -10000.0f;
    func_1502178C(actor, 0, -1);
    if (actor->unkF8 & 0x8000) {
        if (actor->unk5 == 7) {
            actor->unk28 = 0.0f;
        } else {
            actor->unk28 = 1.0f;
        }
        return;
    }
    if (actor->unkF8 & 0x200000) {
        actor->unk180 = (s16)actor->unk224;
        return;
    }
    fastFloor = actor->unkF8 & 2;
    hostIdx = actor->unk13C;
    flags = hostIdx - 100;
    if ((hostIdx >= 100) && (flags == 0)) {
        fastFloor = D_800CC2D0[flags].unkF8 & 2;
    }
    if (!fastFloor ||
        !func_150A3FC4(actor->x_position, actor->z_position, actor->unk188, actor->floorVerts, &groundY)) {
        if (actor->unkF8 & 8) {
            floorOut = &actor->unk118;
        } else {
            floorOut = NULL;
        }
        func_1510E8BC(&actor->unk188, actor->floorVerts, &groundY, floorOut, &actor->unk184, &actor->floorType,
                      actor->x_position, actor->y_position, actor->z_position, prevY, 0, actor, -10000.0f,
                      actor->y_position, actor->unkF8 & 0x04000000);
    } else if (actor->unkF8 & 8) {
        actor->unk118 = func_1510F648(actor->x_position, actor->y_position, actor->z_position);
    }
    flags = actor->unkF8;
    if ((actor->unk13C != 0) && (actor->id != 0x9A)) {
        flags = D_800CC2D0[actor->unk13C - 100].unkF8;
    }
    if ((flags & 4) && (actor->unk13D == 0) &&
        ((actor->unk127 != 0xFF) || (actor->x_position != actor->old_x_position) ||
         (groundY != actor->old_y_position) || (0.0f != actor->unk28) ||
         (actor->z_position != actor->old_z_position))) {
        f32 x;
        f32 z;

        x = actor->x_position;
        z = actor->z_position;
        if ((actor->id == 0x1B) || (actor->id == 0x7E)) {
            D_800CBDD3 = 1;
        }
        func_15044380(actor->old_x_position, actor->old_y_position, actor->old_z_position, actor, 1, 0);
        if ((actor->id == 0x1B) || (actor->id == 0x7E)) {
            D_800CBDD3 = 0;
        }
        if ((x != actor->x_position) || (z != actor->z_position)) {
            actor->unkF4 |= 0x10000;
            if (actor->unk24E != 0) {
                if (actor->id == 0xC) {
                    func_150B9ED0(actor, x, z, 0xFF, 0);
                }
                actor->unk232 = actor->unk24E;
                actor->unk24E = 0;
                actor->unk218 = NULL;
                actor->unk78 = (actor->unk7A = actor->unk76);
            }
            if (actor->unkF8 & 0x1000) {
                /* bounce: reflect the moving angle about the push-out direction */
                u16 angle = func_1505A630(actor->x_position - x, z - actor->z_position, 0);

                actor->unk76 = ((angle - actor->unk76) + angle) + 0x8000;
                if (actor->interaction_state == 0x21) {
                    actor->xz_velocity *= 0.8999999762f;
                } else if (actor->interaction_state == 0x26) {
                    actor->xz_velocity *= 0.3000000119f;
                } else if ((actor->interaction_state != 4) || (actor->unk103 == 0)) {
                    actor->xz_velocity *= 0.3000000119f;
                } else {
                    u16 freeAngle;

                    actor->xz_velocity *= 0.8999999762f;
                    actor->unk7A = actor->unk76;
                    if (func_1505DADC(actor, &freeAngle, 0, actor->unk124 - 1, 20) != 0xFF) {
                        actor->unk76 = freeAngle;
                    }
                }
                if ((actor->unk2CC != 0) && (actor->xz_velocity > 5.0f)) {
                    D_800D1580 = actor->unk2CC;
                    func_1506E5FC();
                }
                if (actor->interaction_state == 4) {
                    actor->unk102 = 0;
                    actor->gravity = 3.0f;
                }
            }
            if (!(actor->unkF8 & 2) ||
                !func_150A3FC4(actor->x_position, actor->z_position, actor->unk188, actor->floorVerts, &groundY)) {
                func_1510E8BC(&actor->unk188, actor->floorVerts, &groundY, &actor->unk118, &actor->unk184,
                              &actor->floorType, actor->x_position, actor->y_position, actor->z_position, prevY, 0,
                              actor, -10000.0f, actor->y_position, actor->unkF8 & 0x04000000);
            }
        }
    }
    {
        u8 fallScript = actor->unk255;

        if (1) { // matching: the extra block makes IDO pick v0 for fallScript
            if ((fallScript != 0) && (actor->stunned == 0) &&
                (groundY < (actor->unk180 - (f32)(actor->unk256 * 8)))) {
                actor->y_velocity = 0.0f;
                actor->gravity = 0.0f;
                actor->xz_velocity = 0.0f;
                actor->unk218 = func_1507BB28(0, actor->unk255);
                actor->unk21C = 0;
                actor->unk255 = 0;
                return;
            }
        }
    }
    actor->unk180 = groundY;
}

/* Picks gravity by height band and clamps the vertical velocity. */
void func_15058EA4(Actor *actor, f32 maxY, f32 gravityAbove, f32 minY, f32 gravityBelow, f32 maxVelY, f32 minVelY) {
    if (maxY < actor->y_position) {
        actor->gravity = gravityAbove;
    } else if (actor->y_position < minY) {
        actor->gravity = gravityBelow;
    }
    if (maxVelY < actor->y_velocity) {
        actor->y_velocity = maxVelY;
    } else if (actor->y_velocity < minVelY) {
        actor->y_velocity = minVelY;
    }
}

/* Floats the actor on the water surface (unk118 + 9); `depth` in [0, 1) sets how low it sits. */
void func_15058F24(Actor *actor, f32 depth, f32 arg2) {
    f32 tmp; // matching: one variable for the damping, offset and sink limit (IDO allocates it as one)
    f32 surfaceY;
    f32 lift;

    tmp = depth;
    lift = 0.5f - depth;
    if (depth >= 0.5f) {
        depth -= 0.5f;
    }
    if (arg2) {} // matching: a use of the unused arg2 keeps it out of its stack slot
    surfaceY = actor->unk118 + 9.0f;
    if ((actor->y_position < (actor->unk118 + 9.0f)) || (actor->in_water >= 100)) {
        if (actor->in_water == 0) {
            actor->y_position = surfaceY;
            actor->in_water = 100;
            actor->gravity = lift * -6.0f;
            actor->y_velocity *= tmp;
        } else {
            if ((surfaceY + 100.0f) < actor->y_position) {
                actor->in_water = 0;
            }
            if (lift < 0.0f) {
                actor->gravity = lift * -6.0f;
            } else {
                tmp = actor->y_position - ((surfaceY + 10.0f) - (120.0f * depth));
                if ((fabsf(tmp) < 2.0f) && (fabsf(actor->y_velocity) < 2.0f)) {
                    actor->y_position -= tmp * 0.6000000238f;
                    actor->y_velocity = 0.0f;
                    actor->gravity = 1.0f;
                } else if (tmp > 0.0f) {
                    if (actor->y_velocity > 0.0f) {
                        actor->gravity = lift * 6.0f;
                        actor->y_velocity /= 2.0f;
                    }
                } else {
                    tmp = lift * 80.0f;
                    if (tmp < actor->y_velocity) {
                        actor->y_velocity = tmp;
                    }
                    actor->gravity = lift * -6.0f;
                }
            }
        }
        tmp = -100.0f * depth;
        if (actor->y_velocity < tmp) {
            actor->y_velocity = tmp;
        }
    }
}

/* Per-frame movement/physics step for a non-player actor. */
void func_15059140(Actor *actor) {
    f32 stepRate;

    stepRate = 0.5f;
    D_800CC268 = 0;
    if ((actor->unk13D == 0) || (actor->unk21C < 16)) {
        if (actor->unk223 != 8) {
            func_15059C84(actor);
            func_1505A770(actor);
        }
    }
    func_1505D6F0(actor, D_800C3E78);
    func_15055E50(actor, actor->unk1E4);
    if (actor->id == 8) {
        func_15056150(actor);
    }
    if (actor->unk13D < 100) {
        func_15058898(actor, actor->old_y_position);
    }
    if (actor->unkF4 & 0x40000) {
        if (((actor->unkF4 & 0x12000) != 0) || (D_800CC268 != 0)) {
            actor->unk21C = 0;
        }
    }
    if (actor->unkB0 != 0) {
        func_15058F24(actor, actor->unkB0 * 0.009999999776f, 1.0f);
    }
    if (actor->unkF8 & 0x20000) {
        func_15056258(actor);
    }
    if (actor->in_water != 0) {
        func_15059444(actor);
    }
    if (actor->id == 0xA) {
        actor->unk180 = -5000.0f;
    }
    if ((actor->unkF8 & 0x180000) != 0) {
        func_150511E8(actor);
    }
    func_1505B5F8(actor, actor->unk180);
    if (((actor->unkF4 & 0x1000) != 0) && (actor->unk28 < 0.200000003f)) {
        func_15056B08(actor);
    }
    if (actor->unkF8 & 0x80000) {
        stepRate = 0.150000006f;
    }
    func_1505A250(0.0f, 0.0f, stepRate, &actor->unk164, &actor->unk168);
    if (actor->unkD0 != 0) {
        func_1505C7D8(actor, D_800C3E78);
    }
    if (actor->unk13D == 0) {
        if (D_800BE9A0 >= actor->unk10F) {
            actor->unk10F = 0;
        } else {
            actor->unk10F -= D_800BE9A0;
        }
    }
    if (actor->unk107 != 0) {
        actor->unk107 -= 1;
    }
    if (actor->immune != 0xFF) {
        if (D_800BE9A0 >= actor->immune) {
            actor->immune = 0;
        } else {
            actor->immune -= D_800BE9A0;
        }
    }
}

/* Eases the actor's push velocity (unk16C, unk170) toward `speed` along `angle`. */
void func_150593C4(Actor *actor, u16 angle, f32 speed, f32 rate) {
    f32 x;
    f32 z;
    f32 y;

    func_1505A184(angle, speed, 0.0f, &x, &z, &y);
    actor->unk16C += (x - actor->unk16C) * rate;
    actor->unk170 += (z - actor->unk170) * rate;
}

/* Applies the current map's (D_800BE9F0) water current to a swimming actor. */
void func_15059444(Actor *actor) {
    s32 pathIdx;
    s32 id;
    s32 tmp;

    pathIdx = -1;

    switch (D_800BE9F0) {
        case 4:
            func_150593C4(actor, 0x4000, 50.0f, 0.05999999865889549f);
            break;
        case 6:
            pathIdx = func_15083E0C(28);
            break;
        case 41:
            if (func_150A29C8(D_800C3E78, 0x4028) == 0) {
                id = 12;
            } else if (func_150A29C8(D_800C3E78, 0x400E) == 0) {
                id = 10;
            } else if (func_150A29C8(D_800C3E78, 0x400D) == 0) {
                id = 9;
            } else {
                id = 6;
            }
            pathIdx = func_15083E0C(id);
        split: // matching: label steers IDO register allocation for id
            break;
        case 43:
            pathIdx = func_15083E0C(18);
            break;
        case 65:
            if (func_150A29C8(D_800C3E78, 0x401F) == 0) {
                id = 27;
            } else {
                id = 26;
            }
            pathIdx = func_15083E0C(id);
            break;
    }

    if (pathIdx != -1) {
        func_150611E8(actor, pathIdx);
    }
}

/* Attaches the actor to D_800CC2D0[hostIdx] (grabbed/carried) and starts both actors' animations. */
void func_1505959C(Actor *actor, s32 hostIdx) {
    Game83300AnimEntry *anims;
    s32 *animTable;
    s32 variant;

    actor->unkF8 &= ~0x800000;
    actor->unk13D = hostIdx + 100;
    actor->unk21C = 0;
    actor->gravity = 4.0f;
    actor->unk25C &= ~8;

    D_800CC2D0[hostIdx].unk83 = 0xFF;
    D_800CC2D0[hostIdx].disable_run = 0xFF;
    variant = 0;
    if (actor->id == 0x57) {
        variant = 1;
    }
    if (D_800CC2D0[hostIdx].id == 0x9B) {
        variant = 2;
    }
    if (actor->id == 0x5E) {
        variant = 3;
    }
    if (actor->id == 0x3C) {
        variant = 4;
        D_800CC2D0[hostIdx].unk13C = 0;
        D_800CC2D0[hostIdx].unk76 = func_1505A630(actor->x_position - D_800CC2D0[hostIdx].x_position, D_800CC2D0[hostIdx].z_position - actor->z_position, 0);
    }
    if (actor->id == 0x89) {
        variant = 5;
    }
    func_1505E650(&D_800CC2D0[hostIdx], D_800860C0[variant], D_800860CC[variant], 0.0f, 0.0f, 0.0f, 0);
    actor->stunned = 0xFE;
    actor->unk105 = 0;
    actor->unk106 = func_1505E7CC(D_800860E4[variant], actor);
    actor->unk84.uh = 0xFFFF;
    func_1505E874(D_800C3E78, actor);
    animTable = (s32 *)D_800D1588[actor->id];
    if (animTable != NULL) {
        anims = (Game83300AnimEntry *)animTable[-2];
        if ((anims != NULL) && (anims[actor->unk106].sub[2].flags & 2)) {
            actor->unk7A = D_800CC2D0[hostIdx].unk7A;
        }
        if ((anims == NULL) || !(anims[actor->unk106].sub[2].flags & 1)) {
            actor->unk13E = (actor->unk7A - D_800CC2D0[hostIdx].unk7A) >> 8;
        } else {
            actor->unk13E = 0;
        }
    } else {
        actor->unk13E = (actor->unk7A - D_800CC2D0[hostIdx].unk7A) >> 8;
    }
}

/* Lets a free player in D_800CC268's mask grab the actor (see func_1505959C). */
void func_150597FC(Actor *actor) {
    s32 i;
    s32 mask;
    ActorCtl *movement;
    Actor *player;

    mask = D_800CC268;
    for (i = 0; i < D_8008FD8C; i++) {
        player = &D_800CC2D0[i];
        if (((1 << i) & mask) && (player->unk13C == 0) && (player->disable_run == 0) &&
            (player->interaction_state == 1) && (player->stunned == 0) && (player->unk127 != 0xFF)) {
            break;
        }
    }
    if (i != D_8008FD8C) {
        player = &D_800CC2D0[i];
        if (player->unk13C == 0) {
            movement = player->unk31C;
            if ((movement->unk27 == 0) && (player->health != 0) && (actor->unk13D == 0) &&
                ((actor->stunned != 0) || (actor->unk25C & 0x1000) || (actor->id == 0x57)) &&
                (actor->unk28 == 0.0f) && (player->unk28 == 0.0f) && (actor->unk25C & 8) &&
                (movement->unk19B == 0) &&
                ((actor->id != 0xA9) || (player->team == 0)) &&
                ((actor->id != 0xA8) || (player->team != 0))) {
                player->unk13C = D_800C3E78 + 100;
                player->xz_velocity = 0.0f;
                movement->unk18 = 0;
                func_1505959C(actor, i);
            }
        }
    }
}

/* Turns the facing angle (unk7A) toward targetAngle, limited per frame; returns the remaining gap >> 8. */
u8 func_150599C8(Actor *actor, u8 turnRate, u16 targetAngle) {
    s16 delta;
    s16 maxDelta;
    s16 minDelta;
    s32 step;
    s32 range;
    s16 signedDelta;

    step = (((turnRate << 8) + actor->unk1E8) * D_800CC264) / 100;
    delta = targetAngle - actor->unk7A;
    if (actor->unk1EA != 0) {
        range = (actor->unk1EA * D_800CC264) / 100;
        maxDelta = actor->unk1EC + range;
        minDelta = actor->unk1EC - range;
        if (maxDelta < delta) {
            delta = maxDelta;
        }
        if (delta < minDelta) {
            delta = minDelta;
        }
    }

    signedDelta = delta;
    if (delta < 0) {
        delta ^= 0xFFFF;
    }
    if (delta < step) {
        step = delta;
    }
    if ((actor->unkF4 & 1) == 0) {
        if (actor->unk80 != 0) {
            if (signedDelta < 0) {
                actor->unk7A -= step;
            } else {
                actor->unk7A += step;
            }
        }
    }
    return delta >> 8;
}

/* Turns the moving angle (unk76) toward unk78 by at most maxStep; returns the gap before turning >> 8. */
u8 func_15059B54(Actor *actor, u16 maxStep) {
    s16 delta;
    s16 maxDelta;
    s16 minDelta;
    s32 range;
    s16 absDiff;
    s16 signedDelta;

    delta = actor->unk78 - actor->unk76;
    absDiff = delta;
    if (absDiff < 0) {
        absDiff ^= 0xFFFF;
    }
    if (actor->unk1EA != 0) {
        range = (actor->unk1EA * D_800CC264) / 100;
        maxDelta = actor->unk1EC + range;
        minDelta = actor->unk1EC - range;
        if (maxDelta < delta) {
            delta = maxDelta;
        }
        if (delta < minDelta) {
            delta = minDelta;
        }
    }

    signedDelta = delta;
    if (delta < 0) {
        delta ^= 0xFFFF;
    }
    if (delta < maxStep) {
        maxStep = delta;
    }
    if ((actor->unkF4 & 1) == 0) {
        if (signedDelta < 0) {
            actor->unk76 -= maxStep;
            actor->unk1EC = -maxStep;
        } else {
            actor->unk76 += maxStep;
            actor->unk1EC = maxStep;
        }
    }
    return absDiff >> 8;
}

typedef struct Game83300Point {
    f32 values[3];
} Game83300Point;

typedef struct Game83300Triangle {
    Game83300Point vertices[3];
} Game83300Triangle;

void func_15049350(Game83300Triangle);

/* Applies one frame of horizontal movement (speed along unk76, plus push velocities), following the floor slope. */
void func_15059C84(Actor *actor) {
    f32 x;
    f32 z;
    f32 y;
    f32 vertical;
    u16 angle;
    Game83300Triangle triangle;
    f32 speed;
    s32 i;
    f32 length;
    f32 scale;

    speed = actor->xz_velocity;
    angle = actor->unk76;
    if (D_800D1548 == 1) {
        return;
    }
    if (actor->unk1FC & 8) {
        actor->unk1FC &= ~8;
        return;
    }
    if (actor->unk127 != 0xFF) {
        if (actor->unk31C->unk46 > 0) {
            angle += *(u16 *)&actor->unk31C->unk10; // matching: read unsigned here (lhu); stored signed elsewhere
            actor->unk7A = angle;
        }
    }
    if (actor->xz_velocity < 1.0f && actor->xz_velocity >= 0.0f && !(actor->unk1FC & 1)) {
        if (actor->unk7E < 250) {
            actor->unk7E += 1;
        }
        if (actor->unk164 == 0.0f && actor->unk168 == 0.0f && actor->unk16C == 0.0f && actor->unk170 == 0.0f) {
            actor->unk1F4 = 0.0f;
            return;
        }
    } else {
        actor->unk7E = 0;
    }
    if ((actor->in_water != 0 && (actor->unk31C == NULL || actor->unk31C->unk21 == 0)) ||
        (actor->unk22C & 8)) {
        x = actor->unkB8;
        if (x > 90.0f) {
            x = 90.0f;
        } else if (x < -90.0f) {
            x = -90.0f;
        }
    } else {
        x = 0.0f;
    }
    if (actor->unk1FC & 1) {
        actor->unk1FC &= ~1;
        if (D_800D1550) {
            speed = 2.0f * (actor->unk1F4 * actor->xz_scale / D_800D1550); // matching: operand order
        }
        actor->unk1F4 = actor->xz_velocity;
        angle = actor->unk7A;
    }
    actor->unk1F4 = speed;
    func_1505A184(angle, speed, x, &x, &z, &y);
    x += actor->unk164 + actor->unk16C;
    z += actor->unk168 + actor->unk170;
    if (actor->in_water == 10) {
        y = 0.0f;
    }
    if ((actor->interaction_state != 1 || actor->unk31C->unk4F == 0) && actor->unk28 <= 0.0f &&
        actor->in_water == 0 && !(actor->unk22C & 8)) {
        /* project the step onto the floor triangle */
        for (i = 0; i < 3; i++) {
            triangle.vertices[i].values[0] = actor->floorVerts[i][0];
            triangle.vertices[i].values[1] = actor->floorVerts[i][1];
            triangle.vertices[i].values[2] = actor->floorVerts[i][2];
        }
        func_15049350(triangle);
        if (D_800CC214 != 0.0f) {
            vertical = (-D_800CC210 * x - D_800CC218 * z) / D_800CC214;
            if (vertical > 4.0f || actor->interaction_state != 1 || actor->unk31C->unk4E != 2) {
                length = sqrtf(x * x + z * z + vertical * vertical);
                if (length != 0.0f) {
                    scale = sqrtf(x * x + z * z);
                    x = x * scale / length;
                    y = vertical * scale / length;
                    z = z * scale / length;
                }
            }
        }
    }
    x *= D_800D1550;
    z *= D_800D1550;
    y *= D_800D1550;
    actor->x_position += x;
    actor->z_position += z;
    actor->y_position += y;
}
f32 func_150AD780(f32);
f32 func_150AD78C(f32);
/* Splits `speed` (halved) along yaw `angle` and pitch `pitch` (degrees) into x/z/y components. */
void func_1505A184(u16 angle, f32 speed, f32 pitch, f32 *outX, f32 *outZ, f32 *outY) {
    f32 sinYaw;
    union { f32 f; f64 d; } yaw; // matching: the 8-byte union gives the frame IDO used
    f32 cosYaw;

    speed = speed * 0.5f;
    pitch = pitch * D_800994B8;
    *outY = func_150AD78C(pitch) * -speed;
    speed = func_150AD780(pitch) * speed;
    yaw.f = (f32)angle * D_800994BC;
    sinYaw = func_150AD780(yaw.f);
    cosYaw = func_150AD78C(yaw.f);
    *outX = sinYaw * speed;
    *outZ = -cosYaw * speed;
}
/* Steps the 2D vector (*x, *z) toward (targetX, targetZ) by rate * dt, without overshooting. */
void func_1505A250(f32 targetX, f32 targetZ, f32 rate, f32 *x, f32 *z) {
    f32 dx = targetX - *x, dz = targetZ - *z, mag, stepX, stepZ;

    if (dx == 0.0f) {
        if (dz == 0.0f) {
            return;
        }
    }
    mag = sqrtf((dx * dx) + (dz * dz));
    rate *= D_800D1550;
    stepX = fabsf((dx / mag) * rate);
    stepZ = fabsf((dz / mag) * rate);
    if (0.0f <= dx) {
        *x = *x + stepX;
        if (targetX < *x) {
            *x = targetX;
        }
    } else {
        *x = *x - stepX;
        if (*x < targetX) {
            *x = targetX;
        }
    }
    if (0.0f <= dz) {
        *z = *z + stepZ;
        if (targetZ < *z) {
            *z = targetZ;
        }
    } else {
        *z = *z - stepZ;
        if (*z < targetZ) {
            *z = targetZ;
        }
    }
}

/* Accelerates/decelerates the actor's xz_velocity toward targetSpeed; player rates are scaled in water and other states. */
void func_1505A3A8(f32 targetSpeed, Actor *actor, f32 accel, f32 decel, u8 turnDiff) {
    accel *= D_800D1550;
    decel *= D_800D1550;
    if (D_800CC27C != 0 && actor->unk28 < 5.0f && actor->interaction_state != 0x1E) {
        accel = 0.0f;
    }
    if (actor->interaction_state == 1) {
        if (actor->in_water != 0) {
            decel *= 0.400000006f;
            accel = 2.0f;
        }
        if (actor->unkA8 != 0) {
            decel *= 0.25f;
        }
        if (actor->unk81 != 0) {
            if (actor->unk81 & 0x40) {
                if (actor->unk81 & 0x20) {
                    decel = 0.0f;
                } else {
                    decel = 1.299999952f;
                }
            } else {
                decel *= 0.349999994f;
                if (turnDiff >= 46) {
                    accel *= 0.5f;
                }
                if (turnDiff >= 91) {
                    targetSpeed *= 0.5f;
                }
            }
        }
        if (actor->unkAE != 0) {
            decel *= 0.200000003f;
        }
    }
    if (targetSpeed < 0.0f) {
        if (0.0f < actor->xz_velocity) {
            decel += accel;
        } else {
            decel = accel;
        }
    } else if (actor->xz_velocity < 0.0f) {
        accel += decel;
    }
    if (targetSpeed < actor->xz_velocity) {
        actor->xz_velocity -= decel;
        if (actor->xz_velocity < targetSpeed) {
            actor->xz_velocity = targetSpeed;
        }
    } else {
        if ((targetSpeed - 1.0f) < actor->xz_velocity) {
            accel *= 0.009999999776f;
        }
        actor->xz_velocity += accel;
        if (targetSpeed < actor->xz_velocity) {
            actor->xz_velocity = targetSpeed;
        }
    }
}

/* Analog stick deflection of a controller pad (unk2/unk3 = stick x/y), scaled and capped at 35. */
f32 func_1505A5CC(struct49 *pad) {
    f32 x, y;
    f32 mag;

    mag = pad->unk2; // matching: staging x through mag steers IDO's register choice
    x = mag;
    y = pad->unk3;

    x = sqrtf(x * x + y * y) * 0.6000000238f;
    mag = x;

    if (mag > 35.0f) {
        mag = 35.0f;
    }

    return mag;
}

/* Binary angle (0x10000 = full turn) of the direction (x, z); the third argument is unused. */
u16 func_1505A630(f32 x, f32 z, s32 unused) {
    return ((u16) (u32) (func_150484A0(-x, z) * 10430.37793f)) + 16384;
}

/* Horizontal (xz) distance between two actors. */
f32 func_1505A6F8(Actor *a, Actor *b) {
    f32 x, z;

    x = a->x_position - b->x_position;
    x *= x;
    z = a->z_position - b->z_position;
    z *= z;
    return sqrtf(x + z);
}

/* 3D distance between two actors. */
f32 func_1505A72C(Actor *a, Actor *b) {
    f32 x, y, z;

    x = a->x_position - b->x_position;
    x *= x;
    z = a->z_position - b->z_position;
    z *= z;
    y = a->y_position - b->y_position;
    y *= y;
    return sqrtf(x + z + y);
}

/* Applies gravity: in the air it integrates in 0.1 steps; on the ground in a single step. Fall speed is capped at 500. */
void func_1505A770(Actor *actor) {
    f32 velocity;
    f32 decrement;
    s32 count;
    s32 i;

    if ((actor->unk28 != 0.0f) || (velocity = actor->y_velocity, velocity > 0.0f)) {
        decrement = actor->gravity * 0.1f;
        count = (s32)(D_800D1550 / 0.1f);
        for (i = 0; i < count; i++) {
            actor->y_velocity -= decrement;
            actor->y_position += actor->y_velocity * 0.05f;
        }
        if (actor->y_velocity < -500.0f) {
            actor->y_velocity = -500.0f;
        }
    } else {
        actor->y_velocity = velocity - (actor->gravity * D_800D1550);
        velocity = actor->y_velocity; actor->y_position += velocity * D_800D1550 * 0.5f; // matching: one line (as1 tie-break)
        if (velocity < -500.0f) {
            actor->y_velocity = -500.0f;
        }
    }
}
// a big one, but could be ok
void func_15055C88(Actor *);
void func_1513783C(Actor *, u8, s32);
void func_1507CD64(Actor *arg0, s32 arg1);
void func_15060A9C(s32 arg0, Actor *arg1);

/* Lands the player on floorY: water entry, fall damage/death, landing animations and bounce. */
void func_1505A9AC(Actor *actor, f32 floorY) {
    f32 dist;
    f32 deathFall;
    u8 bounce;
    u8 keepLandFlags;
    s32 health;
    s32 floorType;
    u8 landState;
    s32 killed;
    s32 anim;
    s32 flags;
    f32 speed;
    s32 hurt;
    u8 found;
    u8 turnGap;

    bounce = 0;
    keepLandFlags = 1;
    dist = actor->y_position - floorY;
    if (actor->unk31C->unk4F != 0) {
        if (dist < 0.0f) {
            actor->unk1CC = floorY;
            dist = 0.0f;
        }
        actor->unk28 = dist;
        dist = actor->y_position - (actor->unk118 - 34.0f);
        if (dist < 70.0f && actor->y_velocity < 18.0f) {
            if (actor->y_velocity > -25.0f) {
                actor->unk31C->unk4F = 2;
            }
            if (dist < 0.0f) {
                if (actor->y_velocity < -12.0f) {
                    actor->unk81 = 0;
                    if (actor->y_velocity < -25.0f) {
                        actor->xz_velocity *= 0.400000006f;
                        actor->y_velocity = -30.0f;
                    }
                    actor->y_velocity *= 0.8500000238f;
                }
                actor->gravity = -1.899999976f;
            } else {
                if ((actor->y_velocity > 10.0f) && (actor->y_velocity < 25.0f)) {
                    actor->y_velocity = 10.0f;
                }
                actor->gravity = 2.200000048f;
            }
        }
    } else if (actor->unk65 != 0) {
        if (dist < 0.0f) {
            actor->unk1CC = floorY;
            dist = 0.0f;
            actor->y_position = floorY;
        }
        actor->unk28 = dist;
    } else {
        if (dist < 0.0f) {
            f32 hurtFall;
            
            landState = 0;
            if (actor->in_water == 0) {
                *((u8 *)actor + 0x137) = 0;
                actor->gravity = 5.0f;
            }
            if (actor->in_water == 0) {
                actor->unk31C->unk55 = 4;
            }
            if (actor->unk28 > 0.0f) {
                actor->unk31C->unk51 = 1;
                actor->unk31C->unk31 = 0;
            }
            actor->unk100 &= 0xFD;
            if ((((u32)actor->unk184 >> 12) & 0xF) == 1) {
                if (D_800BE9F0 == 0x14) {
                    func_1513783C(actor, 0xFF, 1);
                    func_1507CD64(actor, 8);
                    return;
                }
                func_15055C88(actor);
                return;
            }
            hurtFall = 600.0f;
            if (D_800BE9F0 == 0x43) {
                deathFall = 1000.0f;
            } else {
                deathFall = hurtFall * 3.0f;
            }
            if ((actor->unk1CC - actor->y_position) > hurtFall && actor->unk31C->unk120 == 0 &&
                actor->unk31C->unk78 != 1) {
                speed = 1.299999952f;
                do { actor->unk31C->unk95 = 0; } while (0); // matching: the wrappers steer FP register choice
                do { actor->unk31C->unk51 = 2; } while (0);
                killed = 0;
                flags = 0;
                hurt = 0;
                anim = 0x30;
                if (D_800BE9F0 != 0x29) {
                    floorType = actor->unk184;
                    if (((((u32)floorType >> 12) & 0xF) == 2) || ((floorType == 0x13) & 0x1F)) {
                        anim = 0xC8;
                        func_15060B70(0x1B8, actor);
                        speed = 1.0f;
                    } else {
                        health = actor->health;
                        if (health != 0) {
                            if (actor->unk31C->unk78 == 0x21) {
                                flags = 1;
                            }
                            if (actor->id == 0x75) {
                                flags |= 2;
                            }
                        }
                        if ((actor->unk1CC < 90000.0f) && (deathFall < (actor->unk1CC - actor->y_position))) {
                            if (!(flags & 2)) {
                                actor->health = 0;
                                if ((floorType & 0x1F) != 0xE) {
                                    func_1513783C(actor, 0xFF, 1);
                                    killed = 1;
                                }
                            } else {
                                hurt = 1;
                                if (health != 0) {
                                    actor->health = health - 1;
                                }
                            }
                        }
                        if (flags == 0) {
                            health = actor->health;
                            actor->stunned = 8;
                            if (health != 0) {
                                actor->health = health - 1;
                            }
                        }
                        if (flags & 1) {
                            speed = 1.0f;
                            anim = 0x19D;
                        } else if (flags & 2) {
                            if (hurt) {
                                anim = 0x27;
                            } else {
                                anim = 9;
                            }
                        } else if (actor->health != 0) {
                            anim = 0x27;
                        }
                        if ((anim == 0x27) && (actor->unk31C->unk78 == 0x25)) {
                            anim = 0x1AA;
                        }
                    }
                }
                if (killed == 0) {
                    func_1505E650(actor, anim, speed, 2.0f, 6.0f, 0.0f, 0);
                }
                actor->stunned = 0;
                actor->unk83 = 0xFF;
                actor->disable_run = 0xFE;
                if (flags != 0) {
                    actor->unk228 = 10;
                } else {
                    actor->unk228 = 30;
                }
                actor->unk28 = 0.0f;
                actor->unk76 = actor->unk7A;
                actor->unk100 &= 0x8B;
                actor->y_position = floorY;
                actor->unkA9 = 0;
                actor->y_velocity = -4.0f;
                actor->unk1CC = floorY;
                if (killed == 0) {
                    func_15174690(D_800C3E78, 0, 0x1F, 0, 0x199, 4, 0xAA, 0xFF, 0);
                } else {
                    func_1507CD64(actor, 8);
                }
                return;
            }
            actor->unk1CC = floorY;
            if (actor->in_water != 0) {
                actor->y_position = floorY;
                actor->unk28 = 0.0f;
                return;
            }
            if (actor->stunned == 0) {
                if ((actor->unk7F == 0) && (actor->xz_velocity < 9.0f)) {
                    actor->unk76 = actor->unk7A;
                }
                turnGap = (actor->unk76 - actor->unk7A) >> 8;
                if (turnGap > 0x80) {
                    turnGap = -turnGap;
                }
                if (!actor->unk7F && (turnGap < 0x50)) {
                    actor->unk7F = 10;
                }
                if (actor->unkA9 == 2) {
                    Actor *other;
                    found = 0;
                    if (actor->objectNum != 0) {
                        other = &D_800CC2D0[actor->objectNum - 1];
                        if (other->unk24C != 0) {
                            found = 1;
                            other->unk218 = 0;
                            other->unk232 = other->unk24C;
                        }
                    }
                    actor->xz_velocity = 0.0f;
                    actor->unk83 = 0xFF;
                    landState = 0xFF;
                    func_1505E650(actor, 0x4B, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    if (D_800C3E78 < 4) {
                        actor->unk31C->unk57 = 1;
                    }
                    if (found == 0) {
                        func_15060A9C(0x19, actor);
                    }
                } else {
                    s32 landAnim;
                    landAnim = 0;
                    if ((s32)actor->unk28 > 0) {
                        ActorCtl *movement = actor->unk31C;
                        if ((movement->unk11A != 1) && (movement->unk11A != 3) && (movement->unk4E == 0)) {
                            if ((actor->unk100 & 0x10) || (!(actor->unk100 & 8) && (((actor->unk44 == 0.0f) && (actor->unkA8 == 0)) || (actor->unk100 & 4)))) {
                                if (actor->unk100 & 0x10) {
                                    actor->unk83 = 8;
                                    if (!(actor->unk100 & 0x40)) {
                                        actor->xz_velocity = 0.0f;
                                        actor->unk83 = 0xD;
                                        landState = 5;
                                    }
                                } else if (actor->unk100 & 4) {
                                    actor->xz_velocity *= 0.6999999881f;
                                    actor->unk83 = 0x14;
                                } else {
                                    actor->xz_velocity = 0.0f;
                                    actor->unk83 = 0x10;
                                }
                                if (actor->unk31C->unk78 == 0x21) {
                                    landAnim = 0x19D;
                                } else if (actor->unk31C->unk78 == 0x25) {
                                    landAnim = 0xD8;
                                } else {
                                    landAnim = 0x36;
                                }
                            }
                        }
                    }
                    if ((actor->unk84.uh == 0x3B) || (actor->unk84.uh == 0x3C)) {
                        u32 surface;
                        surface = actor->unk184 & 0x1F;
                        landState = 0xFE;
                        keepLandFlags = 0;
                        actor->unk83 = 0xFF;
                        if ((surface == 0xA) || (surface == 0xB) || (surface == 0xC) || (surface == 0xF) || (surface == 0x13)) {
                            landAnim = 0xC8;
                            func_15060B70(0x1B8, actor);
                        } else if ((surface == 1) || (surface == 2) || (surface == 7) || (surface == 0x11)) {
                            landAnim = 0xC8;
                            func_15060B70(0xA7, actor);
                        } else {
                            landAnim = 0x27;
                            func_15060B70(0xA7, actor);
                        }
                    }
                    if ((landAnim != 0) && !(actor->unk100 & 0x20)) {
                        func_1505E650(actor, landAnim, 1.0f, 2.0f, 0.0f, 0.0f, 0);
                    }
                    if (!(actor->unk100 & 0x80)) {
                        actor->unk100 = 1;
                    }
                    if (keepLandFlags == 0) {
                        actor->unk100 = 0x90;
                    }
                }
                if ((actor->unk28 > 0.0f) && !(actor->unk100 & 8)) {
                    if (D_800CC27C != 0) {
                        if (actor->xz_velocity > 20.0f) {
                            actor->xz_velocity = 20.0f;
                        }
                    } else if (*((u8 *)actor + 0x110) == 0) {
                        actor->unk164 = 0.0f;
                        actor->unk168 = 0.0f;
                    }
                    actor->disable_run = landState;
                    if ((actor->unk31C->unk4E == 1) && (actor->y_velocity < -10.0f)) {
                        bounce = 2;
                    }
                }
                actor->unkA9 = 0;
            }
            if (!(actor->unk100 & 8)) {
                dist = 0.0f;
            } else {
                dist = 0.1000000015f;
            }
            actor->y_position = floorY;
            if ((actor->stunned == 0) && !(bounce & 2)) {
                actor->y_velocity = -4.0f;
            } else {
                if (actor->unkF4 & 0x80) {
                    actor->y_velocity = -20.0f;
                } else if (actor->y_velocity > -10.0f) {
                    actor->y_velocity = 0.0f;
                } else {
                    actor->y_velocity = actor->y_velocity * -0.5f;
                    if (actor->y_velocity > 28.0f) {
                        actor->y_velocity = 28.0f;
                    }
                }
                if (actor->unk109 != 0) {
                    actor->xz_velocity *= 0.6000000238f;
                }
            }
            actor->unk81 = 0;
            if (actor->unk7F < 0xFA) {
                actor->unk7F++;
            }
            actor->unk100 &= 0x9F;
        } else if ((dist < 1.0f) && (actor->unk28 > 0.0f)) {
            dist = 1.0f;
        }
        if (dist > 20.0f) {
            actor->unk7F = 0;
        }
        if (actor->in_water != 0) {
            actor->unk1CC = actor->y_position;
        }
        actor->unk28 = dist;
    }
}

/* Lands a non-player actor on floorY: fall damage, landing bounce/sound, and the fall-off reaction (unk252). */
void func_1505B5F8(Actor *actor, f32 floorY) {
    f32 prevHeight;
    f32 y;
    f32 factor;
    f32 gravity;
    s32 animKind;
    s32 health;
    s32 step;
    s32 sound;
    SpawnRecord *owner;

    prevHeight = actor->unk28;
    if (!(actor->unkF8 & 0x8000)) {
        if (actor->unk252 != 0 && (f32)(actor->unk253 * 8) < actor->unk1CC - actor->y_position &&
            actor->stunned == 0) {
            animKind = actor->unk252;
            actor->stunned = 0xFE;
            actor->unk105 = 0;
            actor->unk106 = func_1505E7CC(animKind, actor);
            actor->unk84.uh = 0xFF;
            func_1505E874(D_800C3E78, actor);
            actor->y_position = actor->old_y_position;
        }
        y = actor->y_position;
        actor->unk28 = y - floorY;
        if (y <= floorY) {
            actor->unkF4 |= 0x8000;
            step = ((u8 *)D_800B0DF0)[0x13];
            if (step != 0 && prevHeight != 0.0f) {
                func_15174690(D_800C3E78, 0, 0x1000 / step, 0, 0x199, 4, 0xAA, 0xFF, 0);
            }
            actor->y_position = floorY;
            actor->unk28 = 0.0f;
            if (actor->stunned != 0 || !(actor->unkF8 & 0x10000)) {
                if (actor->interaction_state == 1) {
                    if (actor->y_velocity < -6.0f) {
                        actor->y_velocity = -6.0f;
                    }
                } else {
                    if (actor->unk1CC - actor->y_position > 450.0f) {
                        health = actor->health;
                        if (health > 0) {
                            owner = actor->spawn;
                            if (owner != NULL && !(owner->unkE & 0x80)) {
                                actor->health = health - 1;
                            }
                        }
                    }
                    actor->unk1CC = floorY;
                    if (actor->stunned == 0 && (gravity = actor->gravity) > 0.5f) {
                        if ((actor->unkF8 & 0x800) && prevHeight != 0.0f &&
                            actor->y_velocity < -4.0f * gravity * D_800D1550) {
                            sound = actor->unk2CC;
                            factor = (f32)actor->unk2CB * 0.009999999776f;
                            actor->y_velocity = (0.0f - actor->y_velocity) * factor;
                            if (sound != 0 && actor->y_velocity > 5.0f) {
                                D_800D1580 = sound;
                                func_1506E5FC();
                            }
                        } else if (actor->interaction_state != 7) {
                            actor->y_velocity = -4.0f * D_800D1550;
                        }
                    } else {
                        if (actor->unkF4 & 0x80) {
                            actor->y_velocity = -4.0f;
                        } else if (prevHeight == 0.0f ||
                                   actor->y_velocity > -1.5f * actor->gravity * D_800D1550) {
                            actor->y_velocity = 0.0f;
                        } else {
                            factor = (f32)actor->unk2CB * 0.009999999776f;
                            actor->y_velocity = (0.0f - actor->y_velocity) * factor;
                        }
                        actor->xz_velocity *= 0.8999999762f;
                    }
                }
            }
        }
    }
}
/* One hit-reaction entry (0x28 bytes): what happens to an actor hit by an attack. */
typedef struct Entry1505C1E4 {
    /* 0x00 */ u8 unk0;  /* bit 0x80: this entry's reaction wins; & 0x7F: reaction anim kind */
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ s32 unk4; /* sound to play */
    /* 0x08 */ u8 unk8;  /* handler index into D_80086694 */
    /* 0x09 */ u8 unk9;  /* hit-volume angle offset (high byte) / half width */
    /* 0x0A */ u8 unkA;  /* hit-volume distance / half depth */
    /* 0x0B */ u8 padB;
    /* 0x0C */ s16 unkC; /* hit-volume height offset */
    /* 0x0E */ u8 unkE;  /* hit-volume radius / side offset */
    /* 0x0F */ u8 unkF;  /* 0x40: keep heading, 0x20: sets unk10B bit 2; masked: damage */
    /* 0x10 */ u8 unk10; /* added to the attacker's heading (high byte) */
    /* 0x11 */ u8 pad11[3];
    /* 0x14 */ f32 xzVelocity;
    /* 0x18 */ f32 yVelocity;
    /* 0x1C */ f32 gravity;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ u8 unk22; /* 0x80: sets immune to & 0x3F */
    /* 0x23 */ u8 pad23;
    /* 0x24 */ u16 unk24; /* flags */
    /* 0x26 */ u8 pad26[2];
} Entry1505C1E4;

extern f32 D_800CC270, D_800CC274, D_800CC278;
extern u8 D_800BE616;
extern s16 D_800D1582;
f32 func_1506AD30(Actor *, f32, s32, s32);
void func_151AE264(Actor *);
void func_151E5FF4(void *, Actor *, u8);
void func_15169070(s32, s32, void *, s32);

/*
 * Applies a hit to `victim`: base[index] holds the attack's defaults and hit[index] the reaction chosen
 * for this victim. Sets knockback, damage and the reaction animation, and releases anything it carried.
 */
void func_1505B9C4(Actor *attacker, Actor *victim, Entry1505C1E4 *base, Entry1505C1E4 *hit, u8 index,
                   s32 victimSlot, u8 damageMask) {
    u8 reaction;
    s32 slot;
    Actor *savedActor;
    s32 savedIdx;
    u8 tmp;
    s32 v;
    ActorCtl *movement;
    Actor *other;
    struct S { void *a; u8 b; } msg;

    if (hit[index].unk0 & 0x80) {
        reaction = hit[index].unk0 & 0x7F;
    } else {
        reaction = base[index].unk0 & 0x7F;
    }
    victim->unk10B = 0;
    if ((base[index].unk24 & 1) && (victim->stunned != 0)) {
        damageMask = 0;
    }
    if (!(hit[index].unkF & 0x40)) {
        victim->unk76 = attacker->unk76 + (hit[index].unk10 << 8);
        if (base[index].unk24 & 0x10) {
            victim->unk76 = func_1505A630(victim->x_position - attacker->x_position,
                                          attacker->z_position - victim->z_position, 0);
        }
        if (attacker->id == 0x44) {
            victim->unk76 = 0;
        }
    }
    if ((base[index].unkF | hit[index].unkF) & 0x20) {
        victim->unk10B |= 2;
    }
    victim->unk3A = 0;
    victim->unk109 = hit[index].unk20;
    victim->y_velocity = hit[index].yVelocity;
    victim->xz_velocity = hit[index].xzVelocity;
    victim->gravity = hit[index].gravity;
    victim->unk100 &= 0xD6;
    v = hit[index].unkF & damageMask;
    if (v < victim->health) {
        victim->health -= v;
    } else {
        victim->health = 0;
    }
    if (victim->interaction_state == 1) {
        {
            extern s32 D_800ZZZZ; // matching: block-scope declaration steers IDO register allocation
            savedActor = D_800D154C;
            savedIdx = D_800C3E78;
            D_800D154C = victim;
            D_800C3E78 = (victim - D_800CC2D0);
            movement = D_800CC2D0[D_800C3E78].unk31C;
            if (movement->unk4E != 2) {
                movement->unk4E = 0;
            }
        }
        func_1506AD30(victim, 800.0f, 1, savedIdx);
        D_800D154C = savedActor;
        D_800C3E78 = savedIdx;
        victim->unkF8 &= 0xFEFF7FFF;
        victim->unk31C->unk95 = 0;
        victim->unk31C->unk6B = 0;
        if (victim->unk31C->ropeId != 0) {
            func_151AE264(victim);
        }
        victim->unk31C->unk54 = 0;
        tmp = victim->unk65;
        victim->unk137 = 0;
        if (tmp != 0) {
            extern s32 D_800ZZZZ; // matching: block-scope declaration steers IDO register allocation
            v = tmp - 1;
            other = &D_800CC2D0[(u8)v];

            if (((0x8A != other->id) && (0x23 != other->id)) || (victim->health == 0) ||
                ((D_800BE616 != 0) && (victim->y_velocity > 30.0f))) {
                if (v) {} // matching: extra use of v before it is reused
                v = 0;
                if (0x8A == other->id) {
                    v = 1;
                }
                if (other->id == 0x23) {
                    v = 0xA;
                }
                if (other->id == 0xC) {
                    v = 0xF;
                }
                if (v != 0) {
                    other->unk232 = v;
                    other->unk218 = NULL;
                }
                other->unk2FC = 0;
                other->unk13C = 0;
                victim->unk239 = 0;
                victim->unk65 = 0;
            }
        }
    }
    v = hit[index].unk8;
    if (v == 0) {
        v = base[index].unk8;
    }
    if (reaction != 0x27) {
        D_80086694[v](attacker, victim, &reaction);
    }
    if (D_800BE616 != 0) {
        tmp = reaction;
        v = attacker->unk124;
        other = v != 0 ? &D_800CC2D0[v - 1] : attacker;
        func_151E5FF4(other, victim, tmp);
    }
    {
        extern s32 D_800ZZZZ; // matching: block-scope declaration steers IDO register allocation
        msg.a = attacker;
        msg.b = attacker->unique_id;
        func_15169070(0, 0x68, (void *)(s32)&msg, 2);
        msg.a = victim;
        msg.b = victim->unique_id;
        func_15169070(0, 0x68, (void *)(s32)&msg, 2);
    }
    if (base[index].unk4 != 0) {
        D_800D1580 = base[index].unk4;
        if (func_1506E46C(victim, &D_800D1580, 0) != 0) {
            func_10010630(D_800D1582, victim, 0x6D60, 0x12C, 0x708);
        }
    } else {
        if (hit[index].unk4 != 0) {
            D_800D1580 = hit[index].unk4;
            if (func_1506E46C(victim, &D_800D1580, 0) != 0) {
                func_10010630(D_800D1582, victim, 0x6D60, 0x12C, 0x708);
            }
        }
    }
    attacker->unk107 = 60;
    if ((D_800BE616 == 0) || (victim->interaction_state != 1)) {
        victim->unk21C = 0;
        if (base[index].unk24 & 0x100) {
            victim->unk218 = NULL;
        }
    }
    slot = victimSlot - 1;
    victim->unkD0 = 0;
    v = attacker->unk124;
    if (v) {
        victim->pad10A = v + 0x7F;
    } else {
        victim->pad10A = D_800C3E78 | 0x80;
    }
    attacker->pad108 = slot | 0x80;
    if (reaction != 0x27) {
        if ((base[index].unk24 & 0x40) && (victim->interaction_state == 1)) {
            D_800CC2D0[slot].unk31C->unk46 = 8000;
        } else {
            v = victim->unk23B;
            if (v != 0) {
                reaction = v;
            }
            victim->stunned = 0xFE;
            victim->unk105 = 0;
            victim->unk106 = func_1505E7CC(reaction, victim);
            victim->unk84.uh = 0xFFFF;
            func_1505E874(slot, victim);
            if (base[index].unk24 & 0x200) {
                victim->unk10B |= 0x20;
            }
            if (victim->y_velocity > 0.0f) {
                victim->unk28 += 0.1000000015f;
            }
            if (base[index].unk24 & 8) {
                victim->unk10B |= 0x40;
                D_800CC270 = attacker->x_position;
                D_800CC274 = attacker->z_position;
                D_800CC278 = attacker->y_position;
            }
        }
        if (base[index].unk24 & 2) {
            victim->unk10B |= 0x80;
        }
    }
    if ((base[index].unk24 & 0x400) || (hit[index].unk24 & 0x400)) {
        victim->unk10B |= 4;
    }
    v = base[index].unk22;
    if (v & 0x80) {
        victim->immune = v & 0x3F;
    }
}

/* How far (binary angle, absolute) the attacker is from the victim's facing direction. */
s16 func_1505C140(Actor *attacker, Actor *victim) {
    s16 ret = func_1505A630(attacker->x_position - victim->x_position, victim->z_position - attacker->z_position, 0);
    ret -= victim->unk7A;

    if (ret < 0) {
        ret = -ret;
    }
    return ret;
}

/* The actor type's hit-reaction table (stored before its animation table), or the default one. */
Entry1505C1E4 *func_1505C1A4(Actor *actor) {
    Entry1505C1E4 **slot;
    Entry1505C1E4 *ret;

    if ((actor->id != 0xFF) && (D_800D1588[actor->id] != 0)) {
        slot = (Entry1505C1E4 **)D_800D1588[actor->id] - 3; // matching: the address is formed before the load
        ret = *slot;
    } else {
        ret = (Entry1505C1E4 *)&D_8009A9F8;
    }
    return ret;
}

struct Entry1505C1E4;
struct Entry1505C1E4;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
} Rule1505C1E4;

extern u8 D_8009A6A0[];
extern Rule1505C1E4 *D_80086AA0[];
s32 func_1506E46C(Actor *arg0, s32 *arg1, s32 arg2);
void func_15060B04(s32 arg0, Actor *arg1, s32 arg2);

/*
 * Resolves a hit on `victim` through the victim's reaction rules (unk239): may redirect to another
 * table entry, cancel the hit or handle it specially; otherwise applies it with func_1505B9C4.
 * Returns hitMask with the victim's bit set when the hit landed.
 */
s32 func_1505C1E4(Actor *attacker, Actor *victim, Entry1505C1E4 *table, s32 index, s32 victimSlot, s32 hitMask, s32 damageMask) {
    s32 i;
    u8 done;
    u8 kind;
    u8 count;
    u8 flags;
    Entry1505C1E4 *entry;
    Rule1505C1E4 *rules;
    s32 angle;
    u8 type;
    u8 idx;

    done = FALSE;
    kind = table[index].unk0 & 0x7F;
    entry = table;
    if (((u8)victim->unk239 & 0x7F) != 0) {
        count = D_8009A6A0[((u8)victim->unk239 & 0x7F)];
        rules = D_80086AA0[((u8)victim->unk239 & 0x7F)];
        for (i = 0; i < count; i++) {
            type = rules[i].unk2;
            if (((type & 0x80) == 0 && kind == rules[i].unk1) || ((type & 0x80) != 0 && kind != rules[i].unk1)) {
                type &= 0x7F;
                if (rules[i].unk0 == 1) {
                    if (func_1505C140(attacker, victim) > 0x4000) {
                        type = 0xFF;
                    }
                }
                if (rules[i].unk0 == 2) {
                    if (func_1505C140(attacker, victim) < 0x4000) {
                        type = 0xFF;
                    }
                }
                if (rules[i].unk0 == 3 && victim->stunned == 0) {
                    type = 0xFF;
                }
                if (rules[i].unk0 == 4 && victim->stunned != 0) {
                    type = 0xFF;
                }
                if (type < 2) {
                    angle = func_1505A630(attacker->x_position - victim->x_position, victim->z_position - attacker->z_position, 0) - victim->unk7A + 0x4000;
                    if ((u16)(angle >> 15) == type) {
                        index = rules[i].unk3;
                    }
                    break;
                }
                if (type == 2) {
                    index = rules[i].unk3;
                    break;
                }
                if (type == 3) {
                    done = TRUE;
                    break;
                }
                if (type == 4) {
                    if (rules[i].unk3 != 0xFF) {
                        index = rules[i].unk3;
                    }
                    damageMask = 0;
                    break;
                }
                if (type == 5) {
                    u8 next;
                    next = rules[i].unk3;
                    entry = (Entry1505C1E4 *)&D_8009A6D8[next - index];
                    break;
                }
                if (type == 6) {
                    u8 next;
                    next = rules[i].unk3;
                    damageMask = 0;
                    entry = (Entry1505C1E4 *)&D_8009A6D8[next - index];
                    break;
                }
                if (type == 7) {
                    if ((table[index].unkF & damageMask) >= victim->health) {
                        idx = rules[i].unk3;
                        entry = (Entry1505C1E4 *)&D_8009A6D8[idx - index];
                        victim->health = 0;
                        break;
                    }
                } else if (type == 8) {
                    done = TRUE;
                    victim->unkF4 &= ~0x40040;
                    hitMask |= 1 << (victimSlot + 0x1F);
                    attacker->pad108 = victimSlot + 0x7F;
                    attacker->unk107 = 0x3C;
                    victim->events = NULL;
                    victim->unk232 = rules[i].unk3;
                    victim->unk218 = NULL;
                    victim->disable_run = 1;
                    victim->unkD0 = 0;
                    D_800D1580 = table[index].unk4;
                    if (D_800D1580 != 0) {
                        func_1506E5FC();
                    }
                    if (victim->interaction_state == 1) {
                        if ((victim->unk31C->unk4E == 2) && (victim->unk31C->unk84 == 0)) {
                            victim->unk31C->unk50 = 30;
                            victim->unk81 = 0;
                            victim->y_velocity = 50.0f;
                            victim->gravity = 2.0f;
                            break;
                        }
                    }
                    break;
                } else if (type == 9) {
                    break;
                } else if (type == 0xA) {
                    if (victim->unk84.uh == 0x283) {
                        victim->unk76 = attacker->unk7A;
                        victim->xz_velocity = 25.0f;
                        D_800D1580 = 0xFF0102E2;
                        func_1506E5FC();
                        attacker->unkD0 = 0;
                        attacker->unk232 = 0xD;
                        attacker->unk218 = NULL;
                        done = TRUE;
                    }
                    break;
                } else if (type == 0xB) {
                    if (victim->unk28 > 8.0f) {
                        done = TRUE;
                    }
                    break;
                } else if (type == 0xC) {
                    if (victim->unk28 == 0.0f) {
                        attacker->unk2EC = 100;
                        attacker->unkD0 = 0;
                    }
                    done = TRUE;
                    break;
                } else if (type == 0xD) {
                    s32 reactionBuf;
                    u8 handler;
                    s32 sound;
                    handler = table[index].unk8;
                    D_800D1580 = table[index].unk4;
                    if (D_800D1580 != 0) {
                        func_1506E5FC();
                    }
                    D_80086694[handler](attacker, victim, (u8 *)&reactionBuf);
                    if (victim->unkFC != 0) {
                        sound = victim->unkFC;
                        if (func_1506E46C(attacker, &sound, 1) != 0) {
                            func_15060B04(sound, victim, 0x5DC0);
                        }
                    }
                    hitMask |= 1 << (victimSlot + 0x1F);
                    done = TRUE;
                    break;
                }
            }
        }
    }
    if (done) {
        return hitMask;
    }
    i = victimSlot;
    func_1505B9C4(attacker, victim, table, entry, index, i, damageMask);
    hitMask |= 1 << (i + 0x1F);
    return hitMask;
}
s32 func_1505C7D8(Actor *arg0, s32 arg1);

extern s32 D_800B2CD0;
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, f32 *, s32, s32, f32);
void func_1507C3E0(Actor *, s16 *, s32, s32);
f32 func_15143E64(f32 *);

/* Tests the attacker's active attack (unkD0) against every other actor and applies the hits; returns the hit mask. */
s32 func_1505C7D8(Actor *attacker, s32 selfSlot) {
    s32 i;
    s32 slotEnd;
    s32 attackIdx;
    s32 ret;
    Actor *victim;
    Entry1505C1E4 *hits;
    f32 dz2;
    Entry1505C1E4 *hit;
    f32 x;
    f32 y;
    f32 z;
    f32 radius;
    f32 dx;
    f32 dz;
    f32 dy;
    f32 dist;
    f32 maxDist;
    f32 s;
    f32 c;
    f32 rx;
    f32 rz;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 h;
    f32 r;
    f32 t;
    f32 offZ;
    f32 height;
    f32 pad1; // matching: pad1..pad5 and padBC..padCC reproduce the stack layout
    f32 pad2;
    f32 pad3;
    f32 offZ2;
    f32 pad4;
    f32 pad5;
    Vec3f pos;
    f32 hitZ;
    f32 hitY;
    f32 hitX;
    f32 ray[3];
    f32 padCC;
    f32 hitDist;
    f32 padC4;
    f32 padC0;
    f32 padBC;
    s16 halfHeightRaw;

    selfSlot = (selfSlot & 0x7F) + 1;
    ret = 0;
    if (attacker->unk114 < 254.0f) {
        attacker->unk114 -= D_800D1550;
    }
    if (attacker->unk114 < 0.0f) {
        attacker->unkD0 = 0;
        return 0;
    }
    if (D_800C35EA == 1) {
        return 0;
    }
    attackIdx = attacker->unkD0 - 1;
    hits = func_1505C1A4(attacker);
    if (hits[attackIdx].unk24 & 0x80) {
        // matching: byte-offset form (not &hits[attackIdx]) keeps IDO's addu operand order
        hit = (Entry1505C1E4 *)((u8 *)hits + attackIdx * sizeof(Entry1505C1E4));
        y = attacker->y_position;
    } else {
        hit = (Entry1505C1E4 *)((u8 *)hits + attackIdx * sizeof(Entry1505C1E4));
        dist = (u32)hit->unkA;
        dist *= attacker->xz_scale / 0.125f;
        func_1505A184((hit->unk9 << 8) + attacker->unk7A, dist, 0.0f, &x, &z, &y);
        x += attacker->x_position;
        y += attacker->y_position;
        z += attacker->z_position;
        radius = (u32)hit->unkE;
        radius = attacker->xz_scale * 4.0f * radius;
    }
    dist = hit->unkC;
    y += dist * (attacker->y_scale / 0.25f);
    if (attacker->unkF8 & 0x100) {
        slotEnd = 4;
    } else {
        slotEnd = 26;
    }
    for (i = 1; i < slotEnd; i++) {
        victim = &D_800CC2D0[i - 1];
        if (victim->interaction_state == 0) {
            continue;
        }
        if (i == selfSlot || victim->health == 0 || victim->immune != 0) {
            continue;
        }
        hit = (Entry1505C1E4 *)((u8 *)hits + attackIdx * sizeof(Entry1505C1E4));
        if (i == attacker->unk124 && !(hit->unk24 & 0x1000)) {
            continue;
        }
        if (hit->unk24 & 0x80) {
            s16 sideOffset;

            dist = 2.0f;
            maxDist = 1.0f;
            dx = victim->x_position - attacker->x_position;
            dz = victim->z_position - attacker->z_position;
            if (victim->unkD8 != 0 || victim->unkDA != 0) {
                dx += func_1505D34C(victim->unk40, victim->unkDA, victim->unkD8, 1.0f, &offZ);
                dz += offZ;
            }
            rx = (attacker->unk40 - 90.0f) * 0.01745329238f;
            s = func_150AD780(-rx);
            c = func_150AD78C(-rx);
            rx = dx * s + dz * c;
            dz = -dx * c + dz * s;
            hx = hit->unk9 * 5;
            dz2 = hit->unkA * 5;
            sideOffset = (s8)hit->unkE * 5;
            dy = sideOffset * attacker->xz_scale;
            dx = rx - dy;
            hx *= attacker->xz_scale;
            dz2 *= attacker->xz_scale;
            dy = y - (victim->y_position + victim->unkD6);
            hx += victim->unkD2;
            dz2 += victim->unkD2;
            hy = victim->unkD4;
            hy += 25.0f;
            if (-hy < dy && dy < hy && -hx < dx && dx < hx && -dz2 < dz && dz < dz2) {
                dist = 0.0f;
            }
        } else {
            dx = x - victim->x_position;
            dz2 = z - victim->z_position;
            if (victim->unkD8 != 0 || victim->unkDA != 0) {
                dx -= func_1505D34C(victim->unk40, victim->unkDA, victim->unkD8, 1.0f, &offZ2);
                dz2 -= offZ2;
            }
            {
                typedef s32 T1505C7D8; // matching: block-scope declaration steers IDO's scheduling

                dy = y - (victim->y_position + victim->unkD6);
                r = victim->unkD2;
                h = victim->unkD4;
                if (h == 0.0f) {
                    continue;
                }
            }
            t = h / (radius + h);
            maxDist = (r + radius) * (r + radius);
            dy *= (r / h) * t + (1.0f - t);
            dist = dx * dx + dy * dy + dz2 * dz2;
        }
        if (dist < maxDist) {
            if ((hit->unk24 & 0x2000) && i != attacker->unk13D) {
                if (*(Vec3f **)((u8 *)attacker + 0x2F4) != NULL) {
                    pos = **(Vec3f **)((u8 *)attacker + 0x2F4);
                } else {
                    func_1507C3E0(attacker, &halfHeightRaw, 0, 0);
                    pos.x = attacker->x_position;
                    pos.y = attacker->y_position + (halfHeightRaw >> 1);
                    pos.z = attacker->z_position;
                }
                rx = victim->x_position;
                if (victim->unk31C != NULL) {
                    height = victim->unk31C->height * 0.5f;
                } else {
                    height = 20.0f;
                }
                t = height;
                t += victim->y_position;
                rz = victim->z_position;
                ray[0] = rx - pos.x;
                ray[1] = t - pos.y;
                ray[2] = rz - pos.z;
                if (func_150AC9C0(pos.x, pos.y, pos.z, ray[0], ray[1], ray[2], NULL, NULL, &hitX, &hitY, &hitZ, &hitDist, 0, 0, 0.0f) != 0 &&
                    hitDist < func_15143E64(ray)) {
                    continue;
                }
            }
            D_800CC2C0 = x;
            D_800CC2C4 = y;
            D_800CC2C8 = z;
            ret = func_1505C1E4(attacker, victim, hits, attackIdx, i, ret, 0xF);
            attackIdx = attacker->unkD0 - 1;
        }
    }
    if (ret != 0) {
        hit = (Entry1505C1E4 *)((u8 *)hits + attackIdx * sizeof(Entry1505C1E4));
        /* pad108 holds victim slot | 0x80, and D_800B2CD0 + 0x80 * 0x32C is D_800CC2D0 */
        if (((s32 (*)[0x32C / 4])&D_800B2CD0)[attacker->pad108][0] == 1) {
            if (attacker->unk254 != 0) {
                attacker->unk232 = attacker->unk254;
                attacker->unk218 = NULL;
            }
        }
        if (!(hit->unk24 & 4)) {
            attacker->unk21C = 0;
        }
        if (!(hit->unk24 & 0x800)) {
            attacker->unkD0 = 0;
        }
    }
    return ret;
}

/* Hits `victim` with a scripted attack from the dummy attacker D_800D121C; flags & 0xFF picks the entry. */
s32 func_1505D024(Actor *victim, s32 flags, u16 angle, s32 sourceSlot) {
    s32 pad;
    s32 ret;
    s32 slot;
    Entry1505C1E4 *table;

    table = (Entry1505C1E4 *)&D_8009A9F8;
    if (D_800C35EA == 1) {
        return 0;
    }
    if ((flags & 0x20000) && (victim->immune != 0)) {
        return 0;
    }
    if ((flags & 0x40000) && (victim->stunned != 0)) {
        return 0;
    }
    if (!(flags & 0x100000) && (victim->health == 0)) {
        return 0;
    }
    if (sourceSlot == -1) {
        D_800D1340 = 0;
    } else {
        D_800D1340 = sourceSlot + 1;
    }
    D_800D1292 = angle;
    D_800D1296 = angle;
    if (flags & 0x10000) {
        table = func_1505C1A4(D_800D154C);
    }

    slot = victim - D_800CC2D0;
    ret = 1 << slot;
    if (flags & 0x80000) {
        ret = func_1505C1E4(&D_800D121C, victim, table, flags & 0xFF, slot + 1, 0, 7);
    } else {
        func_1505B9C4(&D_800D121C, victim, table, table, flags & 0xFF, slot + 1, 7);
    }
    return ret;
}

/*
 * Runs attack attackId from point (x, y, z) through the dummy attacker D_800D121C. The D_800D12xx..D_800D15xx
 * globals written here are that dummy's Actor fields: position (0x14), unk124, unk13D, unk76, unk2F4
 * (line-of-sight origin), unk114 (timer), unkD0 (attack + 1) and the xz/y scales.
 */
void func_1505D1C4(f32 x, f32 y, f32 z, s32 attackId, s32 sourceSlot, u16 angle, s32 losSkipSlot, s32 losOrigin) {
    s32 pad;
    s32 savedIdx;

    savedIdx = D_800C3E78;
    func_1505F188(&D_800D121C);
    D_800D1230 = x;
    D_800D1234 = y;
    D_800D1238 = z;
    D_800D1340 = sourceSlot + 1;
    D_800D1359 = losSkipSlot;
    D_800D1292 = angle;
    D_800C3E78 = 25;
    D_800D1510 = losOrigin;
    D_800D1330 = 100.0f;
    D_800D12EC = (attackId & 0xFFFF) + 1;
    D_800D1368 = 0.5f;
    D_800D136C = 0.5f;
    func_1505C7D8(&D_800D121C, 25);
    D_800C3E78 = savedIdx;
}

/* Starts the knockback reaction D_8009A6D8[entryIdx] on the actor. */
void func_1505D2B8(Actor *actor, u8 entryIdx) {
    struct253 *entry;

    entry = &D_8009A6D8[entryIdx];
    actor->y_velocity = entry->unk18;
    actor->xz_velocity = entry->unk14;
    actor->gravity = entry->unk1C;
    actor->stunned = 0xFE;
    actor->unk105 = 0;
    actor->unk106 = func_1505E7CC(actor->unk10E & 0x7F, actor);
    actor->unk10E = 0xFF;
    func_1505E874(D_800C3E78, actor);
}

/* Rotates the offset (a, b) by angleDeg - 90 degrees (after scaling); returns x, writes z. */
f32 func_1505D34C(f32 angleDeg, f32 a, f32 b, f32 scale, f32 *outZ) {
    f32 sinA;
    s32 pad;
    f32 cosA;
    f32 rad;

    if (scale != 1.0f) {
        a = a * scale;
        b = b * scale;
    }
    rad = (angleDeg - 90.0f) * D_80099520; // matching: the 0.01745329238f literal allocates differently
    sinA = func_150AD780(rad);
    cosA = func_150AD78C(rad);
    *outZ = (-a * cosA) + (b * sinA);
    return (a * sinA) + b * cosA;
}

extern u8 D_800CC26C;
extern u8 D_800CC26D;

/* Pushes the actor out of `other` along (nx, ny, nz) unless the other one is lighter. */
void func_1505D408(Actor *actor, Actor *other, f32 nx, f32 ny, f32 nz, f32 distSq, f32 minDist, s32 otherSlot) {
    f32 dist;
    f32 invDist;
    f32 pushFactor;
    s32 temp_a2;
    s32 temp_t0;
    u8 actorWeight;
    u8 otherWeight;

    pushFactor = 0.400000006f;
    actorWeight = 0;
    otherWeight = 0;
    if (distSq < 1.0f) {
        distSq = 1.200000048f;
    }
    if ((nx == 0.0f) && (nz == 0.0f)) {
        nx = 1.0f;
    }
    D_800CC268 |= 1 << otherSlot;
    if (other->unkF8 & 0x10) {
        D_800CC26C = otherSlot;
        pushFactor = 0.8000000119f;
        if ((other->id == 0x61) && (actor->interaction_state == 1)) {
            goto end;
        }
    }
    if (actor->unkF8 & 1) {
        actorWeight = 1;
    }
    if (actor->unkF8 & 0x200) {
        actorWeight |= 2;
    }
    if (other->unkF8 & 1) {
        otherWeight = 1;
    }
    if (other->unkF8 & 0x200) {
        otherWeight |= 2;
    }
    if (otherWeight >= actorWeight) {
        if ((other->unkF8 & 0x400) && (other->unk28 == 0.0f)) {
            D_800CC26D = otherSlot + 100;
        }
        dist = sqrtf(distSq);
        invDist = 1.0f / dist;
        nx *= invDist;
        ny *= invDist;
        nz *= invDist;
        minDist -= dist;
        minDist *= pushFactor;
        actor->x_position += minDist * nx;
        actor->z_position += minDist * nz;
        actor->y_position += minDist * ny;
    }
end: // matching: a jump to the shared epilogue, not an early return
    ;
}
void func_1505D408(Actor *arg0, Actor *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, s32 arg7);

/* Pushes the actor out of an extra collision cylinder attached to `other` (offset in other's frame). */
void func_1505D5D0(Actor *actor, Actor *other, f32 x, f32 y, f32 z, f32 ratio, f32 radius, s32 otherSlot,
                   f32 extraRadius, f32 extraHeight, f32 extraY, f32 offsetA, f32 offsetB) {
    f32 offsetX;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 sumRadius;

    offsetX = func_1505D34C(other->unk40, offsetA, offsetB, other->xz_scale, &offsetB);
    extraRadius *= other->xz_scale;
    extraHeight *= other->y_scale;
    dx = x - (other->x_position + offsetX);
    dz = z - (other->z_position + offsetB);
    dy = y - (other->y_position + (extraY * other->y_scale));
    dy *= (ratio + (extraRadius / extraHeight)) * 0.5f;
    sumRadius = radius + extraRadius;
    distSq = ((dx * dx) + (dy * dy)) + (dz * dz);
    if (distSq < (sumRadius * sumRadius)) {
        func_1505D408(actor, other, dx, dy, dz, distSq, sumRadius, otherSlot);
    }
}
/* Resolves collisions between the actor's body cylinder (unkE4/unkE8/unkEC) and every other actor's. */
void func_1505D6F0(Actor *actor, s32 selfIdx) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 distSq;
    f32 sumRadius;
    f32 radius;
    f32 otherRadius;
    f32 height;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 count;
    Actor *other;
    ActorCtl *movement;

    D_800CC268 = 0;
    D_800CC26C = 0;
    D_800CC26D = 0;
    count = 25;
    if (((actor->interaction_state != 1) ||
         ((D_800CC2D0[selfIdx].unk31C->unk4E == 0) && (D_800CC2D0[selfIdx].unk31C->unk27 == 0))) &&
        (actor->unk13D < 100) && (actor->unkE4 != 0) && (actor->unk65 == 0) && !(actor->unkF8 & 0x01000000) &&
        !(actor->unkF8 & 0x4000)) {
        radius = actor->unkE4;
        height = actor->unkEC;
        x = actor->x_position;
        y = actor->y_position + (s16)actor->unkE8;
        z = actor->z_position;
        if (actor->unkF8 & 0x02000000) {
            count = 1;
        }
        for (i = 0; i < count; i++) {
            other = &D_800CC2D0[i];
            if (other->interaction_state == 0) {
                continue;
            }
            if (other->unkE4 < 2) {
                continue;
            }
            if ((other->unkF8 & 0x02000000) && (actor->unk127 == 0xFF)) {
                continue;
            }
            if (other->unkF8 & 0x01000000) {
                continue;
            }
            if (i == selfIdx) {
                continue;
            }
            if (other->unk65 == (selfIdx + 1)) {
                continue;
            }
            if (other->unk13D >= 100) {
                continue;
            }
            if (other->unk2FA == 0) {
                continue;
            }
            otherRadius = other->unkE4;
            sumRadius = radius + otherRadius;
            dx = x - other->x_position;
            dz = z - other->z_position;
            dy = y - (other->y_position + (s16)other->unkE8);
            dy *= (height + other->unkEC) * 0.5f;
            if ((D_800BE616 != 0) && (((dx * dx) + (dy * dy) + (dz * dz)) < 30000.0f) &&
                (actor->interaction_state == 1) && (other->interaction_state == 1) &&
                (actor->team != other->team)) {
                movement = actor->unk31C;
                if ((movement->unk78 == 0) && (other->unk31C->unk78 != 0)) {
                    movement->chased = 20;
                }
            }
            distSq = (dx * dx) + (dy * dy) + (dz * dz);
            if (distSq < (sumRadius * sumRadius)) {
                func_1505D408(actor, other, dx, dy, dz, distSq, sumRadius, i);
            }
            if (other->id == 0x34) {
                if (other->unk251 == 2) {
                    func_1505D5D0(actor, other, x, y, z, height, radius, i, 120.0f, 250.0f, 120.0f, -130.0f, 0.0f);
                } else {
                    func_1505D5D0(actor, other, x, y, z, height, radius, i, 130.0f, 140.0f, 250.0f, -50.0f, 0.0f);
                }
            }
        }
    }
}
f32 func_1505DF10(Actor *arg0, u8 arg1, u16 *arg2, f32 *arg3, f32 *arg4, s32 arg5);
s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, f32 *, s32, s32, f32);

/*
 * Picks the nearest visible living actor inside a cone around the actor's facing (halfCone wide,
 * pitch +-0x10 around `pitch`), skipping skipIdx. Returns its index (0xFF if none), angle in *outAngle.
 */
u8 func_1505DADC(Actor *actor, u16 *outAngle, u8 pitch, u8 skipIdx, u8 halfCone) {
    u8 bestStunned;
    u8 i;
    u16 angle;
    Actor *other;
    f32 best;
    f32 dist;
    f32 distXZ;
    f32 dy;
    s32 hitA[2];
    s32 hitB[5];
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 hitDist;
    u8 bestIdx;

    best = 10000000.0f;
    bestStunned = 0;
    bestIdx = 0xFF;
    if (skipIdx == 0xFE) {
        best = 40000.0f;
    }
    for (i = 0; i < 25; i++) {
        other = &D_800CC2D0[i];
        if ((other->interaction_state != 0) && (other->health != 0) && (i != (u8)(actor - D_800CC2D0)) &&
            (other->unkF8 & 0x40) && (i != skipIdx) && (other->unk2FA != 0)) {
            dist = func_1505DF10(actor, i, &angle, &distXZ, &dy, 0);
            if (((dist < best) || ((other->stunned == 0) && (bestStunned != 0))) &&
                ((u8)(((s32)(angle - actor->unk7A) >> 8) + halfCone) < (halfCone * 2)) &&
                ((u8)(((u8)(func_1505A630(sqrtf(distXZ), -dy, 0) >> 8) - pitch) + 0x10) < 0x20)) {
                if ((func_150AC9C0(actor->x_position, actor->y_position + 80.0f, actor->z_position,
                                   D_800CC2D0[i].x_position - actor->x_position,
                                   D_800CC2D0[i].y_position - actor->y_position,
                                   D_800CC2D0[i].z_position - actor->z_position, hitA, hitB, &hitX, &hitY, &hitZ,
                                   &hitDist, 0, 0, 0.0f) == 0) ||
                    !((hitDist = hitDist * hitDist) < dist)) {
                    bestStunned = other->stunned;
                    bestIdx = i;
                    best = dist;
                    *outAngle = angle;
                }
            }
        split: // matching: label splits the basic block like the original
            ;
        }
    }

    return bestIdx;
}
f32 func_1505DF10(Actor *arg0, u8 arg1, u16 *arg2, f32 *arg3, f32 *arg4, s32 arg5);

/* The first live actor that the actor would reach within `time` at its speed, inside a cone of its move angle; -1 if none. */
s8 func_1505DDA8(Actor *actor, s32 unused, u16 cone, f32 time) {
    u8 i;
    u16 angle;
    f32 distXZ;
    f32 dy;
    f32 range;

    for (i = 0; i <= D_80082FA0; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        range = actor->xz_velocity * time * 0.5f;
        if (func_1505DF10(actor, i, &angle, &distXZ, &dy, 0) < range * range) {
            if ((u16)(actor->unk76 - angle + cone / 2) < cone) {
                return i;
            }
        }
    }
    return -1;
}
/* Both callers pass a 6th argument (always 0) that the body never reads. */
f32 func_1505DF10(Actor *actor, u8 otherIdx, u16 *outAngle, f32 *outDistSqXZ, f32 *outDy, s32 unused) {
    Actor *other;
    f32 dx;
    f32 dy;
    f32 dz;

    other = &D_800CC2D0[otherIdx];
    dx = other->x_position - actor->x_position;
    dy = actor->y_position - other->y_position;
    dz = actor->z_position - other->z_position;
    *outDy = dy;
    *outAngle = func_1505A630(dx, dz, 0);
    dx = dx * dx;
    dy = dy * dy;
    dz = dz * dz;
    *outDistSqXZ = dx + dz;
    return (dx + dy) + dz;
}
extern u16 D_800C4ED0[];

/* Stops the actor's animation and clears its playback state. */
void func_1505DFDC(Actor *actor) {
    s32 id;
    AnimPlaybackState *state;
    u16 *p;

    actor->unk84.uh = 0xFFFF;
    state = (AnimPlaybackState *)actor->unk2D0;
    if (state != NULL) {
        id = actor->id;
        state->loaded = 0;
        bzero(state->header, sizeof(state->header) + sizeof(state->prevHeader));
        p = &D_800C4ED0[id];
        state->header[1] = *p + 1;
        state->prevHeader[1] = *p + 1;
        state->unk30 = 0;
        state->unk34 = 0;
    }
}

/* Saves the current animation as the previous one, to blend from. */
void func_1505E060(AnimPlaybackState *state) {
    state->prevAnim = state->anim;
    state->prevUnk8 = state->unk8;
    state->prevSpeed = state->speed;
    state->prevUnk20 = state->unk20;
    state->prevFrameCount = state->frameCount;
    state->prevUnk38 = state->unk38;
    state->prevLoaded = state->loaded;
    bcopy(state->header, state->prevHeader, sizeof(state->header));
}

s32 func_1502AF04(u8 *, s32, s32, s32);
s32 func_1502B110(u8 *, s32, s32, u32, ...);
extern s32 D_8008630C[];
extern u16 D_800993F8[];

/*
 * Starts animation `anim` (entry `animEntry`) on a playback state, or only updates its speed when it is
 * already playing. `actor` may be NULL for non-actor users; blendFrames >= 1 blends from the old one.
 */
void func_1505E0C4(Actor *actor, u16 *animEntry, AnimPlaybackState *state, s32 arg3, s32 restart, s32 anim,
                   s32 bank, f32 speed, f32 blendFrames, f32 arg9, f32 arg10, s32 flag8000) {
    s32 index;
    s32 changed;
    s32 assetId;
    s32 kind;
    s32 dataSize;
    s32 romAddr;
    s32 *info;
    s16 blend;
    s32 mask;
    u8 buf[0x2C];

    changed = FALSE;
    if (restart != 0) {
        changed = TRUE;
        blendFrames = 0.0f;
    } else if (anim != (state->anim & 0xFFFF7FFF)) {
        changed = TRUE;
    } else if (actor != NULL && (anim != actor->unk84.uh || actor->unk84.uh == 0xFFFF || actor->unk103 != 0)) {
        changed = TRUE;
    }
    if (changed != FALSE) {
        if (actor != NULL) {
            index = ((s32)actor - (s32)&D_800CC2D0) / 812;
            kind = 2;
            bank = actor->unk6;
        } else {
            kind = 10;
        }
        if (animEntry != NULL) {
            assetId = animEntry[0];
        } else {
            assetId = anim * 2;
        }
        if (actor != NULL) {
            actor->unk84.uh = anim;
            actor->animation_speed = speed;
            actor->events = *(struct TimedEvent **)(animEntry + 2);
            *(s8 *)&actor->unk138 = 0;
            actor->unk103 = 0;
            actor->lastEventTime = 0.0f;
            if (index < D_8008FD8C) {
                Actor *player = &D_800CC2D0[index];

                player->unk31C->idle.active = 0;
                player->unk31C->idle.timer = 0;
                player->unk31C->idle.holdFrames = 0;
            }
        }
        if (state->loaded != 0) {
            blend = state->blend;
            if (blendFrames >= 1.0f) {
                state->blendStep = 500 / (s16)blendFrames;
                if (state->blend > 0 && anim == (state->prevAnim & 0xFFFF7FFF)) {
                    state->blend = 0x3FF - state->blend;
                    blend = 0;
                } else if (blend < 600) {
                    state->blend = 0x3FF;
                }
            } else {
                state->blend = 0;
            }
            if (blend < 600) {
                state->prevAnim = state->anim;
                state->prevUnk8 = state->unk8;
                state->prevSpeed = state->speed;
                state->prevUnk20 = state->unk20;
                state->prevFrameCount = state->frameCount;
                state->prevUnk38 = state->unk38;
                state->prevLoaded = state->loaded;
                bcopy(state->header, state->prevHeader, sizeof(state->header));
            }
        }
        state->anim = anim;
        if (flag8000 != 0) {
            state->anim |= 0x8000;
        }
        state->unk8 = arg9;
        state->speed = speed;
        state->unk20 = arg10;
        if (animEntry != NULL) {
            state->unk0 = D_8008630C[animEntry[1]];
        } else {
            state->unk0 = 0;
        }
        if (actor != NULL && (u8 *)actor->unk58 != NULL) {
            info = (s32 *)func_1502AF04((u8 *)actor->unk58, (s32)buf, assetId, 2);
        } else {
            info = (s32 *)func_1502B110(NULL, 2, (s32)buf, 3, kind, bank, assetId);
        }
        if (info != NULL) {
            dataSize = info[1];
            romAddr = info[0];
            state->loaded = info[2];
        }
        {
            s32 size = (dataSize + 0xF) & ~0xF;

            if (dataSize == 0) {
                func_1505DFDC(actor);
                return;
            }
            if (size < 200) {
                func_1000480C(romAddr, state->header, size);
            } else {
                func_10004514(romAddr, state->header, size, 1);
            }
        }
        state->frameCount = (u32)state->header[3];
        if (state->header[3] == 0xFF && actor != NULL) {
            switch (actor->id) {
            case 0x92:
                state->frameCount = (u32)D_800993F8[actor->unk84.uh];
                break;
            case 0xB7:
            case 0xB8:
            case 0xB9:
                state->frameCount = 641.0f;
                break;
            }
        }
        mask = 0x303;
        if (actor != NULL) {
            actor->unk24E = 0;
            actor->unk1FC |= 2;
            actor->unk1FC &= ~1;
            if (actor->stunned == 0) {
                mask = 0x30B;
            }
            actor->unkF4 &= ~mask;
            if (actor->unkF4 & 0x20) {
                actor->unkF4 |= 2;
                actor->unkF4 &= ~0x20;
            }
            if (state->header[7] & 0x20) {
                actor->unk1FD = state->header[0xB];
            } else {
                actor->unk1FD = 0;
            }
        }
        if (state->header[5] == 0) {
            state->loaded = 1;
        }
    } else {
        if (actor != NULL) {
            actor->animation_speed = speed;
        }
        state->speed = speed;
        if (flag8000 != 0) {
            state->anim |= 0x8000;
        } else {
            state->anim &= 0xFFFF7FFF;
        }
        if (D_800C35EA == 1 && (state->anim & 0x8000)) {
            state->unk8 = arg9;
        }
    }
    if (actor != NULL && changed != FALSE) {
        func_150627D4(actor);
    }
}
/* Starts animation `anim` of the actor's type (see func_1505E0C4); ignored while a cutscene holds the actor. */
void func_1505E650(Actor *actor, u16 anim, f32 speed, f32 blend, f32 arg4, f32 arg5, s32 flag8000) {
    u16 (*animTable)[4];
    s32 idx;
    u16 *entry;
    u8 *data;

    idx = actor->id;
    if ((D_800C3638 != 0) && ((D_800C3654[0] == 0) || (func_150229E4(actor) != 0))) {
        return;
    }
    if (actor->id == 0xFF) {
        return;
    }
    if ((u16)D_800C5A90[actor->id] == 0) {
        return;
    }
    data = (u8 *)actor->unk2D0;
    if (data == NULL) {
        return;
    }
    if ((u16)D_800C5A90[actor->id] != 0) {
        if (anim >= (u16)D_800C5A90[actor->id]) {
            return;
        }
    } else {
        return;
    }
    animTable = (u16 (*)[4])D_800D1588[idx];
    if (animTable == NULL) {
        func_1505DFDC(actor);
        return;
    }
    if (anim < (u16)D_800C5A90[idx]) {
        if (animTable[0][0] != 999) {
            entry = animTable[anim];
            animTable = NULL; // matching: dead store and do-while(0) shape the block like the original
            do {
                if (entry[0] != 999) {
                    if (entry[0] >= 30000) {
                        func_1505DFDC(actor);
                        return;
                    }
                    if (actor->unk2FF == 0) {
                        blend = 0.0f;
                    }
                    func_1505E0C4(actor, entry, (AnimPlaybackState *)data, 0, 0, anim, 0, speed, blend, arg4, arg5, flag8000);
                }
            } while (0);
        }
    }
}
/* Index of the actor type's reaction entry of the given kind (0 if there is none). */
u32 func_1505E7CC(s32 kind, Actor *actor) {
    s32 id;
    s32 *animTable;
    u32 count;
    Game83300AnimEntry *table;
    Game83300AnimEntry *entry;
    u32 i;

    id = actor->id;
    if (id == 0xFF) {
        return 0;
    }
    animTable = (s32 *)D_800D1588[id];
    if (animTable == NULL) {
        return 0;
    }
    count = animTable[-1];
    if (count == 0) {
        return 0;
    }
    count = count / sizeof(Game83300AnimEntry);
    table = (Game83300AnimEntry *)animTable[-2];
    if (table == NULL) {
        return 0;
    }
    if (count != 0) {
        /* matching: a pointer-stepped do-while; a for loop gets unrolled by IDO */
        i = 0;
        entry = table;
        do {
            if (kind == entry->kind) {
                return i;
            }
            i++;
            entry++;
        } while (i < count);
    }
    return 0;
}

s32 func_1506E46C(Actor *arg0, s32 *arg1, s32 arg2);
void func_15060A30(s32 arg0, Actor *arg1);
void func_15060B04(s32 arg0, Actor *arg1, s32 arg2);
void func_1507CD64(Actor *arg0, s32 arg1);
void func_1517F488(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

/*
 * Advances the actor's reaction sequence (entry unk106, step unk105): plays the next step's animation
 * and stun time, starts the death animation when it ends with no health left, or ends the reaction.
 */
void func_1505E874(s32 sourceIdx, Actor *actor) {
    s32 *ptr;
    Game83300AnimEntry *table;
    f32 animSpeed;
    f32 blend;
    s32 i;
    u16 anim;
    u16 pad; // matching: stack slot

    blend = 6.0f;
    if (actor->id == 0xFF) {
        return;
    }
    anim = actor->unk106;
    ptr = (s32 *)D_800D1588[actor->id];
    table = (Game83300AnimEntry *)ptr[-2];
    if (table == NULL) {
        return;
    }
    if (actor->unk105 == 0) {
        actor->unk10F = table[anim].unk8;
        actor->disable_jump = 0;
        if (actor->unk31C != NULL) {
            actor->unk31C->unk26 = 0;
        }
        if ((actor->id == 0x5E) && (actor->unk13D != 0)) {
            blend = 0.0f;
        }
        if (table[anim].sound != 0) {
            s32 sound;
            sound = table[anim].sound;
            if (func_1506E46C(actor, &sound, 1) != 0) {
                if (sound != 0x33) {
                    func_15060A30(sound, actor);
                } else {
                    func_15060B04(sound, actor, 0x5DC0);
                }
            }
        }
    }
    actor->unkF4 &= 0xFFF3FEB1;
    if (actor->unk105 < 3) {
        u8 stunTime;
        stunTime = table[anim].sub[actor->unk105].flags & 0xFE;
        actor->unk10B &= ~1;
        actor->unk10B |= table[anim].sub[actor->unk105].flags & 1;
        if ((actor->unk127 != 0xFF) && (actor->unk105 == 0) && (actor->unk31C != NULL) &&
            (actor->camera != NULL) && (actor->unk31C->aimState != 0)) {
            func_1517F488(0xFF, 0, 0, 0xB4, 0x14, actor->camera->playerIndex);
        }
        if ((actor->unk127 != 0xFF) && (actor->health == 0)) {
            i = actor->unk105;
            while (table[anim].sub[i].anim != 0) {
                i++;
            }
            if (i < 2) {
                i = 2;
            }
            if (i == actor->unk105 + 1) {
                s32 type;
                s32 deathKind;
                type = table[anim].kind;
                if (actor->id == 0x9A) {
                    deathKind = 0xE;
                } else if (actor->id == 0x53) {
                    deathKind = 0xA;
                } else if (actor->in_water != 0) {
                    deathKind = 4;
                } else if ((((actor->unk184 & 0x1F) == 0xE) || ((actor->unk184 & 0x1F) == 0x13)) &&
                           (D_800BE9F0 != 0x34)) {
                    deathKind = 0xD;
                } else {
                    switch (type) {
                        case 0x20:
                        case 0x21:
                        case 0x25:
                        case 0x26:
                        case 0x33:
                        case 0x34:
                        case 0x48:
                            deathKind = 0;
                            break;
                        case 0x2D:
                        case 0x2E:
                        case 0x2F:
                            deathKind = 7;
                            break;
                        case 0x2C:
                            deathKind = 8;
                            break;
                        case 0x42:
                            deathKind = 0xC;
                            break;
                        case 0x43:
                            deathKind = 0xB;
                            break;
                        case 0xE:
                        case 0xF:
                            deathKind = 0x10;
                            break;
                        case 0x10:
                            actor->unk31C->unk1AC = 1;
                            D_800D1580 = 0xFF010434;
                            deathKind = 0;
                            func_1506E5FC();
                            D_800D1580 = 0xFF070169;
                            func_1506E8D8();
                            break;
                        default:
                            deathKind = 1;
                            break;
                    }
                }
                func_1507CD64(actor, deathKind);
                actor->unk105 = 3;
                return;
            }
        }
        if (stunTime == 0xC8) {
            actor->unkF4 |= 8;
            actor->unk10B |= 1;
        }
        actor->unk10C = stunTime * 100;
        animSpeed = (u32)table[anim].sub[actor->unk105].speed;
        animSpeed = animSpeed / 100.0f;
        anim = table[anim].sub[actor->unk105].anim;
        if (anim != 0) {
            actor->unk105++;
        }
        {
            s32 pad;

            if (animSpeed == 0.0f) {
                animSpeed = 1.0f;
            }
        }
    } else {
        actor->unk10C = 20000;
        animSpeed = 1.0f;
        anim = 0;
    }
    if (anim == 0) {
        actor->unkF4 &= ~8;
        actor->stunned = 0;
        actor->unk10B = 0;
        actor->unk10F = 0;
        if (actor->interaction_state == 1) {
            actor->unk31C->idle.active = 0;
            actor->unkF8 &= 0xFEFFFDFF;
        }
    } else {
        func_1505E650(actor, anim, animSpeed, blend, 0.0f, 0.0f, 0);
    }
}
/* Allocates an actor slot: a free one, else one in state 0x27 is released and reused. Returns it cleared. */
Actor *func_1505ED34(void) {
    s32 i;
    Actor *ptr;

    for (i = 0, ptr = D_800CC2D0; (i < 25) && (ptr->interaction_state != 0); i++, ptr++) {
    }

    if (i == 25) {
        ptr = D_800CC2D0;
        i = 0;
        if ((0x27 != ptr->interaction_state) || (D_800CC4A4 != NULL)) {
            do {
                i++;
                ptr++;
            } while ((i < 25) && ((0x27 != ptr->interaction_state) || (ptr->unk1D4 != NULL)));
        }

        if (i != 25) {
            if (ptr->interaction_state != 0) {
                func_15060F28(ptr, 0);
            }
        }
    }

    if (i == 25) {
        for (ptr = D_800CC2D0, i = 0; (i < 25) && (0x27 != ptr->interaction_state); i++, ptr++) {
        }

        if (i != 25) {
            if (ptr->interaction_state != 0) {
                func_15060F28(ptr, 0);
            }
        }
    }

    if (i == 25) {
        if (ptr->interaction_state != 0) {
            func_15060F28(ptr, 0);
        }
    }

    func_1505F188(ptr);
    return ptr;
}

/* The first actor in `state` (or past the end); its index goes to *outIdx. */
Actor *func_1505EEB0(s32 state, s32 *outIdx) {
    Actor *ptr;
    s32 i;

    for (ptr = D_800CC2D0, i = 0; (i < 25) && (state != ptr->interaction_state); i++, ptr++) {
    }

    *outIdx = i;
    return ptr;
}

/* The first live actor following path `pathIdx` (unk13F), or NULL. */
Actor *func_1505EEF4(s32 pathIdx) {
    Actor *actor = D_800CC2D0;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((actor->interaction_state != 0) && (pathIdx == actor->unk13F)) {
            return actor;
        }
        actor++;
    }
    return NULL;
}

/* matching: aliases of D_800CC2D0[0].unk127 and &D_800CC2D0[1]; IDO addresses each symbol separately */
extern u8 D_800CC3F7;
extern Actor D_800CC5FC;

/* The live actor whose unk127 is `key`, or NULL. */
Actor *func_1505EFD0(s32 key) {
    Actor *actor;
    s32 i;

    actor = &D_800CC5FC;
    if (D_800CC2D0[0].interaction_state != 0) {
        if (key == D_800CC3F7) {
            return (Actor *)(D_800CC2D4 - 4); /* &D_800CC2D0[0] */
        }
    }

    for (i = 1; i < 25; i++, actor++) {
        if ((actor->interaction_state != 0) && (key == actor->unk127)) {
            return actor;
        }
    }
    return NULL;
}

/* The first live actor of type `id`, or NULL. */
Actor *func_1505F0AC(s32 id) {
    Actor *actor = D_800CC2D0;
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((actor->interaction_state != 0) && (id == actor->id)) {
            return actor;
        }
        actor++;
    }
    return NULL;
}
/* Clears an actor slot and sets its default fields. */
void func_1505F188(Actor *actor) {
    s32 *p;
    f32 one;
    f32 initial;

    for (p = (s32 *)actor; p < (s32 *)(actor + 1); p++) {
        *p = 0;
    }
    one = 1.0f;
    initial = -10000.0f;
    actor->unk2FD = 2;
    *(s16 *)&actor->unk38 = -10000;
    actor->xz_scale = one;
    actor->y_scale = one;
    actor->unk118 = initial;
    actor->unk180 = initial;
    actor->gravity = 3.3f;
    ((u8 *)&actor->pad1DC)[0] = 0xFF;
    actor->unk127 = 0xFF;
    actor->unk84.uh = 0xFFFF;
    actor->unk13F = 0xFF;
    actor->modelIds = &actor->id;
    actor->lodCount = 1;
    actor->modelCount = 1;
    actor->id = 0xFF;
    actor->unk2CB = 50;
    actor->unk48 = one;
    actor->unk6E = (func_150ADA20() % 50U) + 50;
    func_150615DC(actor);
    ((u8 *)&actor->pad1DC)[1] = 0xFF;
    ((u8 *)&actor->pad1DC)[2] = 0xFF;
    ((u8 *)&actor->pad1DC)[3] = 0xFF;
    actor->floorVerts[0][0] = 0;
    actor->floorVerts[0][1] = 0;
    actor->floorVerts[0][2] = 0;
    actor->floorVerts[1][0] = 0;
    actor->floorVerts[1][1] = 0;
    actor->floorVerts[1][2] = 10;
    actor->floorVerts[2][0] = 10;
    actor->floorVerts[2][1] = 0;
    actor->floorVerts[2][2] = 0;
}
extern u8 D_80099428[];
void func_1000EFB4();
void func_151AE264(Actor *arg0);
void func_151C6EA0(Actor *arg0, s32 arg1, s32 arg2);
void func_1506045C(Actor *arg0, s32 arg1, s32 arg2, f32 arg3, s32 arg4);
/* Player controls for jumping, swimming and the hover (anim 0xE9), from the controller pad. */
void func_1505F298(Actor *actor, OSContPad *pad) {
    s32 anim;
    s32 sound;
    f32 speed;
    f32 animArg4;
    f32 rate;
    s32 id;
    u8 prevJumpState;
    f32 height;
    s32 neverSet;
    s32 buttons;
    s32 flag;
    s32 jumpType;
    s32 idx;
    u8 jumped;
    u8 jumpState;
    f32 savedSpeed;

    prevJumpState = actor->unk31C->unk31;
    sound = 0;
    speed = 1.0f;
    rate = 0.0f;
    neverSet = 0;
    if ((actor->stunned != 0) || (actor->health == 0)) {
        actor->target_speed = 0.0f;
        return;
    }
    if ((actor->unk31C->unk46 > 0) || (actor->unk31C->unk36 >= 0x64) || (actor->unk31C->unk6B != 0)) {
        return;
    }
    height = actor->unk28;
    if (actor->unk31C->unk4E == 2) {
        height = actor->unk28 - 150.0f;
    } else if (actor->unk31C->unk4F == 2) {
        height = 0.0f;
    } else if (actor->y_velocity > 0.0f) {
        height += 50.0f;
    }
    if (actor->unk110 != 0) {
        buttons = pad->button & ~0x2000;
    } else {
        buttons = pad->button;
    }
    if ((actor->in_water != 0) && (actor->unk31C->unk95 == 0) && (actor->unk84.uh != 0x262)) {
        if (actor->in_water == 1) {
            rate = 2.0f;
            anim = 0;
            if ((D_800CC288 & 0x8000) && ((D_800BE9F0 != 0x29) || (actor->unk31C->unk8 <= 0)) &&
                (((actor->unk118 - 60.0f) - 30.0f) < actor->y_position)) {
                func_15052408(actor);
                actor->unk31C->unk31 = 0;
                rate = 0.0f;
                if ((actor->unk31C->unk7B != 7) && (actor->unk31C->unk7B != 8)) {
                    actor->unk31C->unk31 = 1;
                }
                actor->unk76 = actor->unk7A = actor->unk78 = actor->unk31C->unk4C;
                if ((actor->unk31C->unk68 != 0) && ((actor->unk76 - actor->unk31C->unk68 + 0x4000) & 0x8000)) {
                    actor->xz_velocity = 0.0f;
                    actor->disable_run = 0xFF;
                }
                actor->unkAE = 0xC;
                actor->y_velocity = 46.0f;
                actor->gravity = 3.299999952f;
                actor->unk81 = 2;
                actor->unk83 = 0;
                anim = 0x32;
            }
            actor->target_speed = 0.0f;
            if ((D_800CC288 & 0x4000) && !(pad->button & 0x10)) {
                if (actor->unk31C->unk22 < 0x69) {
                    actor->unk31C->unk22 = 0xF;
                }
            }
            if (((actor->xz_velocity > 2.0f) && (actor->unk31C->unk21 != 0)) ||
                ((actor->unk31C->unk21 == 0) && ((actor->unk31C->unk22 > 0) || (!(pad->button & 0x10) && (pad->button & 0x4000))))) {
                if (anim != 0x32) {
                    rate = 9.0f;
                    actor->unk83 = 0;
                    if ((actor->unk118 - 60.0f) < actor->y_position) {
                        speed = 1.200000048f;
                        anim = 0x22;
                        actor->target_speed = 15.0f;
                    } else {
                        speed = 1.200000048f;
                        anim = 0xCE;
                        actor->target_speed = 23.0f;
                    }
                }
            }
        }
        if (anim == 0) {
            rate = 10.0f;
            speed = 1.200000048f;
            anim = 0xD1;
            if ((actor->unk118 - actor->y_position) < 70.0f) {
                anim = 0xCC;
                speed = 0.8999999762f;
            }
            actor->target_speed -= 0.200000003f;
            if (actor->target_speed < 0.0f) {
                actor->target_speed = 0.0f;
            }
        }
        if ((actor->in_water != 0) && (actor->unk13C != 0)) {
            rate = 7.0f;
            anim = 0xCB;
        }
        if ((actor->unk31C->unk11A != 0) || ((actor->unk31C->unk78 != 0) && (actor->unk31C->unk78 != 0x27))) {
            func_1506045C(actor, 0, 0, height, 1);
        }
        if (anim != 0) {
            func_1505E650(actor, anim, speed, rate, 0.0f, 0.0f, 0);
        }
        return;
    }
    if (actor->unk137 == 1) {
        anim = 0;
        if (actor->unk31C->unk53 == 0) {
            if (pad->stick_y >= 0x33) {
                speed = 2.200000048f;
                actor->unk31C->unk54 = 0xC;
                anim = 0xEA;
                actor->unk137 = 0;
                actor->disable_run = 0x10;
                actor->unk81 = 1;
                actor->gravity = 5.0f;
                actor->y_velocity = 48.0f;
            } else if (pad->stick_y < -0x32) {
                anim = 0x38;
                actor->unk137 = 0;
                actor->gravity = 3.5f;
            }
        }
        if ((D_800CC288 & 0x8000) || (actor->unk31C->unk84 != 0)) {
            actor->unk137 = 2;
            anim = 0x40;
            speed = 3.0f;
        }
        if (anim != 0) {
            func_1505E650(actor, anim, speed, 4.0f, 0.0f, 0.0f, 0);
            return;
        }
    }
    idx = actor->unk65;
    jumpType = 0;
    flag = 0;
    anim = 0;
    if (idx != 0) {
        id = D_800CC2D0[idx - 1].id;
        if (((id == 0x28) && (D_800BE9F0 != 0x23)) || (id == 0x8A) || (id == 0x23)) {
            jumpType = id + 1;
            flag = 1;
        }
    }
    if ((jumpType != 0) || (height <= 0.0f) || (actor->unk31C->unk55 != 0)) {
        actor->unk137 = 0;
        jumped = 0;
        animArg4 = 0.0f;
        if (D_800CC288 & 0x8000) {
            if ((actor->unk31C->unk11A != 1) && (actor->unk31C->unk11A != 3) &&
                ((jumpType != 0) || ((actor->unk13C < 0x64) && (actor->disable_jump == 0) && (actor->disable_run != 0xFF)))) {
                jumped = 1;
                animArg4 = 15.0f;
                actor->unk100 &= 0xFE;
                if (flag != 0) {
                    func_1506160C(actor, 1, 0xFF, 0, 0);
                }
                if (actor->unk31C->unk17 != 0) {
                    animArg4 = 0.0f;
                    anim = 0x279;
                    speed = 1.25f;
                    actor->unk84.uh = 0;
                    actor->disable_jump = 0xA;
                    actor->disable_run = 0xFF;
                    actor->unk83 = 0xFF;
                } else if (actor->unk31C->unk8 != 0) {
                    actor->unk83 = 0xFF;
                    actor->disable_run = 0xFF;
                    jumped = 0;
                    func_1505E650(actor, 0x43, 1.299999952f, 2.0f, 0.0f, 0.0f, 0);
                } else {
                    if (actor->unk31C->unk1A != 0) {
                        anim = 0x32;
                        animArg4 = 0.0f;
                        actor->disable_run = 0xFF;
                        actor->unk81 = 1;
                        actor->y_velocity = 47.0f;
                        actor->gravity = 6.0f;
                    } else if (!(buttons & 0x2000) || (actor->unk31C->unk7B == 7) || (actor->unk31C->unk7B == 8)) {
                        if (actor->disable_run != 0) {
                            actor->disable_run = 0;
                            actor->unk7A = actor->unk76 = actor->unk31C->unk4C;
                        }
                        anim = 0x32;
                        if (actor->xz_velocity < 9.0f) {
                            animArg4 = 0.0f;
                            actor->unk31C->unk31 = 1;
                        } else {
                            speed = 0.8000000119f;
                            actor->unk31C->unk31 = 2;
                            anim = 0x37;
                            if (actor->unk31C->unk78 == 0x21) {
                                anim = 0x19E;
                                speed = 1.399999976f;
                            }
                        }
                        if ((actor->unk31C->unk7B == 7) || (actor->unk31C->unk7B == 8)) {
                            actor->unk31C->unk31 = 0;
                        }
                        if (actor->unk31C->unk4E == 1) {
                            actor->y_velocity = 40.0f;
                        } else {
                            actor->y_velocity = 59.0f;
                        }
                        if (actor->unk31C->unk4E == 2) {
                            actor->y_velocity = 80.0f;
                            sound = 0x7E2;
                        }
                        actor->gravity = 4.0f;
                        if ((actor->unk184 & 0x1F) == 0xC) {
                            actor->y_velocity *= 0.6999999881f;
                        }
                        if ((actor->unk184 & 0x1F) == 0x13) {
                            anim = 0x32;
                            actor->y_velocity *= 0.5500000119f;
                        }
                        if (D_800CC27C) {
                            actor->unk81 = 0x82;
                            /* never taken: neverSet is always 0 (the compiler folds this away) */
                            if (neverSet) {
                                actor->unk83 = 0x82;
                            }
                        } else {
                            actor->unk81 = 2;
                        }
                    } else {
                        savedSpeed = actor->xz_velocity;
                        if (D_800CC27C == 0) {
                            actor->unk81 = 1;
                        } else {
                            actor->unk81 = 0x81;
                        }
                        animArg4 = 0.0f;
                        speed = 1.080000043f;
                        actor->disable_run = 0x17;
                        actor->unk83 = 0x64;
                        anim = 0xEB;
                        if (D_800CC27C != 0) {
                            actor->xz_velocity = savedSpeed;
                            actor->y_velocity *= 1.399999976f;
                            actor->gravity = 3.799999952f;
                        }
                    }
                }
            }
        }
        if (jumped != 0) {
            if (sound != 0) {
                if (func_1506E46C(actor, &sound, 1) != 0) {
                    func_15060A30(sound, actor);
                }
            }
            prevJumpState = 0;
            if (jumpType != 0) {
                actor->xz_velocity = 20.0f;
                if (jumpType == 0x29) {
                    func_15052F58(actor->unk65 - 1, 0xE);
                } else if (jumpType == 0x8B) {
                    func_15052F58(actor->unk65 - 1, 5);
                    actor->xz_velocity = 30.0f;
                    if (D_800BE616 != 0) {
                        actor->unk7A = 0x3000;
                        if (actor->z_position > 0.0f) {
                            actor->unk7A = 0xB000;
                        }
                    }
                } else if (jumpType == 0x24) {
                    func_15052F58(actor->unk65 - 1, 0xE);
                    actor->xz_velocity = 27.0f;
                }
                actor->unk31C->unk30 = 0;
                actor->immune = 0x14;
                actor->unk76 = actor->unk7A + 0x1200;
                actor->unk65 = 0;
                actor->disable_run = 0xFF;
                actor->y_velocity = 30.0f;
                actor->unk31C->unk31 = 0;
            }
            sound = 0;
            actor->unk31C->unk55 = 0;
            if (actor->unk31C->unk4E != 0) {
                actor->unkA9 = 5;
            }
            if ((anim != 0xEB) && (anim != 0x279)) {
                actor->unk83 = 0;
                actor->unk28 = 80.0f;
            }
            if (actor->unk31C->unk4E != 2) {
                func_1505E650(actor, anim, speed, 2.0f, animArg4, 0.0f, 0);
            }
            height = 25.0f;
            actor->unkB8 = 0.0f;
            actor->unk110 = 0;
            actor->unk100 &= 0x53;
        }
        anim = 0;
    }
    func_1506045C(actor, buttons, D_800CC288, height, 0);
    {
    s32 pad;
    rate = 2.0f;
    if (((actor->unk31C->ropeId != 0) || (actor->unk31C->unk95 != 0)) && (actor->disable_jump == 0) && (D_800CC288 & 0x8000)) {
        actor->unk76 = actor->unk7A = actor->unk31C->unk4C;
        actor->unk31C->unk31 = 1;
        actor->unk80 = 0xA;
        actor->unk83 = 0;
        actor->unk100 |= 8;
        actor->disable_run = 0;
        actor->unk100 &= 0xDE;
        anim = 0x37;
        speed = 1.299999952f;
        if (actor->in_water == 0) {
            actor->y_velocity = 40.0f;
            actor->gravity = 3.200000048f;
        }
        actor->xz_velocity = (actor->unk44 * 0.6000000238f) + 15.0f;
        if (actor->unk44 < 4.0f) {
            actor->unk76 = actor->unk7A = actor->unk76 + 0x8000;
        }
        actor->unk31C->unk95 = 0;
        actor->unk31C->unk97 = 0;
        if (actor->unk31C->ropeId != 0) {
            func_151AE264(actor);
        }
        actor->unk31C->unk54 = 0;
    } else if ((D_800CC288 & 0x8000) && (actor->unk31C->unk4E == 0)) {
        if ((actor->unk84.uh != 0x3B) && (actor->unk84.uh != 0x3C) && (actor->unk137 == 0) && (actor->unk13C == 0) &&
            (actor->unk31C->unk95 == 0) && (actor->unk31C->unk31 == 0) && (height > 55.0f)) {
            if ((actor->unk118 != -10000.0f) && ((actor->unk118 - actor->unk180) > 100.0f)) {
                actor->gravity = 6.0f;
                actor->unk81 = 1;
                actor->disable_run = 0x78;
                actor->unk137 = 0;
                anim = 0x3B;
                actor->unk1CC = actor->y_position;
            }
        }
    }
    }
    if ((D_800BE616 == 0) && (prevJumpState != 0) && (actor->in_water == 0) && (actor->unk13C < 0x65) && (actor->id != 0x96)) {
        jumpState = actor->unk31C->unk31;
        if ((jumpState != 0) && (actor->unk31C->unk4E == 0)) {
            if (actor->unk31C->unk31 < 3) {
                if ((D_800CC288 & 0x8000) && (actor->unk28 > 1.0f)) {
                    actor->xz_velocity *= 0.3000000119f;
                    actor->unk31C->unk31 = 3;
                    rate = 6.0f;
                    anim = 0xE9;
                    actor->y_velocity = 0.0f;
                    actor->unk81 = 1;
                    actor->unk1CC = actor->y_position;
                    actor->gravity = 1.0f;
                    if (actor->unk84.uh != 0xE9) {
                        func_100109D0(actor);
                        func_1000FA64(0x61E, (s16)(s32)actor->x_position, (s16)(s32)actor->y_position, (s16)(s32)actor->z_position,
                                      0x7FFF, 0x1F4, 0xC8, (SoundRecordCallback)func_1000EFB4, actor, (s32)D_80099428, 0, 0);
                        func_151C6EA0(actor, 0xFF, 1);
                    }
                }
            } else {
                actor->unk31C->unk31 += D_800BE9A0;
                if (actor->unk31C->unk31 < 0xD) {
                    actor->y_velocity = 8.0f;
                }
                if (actor->unk31C->unk31 < 0x11) {
                    actor->animation_speed += 0.25f;
                } else {
                    actor->animation_speed += (0.6000000238f - actor->animation_speed) * 0.1000000015f;
                }
                anim = 0xE9;
                speed = actor->animation_speed;
                actor->unk31C->unk3C = 6;
                if ((!(pad->button & 0x8000) && (actor->unk31C->unk31 >= 0x13)) || (actor->unk31C->unk31 >= 0x3E)) {
                    actor->disable_run = 0xFF;
                    actor->unk100 = 0x55;
                    actor->unk31C->unk31 = 0;
                    actor->gravity = 4.5f;
                    func_1506AF74();
                    anim = 0;
                } else {
                    actor->unk80 = 1;
                    actor->unkAA = 0x4B;
                    actor->unk83 = 1;
                    if (actor->y_velocity < -8.0f) {
                        actor->y_velocity = -8.0f;
                    }
                }
            }
        }
    }
    if (anim != 0) {
        func_1505E650(actor, anim, speed, rate, 0.0f, 0.0f, 0);
    }
    if (sound != 0) {
        func_15060A30(sound, actor);
    }
    id = actor->unk81;
    if (((id & 7) == 2) && !(pad->button & 0x8000)) {
        actor->unk81 = id - 1;
        actor->gravity = 6.199999809f;
    }
}
/* Runs the player's current action/item (unk31C->unk75, overridden by context) for this frame's input. */
void func_1506045C(Actor *actor, s32 buttons, s32 pressed, f32 height, s32 nested) {
    s32 stunned;
    s32 state;
    s32 expired;
    s32 held;
    s32 result;
    s32 savedUnk75;
    s32 idx;

    result = 0;
    if (D_800C35EA == 1) {
        return;
    }
    held = 0;
    stunned = 0;
    expired = 0;

    if ((actor->unk31C->unk72 != 0) && (nested == 0)) {
        if (D_800BE9E4 < actor->unk31C->unk72) {
            actor->unk31C->unk72 -= D_800BE9E4;
        } else {
            expired = 1;
            actor->unk31C->unk72 = 0;
        }
    }

    state = actor->unk31C->unk75 & ~0x80;
    if ((actor->unk31C->unk7C != 0) && (state == 0)) {
        state = actor->unk31C->unk7C;
    }

    if ((D_800BE9F0 != 0x29) && (D_800BE9F0 != 0x2E) && (actor->unk31C->unk78 == 0) &&
        (*(u16 *)D_800CC284 & 0x20) && (func_15080738(state) != 0)) {
        state = 0x1A;
    }

    if (actor->unk31C->unk78 == 0x1A) {
        state = 0x1A;
    }
    if (actor->unk31C->unk78 == 0x2A) {
        state = 0x2A;
    }

    if ((actor->unk13C != 0) || (actor->unk137 != 0)) {
        if (nested == 0) {
            state = 0;
        } else {
            held = 1;
        }
    }

    idx = actor->unk65;
    if ((idx != 0) && (nested == 0)) {
        switch (D_800CC2D0[idx - 1].id) {
        case 0x23:
        case 0x8A:
            state = 0x1B;
            break;
        case 0xC:
            state = 8;
            break;
        case 0x28:
            state = 0;
            break;
        default:
            state = 0xE;
            break;
        }
    }

    if (actor->unk31C->unk17 != 0) {
        state = 0x3C;
    }

    if ((nested == 0) && actor->unk31C->unk78 && (state != (actor->unk31C->unk78 & ~0x80))) {
        savedUnk75 = actor->unk31C->unk75;
        actor->unk31C->unk75 = actor->unk31C->unk78;
        func_1506045C(actor, buttons, pressed, height, 1);
        actor->unk31C->unk75 = savedUnk75;
        actor->unk31C->unk78 = 0;
    }

    if (actor->stunned != 0) {
        stunned = 1;
        pressed = 0;
        buttons = 0;
    }

    if ((state != 0) && (actor->unk31C->unk4E == 0)) {
        result = func_15065A5C(actor, buttons, pressed, height, nested, state, stunned, expired, held);
    }

    if ((actor->unk31C->unk7B != 0) || (pressed & 0x4000)) {
        func_150642AC(actor, pressed, height, held, result);
    }
}
/*
 * Plays sound `sound` on an actor that has a camera (a player). flags & 3 picks the handle slot it
 * reuses (1: unk8C, 2/3: unk8E; 3 only retunes it if the same sound is still playing), flags & 4 makes
 * it positional, flags & 8 forces channel 0.
 */
void func_15060778(s32 sound, Actor *actor, u16 rate, s32 level, s32 minDist, s32 maxDist, s32 flags) {
    s32 channel;
    u32 handle;
    u16 *slot;

    handle = 0;
    slot = NULL;
    if (flags & 8) {
        channel = 0;
    } else {
        channel = ((u32)actor->unk184 >> 3) & 0x30;
    }
    switch (flags & 3) {
    case 1:
        slot = &actor->unk8C;
        handle = actor->unk8C;
        break;
    case 2:
    case 3:
        slot = &actor->unk8E;
        handle = actor->unk8E;
        if ((flags & 3) == 2) {
            break;
        }
        if ((func_1001147C(handle) == sound) & 0x7FFF) {
            if (flags & 4) {
                func_1000F91C(handle, (u16)(rate + channel * 50), (s16)(actor->unk13F * 10 + level), channel & 0xFF, 0,
                              (s32)actor->x_position, (s32)actor->y_position, (s32)actor->z_position, minDist, maxDist);
            } else {
                func_1000F85C(handle, 0x10, level);
                func_1000F85C(handle, 8, rate + channel * 50);
            }
            return;
        }
        sound |= 0x8000;
        break;
    }
    if (flags & 4) {
        handle = func_10010E78(handle, sound, (u16)(rate + channel * 50), (s16)level, channel, 0,
                               (s32)actor->x_position, (s32)actor->y_position, (s32)actor->z_position, minDist,
                               maxDist);
    } else {
        handle = func_10010BE8(handle, sound, (u16)(rate + channel * 50), 0x40, level, channel, 1);
    }
    if (slot != NULL) {
        *slot = handle;
    }
}
/* Plays an actor sound (players use handle slot 1). */
void func_15060A30(s32 sound, Actor *actor) {
    if (actor->camera == NULL) {
        func_10010344(sound, actor, 28000, 500, 2500);
    } else {
        func_15060778(sound, actor, 24000, 0, 500, 2500, 1);
    }
}
/* Plays an actor sound without a handle slot. */
void func_15060A9C(s32 sound, Actor *actor) {
    if (actor->camera == NULL) {
        func_10010630(sound, actor, 24000, 500, 2500);
    } else {
        func_15060778(sound, actor, 24000, 0, 500, 2500, 0);
    }
}

/* func_15060A9C with an explicit rate. */
void func_15060B04(s32 sound, Actor *actor, s32 rate) {
    if (actor->camera == NULL) {
        func_10010630(sound, actor, rate, 500, 2500);
    } else {
        func_15060778(sound, actor, rate, 0, 500, 2500, 0);
    }
}

/* Plays a sound at the actor. */
void func_15060B70(s32 sound, void *actor) {
    func_10010154(sound, actor, 28000, 500, 2500);
}

/* Adds health up to the maximum of 6; returns FALSE when already full. */
s32 func_15060BA4(Actor *actor, s32 amount) {
    if (actor->health == 6) {
        return FALSE;
    }
    actor->health += amount;
    if (actor->health >= 7) {
        actor->health = 6;
    }
    return TRUE;
}
extern u8 D_800C3E90;

/* Stores the actor's head position (s16 at 0x1BC): headHeight along its head bone's up axis, or its origin. */
void func_15060BE0(Actor *actor) {
    f32 pad0; // matching: pad0..pad3 reproduce the stack layout
    f32 pad1;
    f32 pad2;
    f32 pad3;
    f32 offZ;
    f32 len;
    f32 upX;
    f32 upY;
    f32 upZ;
    f32 height;
    f32 offY;
    f32 scale;
    f32 offX;
    s32 done;
    ActorInfo15056B08 *info;
    f32 (*bone)[4];

    done = FALSE;
    if (actor->id != 0xFF) {
        if ((actor->unk1D4 != 0) && (D_800C3E90 == 0)) {
            info = (ActorInfo15056B08 *)D_800D1C90[actor->id];
            bone = (f32 (*)[4])((u8 *)actor->unk1D4 + (info->headBone << 6)); /* 4x4 float matrices */
            height = (f32)info->headHeight * actor->y_scale;
            if (height) {
                upX = bone[1][0];
                upY = bone[1][1];
                upZ = bone[1][2];
                len = sqrtf((upX * upX) + (upY * upY) + (upZ * upZ));
                scale = len;
                if (len) {
                    scale = height / len;
                }
                offX = upX * scale;
                offY = upY * scale;
                offZ = upZ * scale;
            } else {
                offY = (offZ = 0.0f);
                offX = offY;
            }
            done = TRUE;
            ((s16 *)&actor->pad1BC)[0] = (s16)(s32)(bone[3][0] + offX);
            ((s16 *)&actor->pad1BC)[1] = (s16)(s32)(bone[3][1] + offY);
            ((s16 *)&actor->pad1BC)[2] = (s16)(s32)(bone[3][2] + offZ);
        }
    }
    if (done == FALSE) {
        ((s16 *)&actor->pad1BC)[0] = (s16)(s32)actor->x_position;
        ((s16 *)&actor->pad1BC)[1] = (s16)(s32)actor->y_position;
        ((s16 *)&actor->pad1BC)[2] = (s16)(s32)actor->z_position;
    }
}
/* Detaches every actor carried by (unk65 pointing at) this one. */
void func_15060D54(Actor *actor) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) && ((actor - D_800CC2D0) + 1 == D_800CC2D0[i].unk65)) {
            D_800CC2D0[i].unk65 = 0;
        }
    }
}
extern s32 func_1514D310(Actor *);
extern void func_151695F0(Actor *, s32);
extern void func_15084558(Actor *);
extern void func_1504AF10(Actor *, s32, s32);
extern void func_1503E260(s32);
extern s32 func_150303E4(Actor *);
extern void func_15188AD0(Actor *);
extern void func_10004074(void *);
extern u8 *D_800D210C;
void func_150626EC(Actor *arg0, s32 arg1);

/* Removes the actor: releases its resources and attachments, then clears its slot. */
void func_15060F28(Actor *actor, s32 mode) {
    func_1514D310(actor);
    func_151695F0(actor, 0);
    if ((actor->unk5 != 2) && (actor->unk5 != 3)) {
        func_15084558(actor);
        func_150626EC(actor, mode);
    }
    if (mode == 1) {
        func_1504AF10(actor, 1, 0);
        func_1503E260(actor - D_800CC2D0);
    }
    if (actor->unk13F != 0xFF) {
        if (mode != 2) {
            /* matching: spawn is read as an s32 at each use; pointer reads let IDO keep it in a register */
            if ((*(s32 *)&actor->spawn) != 0) {
                if (((SpawnRecord *)*(s32 *)&actor->spawn)->flags & 0x20) {
                    ((SpawnRecord *)*(s32 *)&actor->spawn)->unk2 = 1;
                    D_800D210C[actor->unk13F] = 0;
                } else if (((SpawnRecord *)*(s32 *)&actor->spawn)->flags & 1) {
                    D_800D210C[actor->unk13F] = 0;
                } else {
                    s32 pad;
                    ((SpawnRecord *)*(s32 *)&actor->spawn)->unk2 = 1;
                    D_800D210C[actor->unk13F] &= 0x80;
                }
            } else {
                D_800D210C[actor->unk13F] &= 0x80;
            }
        } else {
            D_800D210C[actor->unk13F] = 0;
        }
    }
    /* free the allocations held in the actor's padding (0x260..0x26C, 0x1D8, ...) */
    if (*(void **)((u8 *)actor + 0x260) != NULL) {
        func_10004074(*(void **)((u8 *)actor + 0x260));
    }
    if (*(s32 *)((u8 *)actor + 0x264) != 0) {
        func_10004074((void *)*(s32 *)((u8 *)actor + 0x264));
    }
    if (*(s32 *)((u8 *)actor + 0x268) != 0) {
        func_10004074((void *)*(s32 *)((u8 *)actor + 0x268));
    }
    if (*(s32 *)((u8 *)actor + 0x26C) != 0) {
        func_10004074((void *)*(s32 *)((u8 *)actor + 0x26C));
    }
    if (*(void **)((u8 *)actor + 0x1D8) != NULL) {
        func_10004074(*(void **)((u8 *)actor + 0x1D8));
    }
    if (*(struct102 **)((u8 *)actor + 0x2D4) != NULL) {
        func_1516972C(*(struct102 **)((u8 *)actor + 0x2D4));
    }
    if (*(s32 **)((u8 *)actor + 0x304) != NULL) {
        s32 i;
        for (i = 0; i <= D_80082FA0; i = (u8)(i + 1)) {
            func_100043B4(*(s32 **)((u8 *)actor + (i * 4) + 0x304), 4);
        }
    }
    if (*(s32 *)((u8 *)actor + 0x314) != 0) {
        func_100043B4((s32 *)*(s32 *)((u8 *)actor + 0x314), 4);
    }
    if (actor->unk2D0 != NULL) {
        func_10004074(actor->unk2D0);
    }
    if (*(void **)((u8 *)actor + 0x324) != NULL) {
        func_10004074(*(void **)((u8 *)actor + 0x324));
    }
    if ((actor->unk31C != NULL) && (actor->unk31C->boneChain != NULL)) {
        func_10004074(actor->unk31C->boneChain);
    }
    func_10010AA8(actor);
    func_150303E4(actor);
    func_15060D54(actor);
    func_15188AD0(actor);
    if (actor->unk31C != NULL) {
        func_10004074((void *)(s32)actor->unk31C); // matching: passing actor->unk31C directly mismatches
    }
    func_1505F188(actor);
}

/* Drifts the actor along water-current path pathIdx, from the nearest of its points. */
void func_150611E8(Actor *actor, s32 pathIdx) {
    s32 count;
    s32 closest;
    s32 i;
    s32 best;
    s16 x;
    s16 z;
    PathPoint *pts;
    f32 speed;
    f32 scale;
    s32 dx;
    s32 dz;
    u16 angle;
    s32 dist;

    count = D_800D2108[pathIdx];
    if (count < 3) {
        return;
    }
    x = actor->x_position;
    z = actor->z_position;
    best = 0x7FFFFFFF;
    for (i = 0; i < count; i++) {
        dx = x - ((PathPoint **)D_800D2104)[pathIdx][i].x;
        dz = z - ((PathPoint **)D_800D2104)[pathIdx][i].z;
        if (dx * dx + dz * dz < best) {
            closest = i;
            best = dx * dx + dz * dz;
        }
    }
    if (closest != 0 && closest != count - 1) {
        pts = ((PathPoint **)D_800D2104)[pathIdx];
        if (best > 28000) {
            dx = x - pts[closest + 1].x;
            dz = z - pts[closest + 1].z;
            dist = dx * dx + dz * dz;
            dx = x - pts[closest - 1].x;
            dz = z - pts[closest - 1].z;
            if (dx * dx + dz * dz < dist) {
                closest--;
            }
        }
        if (pts[closest + 1].param == 0) {
            speed = 8.0f;
        } else {
            speed = (u32)pts[closest + 1].param;
        }
        scale = speed * 0.0025f;
        if (scale > 1.0f) {
            scale = 1.0f;
        }
        angle = func_1505A630(pts[closest + 1].x - pts[closest].x, pts[closest].z - pts[closest + 1].z, 0);
        if (actor->unk31C != NULL && speed > 20.0f) {
            actor->unk31C->unk68 = angle | 1;
        }
        func_150593C4(actor, angle, speed, scale);
    }
}

// TODO: is this really Actor?
/* Resets the actor's bytes 0x7..0xF. */
void func_150615DC(Actor *actor) {
    actor->unk7 = 0xFF;
    actor->unk8 = 0xFF;
    actor->unk9 = 0;
    actor->unkA = 0;
    actor->unkF = 0;
    actor->unkB[0] = actor->unkB[1] = actor->unkB[2] = actor->unkB[3] = 0xFF;
}

/*
 * Actor fading. unk7 is the alpha, faded toward unk8 by unk9 per frame in mode unkA (4: in, 5/6: out,
 * then removed). Bytes unkB..unkE.ub[0] are per-player alphas, unkE.ub[1] holds per-player hide bits
 * (low nibble) and pending bits (high nibble). Commands 8 and up act on player `player` only.
 */
void func_1506160C(Actor *actor, u8 cmd, u8 value, u8 step, s32 player) {
    s32 bit;
    s32 hi;

    if (cmd >= 8) {
        if (cmd != 10) {
            if (cmd == 8) {
                bit = 1 << (u8)player;
                actor->unkF |= bit;
                value = 0;
            } else {
                bit = 1 << (u8)player;
                actor->unkF &= ~bit;
                value = 0xFF;
            }
            hi = bit << 4;
            if (actor->unkF & hi) {
                actor->unkB[(u8)player] = value;
            }
            actor->unkF &= ~hi;
        } else {
            actor->unkF |= (1 << (u8)player) << 4;
            value = 0;
        }
        if ((actor->unk2FD != 0) || (D_800DBFF4[(u8)player] != 0)) {
            actor->unkB[(u8)player] = value;
        }
    } else if (cmd >= actor->unkA) {
        if (cmd == 4) {
            actor->unkA = cmd;
            actor->unk7 = 0;
            actor->unk8 = 0xFF;
            actor->unk9 = 0x20;
        } else if (cmd == 5) {
            actor->unkA = cmd;
            actor->unk8 = 0;
            actor->unk9 = 0x20;
        } else if (cmd == 6) {
            actor->unkA = cmd;
            actor->unk8 = 0;
            actor->unk9 = 8;
        } else if (cmd == 2) {
            actor->unkA = cmd;
            actor->unk8 = value;
            actor->unk9 = step;
        } else if (cmd == 1) {
            actor->unkA = 0;
            actor->unk7 = value;
            actor->unk8 = value;
        }
        if (*(s32 *)D_800DBFF4 != 0) {
            actor->unk7 = actor->unk8;
        }
    }
}

/* Per-frame fade step (see func_1506160C); removes the actor when a fade-out finishes. */
void func_150617BC(Actor *actor) {
    s32 i;
    s32 mask;
    s32 value;

    if (actor->interaction_state != 0) {
        mask = 1;
        for (i = 0; i <= D_80082FA0; i++) {
            if (actor->unkF & mask) {
                if (actor->unkB[i] > 0) {
                    if (((u8 *)&D_800DBFF4)[i] == 0) {
                        value = actor->unkB[i] - 50;
                        if (value < 0) {
                            value = 0;
                        }
                    } else {
                        value = 0;
                    }
                    actor->unkB[i] = value;
                }
            } else {
                if (actor->unkB[i] < 0xFF) {
                    if (((u8 *)&D_800DBFF4)[i] == 0) {
                        value = actor->unkB[i] + 50;
                        if (value > 0xFF) {
                            value = 0xFF;
                        }
                    } else {
                        value = 0xFF;
                    }
                    actor->unkB[i] = value;
                }
            }
            mask <<= 1;
        }
        /* matching: byte indexing (7: unk7, 8: unk8, 9: unk9, 0xA: unkA); field access loads in another order */
        if (((u8 *)actor)[0xA] != 0) {
            if (((u8 *)actor)[8] != ((u8 *)actor)[7]) {
                if (((u8 *)actor)[7] < ((u8 *)actor)[8]) {
                    value = ((u8 *)actor)[9] + ((u8 *)actor)[7];
                    if (((u8 *)actor)[8] < value) {
                        value = ((u8 *)actor)[8];
                    }
                } else {
                    value = ((u8 *)actor)[7] - ((u8 *)actor)[9];
                    if (value < ((u8 *)actor)[8]) {
                        value = ((u8 *)actor)[8];
                    }
                }
                ((u8 *)actor)[7] = value;
            }
            if (((u8 *)actor)[8] == ((u8 *)actor)[7]) {
                if (((u8 *)actor)[0xA] == 6) {
                    func_15060F28(actor, 1);
                } else if (((u8 *)actor)[0xA] == 5) {
                    func_15060F28(actor, 2);
                } else {
                    ((u8 *)actor)[0xA] = 0;
                }
            }
        }
    }
}

/* The actor's alpha as seen by `player` (0..255). */
s32 func_1506196C(Actor *actor, s32 player) {
    s32 alpha = actor->unk7 * actor->unkB[player];

    if (alpha == 255 * 255) {
        alpha = 255;
    } else {
        alpha = alpha >> 8;
    }
    return alpha;
}

s32 func_150623F4(Actor *arg0);
/*
 * Per-frame fade and visibility update of all actors: unk2FA bit 0 is set when func_150623F4 says the
 * actor is visible, or when one of its linked actors (keys in bytes 0x20B..0x20E) is.
 */
void func_150619A8(void) {
    s32 i;
    s32 count;
    u8 list[25];
    s32 j;
    Actor *actor;
    Actor *other;
    s32 idx;

    count = 0;
    for (i = 0; i < 25; i++) {
        actor = &D_800CC2D0[i];
        if (actor->interaction_state != 0) {
            if (actor->unk2FD != 0) {
                actor->unk2FD--;
            }
            func_150617BC(actor);
            if (func_150623F4(actor) != 0) {
                actor->unk2FA |= 1;
            } else {
                actor->unk2FA &= ~1;
                if (((u8 *)actor)[0x20B] != 0 || ((u8 *)actor)[0x20C] != 0 || ((u8 *)actor)[0x20D] != 0 || ((u8 *)actor)[0x20E] != 0) {
                    list[count] = i;
                    count++;
                }
            }
        }
    }
    if (count != 0) {
        for (i = 0; i < count; i++) {
            idx = list[i];
            actor = &D_800CC2D0[idx];
            for (j = 0; j < 4; j++) {
                if (((u8 *)actor)[0x20B + j] != 0) {
                    other = func_15083E90(((u8 *)actor)[0x20B + j]);
                    if (other != NULL && (other->unk2FA & 1)) {
                        actor->unk2FA |= 1;
                        break;
                    }
                }
            }
        }
    }
}

typedef struct Game83300Step4 {
    u8 b[4];
} Game83300Step4;
extern Game83300Step4 D_800860F0;

void func_15062D10(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_15062E24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_15062FC0(Actor *actor, s32 group, u8 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt);
void func_1501B660(Actor *arg0, f32 *arg1, s32 arg2);
f32 func_150489B0(u8 arg0);
void func_150F1CB0(Actor *arg0);
extern s32 *D_800C4020[];
extern s32 D_8009942C[];

/* Per-frame texture/morph animations of actor types that have them (once per type for most). */
void func_15061B4C(void) {
    Actor *unused; // matching: stack slot
    s32 id;
    s32 i;
    u8 done[10];

    bzero(done, sizeof(done));
    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state == 0) || (D_800CC2D0[i].unk2FA == 0)) {
            continue;
        }
        id = D_800CC2D0[i].id;
        if ((id == 0x13) && (done[0] == 0)) {
            func_15062D10(id, 0x5E, 0, -1, 0, 0);
            func_15062D10(id, 0x60, 0, -1, 0, 0);
            func_15062D10(id, 0x62, 0, -1, 0, 0);
            func_15062D10(id, 0x64, 0, -1, 0, 0);
            done[0] = 1;
        } else if ((id == 0x23) && (done[1] == 0)) {
            func_15062D10(id, 0x1E9, 0, -1, 0, 0);
            func_15062D10(id, 0x1EB, 0, -1, 0, 0);
            func_15062D10(id, 0x1ED, 0, -1, 0, 0);
            func_15062D10(id, 0x1EF, 0, -1, 0, 0);
            done[1] = 1;
        } else if ((id == 0x24) && (done[2] == 0)) {
            if ((func_150ADA20() % 1000U) < 300) {
                D_800CC298.unk8 = (func_150ADA20() % 5U) - 2;
            }
            func_15062D10(id, 4, -4, D_800CC298.unk8, 0, 0);
            if (D_800C4020[id] != NULL) {
                func_15062E24(id, D_800C4020[id][0], 0x37, 0, D_800CC298.unk4, 2, 1);
                func_15062E24(id, D_800C4020[id][1], 0x27, 0x37, D_800CC298.unk0, 1, 0);
            }
            D_800CC298.unk0 += -4.0f * D_800BE9E4;
            if (D_800CC298.unk0 <= 0.0f) {
                D_800CC298.unk0 += 360.0f;
            }
            D_800CC298.unk4 += -0.5f * D_800BE9E4;
            if (D_800CC298.unk4 <= 0.0f) {
                D_800CC298.unk4 += 360.0f;
            }
            done[2] = 1;
        } else if ((id == 0x2C) && (done[3] == 0)) {
            if (D_800CC298.unk9 >= 0x29) {
                D_800CC298.unkA = -1;
            } else if (D_800CC298.unk9 <= 0) {
                D_800CC298.unkA = 1;
            }
            D_800CC298.unk9 += D_800CC298.unkA;
            func_15062D10(id, 0xAA, D_800CC298.unk9, 0, 0, 1);
            done[3] = 1;
        } else if ((id == 0x3A) && (done[4] == 0)) {
            f32 weights[4];
            s32 j;
            Game83300Step4 step = D_800860F0;

            if (D_800CC2D0[i].unk1D4 != NULL) {
                for (j = 0; j < 4; j++) {
                    weights[j] = (func_150489B0(D_800CC298.unkB[j]) + 1.0f) * 0.5f;
                    D_800CC298.unkB[j] += step.b[j];
                }
                func_1501B660(&D_800CC2D0[i], weights, 1);
                done[4] = 1;
            }
        } else if ((id == 0x3F) && (done[5] == 0)) {
            func_15062D10(id, 4, 0, -4, 0, 0);
            done[5] = 1;
        } else if (id == 0x4B) {
            if (done[6] == 0) {
                done[6] = 1;
                func_15062D10(id, 0x173, -4, 0, 0, 0);
                func_15062D10(id, 0x175, -2, 0, 0, 0);
                func_15062D10(id, 0x177, -1, 0, 0, 0);
            }
            if (D_800CC298.unkF >= 4) {
                D_800CC298.unkF = 0;
                if (D_800CC2D0[i].unk68[1] == 5) {
                    D_800CC2D0[i].unk68[1] = 0;
                } else {
                    D_800CC2D0[i].unk68[1]++;
                }
                if (D_800CC2D0[i].unk68[1] >= 3) {
                    D_800CC2D0[i].unk68[0] = 5 - D_800CC2D0[i].unk68[1];
                } else {
                    D_800CC2D0[i].unk68[0] = D_800CC2D0[i].unk68[1];
                }
                D_800CC2D0[i].unk68[0] += 0xF;
            } else {
                D_800CC298.unkF += D_800BE9E4;
            }
        } else if (id == 0x45) {
            s32 val = D_800CC2D0[i].eyeCode[1];

            if (val >= 10) {
                val -= 10;
            } else {
                val = val * 2 + 6;
            }
            D_800CC2D0[i].unk68[0] = val;
        } else if (id == 0x5B) {
            s32 v;
            s32 k;

            // matching: eyeCode[k] / unk68[k] as fields change how IDO strength-reduces k
            for (k = 0; k < 2; k++) {
                v = ((u8 *)&D_800CC2D0[i])[0x6C + k]; // eyeCode[k]
                if (v >= 10) {
                    v -= 10;
                } else {
                    v = D_8009942C[v];
                }
                ((u8 *)&D_800CC2D0[i])[0x68 + k] = v; // unk68[k]
            }
        } else if (id == 0x54) {
            f32 scale;

            if (D_800CC2D0[i].unk2EC != 0) {
                if (D_800CC298.unk10 < 0xEB) {
                    D_800CC298.unk10 += 0x14;
                } else {
                    D_800CC298.unk10 = 0xFF;
                }
            } else if (D_800CC298.unk10 >= 0x15) {
                D_800CC298.unk10 -= 0x14;
            } else {
                D_800CC298.unk10 = 0;
            }
            scale = (u32)D_800CC298.unk10 * 0.003921568859f;
            func_1501B660(&D_800CC2D0[i], &scale, 0);
        } else if (id == 0x8A) {
            s32 delta;
            s32 pad;

            if (D_800BE9F0 != 0x30) {
                if (D_800CC2D0[i].unk84.uh == 1) {
                    func_15062D10(id, 0x253, 0, 0x10, 0, 0);
                    func_15062D10(id, 0x255, 0, 8, 0, 0);
                    func_15062D10(id, 0x257, 0, 4, 0, 0);
                }
            } else {
                if (D_800CC2D0[i].unk84.uh == 1) {
                    delta = D_800BE9E4 * -40;
                } else {
                    delta = 0;
                }
                func_15062FC0(&D_800CC2D0[i], 0, 0, 0x1000, 0x1000, 0xC, 0, delta);
            }
        } else if (id == 0x42) {
            if ((D_800CC2D0[i].unk84.uh == 2) || (D_800CC2D0[i].unk84.uh == 1)) {
                if (D_800CC2D0[i].unk68[0] == 0xD) {
                    D_800CC2D0[i].unk68[0] = 0xC;
                }
                if ((func_150ADA20() & 3) == 0) {
                    if (D_800CC2D0[i].unk68[0] == 0x15) {
                        D_800CC2D0[i].unk68[0] = 0xC;
                    } else {
                        D_800CC2D0[i].unk68[0] = 0x15;
                    }
                }
            } else {
                D_800CC2D0[i].unk68[0] = 0xD;
            }
        } else if (id == 0x7B) {
            func_150F1CB0(&D_800CC2D0[i]);
        } else if ((id == 0x92) && (done[9] == 0)) {
            func_15062D10(id, 4, 0, -2, 0, 0);
            func_15062D10(id, 0x13B, 0, -2, 0, 0);
            func_15062D10(id, 0x20D, 0, -2, 0, 0);
            done[9] = 1;
        } else if (id == 0x43) {
            func_15062D10(id, 4, 0x14, 8, 0, 0);
        }
    }
}
s32 func_150229E4(Actor *);
s32 func_150A6360(void *, void *, f32, f32, f32, f32, f32, f32);

/*
 * Whether the actor should be drawn: TRUE when it has no cull radius (spawn->unk2A) or is a player, when
 * a player is inside its path area, or when it is inside some player's view; FALSE if fully faded.
 */
s32 func_150623F4(Actor *actor) {
    f32 scale;
    s32 sum;
    s32 hidden;
    s32 i;
    u8 idx;
    f32 dx;
    f32 dz;
    f32 height;
    s32 id;
    u8 *p;

    scale = (actor->spawn != NULL) ? (f32)actor->spawn->unk2A : 0.0f;
    if (scale == 0.0f) {
        return 1;
    }
    if (actor->unk127 != 0xFF) {
        return 1;
    }
    if ((D_800C3638 != 0) && (func_150229E4(actor) != 0)) {
        return 1;
    }
    sum = 0;
    p = (u8 *)actor;
    if (D_80082FA0 >= 0) {
        do {
            sum += p[0xB];
            p++;
        } while (p <= (0, D_80082FA0) + (u8 *)actor); // matching: the comma expression shapes the loop test
    }
    if (sum == 0) {
        return 0;
    }
    if ((actor->unk74 & 0xF) != 0xF) {
        return 1;
    }
    idx = actor->unk13F;
    if (D_800D2108[idx] != 0) {
        hidden = 1;
        for (i = 0; i < D_8008FD8C; i++) {
            dx = ((PathPoint **)D_800D2104)[idx]->x - D_800CC2D0[i].x_position;
            dz = ((PathPoint **)D_800D2104)[idx]->z - D_800CC2D0[i].z_position;
            if ((dx * dx) + (dz * dz) < ((PathPoint **)D_800D2104)[idx]->param * ((PathPoint **)D_800D2104)[idx]->param) {
                hidden = 0;
                break;
            }
        }
        if (!hidden) {
            return 1;
        }
    }
    id = actor->id;
    height = *(s16 *)((u8 *)D_800D1C90[id] + 0x10) * actor->y_scale;
    for (i = 0; i <= D_80082FA0; i++) {
        hidden = 1;
        if (func_150A6360(i * 0x180 + (u8 *)D_800BE628, D_800D9C10[i], actor->x_position,
                          actor->y_position + height, actor->z_position, scale, scale, 20000.0f) != 0) {
            hidden = 0;
            break;
        }
    }
    if (!hidden) {
        return 1;
    }
    return 0;
}
/* Removes the non-player actors carried by this one. */
void func_150626EC(Actor *actor, s32 mode) {
    s32 i;

    for (i = 0; i < 25; i++) {
        if ((D_800CC2D0[i].interaction_state != 0) &&
            (((actor - D_800CC2D0) + 1) == D_800CC2D0[i].unk65) &&
            (D_800CC2D0[i].unk127 == 0xFF)) {
            func_15060F28(&D_800CC2D0[i], mode);
        }
    }
}

/* Called after the actor's animation changed. */
void func_150627D4(Actor *actor) {
    actor->unk2FB = 0;
    func_1503B840(actor);
    func_15039CC8(actor);
}

void func_15033EC4(s32 arg0, s32 arg1);

#define OBJ_INDEX(obj) ((u32)((obj) - D_800CC2D0))

/* Swaps two actor slots (b becomes the player with a camera), fixing up indices and carry links. */
void func_15062800(Actor *a, Actor *b) {
    s32 idx0; // matching: idx0, cam and pad are dead, but keep the frame layout
    s32 cam;
    Actor swap;
    SpawnRecord *pad;
    s32 i;
    s32 u;

    func_15146508(a, b);
    bcopy(b, &swap, sizeof(Actor));
    bcopy(a, b, sizeof(Actor));
    bcopy(&swap, a, sizeof(Actor));
    idx0 = OBJ_INDEX(a);
    cam = (s32)&D_800DBFF0[OBJ_INDEX(b)];
    b->unk127 = OBJ_INDEX(b);
    b->unique_id = OBJ_INDEX(b) + 1;
    b->unk13F = OBJ_INDEX(b);
    b->camera = &D_800DBFF0[OBJ_INDEX(b)];
    a->unk127 = OBJ_INDEX(a);
    a->unique_id = OBJ_INDEX(a) + 1;
    a->unk13F = OBJ_INDEX(a);
    a->camera = NULL;
    u = (s32)a->spawn;
    a->spawn = b->spawn;
    b->spawn = (SpawnRecord *)u;
    D_800DBFF0[OBJ_INDEX(b)].playerExt = b->unk31C;
    b->unk31C->aimState = 0;
    func_15033EC4(b->unique_id, a->unique_id);
    if (b->unk2D4 != NULL) {
        func_1516972C(b->unk2D4);
        b->unk2D4 = NULL;
    }
    if (a->unk2D4 != NULL) {
        func_1516972C(a->unk2D4);
        a->unk2D4 = NULL;
    }
    func_150615DC(b);
    for (i = 0; i < 25; i++) {
        if (i == OBJ_INDEX(b) || i == OBJ_INDEX(a)) {
            continue;
        }
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        u = D_800CC2D0[i].unk65;
        if (u != 0) {
            u--;
            if (u == OBJ_INDEX(b)) {
                D_800CC2D0[i].unk65 = OBJ_INDEX(a) + 1;
            } else if (u == OBJ_INDEX(a)) {
                D_800CC2D0[i].unk65 = OBJ_INDEX(b) + 1;
            }
        }
    }
    if (b->unk13C >= 100) {
        u8 x = b->unk13C - 100;
        D_800CC2D0[x].unk13D = OBJ_INDEX(b) + 100;
    }
    func_150627D4(b);
}

/* Recomputes the body cylinder ratios: unkEC = radius / height, unkF0 = height / radius (radius unkE4, height unkE6). */
void func_15062AC4(Actor *actor) {
    if (actor->unkE6 && actor->unkE4) {
        f32 radius = actor->unkE4;
        actor->unkEC = radius / actor->unkE6;
        actor->unkF0 = actor->unkE6 / radius;
    } else {
        actor->unkF0 = actor->unkEC = 0.0f;
    }
}

/* Sets the body radius to xz_scale * radius. */
void func_15062B1C(Actor *actor, f32 radius) {
    actor->unkE4 = actor->xz_scale * radius;
    func_15062AC4(actor);
}

/* Sets the body height to y_scale * height. */
void func_15062B50(Actor *actor, f32 height) {
    actor->unkE6 = actor->y_scale * height;
    func_15062AC4(actor);
}

/* func_15062AC4 for the hit cylinder (radius unkD2, height unkD4). */
void func_15062B84(Actor *actor) {
    if (actor->unkD4 && actor->unkD2) {
        f32 radius = actor->unkD2;
        actor->unkDC = radius / actor->unkD4;
        actor->unkE0 = actor->unkD4 / radius;
    } else {
        actor->unkDC = 0.0f;
        actor->unkE0 = 0.0f;
    }
}

/* Sets the actor's scale and rescales its hit and body cylinders from the type info. */
void func_15062BDC(Actor *actor, f32 xzScale, f32 yScale) {
    struct124 *info;

    actor->xz_scale = xzScale;
    actor->y_scale = yScale;
    if (actor->id != 255) {
        info = D_800D1C90[actor->id];
        actor->unkD2 = (s32) (info->unk20 * actor->xz_scale);
        actor->unkD4 = (s32) (info->unk22 * actor->y_scale);
        actor->unkD6 = (s32) (info->unk24 * actor->y_scale);
        actor->unkE4 = (s32) (info->unk1A * actor->xz_scale);
        actor->unkE6 = (s32) (info->unk1C * actor->y_scale);
        actor->unkE8 = (s32) (info->unk1E * actor->y_scale);
        func_15062AC4(actor);
        func_15062B84(actor);
    }
}

typedef struct {
    s32 unk0; /* flags in the top byte, s:t packed 12:12 */
    s32 unk4; /* tile size - 2, packed the same way */
} Entry62D10;
extern Entry62D10 **D_800C4488[];
extern s32 D_800D19A0;
/*
 * Scrolls texture tile `tile` of actor type `type` (set `set`) by (ds, dt), wrapping at the tile size;
 * with `absolute` set the coordinates are set instead. Coordinates are packed 12:12 in unk0.
 */
void func_15062D10(s32 type, s32 tile, s32 ds, s32 dt, s32 set, s32 absolute) {
    Entry62D10 *base, *entry;
    s32 packed, wrapS, wrapT, newS, newT, tmp, flag;

    newS = 0;
    wrapS = 0;
    wrapT = 0;
    if ((&D_800D19A0)[type] == 0) {
        return;
    }
    base = D_800C4488[type][set];
    packed = ((Entry62D10 *)((u8 *)base + tile * 8))->unk4; // matching: byte offsets, not base[tile]
    wrapS = ((packed >> 12) & 0xFFF) + 2;
    wrapT = (packed & 0xFFF) + 2;
    entry = ((Entry62D10 *)((u8 *)base + tile * 8));
    packed = entry->unk0;
    flag = absolute;
    if (flag != 0) {
        newS = ds & 0xFFF;
    } else {
        tmp = ((packed >> 12) & 0xFFF) + ds;
        newS = tmp;
        if (wrapS < tmp) {
            newS = tmp - wrapS;
        } else if (tmp < 0) {
            newS = tmp + wrapS;
        }
    }
    if (flag != 0) {
        newT = dt & 0xFFF;
    } else {
        tmp = (packed & 0xFFF) + dt;
        newT = tmp;
        if (wrapT < tmp) {
            newT = tmp - wrapT;
        } else if (tmp < 0) {
            newT = tmp + wrapT;
        }
    }
    packed = (packed & 0xFF000000) | (newT & 0xFFF) | ((newS & 0xFFF) << 12);
    entry->unk0 = packed;
}
typedef struct Entry62E24 {
    u8 pad0[8];
    u16 unk8;
    u16 unkA;
} Entry62E24;

extern Entry62E24 *D_800C5338[];
void func_15094AB8(Vtx *arg0, s16 (*arg1)[2], s32 arg2, f32 arg3, s32 arg4, s32 arg5);

/* Rewrites texture coordinates in the current vertex buffer of every visible actor of `type`. */
void func_15062E24(s32 type, s32 firstVtx, s32 arg2, s32 arg3, s32 arg4, s32 set, s32 doubleSize) {
    s32 i;
    s32 w;
    s32 h;
    Vtx *vtx;

    if (D_800C6360[type] != 0) {
        for (i = 0; i < 25; i++) {
            if (D_800CC2D0[i].interaction_state == 0) continue;
            if (type != D_800CC2D0[i].id) continue;
            if (D_800CC2D0[i].unk2FA == 0) continue;
            w = D_800C5338[type][set].unk8;
            h = D_800C5338[type][set].unkA;
            if (doubleSize != 0) {
                w *= 2;
                h *= 2;
            }
            vtx = ((Vtx **)D_800CC2D0[i].modelData)[D_800BE9C0];
            if (vtx != NULL) {
                vtx += firstVtx;
                func_15094AB8(vtx, (s16 (*)[2])D_800C6360[type] + arg3, arg2, arg4, h, w);
            }
        }
    }
}
extern s32 *D_800C4020[];

/*
 * Scrolls the texture coordinates of `count` vertices (vertex group `group`, or from the start) in the
 * actor's current vertex buffer, copying from the other buffer and wrapping at (wrapS, wrapT).
 */
void func_15062FC0(Actor *actor, s32 group, u8 arg2, s32 wrapS, s32 wrapT, s32 count, s32 ds, s32 dt) {
    u8 *channelBase;
    Vtx *source;
    Vtx *target;
    s32 offset;
    s32 i;

    /* two vertex buffers per channel at 0x28C (double-buffered by D_800BE9C0) */
    channelBase = (u8 *)actor + (actor->lod * 8);
    target = ((Vtx **)(channelBase + 0x28C))[D_800BE9C0];
    source = ((Vtx **)(channelBase + 0x28C))[!D_800BE9C0];
    if (target == NULL) {
        return;
    }
    if (group != -1) {
        offset = D_800C4020[actor->id][group] * sizeof(Vtx);
        target = (Vtx *)((u8 *)target + offset);
        source = (Vtx *)((u8 *)source + offset);
    }
    if (ds != 0) {
        if (wrapS < target->v.tc[0]) {
            ds -= wrapS;
        } else if (target->v.tc[0] < 0) {
            ds += wrapS;
        }
    }
    if (dt != 0) {
        if (wrapT < target->v.tc[1]) {
            dt -= wrapT;
        } else if (target->v.tc[1] < 0) {
            dt += wrapT;
        }
    }
    for (i = 0; i < count; i++) {
        target[i].v.tc[0] = source[i].v.tc[0] + ds;
        target[i].v.tc[1] = source[i].v.tc[1] + dt;
    }
}
void func_15194FF4(Actor *, Actor *, s32);

/* Calls func_15194FF4 for every other player touching the actor this frame (D_800CC268 mask). */
void func_15063168(Actor *actor) {
    s32 i;
    s32 self;
    ActorCtl *movement;

    self = ((s32)actor - (s32)D_800CC2D0) / (s32)sizeof(Actor);
    for (i = 0; i < D_8008FD8C; i++) {
        if (i == self) {
            continue;
        }
        if (((1 << i) & D_800CC268) == 0) {
            continue;
        }
        movement = D_800CC2D0[i].unk31C;
        if (movement == NULL) {
            continue;
        }
        if (movement->unk1AC != 0) {
            continue;
        }
        func_15194FF4(actor, &D_800CC2D0[i], 1);
    }
}

void func_15082A44(void *, s32, s32, s32, s32);
/* Respawns the actor in place: rebuilds its slot from its spawn entry (spawn), keeping position, angles and camera. */
void func_15063254(Actor *actor, s32 type, s32 interaction_state, f32 scale) {
    SpawnRecord *spawn;
    s32 pathIdx;
    s32 index;
    Camera *camera;
    f32 x;
    f32 y;
    f32 z;
    f32 pitch;
    f32 unk40;
    f32 unkC4;
    s32 angle;

    pathIdx = actor->unk13F;
    spawn = actor->spawn;
    spawn->type = type;
    spawn->interaction_state = interaction_state;
    spawn->xz_scale = scale;
    spawn->y_scale = scale;
    spawn->unk2 = 0;
    camera = actor->camera;
    x = actor->x_position;
    y = actor->y_position;
    z = actor->z_position;
    pitch = actor->unkB8;
    unk40 = actor->unk40;
    unkC4 = actor->unkC4;
    angle = actor->unk76;
    index = ((s32)actor - (s32)&D_800CC2D0) / 812;
    func_15060F28(actor, 0);
    func_15082A44(spawn, pathIdx, 0, 0, index + 1);
    actor->camera = camera;
    actor->x_position = x;
    actor->y_position = y;
    actor->z_position = z;
    actor->unkB8 = pitch;
    actor->unk40 = unk40;
    actor->unkC4 = unkC4;
    actor->unk76 = angle;
    actor->unk7A = angle;
    actor->old_x_position = x;
    actor->old_y_position = y;
    actor->old_z_position = actor->z_position;
    actor->unk1CC = actor->y_position;
    actor->camera->playerExt = actor->unk31C;
}
