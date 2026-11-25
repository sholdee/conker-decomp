#include <ultra64.h>
#define func_150EB430 func_150EB430_header
#include "functions.h"
#undef func_150EB430
#define D_800D3098 D_800D3098_header
#define D_800A5480 D_800A5480_header
#include "variables.h"
#undef D_800D3098
#undef D_800A5480

/* An entry of the object table at D_800D3098 (struct178, a blob in structs.h): a collision
   cylinder at (x, y, z); see CollisionCylinder in game_D0960.c. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ u8 padA[0x18 - 0xA];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
} GateCylinder; /* size 0x34 */

/* variables.h declares D_800D3098 as an array; the code loads it as a pointer to the table. */
extern GateCylinder *D_800D3098;
extern s32 D_80088AA0[4];
extern s32 D_80088AB0[8];
extern s32 func_150A34B0(GateCylinder *cyl, vertex *start, vertex *end, vertex *hit);
extern void func_150A2864(s32, s32);
s32 func_150EB484(vertex *start, vertex *end, vertex *hit);
void func_150EBC80(GateCylinder *gate, u8 owner, s32 kind);

/* Tests the segment from start to start + delta against the gate cylinders. */
void func_150EB430(vertex *start, vertex *delta, vertex *hit) {
    vertex end;

    end.x = start->x + delta->x;
    end.y = start->y + delta->y;
    end.z = start->z + delta->z;
    func_150EB484(start, &end, hit);
}

/*
 * Tests the segment from start to end against the D_800D3098 cylinders. While object 9 exists,
 * a hit on one of the four D_80088AA0 cylinders disables it (func_150A2864(id, 1)), stores
 * which one (1-4) in the object's unk2E8 and plays func_150EBC80's effect at it; the eight
 * D_80088AB0 cylinders are only tested. Otherwise only cylinder 76 is tested, and a hit sets
 * its unk18. Returns TRUE when object 9 exists and the segment hit one of the twelve.
 */
s32 func_150EB484(vertex *start, vertex *end, vertex *hit) {
    s32 i;
    s32 id;
    struct127 *obj;

    obj = func_15083E90(9);
    if (obj != NULL) {
        for (i = 0; i < 4; i++) {
            id = D_80088AA0[i];
            if (func_150A34B0(&D_800D3098[id], start, end, hit)) {
                func_150A2864(id, 1);
                obj->unk2E8 = i + 1;
                func_150EBC80(&D_800D3098[id], 0xFF, 1);
                return TRUE;
            }
        }
        for (i = 0; i < 8; i++) {
            if (func_150A34B0(&D_800D3098[D_80088AB0[i]], start, end, hit)) {
                return TRUE;
            }
        }
        return FALSE;
    }
    if (func_150A34B0(&D_800D3098[76], start, end, hit)) {
        D_800D3098[76].unk18 = 1;
    }
    return FALSE;
}

/* A scenery object that swings one rotation axis; same layout as MoverObj in game_1449A0.c. */
typedef struct MoverObj {
    /* 0x00 */ f32 rot[3]; /* degrees */
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 pos[3];
    /* 0x16 */ u8 pad16[0x26];
    /* 0x3C */ s32 params; /* low 10 bits: func_15117770's target angle */
    /* 0x40 */ u8 pad40[0xF];
    /* 0x4F */ u8 flags;
    /* 0x50 */ u8 pad50[0x23];
    /* 0x73 */ u8 state;
    /* 0x74 */ u8 pad74[8];
    /* 0x7C */ union {
        struct {
            f32 damping;
            f32 acceleration;
            f32 velocity;
        } swing;
        struct {
            f32 rebound;
            f32 acceleration;
            f32 velocity;
        } bounce;
    } work;
} MoverObj;

void func_15117770(MoverObj *obj);

/*
 * Mover handler. In states 0-1 it swings rot[0] on a damped spring toward an angle set by the
 * player's distance (with flags & 4: -40 degrees at the player rising to -32 at 740 units;
 * otherwise -40). Once the player's unk31C->unk57 is set it switches to state 2, which bounces
 * rot[0] back to 0 with func_15117770 until it stops; then state 3 keeps it at 0.
 */
