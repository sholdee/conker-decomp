#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Ground contact found by func_15046C80 below a probe point (as in game_173D40.c). */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 type;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} GroundHit; // size 0x24

void func_15142314(struct255 *bones, s32 bone, vertex *out);
void func_1504715C(GroundHit *hit, struct127 *actor);
s32 func_15046C80(vertex *probe, s32 arg1, f32 bottom, GroundHit *hit);
void func_15141F78(u8, GroundHit *, f32, u8, vertex *, u8);
s32 func_151420F8(struct127 *actor);

/* Get the position of one of the actor's bones (0x21 for side 1, 0x1D for side 2; any other side
   fails with 0).  With a `hit` buffer, also look for the ground below that point and return
   func_15046C80's result.  (Same as func_15146890 with different bones.) */
s32 func_150EAF50(vertex *pos, struct127 *actor, GroundHit *hit, u8 side) {
    vertex probe;
    s32 bone;

    if ((side != 1) && (side != 2)) {
        return 0;
    }

    if (side == 1) {
        bone = 0x21;
    } else {
        bone = 0x1D;
    }

    func_15142314(actor->unk1D4, bone, pos);
    if (hit == NULL) {
        return 1;
    }

    probe.x = pos->x;
    probe.y = pos->y + 10.0f;
    probe.z = pos->z;
    func_1504715C(hit, actor);
    return func_15046C80(&probe, 0, pos->y - 50.0f, hit);
}

/* Effect id for an actor's effect kind: kind 1 in world 4 (D_800BE9F0) is 6 when func_151420F8
   accepts the actor and 3 otherwise; everything else is unmapped (-1). */
s32 func_150EB030(s32 kind, struct127 *actor) {
    if (kind == 1) {
        if (D_800BE9F0 == 4) {
            if (func_151420F8(actor) != 0) {
                return 6;
            }
            return 3;
        }
        return -1;
    }
    return -1;
}

/* Effect 0xA at the ground below the actor's bone on `side`. */
// matching: `actor` is an int cast at each use; with a pointer parameter IDO merges the two
// loads of it that the original makes (as func_15146D60 in game_173D40.c).
void func_150EB090(s32 actor, s32 side, s32 strength) {
    vertex pos;
    GroundHit hit;

    if (((struct127 *)actor)->unk1D4 != NULL) {
        if (func_150EAF50(&pos, (struct127 *)actor, &hit, side) != 0) {
            func_15141F78(0xA, &hit, strength * (1.0f / 85.0f),
                          (u32)(((struct127 *)actor)->unk40 * (256.0f / 360.0f)), &pos, side);
        }
    }
}