void func_150EB614(MoverObj *obj) {
    f32 velocity;
    f32 dz;
    f32 target;
    f32 dx;
    f32 damping;
    f32 acceleration;
    f32 dy;
    f32 angle;
    f32 before;
    f32 distance;
    f32 delta;
    f32 magnitude;

    if ((obj->state & 3) == 3) {
        return;
    }
    if ((obj->state & 3) != 2) {
        if (obj->flags & 4) {
            if (D_800CC2D0->unk31C->unk57 != 0) {
                obj->state &= ~3;
                obj->state |= 2;
                obj->params = 0;
                obj->work.bounce.rebound = 0.5f;
                obj->work.bounce.acceleration = 0.3000000119f;
                obj->work.bounce.velocity = 0.0f;
            }
            dz = obj->pos[2] - D_800CC2D0->z_position;
            dx = obj->pos[0] - D_800CC2D0->x_position;
            dy = obj->pos[1] - D_800CC2D0->y_position;
            distance = sqrtf(dz * dz + (dx * dx + dy * dy));
            if (distance > 740.0f) {
                distance = 740.0f;
            }
            target = distance * 0.01081081107f + -40.0f;
        } else {
            target = -40.0f;
        }
        angle = obj->rot[0];
        damping = obj->work.swing.damping;
        acceleration = obj->work.swing.acceleration;
        velocity = obj->work.swing.velocity;
        if (angle != target || velocity != 0.0f) {
            angle += velocity * D_800BE9E4;
            delta = func_15048A70(angle, target);
            magnitude = fabsf(delta);
            if (delta > 0.0f) {
                velocity += magnitude * acceleration;
            } else {
                velocity -= magnitude * acceleration;
            }
            velocity *= damping;
            if (fabsf(delta) < 0.009999999776f && fabsf(velocity) < 0.009999999776f) {
                velocity = 0.0f;
                angle = target;
            } else if (angle < 0.0f) {
                angle += 360.0f;
            } else if (angle >= 360.0f) {
                angle -= 360.0f;
            }
            obj->work.swing.velocity = velocity;
        }
        obj->rot[0] = angle;
    } else {
        before = obj->rot[0];
        func_15117770(obj);
        if (before == obj->rot[0]) {
            obj->state &= ~3;
            obj->state |= 3;
            obj->rot[0] = 0.0f;
        }
    }
}

/* Spawn descriptor for func_1513264C (0x7C bytes; EffectSpawn in game_124920.c). */
typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ vertex unk10;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ vertex pos;
    /* 0x34 */ vertex vel;
    /* 0x40 */ vertex unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ s8 unk69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ struct127 *owner;
    /* 0x70 */ u8 ownerId;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ s32 sound;
} EffectSpawn;

extern u8 D_800BE9EB;
extern vertex D_800A1490;
extern vertex D_800A149C;
extern vertex D_800A14A8[4];
extern u8 D_800A14D8[4];
extern vertex D_800A5480;
void func_150E83AC(vertex *position, s16 duration, u8 owner, s32 kind);
void *func_15164F0C(u8, u8, void *, u8 owner, s32 kind);
void func_151D3FF4(vertex *pos, u8 owner, s32 kind);
void func_151D5514(vertex *pos, u8 owner, s32 kind);
void *func_151541B8(vertex *position, f32 duration, f32 growth, f32 alpha, f32 arg4, u8 owner, s32 kind);
extern void func_15143794(s16 yaw, s16 pitch, f32 length, vertex *out);
extern s32 func_1513264C(EffectSpawn *spawn, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 owner, s32 kind);

/*
 * Plays a fixed burst of effects: func_15164F0C, a func_150E83AC effect at D_800A149C lasting
 * 120-181 frames, the func_151D5514/func_151D3FF4/func_151541B8 trio at D_800A1490, and four
 * func_1513264C debris pieces thrown from the D_800A14A8 points at random yaw/pitch/speed.
 */
void func_150EB8C4(void) {
    EffectSpawn spawn;

    func_15164F0C(2, D_800BE9EB, NULL, 0xFF, 1);
    func_150E83AC(&D_800A149C, func_150ADA20() % 62U + 120, 0xFF, 1);
    func_151D5514(&D_800A1490, 0xFF, 1);
    func_151D3FF4(&D_800A1490, 0xFF, 1);
    func_151541B8(&D_800A1490, func_150ADA68() * 4.0f + 12.0f, 1.640999913f,
                  func_150ADA20() % 56U + 200, 0.0f, 0xFF, 1);
    {
        u8 i;
        s32 yaw;
        s32 pitch;

        spawn.unk0C = 1.0f;
        spawn.unk08 = 1.0f;
        spawn.unk00 = 9.0f;
        spawn.unk04 = 0.4350000322f;
        spawn.unk10 = D_800A5480;
        spawn.unk40.y = 0.0f;
        spawn.unk1C = 1.0f;
        spawn.unk20 = 1.0f;
        spawn.unk24 = 1.0f;
        spawn.unk50 = 0x29E9;
        spawn.unk58 = 0;
        spawn.unk5C = 0;
        spawn.unk60 = 0xFF;
        spawn.unk61 = 13;
        spawn.unk62 = 0;
        spawn.unk63 = 12;
        spawn.unk64 = 0;
        spawn.unk65 = 0;
        spawn.unk66 = 0;
        spawn.unk67 = 0;
        spawn.unk68 = 2;
        spawn.unk6A = 2;
        spawn.owner = NULL;
        spawn.ownerId = 0;
        spawn.unk72 = 25;
        spawn.unk74 = 10;
        spawn.sound = 0;
        // matching: an early use of pitch makes IDO give it $s1 and yaw $s2
        if (pitch) {
        }
        for (i = 0; i < 4; i++) {
            spawn.pos = D_800A14A8[i];
            yaw = func_150ADA20();
            pitch = func_150ADA20();
            func_15143794(yaw % 26U + 45, (pitch & 0xF) - 25, func_150ADA68() * 30.0f + 15.0f, &spawn.vel);
            spawn.unk40.x = func_150ADA68() * 15.10800076f + -7.554000378f;
            spawn.unk40.z = func_150ADA68() * 15.10800076f + -7.554000378f;
            spawn.unk4C = func_150ADA68() * 0.6170000434f + -1.621000051f;
            spawn.unk54 = func_150ADA20() % 51U + 50;
            spawn.unk56 = D_800A14D8[i];
            func_1513264C(&spawn, 3, 0xFF, 0, 0, 0xFF, 1);
        }
    }
}

/* Argument of func_151D8868 (CameraEvent in game_E0F60.c). */
typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 pad1;
    /* 0x2 */ s16 duration;
    /* 0x4 */ u8 unk4;
    /* 0x5 */ u8 cameraMask;
    /* 0x6 */ s8 unk6;
    /* 0x7 */ u8 pad7;
} CameraEvent;

void *func_151D8868(CameraEvent *event, s32 arg1, u8 owner, s32 kind);

/*
 * Effect burst at a gate cylinder: a func_150E83AC effect lasting 300-400 frames, a camera event
 * (type 1, 15-35 long), func_15164F0C, and the func_151D3FF4/func_151D5514/func_151541B8 trio at
 * the cylinder's base and again half its height up.
 */
void func_150EBC80(GateCylinder *gate, u8 owner, s32 kind) {
    vertex position;
    f32 random;
    f32 step;
    CameraEvent event;
    f32 growth;
    u8 i;

    position.x = gate->x;
    position.y = gate->y;
    position.z = gate->z;
    step = gate->height * 0.5f;
    func_150E83AC(&position, func_150ADA20() % 101U + 300, owner, kind);
    event.type = 1;
    event.duration = func_150ADA20() % 21U + 15;
    event.unk4 = 0;
    event.unk6 = -1;
    event.cameraMask = 1;
    func_151D8868(&event, 0, owner, kind);
    func_15164F0C(0, 0, NULL, owner, kind);
    growth = 1.640999913f; // matching: a local (not a literal) is loaded once into a saved FP register
    for (i = 0; i < 2; i++) {
        func_151D3FF4(&position, owner, kind);
        func_151D5514(&position, owner, kind);
        random = func_150ADA68();
        func_151541B8(&position, random * 4.0f + 12.0f, growth,
                      func_150ADA20() % 56U + 200, 0.0f, owner, kind);
        position.y += step;
    }
}
