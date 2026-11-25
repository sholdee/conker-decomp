#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1513D4B8 func_1513D4B8_void_decl
#include "functions.h"
#undef func_150ADA20
#undef func_1513D4B8
s32 func_1513D4B8(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
u32 func_150ADA20(void);
#include "variables.h"


extern f32 sinf(f32);
extern void func_1514373C(f32, f32, f32 *, f32 *);
extern void func_15145974(vertex *, f32 *, f32 *);
extern void func_1000FD38(void *, void *, s32);
extern void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
extern void *func_1515FF74(void *, s32, u8, s32);
extern void func_15160274(s32, s32, s32);
extern void func_1515572C(s32, s32);
extern void func_151A561C(s32, s32);

/* Copy of the 9 s16 words that func_150AC9C0 reports for the surface triangle a ray hit. */
typedef struct {
    s16 v[9];
} SightTriangle;

/* Generic actor fields used here; kind indexes D_800D1C90. */
typedef struct SightActor {
    u8 pad0[4];
    u8 kind;
    u8 pad5[0xF];
    f32 x;
    f32 y;
    f32 z;
    u8 pad20[0x12C];
    f32 scale;
    u8 pad150[0x30];
    f32 unk180;
    u8 pad184[0x154];
    f32 unk2D8;
    u8 pad2DC[0x3C];
    struct108 *player;
} SightActor;

/* Collision query context: set up from an actor by func_1504715C, filled in by func_15046C80. */
typedef struct {
    f32 hitY;
    SightTriangle triangle;
    u8 pad16[6];
    u8 surfaceFlags;
    u8 pad1D[7];
} SightCollision;
extern void func_1504715C(SightCollision *, SightActor *);
extern s32 func_151ABE40(f32 *, SightCollision *, s32, u8, s32);

/* Placement parameters for a sight beam (laser tripwire) between two surfaces. */
typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u8 pad6[2];
    s16 length;
    u8 padA[2];
    f32 rotationX;
    f32 rotationY;
    u8 disabled;
    u8 pad15[3];
    u32 flags;
    u32 colors;
    u32 widths;
} Game1F4650Beam;
extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, void *, void *, s32, f32);
extern s32 func_151C756C(vertex *start, SightTriangle *startTri, vertex *end, SightTriangle *endTri, Game1F4650Beam *beam, Game1F4650Beam *source, u8 red, u8 green, u8 blue, u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks, u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle, f32 pulseSpeed, u8 keepEnabled, u8 arg16, s32 arg17);

/* Casts rays both ways from the beam's midpoint and spawns a sight beam between the two surfaces hit. */
s32 func_151C71A0(Game1F4650Beam *beam) {
    f32 originZ;
    f32 originY;
    f32 originX;
    f32 endZ;
    f32 endY;
    f32 endX;
    f32 dirZ;
    f32 dirY;
    f32 dirX;
    f32 revZ;
    f32 revY;
    f32 revX;
    f32 midZ;
    f32 midY;
    f32 midX;
    f32 matrix[4][4];
    s32 hit1;
    s32 hit2;
    SightTriangle triangle1;
    SightTriangle triangle2;
    f32 point1[3];
    f32 point2[3];
    s32 distance1;
    s32 distance2;
    s32 object1;
    s32 object2;

    if (beam->disabled == 1) {
        return 1;
    }
    originX = beam->x;
    originY = beam->y;
    originZ = beam->z;
    func_150A8050(matrix, beam->rotationX, beam->rotationY, 0);
    func_150A7960(&matrix[0][0], 0.0f, beam->length, 0.0f, &endX, &endY, &endZ);
    endX += originX;
    endY += originY;
    endZ += originZ;
    dirX = endX - originX;
    dirY = endY - originY;
    dirZ = endZ - originZ;
    midX = dirX * 0.5f + originX;
    midY = dirY * 0.5f + originY;
    midZ = dirZ * 0.5f + originZ;
    revX = -dirX;
    revY = -dirY;
    revZ = -dirZ;
    hit1 = func_150AC9C0(midX, midY, midZ, revX, revY, revZ, 0, &triangle1, &point1[0], &point1[1], &point1[2],
                         &distance1, &object1, 0, 0.0f);
    hit2 = func_150AC9C0(midX, midY, midZ, dirX, dirY, dirZ, 0, &triangle2, &point2[0], &point2[1], &point2[2],
                         &distance2, &object2, 0, 0.0f);
    if (hit1 == 0 || hit2 == 0) {
        return 1;
    }
    func_151C756C(point1, &triangle1, point2, &triangle2, beam, beam,
                  beam->colors & 0xFF, (beam->colors & 0xFF00) >> 8, (beam->colors & 0xFF0000) >> 16,
                  (beam->flags & 0x1000000) >> 24,
                  beam->flags & 0xFF, (beam->flags & 0xFF00) >> 8, (beam->flags & 0xFF0000) >> 16,
                  (beam->flags & 0x2000000) >> 25, (beam->flags & 0x4000000) >> 26,
                  !((beam->flags & 0x8000000) >> 27), !((beam->flags & 0x20000000) >> 29),
                  ((beam->flags & 0x40000000) >> 30) ? 1 : 0, (beam->flags & 0x10000000) >> 28,
                  (f32)(beam->widths & 0xFF) * 0.02463994361f,
                  (f32)((beam->widths & 0xFF00) >> 8) * 0.003079992952f,
                  !((beam->flags & 0x80000000) >> 31), 255, 1);
    return 1;
}

typedef struct {
    u8 pad0[9];
    u8 hidden;
} SightModel;

typedef struct {
    u8 pad0[0x6F];
    u8 unk6F;
} SightStatusBlock;

/* An effect object whose model can be hidden. */
typedef struct {
    u8 pad0[0x14];
    SightModel *model;
    SightStatusBlock *status;
} SightModelOwner;

typedef struct {
    u8 pad0[0x58];
    s32 flags;
    u8 pad5C[0x12C];
    SightStatusBlock *status;
} SightStatusObject;

/* Animation parameters copied into an emitter part by func_151C9198. */
typedef struct {
    /* 0x00 */ u8  unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8  unk6;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ u8  unkE;
    /* 0x10 */ s32 unk10;
} SightEmitterAnim;

/* Emitter object created by func_151C9198; flag 0x20000 toggles its flash. */
typedef struct {
    u8 pad0[0x60];
    u32 flags;
    u8 pad64[0x2C];
    u8 parts[2][0x40];
    u8 pad110[0x60];
    SightEmitterAnim anim;
} SightEmitter;

/* Message 0x20 (see func_151C8674). */
typedef struct {
    SightActor *actor;
    Game1F4650Beam *source;
} SightTouchMsg;

/* Message 0x3A: 0 = switch on, 1 = switch off, 2 = trigger. */
typedef struct {
    Game1F4650Beam *source;
    u8 command;
} SightCommandMsg;

/* Visibility record returned by func_1510FD20 for a map position. */
typedef struct {
    u8 pad0[2];
    s16 viewMask;
} SightZone;

/* Sight beam state, stored at +0x110 of the beam object (see func_151C82D0). */
typedef struct {
    f32 texS;
    f32 texT;
    f32 centerS;
    f32 centerT;
    f32 radiusS;
    f32 radiusT;
    f32 angleS;
    f32 angleT;
    f32 speedS;
    f32 speedT;
    Game1F4650Beam *source;
    SightModelOwner *midEffect;
    s32 unk30;
    s32 endEffect0;
    s32 endEffect1;
    SightEmitter *emitter0[3];
    SightEmitter *emitter1[3];
    f32 phase;
    u8 alternate;
    SightTriangle triangle0;
    SightTriangle triangle1;
    s16 activeTime;
    s16 inactiveTime;
    s16 timer;
    s16 period;
    u8 flags;
    Game1F4650Beam *beam;
    SightZone *zone0;
    SightZone *zone1;
    u8 unk94;
    f32 pulseAngle;
    f32 pulseSpeed;
} SightBeamState;

/* Sound handle from func_1000FA64; `state` is the user pointer passed to it. */
typedef struct {
    u8 pad0[0x18];
    SightBeamState *state;
} SightSoundHandle;

/* Spawn descriptor for func_1513D4B8 / func_151407D0; the spawned object holds a copy at +0x18. */
typedef struct {
    u8 kind;
    u8 mode;
    s16 flags;
    s16 lifetime;
    u8 pad06[2];
    s32 callback0;
    s32 callback1;
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
    f32 unk14;
    f32 width;
    vertex start;
    vertex end;
    vertex unk34;
    s32 options;
    u8 brightness;
    u8 unk45;
    u8 unk46;
    u8 unk47;
    SightZone *zone;
    u8 unk4C;
    u8 pad4D[0xB];
} SightSpawnDesc;


extern void func_1510F800(s32);
extern s32 func_15145128(vertex *, vertex *, f32 *, f32 *);
extern SightModelOwner *func_1516284C(Header *, s32 *, s32, s32, s32, u8, u8, s32, u8, u8, s32);
s32 func_151C8FCC(s32 owner, vertex *start, vertex *end, u8 red, u8 green, u8 blue, SightZone *zone, u8 zoneMasked, u8 arg8, s32 arg9);
s32 func_151C87AC(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume);

/* Spawns a sight beam object between two surface points, with emitters at both ends. */
s32 func_151C756C(vertex *start, SightTriangle *startTri, vertex *end, SightTriangle *endTri, Game1F4650Beam *beam, Game1F4650Beam *source, u8 red, u8 green, u8 blue, u8 hasMidEffect, s16 activeTime, s16 inactiveTime, s16 startTime, u8 blinks, u8 startsOff, u8 zoneMasked, u8 hasEndEffects, u8 arg11, u8 pulses, f32 pulseAngle, f32 pulseSpeed, u8 keepEnabled, u8 arg16, s32 arg17) {
    s32 obj;
    SightSpawnDesc spawn;
    SightBeamState state;
    s32 pos[3];
    vertex ndir;

    func_1510F800(0);
    state.zone0 = (SightZone *)func_1510FD20(start->x, start->z);
    state.zone1 = (SightZone *)func_1510FD20(end->x, end->z);
    if (state.zone0 == NULL || state.zone1 == NULL) {
        return 0;
    }
    state.beam = beam;
    // matching: the right-nested grouping fixes the order IDO evaluates the flag bits in
    state.flags = (pulses ? 8 : 0) | ((zoneMasked ? 4 : 0) | ((startsOff ? 0 : 2) | ((blinks ? 1 : 0) | (keepEnabled ? 0x10 : 0))));
    if (!keepEnabled) {
        beam->disabled = 1;
    }
    state.pulseAngle = pulseAngle;
    state.pulseSpeed = pulseSpeed;
    state.unk94 = arg11;
    state.triangle0 = *startTri;
    state.triangle1 = *endTri;
    state.activeTime = activeTime;
    state.inactiveTime = inactiveTime;
    state.timer = startTime;
    state.period = activeTime + inactiveTime;
    state.texS = state.centerS = func_150ADA68() * 1024.0f + -2560.0f + 2048.0f;
    state.texT = state.centerT = func_150ADA68() * 1024.0f + 1536.0f + 2048.0f;
    state.radiusS = func_150ADA68() * 768.0f + 768.0f;
    state.radiusT = func_150ADA68() * 768.0f + 768.0f;
    state.angleS = func_150ADA68() * 6.283185482f;
    state.angleT = func_150ADA68() * 6.283185482f;
    state.speedS = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
    state.speedT = (func_150ADA68() * 80.0f + 50.0f) * 0.001000000047f;
    {
        extern void func_151C9198(SightEmitter **, vertex *, SightTriangle *, vertex *, SightZone *, u8, u8, u8, s32);

        spawn.unk34.x = end->x - start->x;
        spawn.unk34.y = end->y - start->y;
        spawn.unk34.z = end->z - start->z;
        ndir.x = -spawn.unk34.x;
        ndir.y = -spawn.unk34.y;
        ndir.z = -spawn.unk34.z;
        func_151C9198(state.emitter0, start, startTri, &ndir, state.zone0, red < blue, zoneMasked, arg16, 0);
        func_151C9198(state.emitter1, end, endTri, &spawn.unk34, state.zone1, red < blue, zoneMasked, arg16, 0);
        if (state.emitter0[0] == NULL || state.emitter0[1] == NULL || state.emitter0[2] == NULL ||
            state.emitter1[0] == NULL || state.emitter1[1] == NULL || state.emitter1[2] == NULL) {
            if (state.emitter0[0] != NULL) {
                func_1516972C(state.emitter0[0]);
            }
            if (state.emitter0[1] != NULL) {
                func_1516972C(state.emitter0[1]);
            }
            if (state.emitter0[2] != NULL) {
                func_1516972C(state.emitter0[2]);
            }
            if (state.emitter1[0] != NULL) {
                func_1516972C(state.emitter1[0]);
            }
            if (state.emitter1[1] != NULL) {
                func_1516972C(state.emitter1[1]);
            }
            if (state.emitter1[2] != NULL) {
                func_1516972C(state.emitter1[2]);
            }
            return 0;
        }
    }
    state.alternate = 0;
    state.phase = 0.0f;
    state.source = source;
    if (hasMidEffect) {
        Header hdr;

        pos[0] = start->x + spawn.unk34.x * 0.5f;
        pos[1] = start->y + spawn.unk34.y * 0.5f;
        pos[2] = start->z + spawn.unk34.z * 0.5f;
        hdr.unk0 = 2;
        hdr.unk1 = 2;
        hdr.unk2 = 0x12C;
        hdr.unk4 = 0;
        state.midEffect = func_1516284C(&hdr, pos, red, green, blue, 0xFF, 0, 0, 0x10, arg16, arg17);
    } else {
        state.midEffect = NULL;
    }
    spawn.kind = 0x59;
    spawn.mode = 6;
    spawn.flags = 0x5A03;
    spawn.lifetime = 0x12C;
    spawn.callback0 = 0;
    spawn.callback1 = 0;
    spawn.red = red;
    spawn.green = green;
    spawn.blue = blue;
    spawn.alpha = 0xFF;
    spawn.unk14 = 0.0f;
    spawn.width = 20.0f;
    spawn.start = *start;
    spawn.end = *end;
    spawn.options = (D_800BE9F0 == 0x36 ? 0 : 0x02000000) | 0x6C0000 | 0x40000000;
    spawn.brightness = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 6;
    obj = func_1513D4B8((s32)&spawn, (s32)&D_800A4AA0, 0x1A, 0, 0, 0x18, 0, 0xA0, arg16, arg17);
    if (obj != 0) {
        SightBeamState *objState;
        vertex dir;
        f32 len;
        f32 inv;
        vertex p0;
        vertex p1;
        vertex q0;
        vertex q1;

        {
            // matching: a block-scope declaration gives IDO a scope boundary here
            extern s32 D_800BE9F0;

            objState = (SightBeamState *)(obj + 0x110);
            memcpy(objState, &state, sizeof(SightBeamState));
        }
        dir = spawn.unk34;
        if (func_15145128(&dir, &dir, &len, &inv) != 0 && hasEndEffects) {
            p0.x = start->x + dir.x * 20.0f;
            p0.y = start->y + dir.y * 20.0f;
            p0.z = start->z + dir.z * 20.0f;
            p1.x = p0.x + dir.x * 500.0f;
            p1.y = p0.y + dir.y * 500.0f;
            p1.z = p0.z + dir.z * 500.0f;
            q0.x = end->x - dir.x * 20.0f;
            q0.y = end->y - dir.y * 20.0f;
            q0.z = end->z - dir.z * 20.0f;
            q1.x = q0.x - dir.x * 500.0f;
            q1.y = q0.y - dir.y * 500.0f;
            q1.z = q0.z - dir.z * 500.0f;
            objState->endEffect0 = func_151C8FCC(obj, &p0, &p1, red, green, blue, state.zone0, zoneMasked, arg16, arg17);
            objState->endEffect1 = func_151C8FCC(obj, &q0, &q1, red, green, blue, state.zone1, zoneMasked, arg16, arg17);
        } else {
            objState->endEffect0 = 0;
            objState->endEffect1 = 0;
        }
        if (objState->midEffect != NULL) {
            func_1000FA64(0x4D0, pos[0], pos[1], pos[2], 0x1F40, 0xFA, 0x64, (s32)func_151C87AC, objState, 0, 8, (u8)func_150ADA20());
        }
    }
    return obj;
}

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    SightSpawnDesc spawn;
    u8 pad70[0x50];
    u8 vtxTemplate[0x40];
    u8 *vtxBuffers[4];
    SightBeamState state;
} SightBeamObject;

extern vertex *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
void func_15145548(vertex *arg0, vertex *arg1, vertex *arg2, vertex *arg3, f32 *arg4);

/* Builds the beam's quad for one view: a strip of the beam's width facing the camera. */
void *func_151C7E98(SightBeamObject *obj, s16 view) {
    Vtx *vertices;
    void *result;
    vertex *camera;
    vertex dir;
    vertex closest;
    f32 dx, dy, dz;
    f32 nx, ny, nz;
    f32 lengthSquared;
    f32 scale;
    u8 fresh;
    SightBeamState *state;

    state = &obj->state;
    if (!(state->flags & 2)) {
        return NULL;
    }
    if (state->flags & 4) {
        if (!(state->zone0->viewMask & (1 << view)) && !(state->zone1->viewMask & (1 << view))) {
            return NULL;
        }
    }
    func_151D5D60(obj->vtxBuffers, view, 0x40, (void **)&vertices, &fresh);
    result = vertices;
    if (vertices != NULL) {
        if (fresh) {
            memcpy(obj->vtxBuffers[view], obj->vtxTemplate, 0x40);
            memcpy(obj->vtxBuffers[view] + 0x40, obj->vtxTemplate, 0x40);
        }
    } else {
        return NULL;
    }
    camera = func_15144B34(view);
    dir.x = obj->spawn.end.x - obj->spawn.start.x;
    dir.y = obj->spawn.end.y - obj->spawn.start.y;
    dir.z = obj->spawn.end.z - obj->spawn.start.z;
    func_15145548(&obj->spawn.start, &dir, camera, &closest, 0);
    dx = closest.x - camera->x;
    dy = closest.y - camera->y;
    dz = closest.z - camera->z;
    nx = dir.y * dz - dy * dir.z;
    ny = dir.z * dx - dz * dir.x;
    nz = dir.x * dy - dx * dir.y;
    lengthSquared = nx * nx + ny * ny + nz * nz;
    if (lengthSquared == 0.0f) {
        nx = 0.0f;
        ny = 0.0f;
        nz = 0.0f;
    } else {
        scale = obj->spawn.width / sqrtf(lengthSquared);
        nx *= scale;
        ny *= scale;
        nz *= scale;
    }
    vertices->v.ob[0] = obj->spawn.end.x - nx;
    vertices->v.ob[1] = obj->spawn.end.y - ny;
    vertices->v.ob[2] = obj->spawn.end.z - nz;
    vertices->v.tc[0] = state->texS;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->spawn.start.x - nx;
    vertices->v.ob[1] = obj->spawn.start.y - ny;
    vertices->v.ob[2] = obj->spawn.start.z - nz;
    vertices->v.tc[0] = state->texT;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->spawn.start.x + nx;
    vertices->v.ob[1] = obj->spawn.start.y + ny;
    vertices->v.ob[2] = obj->spawn.start.z + nz;
    vertices->v.tc[0] = state->texT;
    vertices->v.flag = 0;
    vertices++;
    vertices->v.ob[0] = obj->spawn.end.x + nx;
    vertices->v.ob[1] = obj->spawn.end.y + ny;
    vertices->v.ob[2] = obj->spawn.end.z + nz;
    vertices->v.tc[0] = state->texS;
    vertices->v.flag = 0;
    return result;
}

/* Per-frame update: blink timer, texture scroll, emitter flashing and brightness pulse. */
s32 func_151C82D0(SightBeamObject *obj) {
    SightBeamState *state = &obj->state;
    s16 period;
    SightModelOwner *midEffect;
    Game1F4650Beam *beam;
    SightEmitter *emitter0;
    SightEmitter *emitter1;
    SightEmitter *emitter2;
    SightEmitter *emitter3;

    if (state->flags & 1) {
        period = state->period;
        state->timer += D_800BE9E4;
        while (state->timer >= period) {
            state->timer -= period;
        }
        if (state->timer < state->activeTime) {
            state->flags |= 2;
        } else {
            state->flags &= ~2;
        }
    }
    if (state->flags & 2) {
        state->angleS += state->speedS * D_800BE9A4;
        state->angleT += state->speedT * D_800BE9A4;
        state->angleS = func_15144B68(state->angleS);
        state->angleT = func_15144B68(state->angleT);
        state->texS = func_150AD78C(state->angleS) * state->radiusS + state->centerS;
        state->texT = func_150AD78C(state->angleT) * state->radiusT + state->centerT;
        midEffect = state->midEffect;
        if (midEffect != NULL) {
            midEffect->model->hidden = 0;
        }
        beam = state->beam;
        if (beam != NULL && (state->flags & 0x10)) {
            beam->disabled = 0;
        }
    } else {
        midEffect = state->midEffect;
        if (midEffect != NULL) {
            midEffect->model->hidden = 1;
        }
        beam = state->beam;
        if (beam != NULL) {
            beam->disabled = 1;
        }
    }
    state->phase += 0.08000000566f * D_800BE9A4;
    while (state->phase > 1.0f) {
        if (state->flags & 2) {
            state->alternate ^= 1;
        }
        state->phase -= 1.0f;
    }
    if (state->alternate) {
        emitter0 = state->emitter0[1];
        emitter0->flags &= ~0x20000;
        emitter1 = state->emitter0[2];
        emitter1->flags |= 0x20000;
        emitter2 = state->emitter1[1];
        emitter2->flags &= ~0x20000;
        emitter3 = state->emitter1[2];
        emitter3->flags |= 0x20000;
    } else {
        emitter0 = state->emitter0[1];
        emitter0->flags |= 0x20000;
        emitter1 = state->emitter0[2];
        emitter1->flags &= ~0x20000;
        emitter2 = state->emitter1[1];
        emitter2->flags |= 0x20000;
        emitter3 = state->emitter1[2];
        emitter3->flags &= ~0x20000;
    }
    if (state->flags & 8) {
        obj->spawn.brightness = func_150AD78C(state->pulseAngle) * 112.5f + 142.5f;
        state->pulseAngle = func_15144B68(state->pulseAngle + state->pulseSpeed * D_800BE9A4);
    }
    return 1;
}

/* Tells sight beams that `actor` moved, so beams keyed to `source` can test it (message 0x20). */
void func_151C8674(s32 actor, s32 source) {
    struct {
        s32 actor;
        s32 source;
    } msg;
    if (actor != 0) {
        msg.actor = actor;
        msg.source = source;
        func_151403A8((s32)&msg, 0x20);
    }
}

s32 func_151C87E0(SightActor *actor, SightBeamObject *obj);
void func_151C899C(SightActor *actor, SightBeamObject *obj);

/* Message handler of a sight beam object. */
void func_151C86AC(SightBeamObject *obj, void *msg, u8 msgId) {
    // matching: taking obj's address keeps it in its stack slot, reloaded at each use
    SightBeamObject **objp = &obj;
    SightBeamState *state;

    switch (msgId) {
    case 0x20:
        state = &(*objp)->state;
        if ((state->flags & 0x10) && ((SightTouchMsg *)msg)->source == state->source) {
            if (func_151C87E0(((SightTouchMsg *)msg)->actor, *objp) != 0) {
                func_151C899C(((SightTouchMsg *)msg)->actor, *objp);
            }
        }
        break;
    case 0x3A:
        state = &(*objp)->state;
        if (((SightCommandMsg *)msg)->source == state->source) {
            switch (((SightCommandMsg *)msg)->command) {
            case 0:
                state->flags |= 2;
                break;
            case 1:
                state->flags &= ~2;
                break;
            case 2:
                func_151C899C(NULL, *objp);
                break;
            }
        }
        break;
    }
}

/* Sound callback (registered by func_151C756C): mutes the hum while the beam is off. */
s32 func_151C87AC(SightSoundHandle *sound, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s16 *volume) {
    SightBeamState *state = sound->state;
    if (!(state->flags & 2)) {
        *volume = 0;
    }
    return 0;
}

extern void func_1507C3E0(void *, s16 *, s16 *, s16 *);
extern s32 func_151451F0(vertex *, vertex *, vertex *, f32, f32, vertex *, vertex *, f32 *, f32 *);

/* Returns whether the beam segment passes through the actor's bounding sphere. */
s32 func_151C87E0(SightActor *actor, SightBeamObject *obj) {
    s32 kind;
    f32 radius;
    f32 scale;
    vertex center;
    f32 halfHeight;
    vertex start;
    f32 endX;
    f32 endY;
    f32 endZ;
    vertex dir;
    f32 length;
    vertex nearHit;
    vertex farHit;
    f32 nearT;
    f32 farT;
    s16 height;
    s16 width;
    s16 depth;

    kind = actor->kind;
    if (kind < 187 && kind != 0xFF) {
        radius = actor->scale * D_800D1C90[kind]->unk1A;
    } else {
        radius = 50.0f;
    }
    func_1507C3E0(actor, &height, &width, &depth);
    if ((f32)height == 0.0f) {
        return 0;
    }
    halfHeight = (f32)height * 0.5f;
    center.x = actor->x;
    center.y = actor->y + halfHeight;
    center.z = actor->z;
    scale = radius / halfHeight;
    start.x = obj->spawn.start.x;
    start.y = (obj->spawn.start.y - center.y) * scale + center.y;
    start.z = obj->spawn.start.z;
    endX = obj->spawn.end.x;
    endY = (obj->spawn.end.y - center.y) * scale + center.y;
    endZ = obj->spawn.end.z;
    dir.x = endX - start.x;
    dir.y = endY - start.y;
    dir.z = endZ - start.z;
    {
        f32 invLength;
        if (func_15145128(&dir, &dir, &length, &invLength) == 0) {
            return 0;
        }
        if (func_151451F0(&start, &dir, &center, radius, length, &nearHit, &farHit, &nearT, &farT) != 0) {
            return 1;
        }
    }
    return 0;
}

/* Parameters for func_1514FBFC (a burst of debris off a surface triangle). */
typedef struct {
    vertex dir;
    SightTriangle triangle;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    vertex pos;
    f32 unk38, unk3C, unk40, unk44, unk48, unk4C;
    s32 unk50, unk54;
    f32 unk58, unk5C, unk60, unk64;
    s16 unk68, unk6A, unk6C, unk6E, unk70, unk72;
    s8 unk74;
} SightBurst;
typedef struct { s8 a, b; s16 c; s8 d; } S60_899C;
typedef struct { s8 a; s16 b; s8 c, d, e; } S58_899C;
extern u8 *D_800AAC30[];
extern void func_1514FBFC(void *, u8, s32);
extern void func_151602C0(void *, s32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_150E7FEC(f32, s32, void *, void *, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15164F0C(s32, u8, s32, u8, s32);
extern void func_151D8868(void *, s32, s32, s32);
extern void func_1505D024(s32, u8 *, s32, s32);
extern void func_1000EC24();

/* Breaks the beam: debris and sparks at both ends, sounds, and (if `actor` tripped it) a reaction. */
void func_151C899C(SightActor *actor, SightBeamObject *obj) {
    SightBeamState *state;
    SightBurst burst;
    s32 pos[3];
    S60_899C s60;
    S58_899C s58;

    state = &obj->state;
    burst.unk20 = 491.0f;
    burst.unk38 = 0.06049999967f;
    burst.unk3C = 0.0522999987f;
    burst.unk40 = 30.0f;
    burst.unk44 = 83.0f;
    burst.unk24 = 5;
    burst.unk28 = 3;
    burst.unk50 = 5;
    burst.unk54 = 4;
    burst.unk68 = 18;
    burst.unk6A = 9;
    burst.unk6C = 100;
    burst.unk6E = 100;
    burst.unk70 = 12;
    burst.unk72 = 15;
    burst.unk74 = 0;
    burst.unk48 = 211.0f;
    burst.unk4C = 150.0f;
    burst.unk58 = 21.0f;
    burst.unk5C = 0.6980000138f;
    burst.unk60 = -0.4450000226f;
    burst.unk64 = 0.4410000145f;
    burst.pos = obj->spawn.start;
    burst.dir = obj->spawn.unk34;
    burst.triangle = state->triangle0;
    func_1514FBFC(&burst, obj->unkC, obj->unk1);
    burst.pos = obj->spawn.end;
    burst.dir.x = -obj->spawn.unk34.x;
    burst.dir.y = -obj->spawn.unk34.y;
    burst.dir.z = -obj->spawn.unk34.z;
    burst.triangle = state->triangle1;
    func_1514FBFC(&burst, obj->unkC, obj->unk1);
    pos[0] = obj->spawn.start.x;
    pos[1] = obj->spawn.start.y;
    pos[2] = obj->spawn.start.z;
    s60.a = 3;
    s60.b = -1;
    s60.c = func_150ADA20() % 9U + 17;
    s60.d = 0;
    func_151602C0(&s60, pos, func_150ADA20() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, obj->unkC, obj->unk1);
    pos[0] = obj->spawn.end.x;
    pos[1] = obj->spawn.end.y;
    pos[2] = obj->spawn.end.z;
    func_151602C0(&s60, pos, func_150ADA20() % 9U + 5, 0xFF, 0x59, 0, 0xFF, 0, 0, obj->unkC, obj->unk1);
    func_150E7FEC(func_150ADA68() * 101.0f + 55.0f, (u8)(func_150ADA20() % 66U + 190), &state->triangle0, &obj->spawn.start, -1, 1, 1, 0, 0, 0, obj->unkC, 0);
    func_150E7FEC(func_150ADA68() * 101.0f + 55.0f, (u8)(func_150ADA20() % 66U + 190), &state->triangle1, &obj->spawn.end, -1, 1, 1, 0, 0, 0, obj->unkC, 0);
    if (actor != NULL) {
        func_15164F0C(2, actor->player->unk23D, 0, obj->unkC, obj->unk1);
        s58.a = 1;
        s58.b = 40;
        s58.d = 1;
        s58.c = 8;
        s58.e = -1;
        func_151D8868(&s58, 0, 0xFF, 0);
    }
    if (actor != NULL) {
        func_1505D024((s32)actor, D_800AAC30[state->unk94], 0, -1);
    }
    func_10010F88(0x2F5, 0x7FFF, func_150ADA20() % 500U, 0, 0, obj->spawn.start.x, obj->spawn.start.y, obj->spawn.start.z, 2000, 2200);
    func_1000FA64(0x2F6, obj->spawn.end.x, obj->spawn.end.y, obj->spawn.end.z, 0x7FFF, 2200, 2000, (s32)&func_1000EC24, (void *)5, 0, 0, func_150ADA20() % 500U);
    func_1516972C((struct102 *)obj);
}


typedef struct {
    f32 unk00;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    f32 unk48;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    s8 unk59;
    u8 pad5A[2];
} SightExtensionParams;


void *func_151407D0(void *, s32, void *, s32, s32, s32, s32, s32, s32, s32);

/* Effect spawned by func_151C8FCC; +0x170 holds the beam object that owns it. */
typedef struct {
    u8 pad0[0x170];
    SightBeamObject *owner;
} SightExtension;

/* Spawns a beam extension effect from `start` to `end`, owned by beam object `owner`. */
s32 func_151C8FCC(s32 owner, vertex *start, vertex *end, u8 red, u8 green, u8 blue, SightZone *zone, u8 zoneMasked, u8 arg8, s32 arg9) {
    void *effect;
    SightExtensionParams params;
    SightSpawnDesc desc;
    s32 ownerCopy; // matching: copied to the stack before the memcpy below

    desc.start = *start;
    desc.end = *end;
    ownerCopy = owner;
    params.unk00 = params.unk04 = 30.10000038f;
    params.unk08 = params.unk0C = 40.0f;
    params.unk18 = 1.0f;
    params.unk10 = 0.0f;
    params.unk14 = 0.0f;
    desc.kind = 0x15;
    desc.flags = 0x3403;
    desc.lifetime = 300;
    desc.callback0 = 0;
    desc.callback1 = 0;
    desc.red = red;
    params.unk1C = 0.3000000119f;
    desc.green = green;
    desc.blue = blue;
    desc.alpha = 0xFF;
    desc.unk14 = 0.0f;
    desc.width = 0.0f;
    desc.unk34.x = 1.0f;
    desc.unk34.y = 1.0f;
    desc.unk34.z = 1.0f;
    desc.options = (zoneMasked ? 0x01000000 : 0) | 0x024D2006 | 0x40000000;
    desc.unk45 = 0xFF;
    desc.brightness = 0xFF;
    desc.unk46 = 0;
    desc.unk47 = 6;
    desc.mode = 3;
    params.unk54 = 0;
    params.unk55 = 0;
    params.unk56 = 0;
    params.unk57 = 0;
    params.unk48 = 1.0f;
    params.unk58 = 0;
    desc.zone = zone;
    params.unk20 = 9.999999975e-07f;
    effect = func_151407D0(&params, 100, &desc, 0, 0, 0, 0, 1, arg8, arg9);
    if (effect != NULL) {
        memcpy((u8 *)effect + 0x170, &ownerCopy, 4);
    }
    return (s32)effect;
}

/* Spawn descriptor for func_15132A4C. */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 pitch;
    /* 0x14 */ f32 yaw;
    /* 0x18 */ f32 roll;
    /* 0x1C */ f32 scale[3];
    /* 0x28 */ vertex position;
    /* 0x34 */ vertex unk34;
    /* 0x40 */ vertex unk40;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 flags;
    /* 0x54 */ s16 lifetime;
    /* 0x56 */ s16 model;
    /* 0x58 */ u8  unk58;
    /* 0x59 */ u8  pad59[3];
    /* 0x5C */ SightZone *zone;
    /* 0x60 */ u8  unk60[8];
    /* 0x68 */ u8  unk68;
    /* 0x69 */ s8  unk69;
    /* 0x6A */ u8  unk6A;
    /* 0x6B */ u8  pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8  unk70;
    /* 0x71 */ u8  pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8  pad76[2];
} SightEmitterDesc;

extern s32 func_15144E80(SightTriangle *, vertex *, vertex *, vertex *);
extern void *func_15132A4C(void *, s32, s32, s32, u8, s32);
extern void func_15133760(void *, void *);

/* Spawns the three parts of a beam emitter on the surface triangle at `pos`. */
void func_151C9198(SightEmitter **emitters, vertex *pos, SightTriangle *triangle, vertex *dir, SightZone *zone, u8 altColors, u8 zoneMasked, u8 arg7, s32 arg8) {
    vertex edge0;
    vertex edge1;
    vertex normal;
    s32 i;
    SightEmitterDesc desc;
    void *unused; // matching: reserves a stack slot
    SightEmitterAnim anim;

    if (func_15144E80(triangle, &edge0, &edge1, &normal) != 0) {
        func_15145974(&normal, &desc.yaw, &desc.pitch);
        desc.unk0 = 1.0f;
        desc.unk4 = 1.0f;
        desc.scale[0] = 1.0f;
        desc.scale[1] = 1.0f;
        desc.pitch += 90.0f;
        desc.scale[2] = 1.0f;
        desc.unkC = 0.4000000059604645f;
        desc.unk8 = 0.4000000059604645f;
        desc.roll = 0.0f;
        desc.position = *pos;
        desc.unk34.x = 0.0f;
        desc.unk34.y = 0.0f;
        desc.unk34.z = 0.0f;
        desc.unk40.x = 0.0f;
        desc.unk40.y = 0.0f;
        desc.unk40.z = 0.0f;
        desc.unk4C = 0.0f;
        desc.flags = (zoneMasked ? 0x400 : 0) | 0x100 | 0x800 | 0x10000 | 0x4000 | 0x40000 | 0x80000;
        desc.lifetime = 300;
        desc.model = 0x36;
        desc.unk58 = 0;
        desc.zone = zone;
        desc.unk60[0] = 0xFF;
        desc.unk60[1] = 0;
        desc.unk60[2] = 0;
        desc.unk60[3] = 0;
        desc.unk60[4] = 0;
        desc.unk60[5] = 0;
        desc.unk60[6] = 0;
        desc.unk60[7] = 0;
        desc.unk68 = 2;
        desc.unk6A = 0;
        desc.unk6C = 0;
        desc.unk70 = 0;
        desc.unk72 = 1;
        desc.unk74 = 0xFF;
        desc.unk69 = -1;
        emitters[0] = func_15132A4C(&desc, 3, 0xFF, 0, arg7, arg8);
        if (emitters[0] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760(emitters[0]->parts[i], emitters[0]);
            }
        }
        desc.unk69 = 2;
        anim.unk0 = 2;
        anim.unk6 = 4;
        anim.unkE = 6;
        anim.unk8 = 0x3E;
        anim.unk10 = 0x3E;
        if (altColors) {
            anim.unk4 = 0x87;
            anim.unkC = 0x88;
        } else {
            anim.unk4 = 0x8B;
            anim.unkC = 0x8C;
        }
        desc.model = 0x37;
        desc.unk68 = 0;
        emitters[1] = func_15132A4C(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (emitters[1] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760(emitters[1]->parts[i], emitters[1]);
            }
            memcpy(&emitters[1]->anim, &anim, sizeof(SightEmitterAnim));
        }
        desc.model = 0x38;
        if (altColors) {
            anim.unk4 = 0x89;
            anim.unkC = 0x8A;
        } else {
            anim.unk4 = 0x8D;
            anim.unkC = 0x8E;
        }
        emitters[2] = func_15132A4C(&desc, 3, 0xFF, 0x14, arg7, arg8);
        if (emitters[2] != NULL) {
            for (i = 0; i < 2; i++) {
                func_15133760(emitters[2]->parts[i], emitters[2]);
            }
            memcpy(&emitters[2]->anim, &anim, sizeof(SightEmitterAnim));
        }
    }
}

/* Frees the beam's effects and emitters, then removes the beam object with func_1513CA6C. */
void func_151C94D4(SightBeamObject *obj) {
    // matching: one register first holds the object, then its state (as IDO allocated it)
    register SightBeamState *state;
    void *part;

    state = &obj->state;
    if (obj->state.midEffect != NULL) {
        func_1516972C((struct102 *)state->midEffect);
        func_1000FD38(func_151C87AC, state, 0);
    }
    state = *(SightBeamState **)&obj; // matching: read through &obj so IDO reloads it into this register
    part = (void *)((SightBeamObject *)state)->state.endEffect0;
    state = &((SightBeamObject *)state)->state;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = (void *)state->endEffect1;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[0];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[1];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[2];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[0];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[1];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[2];
    if (part != NULL) {
        func_1516972C(part);
    }
    func_1513CA6C(obj);
}

/* Same as func_151C94D4, but finishes with func_1513CAA0. */
void func_151C95D8(SightBeamObject *obj) {
    // matching: one register first holds the object, then its state (as IDO allocated it)
    register SightBeamState *state;
    void *part;

    state = &obj->state;
    if (obj->state.midEffect != NULL) {
        func_1516972C((struct102 *)state->midEffect);
        func_1000FD38(func_151C87AC, state, 0);
    }
    state = *(SightBeamState **)&obj; // matching: read through &obj so IDO reloads it into this register
    part = (void *)((SightBeamObject *)state)->state.endEffect0;
    state = &((SightBeamObject *)state)->state;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = (void *)state->endEffect1;
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[0];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[1];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter0[2];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[0];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[1];
    if (part != NULL) {
        func_1516972C(part);
    }
    part = state->emitter1[2];
    if (part != NULL) {
        func_1516972C(part);
    }
    func_1513CAA0(obj);
}

/* Returns whether the beam that owns this extension effect is switched on. */
s32 func_151C96DC(SightExtension *effect, s32 arg1) {
    SightBeamState *state;

    state = &effect->owner->state;
    if (state->flags & 2) {
        return 1;
    }
    return 0;
}

/* Sends `command` (see SightCommandMsg) to the sight beams keyed to `source`. */
void func_151C970C(s32 command, s32 source) {
    struct {
        s32 source;
        u8 command;
    } msg;
    msg.source = source;
    msg.command = command;
    func_151403A8((s32)&msg, 0x3A);
}

extern s32 func_15046C80(f32 *, u16, f32, void *);
extern void func_150E83AC(void *, s16, u8, s32);
extern void func_15136C3C(void *, u8, u8, u8, u8, u8, u8, s32);
extern void *func_151541B8(void *, f32, f32, f32, f32, u8, s32);
extern void func_151D5334(void *, f32, f32, f32, u8, u8, s32);
extern void func_151D5514(void *, u8, s32);
extern void func_151D3FF4(f32 *, u8, s32);
void func_151C9AC0(SightActor *actor, u8 arg1, s32 arg2);

/* Explosion at an actor's position: ground scorch, smoke, debris and sound. */
void func_151C9740(SightActor *actor, u8 arg1, s32 arg2) {
    vertex origin;
    vertex impact;
    u8 hit;
    u8 airborne;
    vertex query;
    SightCollision collision;

    if (actor == NULL) {
        return;
    }
    origin.x = actor->x;
    origin.y = actor->y + 135.0f;
    origin.z = actor->z;
    hit = 0;
    airborne = actor->unk2D8 != 0.0f;
    if (!airborne) {
        query.x = origin.x;
        query.y = origin.y;
        query.z = origin.z;
        func_1504715C(&collision, actor);
        if (func_15046C80(&query.x, 0, origin.y - 200.0f, &collision) != 0 && (collision.surfaceFlags & 1)) {
            impact.x = query.x;
            hit = 1;
            impact.y = collision.hitY;
            impact.z = query.z;
            func_150E7FEC(func_150ADA68() * 125.0f + 204.0f, (func_150ADA20() % 101U + 155) & 0xFF,
                          &collision.triangle, &impact, func_150ADA20() % 302U + 500, 0, 1, 0, 0, 0, arg1, 0);
        }
    }
    func_151D5404(&origin, 506.0f, 1013.0f, 0.000987166888f, 15, 20, arg1, arg2);
    func_151D5334(&origin, 506.0f, 1013.0f, 0.000987166888f, 5, arg1, arg2);
    func_151D5514(&origin, arg1, arg2);
    func_151D3FF4(&origin.x, arg1, arg2);
    func_150E83AC(hit ? &impact.x : &origin.x, (s16)(func_150ADA20() % 62U + 120), arg1, arg2);
    func_151541B8(&origin, 2.0f * func_150ADA68() + 8.0f, 2.4641099f,
        (f32)(func_150ADA20() % 56U + 200), 0.0f, arg1, arg2);
    if (!airborne) {
        func_15136C3C(actor, 0, 0, 1, 0, 0, arg1, arg2);
    }
    func_10010F88(0x60D, 0x7D00, 0, 0, 0, (s32)origin.x,
        (s32)origin.y, (s32)origin.z, 500, 1000);
    if (airborne) {
        func_151C9AC0(actor, arg1, arg2);
    }
}

/* Spawns func_151ABE40's effect (type 2) just above the actor's unk180 height. */
void func_151C9AC0(SightActor *actor, u8 arg1, s32 arg2) {
    f32 pos[3];
    SightCollision collision;

    pos[0] = actor->x;
    pos[1] = actor->unk180 + 2.0f;
    pos[2] = actor->z;
    func_1504715C(&collision, actor);
    func_151ABE40(pos, &collision, 2, arg1, arg2);
}

/* Hides the object's model while its status block's unk6F is set. */
s32 func_151C9B30(SightModelOwner *obj) {
    SightStatusBlock *status = obj->status;
    if (status->unk6F == 0) {
        obj->model->hidden = 0;
    } else {
        obj->model->hidden = 1;
    }
    return 1;
}

/* Reports through `visible` whether the status block's unk6F is clear; clears flag 2 otherwise. */
s32 func_151C9B64(SightStatusObject *obj, u8 *visible) {
    SightStatusBlock *status = obj->status;
    if (status->unk6F == 0) {
        *visible = 1;
    } else {
        obj->flags &= ~2;
        *visible = 0;
    }
    return 1;
}

extern u8 D_800CBFA8[][0x32C];
void func_151C9ED4(struct108 *player);
void func_151C9F38(struct108 *player, u8 type, u8 arg2, s8 arg3, u8 arg4);
void func_151CA6A0(struct108 *player, u8 big);
void func_151CAD28(struct108 *player, u8 big);
void func_151CAB78(struct108 *player, u8 big);
void func_151CCE94(struct108 *);
void func_151C9DE8(struct108 *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4);
void func_151CC524(struct108 *, u8, u8, u8, u8, u8);
void func_151CC840(struct108 *, u8, u8, u8, u8, u8);
void func_151CCD1C(struct108 *, u8, u8, u8);
void func_151CB5FC(struct108 *player, u8 kind, u8 arg2, s8 arg3, u8 arg4);

/* Starts the screen/player effects for player state `state` (some held items force state 27). */
void func_151C9BA0(struct108 *player, s32 state) {
    u8 idx;

    func_151C9ED4(player);
    idx = player->unk3D0->unk65;
    if (idx != 0) {
        if (D_800CBFA8[idx][0] == 0x23 || D_800CBFA8[idx][0] == 0x8A) {
            state = 0x1B;
        }
    }
    switch (state) {
        case 35:
            func_151C9F38(player, 1, 1, -1, 0xFF);
            func_151CA6A0(player, 0);
            break;
        case 31:
        case 41:
            func_151C9F38(player, 2, 1, -1, 0xFF);
            func_151CAD28(player, 0);
            break;
        case 12:
        case 22:
            func_151CAD28(player, 0);
            func_151CCE94(player);
            break;
        case 27:
            if (D_800BE9F0 == 0x27) {
                func_151C9DE8(player, 1, -1, 0xFF, 1);
            } else {
                if (D_80082FA0 == 0) {
                    func_151CC524(player, 0xFF, 0, 0, 1, 0xFF);
                    func_151CC840(player, 0xFF, 0, 0, 1, 0xFF);
                } else {
                    func_151CCD1C(player, 0xFF, 0, 0);
                }
                func_151C9F38(player, 3, 1, -1, 0xFF);
                func_151CB5FC(player, 1, 1, -1, 0xFF);
            }
            break;
        case 36:
            func_151C9F38(player, 0, 1, -1, 0xFF);
            func_151CA6A0(player, 0);
            break;
        case 18:
        case 24:
        case 56:
        case 59:
            break;
        case 9:
        default:
            func_151CAB78(player, 0);
            break;
    }
}

void func_150BDE90(void *, u8, s32);
void func_151C9F38(struct108 *player, u8 type, u8 arg2, s8 arg3, u8 arg4);
void func_151CC524(struct108 *, u8, u8, u8, u8, u8);
void func_151CC840(struct108 *, u8, u8, u8, u8, u8);
void func_151CCF08(struct108 *, u8, s8, u8);
void func_151CB5FC(struct108 *player, u8 kind, u8 arg2, s8 arg3, u8 arg4);

/* Effects for state 27 when D_800BE9F0 is 0x27 (see func_151C9BA0). */
void func_151C9DE8(struct108 *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4) {
    func_151C9F38(player, 4, arg1, arg2, arg3);
    func_151CC524(player, 0, 50, 0xFF, arg1, arg3);
    func_151CC840(player, 0, 50, 0xFF, arg1, arg3);
    func_151CCF08(player, arg1, arg2, arg3);
    func_150BDE90(player, arg3, arg4);
    func_151CB5FC(player, 2, arg1, arg2, arg3);
    if (D_800BE9F0 == 0x27) {
        D_8008CD00 = 1;
    }
}

/* Broadcasts message 0x21 (player effects reset) to the effect systems and clears D_8008CD00. */
void func_151C9ED4(struct108 *player) {
    struct108 *msg;

    msg = player;
    func_15160274((s32)&msg, 0x21, (s32)player);
    func_1515572C((s32)&msg, 0x21);
    func_151A561C((s32)&msg, 0x21);
    func_151494E0((s32)&msg, 0x21);
    D_8008CD00 = 0;
}

typedef struct {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} SightColor;

/* Spawn descriptor for a screen-space sight sprite (func_1515548C). */
typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 width;
    /* 0x0C */ f32 height;
    /* 0x10 */ u8  texture;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 lifetime;
    /* 0x14 */ u16 flags;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  red;
    /* 0x1C */ u8  green;
    /* 0x1D */ u8  blue;
    /* 0x1E */ u8  alpha;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u8  unk20;
    /* 0x21 */ u8  unk21;
    /* 0x22 */ u8  unk22;
    /* 0x23 */ u8  unk23;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ u8  unk40;
    /* 0x41 */ u8  unk41;
    /* 0x42 */ u8  pad42[2];
    /* 0x44 */ u8  playerNum;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} SightSpriteDesc;

/* Per-sprite state copied to +0x70 of each spawned sight sprite. */
typedef struct {
    /* 0x00 */ struct108 *player;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 flags; /* 1: bouncing, 2: colour flash, 4: value from D_800C3958[sourceIndex] */
    /* 0x0D */ u8 kind;  /* index into the D_8008FC10 message handlers */
    /* 0x0E */ u8 side;
    /* 0x0F */ u8 padF;
    /* 0x10 */ f32 bouncePhase;
    /* 0x14 */ f32 fade;
    /* 0x18 */ f32 rangeMin;
    /* 0x1C */ f32 rangeMax;
    /* 0x20 */ f32 rangeScale;
    /* 0x24 */ f32 angle;
    /* 0x28 */ f32 lastValue;
    /* 0x2C */ SightColor flashColor;
    /* 0x3C */ SightColor color;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 flashTimer;
    /* 0x54 */ s8 sourceIndex;
    /* 0x55 */ u8 pad55[3];
} SightSpritePayload;

/* A spawned sight sprite: the spawn descriptor lives on at +0x10, the payload at +0x70. */
typedef struct {
    u8 pad0[0x10];
    SightSpriteDesc desc;
    u8 pad68[8];
    SightSpritePayload payload;
} SightSprite;

/* Message 0x37 (see func_151CB918). */
typedef struct {
    u8 side;
    struct108 *player;
} SightPanelMsg;

f32 func_151CC1D4(SightSprite *sprite);

/* Effect spawned by func_151CCE94; +0x20 holds the owning player. */
typedef struct {
    u8 pad0[0x20];
    struct108 *player;
} SightOwnedEffect;

extern void (*D_8008FC10[])(SightSprite *, void **, u8);
extern SightColor D_800AAC80;
extern SightColor D_800AAC90[];
extern SightColor D_800AACE0[];
extern f32 D_800AAD30[];
extern f32 D_800AAD44[];
extern f32 D_800AAD58[];
extern f32 D_800AAD6C[][2];
extern f32 D_800AAD94[][2];

/* Spawns the two layers of sight ring `type` (colour/size tables indexed by type) for `player`. */
void func_151C9F38(struct108 *player, u8 type, u8 arg2, s8 arg3, u8 arg4) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.flashColor = D_800AAC80;
    payload.color = D_800AAC90[type];
    payload.unk4C = D_800AAD44[type] - 1.0f;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.unk8 = 130.0f;
    payload.player = player;
    payload.flags = (s32)((arg3 == -1) ? 0 : 4); // matching: the s32 cast changes how IDO narrows the result
    payload.sourceIndex = arg3;
    desc.lifetime = 300;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.playerNum = player->unk23D;
    desc.x = D_800AAD58[type];
    desc.y = 0.0f;
    desc.width = D_800AAD30[type];
    desc.height = D_800AAD44[type];
    desc.texture = 0xFF;
    desc.flags = (1 << (player->unk23D + 11)) | ((arg2 ? 0x40 : 0) | 0x10);
    desc.unk30 = 8;
    desc.unk34 = 0x54;
    payload.rangeMax = 0.0f;
    payload.lastValue = 99999.0f;
    payload.rangeMin = D_800AAD6C[type][D_80082FA0 == 1];
    payload.rangeScale = D_800AAD94[type][D_80082FA0 == 1];
    payload.kind = 3;

    desc.red = D_800AAC90[type].r;
    desc.green = D_800AAC90[type].g;
    desc.blue = D_800AAC90[type].b;
    desc.alpha = D_800AAC90[type].a;
    sprite = func_1515548C(&desc, 8, 0, 0, sizeof(SightSpritePayload), arg4, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    payload.kind = 0;
    desc.width = D_800AAD30[type] + 2.0f;
    desc.height = D_800AAD44[type] + 2.0f;
    desc.red = D_800AACE0[type].r;
    desc.green = D_800AACE0[type].g;
    desc.blue = D_800AACE0[type].b;
    desc.alpha = D_800AACE0[type].a;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Spawns the four corner brackets and a ring of 12 dots of the basic sight for `player`. */
void func_151CA6A0(struct108 *player, u8 big) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;
    s32 pad[2]; // matching: unused stack space in the original frame

    payload.kind = 0;
    payload.flags = 0;
    desc.height = 25.0f;
    desc.width = 25.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        desc.texture = 0x73;
    } else {
        desc.texture = 0x5D;
    }
    desc.lifetime = 300;
    desc.flags = (1 << (player->unk23D + 0xB)) | 0x50;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = 0;
    desc.green = 0xFF;
    desc.blue = 0;
    desc.alpha = 0x82;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.playerNum = player->unk23D;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.x = 25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    {
        s32 i;

        desc.x = 0.0f;
        desc.y = 0.0f;
        desc.height = 3.0f;
        desc.width = 3.0f;
        desc.texture = 0x99;
        desc.flags = (1 << (player->unk23D + 0xB)) | 0x50;
        desc.unk30 = 8;
        desc.unk34 = 0x44;
        payload.lastValue = 99999.0f;
        if (D_80082FA0 == 1) {
            payload.rangeMin = -62.0f;
        } else {
            payload.rangeMin = -50.0f;
        }
        payload.rangeMax = 0.0f;
        if (D_80082FA0 == 1) {
            payload.rangeScale = 0.01612903178f;
        } else {
            payload.rangeScale = 0.01999999955f;
        }
        i = 0; // matching: the counter is initialised before the angle
        payload.angle = 0.0f;
        for (; i < 12; i++) {
            sprite = func_1515548C(&desc, 7, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
            if (sprite != NULL) {
                memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
            }
            payload.angle += 0.5235987902f;
        }
    }
}

/* Message handler shared by sight sprites: removes the sprite when its player's effects are
 * reset (0x21) or when message 0 names an actor controlled by that player, then runs the
 * handler for the sprite's kind. */
void func_151CAACC(SightSprite *sprite, void **msg, u8 msgId) {
    void (*handler)(SightSprite *, void **, u8);
    SightActor *actor;

    if (msgId == 0x21) {
        if (sprite->payload.player == *(struct108 **)msg) {
            func_1516972C((struct102 *)sprite);
        // matching: an empty labelled statement keeps IDO from merging this branch's exit
        label:
            ;
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->player == sprite->payload.player) {
            func_1516972C((struct102 *)sprite);
        }
    }
    handler = D_8008FC10[sprite->payload.kind];
    if (handler != NULL) {
        handler(sprite, msg, msgId);
    }
}

/* Spawns the plain dot sight for `player`. */
void func_151CAB78(struct108 *player, u8 big) {
    SightSpriteDesc desc;
    s32 pad; // matching: unused stack slot
    SightSpritePayload payload;
    SightSprite *sprite;
    s32 kind;
    f32 scale;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    if (D_80082FA0 > 0) {
        scale = 2.0f;
    } else {
        scale = 1.0f;
    }

    desc.texture = 100;
    desc.lifetime = 300;
    desc.height = scale * 12.0f;
    desc.width = desc.height;
    desc.flags = (1 << (player->unk23D + 11)) | 0x50;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = 0xFF;
    desc.green = 200;
    desc.blue = 0;
    desc.alpha = 130;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.playerNum = player->unk23D;

    if (big) {
        kind = 4;
    } else {
        kind = 0;
    }

    sprite = func_1515548C(&desc, kind, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* One viewport of the D_800BE628 array (0x180 bytes each). */
typedef struct {
    u8 pad0[0x34];
    f32 centerX;
    f32 centerY;
    u8 pad3C[0x144];
} SightViewport;

/* State of the lens effect (func_151CB110), stored at +0x18 of its object. */
typedef struct {
    struct108 *player;
    SightViewport *view;
    u16 *pixels;
    s32 left;
    u32 topOffset;
} SightLens;

typedef struct {
    u8 pad0[0x18];
    SightLens lens;
    u8 pad2C[4];
    u16 pixels[24 * 24]; /* 24x24 copy of the framebuffer under the lens */
} SightLensObject;

/* Spawns the corner brackets of the zoom sight and, unless D_800BE616 is set, its centre lens. */
void func_151CAD28(struct108 *player, u8 big) {
    typedef struct {
        s8 unk0;
        s8 unk1;
        s8 unk2;
        s16 unk4;
        s8 unk6;
    } LensSpawn;
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.kind = 0;
    payload.flags = 0;
    desc.height = 25.0f;
    desc.width = 25.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        desc.texture = 0x72;
    } else {
        desc.texture = 0x65;
    }
    desc.lifetime = 300;
    desc.flags = (1 << (player->unk23D + 11)) | 0x50;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = 0;
    desc.green = 0xFF;
    desc.blue = 0;
    desc.alpha = 130;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.playerNum = player->unk23D;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.x = 25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    if (D_800BE616 == 0) {
        LensSpawn spawn;
        SightLens lens;
        SightLensObject *lensObj;

        lens.player = player;
        lens.view = (SightViewport *)(player->unk23D * sizeof(SightViewport) + D_800BE628); // D_800BE628 holds the array address
        lens.left = lens.view->centerX - 12.0f;
        lens.topOffset = (u32)(s32)(lens.view->centerY - 12.0f) * D_800BE620;
        spawn.unk0 = 0xE;
        spawn.unk1 = -1;
        spawn.unk2 = 1;
        spawn.unk4 = 300;
        spawn.unk6 = 1;
        lensObj = func_1515FF74(&spawn, 0x498, 0xFF, 1);
        if (lensObj != NULL) {
            lens.pixels = lensObj->pixels;
            memcpy(&lensObj->lens, &lens, sizeof(SightLens));
        }
    }
}

extern u8 D_800BEAC2;

/* Draws the zoom lens: saves the 24x24 framebuffer block under it, then writes it back
 * magnified towards the centre, mirrored into all four quadrants. */
void func_151CB110(SightLensObject *obj, u16 *framebuffer) {
    SightViewport *view;
    s32 row;
    s32 lensOffset;
    s32 screenOffset;
    f32 x;
    f32 y;
    f32 radius;
    f32 invRadius;
    f32 strength;
    s32 screenPos[2][2];
    s32 pad; // matching: unused stack slot
    s32 lensPos[2][2];

    if (D_800BEAC0 != 0 || D_800BEAC1 != 0) {
        return;
    }
    if (D_800BEAC2 != 0) {
        return;
    }
    row = 0;
    view = obj->lens.view;
    {
        SightLens *lens = &obj->lens;
        screenOffset = obj->lens.left + obj->lens.topOffset;
        lensOffset = 0;
        do {
            // matching: byte-pointer form gives IDO's operand order for the address add
            memcpy(lens->pixels + lensOffset, (u8 *)framebuffer + screenOffset * 2, 24 * sizeof(u16));
            row++;
            lensOffset += 24;
            screenOffset += D_800BE620;
        } while (row != 24);

        {
            s32 unused; // matching: a block-scope declaration here steers IDO's scheduling
            for (y = -1.0f; y < 0.0f; y += 0.08333333582f) {
                radius = func_15145A0C(fabsf(y), 1.0f, 1.0f);
                if (radius) {
                    screenPos[0][1] = (s32)(view->centerY + y * 12.0f) * D_800BE620;
                    screenPos[1][1] = (s32)(view->centerY - y * 12.0f) * D_800BE620;
                    invRadius = 1.0f / radius;
                    strength = 1.0f - (1.0f - radius * 0.6693121791f);
                    x = -radius;
                    while (x < 0.0f) {
                        f32 scale;
                        scale = func_15145A0C(fabsf(x), radius, invRadius);
                        screenPos[0][0] = (s32)(view->centerX + x * 12.0f);
                        screenPos[1][0] = (s32)(view->centerX - x * 12.0f);
                        scale = 1.0f - scale * strength;
                        lensPos[0][0] = (s32)(x * scale * 12.0f + 12.0f);
                        lensPos[0][1] = (s32)(y * scale * 12.0f + 12.0f) * 24;
                        lensPos[1][0] = (s32)(12.0f - x * scale * 12.0f);
                        lensPos[1][1] = (s32)(12.0f - y * scale * 12.0f) * 24;
                        *(framebuffer + screenPos[0][1] + screenPos[0][0]) = *(lens->pixels + lensPos[0][0] + lensPos[0][1]);
                        *(framebuffer + screenPos[0][1] + screenPos[1][0]) = *(lens->pixels + lensPos[1][0] + lensPos[0][1]);
                        *(framebuffer + screenPos[1][1] + screenPos[1][0]) = *(lens->pixels + lensPos[1][0] + lensPos[1][1]);
                        *(framebuffer + screenPos[1][1] + screenPos[0][0]) = *(lens->pixels + lensPos[0][0] + lensPos[1][1]);
                        x += 0.08333333582f;
                    }
                }
            }
        }
    }
}

/* Message handler of the lens: removed like the sight sprites (see func_151CAACC). */
void func_151CB49C(SightLensObject *obj, void **msg, u8 msgId) {
    SightActor *actor;

    if (msgId == 0x21) {
        if (obj->lens.player == *(struct108 **)msg) {
            func_1516972C((struct102 *)obj);
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->player == obj->lens.player) {
            func_1516972C((struct102 *)obj);
        }
    }
}

/* The part of struct127 read through struct108.unk3D4 here (struct127 does not declare it). */
typedef struct {
    u8 pad0[0x19E];
    u16 unk19E;
} SightPlayerState;

/* Fades the sprite towards payload.unk8 while the player's 0x2000 input flag is held
 * (and unk3D4->unk19E is clear), and towards 0 otherwise. */
void func_151CB510(SightSprite *sprite) {
    struct108 *player;
    SightSpritePayload *payload;
    f32 target;

    player = sprite->payload.player;
    payload = &sprite->payload;
    if ((*player->unk36C & 0x2000) && ((SightPlayerState *)player->unk3D4)->unk19E == 0) {
        target = payload->unk8;
    } else {
        target = 0.0f;
    }
    payload->unk4 += (target - payload->unk4) * 0.400000006f;
    sprite->desc.alpha = payload->unk4;
}

extern f32 D_800AAD84[];
extern f32 D_800AADAC[];

/* Spawns the four pieces (two sides, two layers each) of side panel `kind` for `player`. */
void func_151CB5FC(struct108 *player, u8 kind, u8 arg2, s8 arg3, u8 arg4) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.unk4 = 255.0f;
    payload.unk8 = 255.0f;
    payload.kind = kind;
    payload.player = player;
    payload.flags = (s32)((arg3 == -1) ? 0 : 4); // matching: see func_151C9F38
    payload.sourceIndex = arg3;
    payload.fade = 255.0f;
    desc.lifetime = 300;
    desc.width = 49.0f;
    payload.bouncePhase = 0.0f;
    desc.height = 18.0f;
    desc.flags = (1 << (player->unk23D + 11)) | ((arg2 ? 0x40 : 0) | 0x32);
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 7;
    desc.unk34 = 0x22;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 7;
    desc.playerNum = player->unk23D;
    payload.side = 0;
    desc.alpha = 0xFF;
    desc.texture = 0x7B;
    payload.rangeMax = 0.0f;
    desc.x = 88.0f;
    desc.y = 91.0f;
    payload.rangeMin = D_800AAD84[D_80082FA0 == 1];
    payload.rangeScale = D_800AADAC[D_80082FA0 == 1];
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.alpha = 0;
    desc.texture = 0x7C;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~2;
    desc.alpha = 0xFF;
    desc.texture = 0x7B;
    payload.side = 1;
    desc.x = -88.0f;
    desc.y = 91.0f;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.alpha = 0;
    desc.texture = 0x7C;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Message 0x37 for a side panel: restart its bounce when `side` and `player` match. */
void func_151CB918(SightSprite *sprite, SightPanelMsg *msg, u8 msgId) {
    SightSpritePayload *payload = &sprite->payload;

    if (msgId == 0x37) {
        if (payload->side == msg->side) {
            if (payload->player == msg->player) {
                payload->bouncePhase = 0.0f;
                payload->flags |= 1;
                payload->fade = 0.0f;
            }
        }
    }
}

/* Side panel update: height follows the sight value, optional bounce, fade-in. */
s32 func_151CB970(SightSprite *sprite) {
    register SightSpritePayload *payload;
    f32 value;
    f32 range; // matching: kept as a named local
    f32 offset;
    f32 baseY;
    u8 alpha;

    payload = &sprite->payload;
    value = func_151CC1D4(sprite);
    offset = (1.0f - ((value - payload->rangeMin) * payload->rangeScale)) * 75.0f;

    if (payload->flags & 1) {
        value = sinf(payload->bouncePhase);
        baseY = 91.0f + offset;
        range = (112.0f + offset) - baseY;
        sprite->desc.y = (value * range) + baseY;
        payload->bouncePhase += 0.5049999952f * D_800BE9A4;
        if (3.141592741f <= payload->bouncePhase) {
            sprite->desc.y = baseY;
            payload->flags &= ~1;
        }
    } else {
        sprite->desc.y = 91.0f + offset;
    }

    if (payload->kind == 2) {
        sprite->desc.y -= 20.0f;
    }

    alpha = (u32)payload->fade;
    if (sprite->desc.texture == 0x7C) {
        sprite->desc.alpha = 255 - alpha;
    } else {
        sprite->desc.alpha = alpha;
    }

    payload->fade += (255.0f - payload->fade) * 0.6760000587f;
    return 1;
}

/* Message handler of the effect spawned by func_151CCE94 (owner player at +0x20). */
void func_151CBB6C(SightOwnedEffect *effect, void **msg, u8 msgId) {
    SightActor *actor;

    if (msgId == 0x21) {
        if (effect->player == *(struct108 **)msg) {
            func_1516972C((struct102 *)effect);
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->player == effect->player) {
            func_1516972C((struct102 *)effect);
        }
    }
}

/* Moves the sprite around a circle of radius 55 as the sight value changes. */
s32 func_151CBBE0(SightSprite *sprite) {
    register SightSpritePayload *payload;
    f32 scaled;
    f32 value;

    payload = &sprite->payload;
    value = func_151CC1D4(sprite);
    if (value != payload->lastValue) {
        payload->lastValue = value;
        scaled = ((value - payload->rangeMin) * payload->rangeScale) * 6.283185959f;
        func_1514373C(payload->angle + scaled, 55.0f, &sprite->desc.x, &sprite->desc.y);
    }
    return 1;
}

/* Scales the sprite's height with the sight value and fades its flash colour back to normal. */
s32 func_151CBC60(SightSprite *sprite) {
    SightSpritePayload *payload = &sprite->payload;
    f32 value;
    f32 base;

    value = func_151CC1D4(sprite);
    if (value != payload->lastValue) {
        base = payload->unk4C;
        payload->lastValue = value;
        sprite->desc.height = base - (value - payload->rangeMin) * payload->rangeScale * base + 1.0f;
    }
    if (payload->flags & 2) {
        payload->flashTimer -= D_800BE9A4;
        value = payload->flashTimer;
        if (value <= 0.0f) {
            sprite->desc.red = payload->color.r;
            sprite->desc.green = payload->color.g;
            sprite->desc.blue = payload->color.b;
            sprite->desc.alpha = payload->color.a;
            payload->flags &= ~2;
        } else {
            sprite->desc.red = payload->color.r + (payload->flashColor.r - payload->color.r) * (value * 0.028571429f);
            sprite->desc.green = payload->color.g + (payload->flashColor.g - payload->color.g) * (payload->flashTimer * 0.028571429f);
            sprite->desc.blue = payload->color.b + (payload->flashColor.b - payload->color.b) * (payload->flashTimer * 0.028571429f);
            sprite->desc.alpha = payload->color.a + (payload->flashColor.a - payload->color.a) * (payload->flashTimer * 0.028571429f);
        }
    }
    return 1;
}

/* Current sight value of a sprite, clamped to [rangeMin, rangeMax]: either from the
 * D_800C3958 entry chosen at spawn (flag 4) or the mean of the player's unk19C/unk1A0. */
f32 func_151CC1D4(SightSprite *sprite) {
    SightSpritePayload *payload;
    f32 value;

    payload = &sprite->payload;
    if (payload->flags & 4) {
        f32 t = D_800C3958[payload->sourceIndex].unk28 * 0.009999999776f;

        value = payload->rangeMin + (payload->rangeMax - payload->rangeMin) * t;
    } else {
        value = (payload->player->unk19C + payload->player->unk1A0) * 0.5f;
    }
    if (value < payload->rangeMin) {
        value = payload->rangeMin;
    } else if (payload->rangeMax < value) {
        value = payload->rangeMax;
    }
    return value;
}


extern void func_1515572C(s32, s32);

/* Broadcasts message 0x46 (flash the sight sprites of `player`). */
void func_151CC290(s32 player) {
    s32 msg;

    msg = player;
    func_1515572C((s32)&msg, 0x46);
}

/* Message 0x46 handler: starts the 35-frame colour flash. */
void func_151CC2BC(SightSprite *sprite, s32 *msg, u8 msgId) {
    SightSpritePayload *payload;

    payload = &sprite->payload;
    // matching: reading payload->player as *(s32 *)payload keeps IDO's load order
    if ((msgId == 0x46) && (*msg == *(s32 *)payload)) {
        payload->flags |= 2;
        payload->flashTimer = 35.0f;
        sprite->desc.red = (u32)payload->flashColor.r;
        sprite->desc.green = (u32)payload->flashColor.g;
        sprite->desc.blue = (u32)payload->flashColor.b;
        sprite->desc.alpha = (u32)payload->flashColor.a;
    }
}

/* Spawns the two bars of the sight's cross (colour arg1..arg3) for `player`. */
void func_151CC524(struct108 *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.unk50 = 0.0f;
    desc.unk54 = 0.0f;
    desc.lifetime = 300;
    desc.flags = (1 << (player->unk23D + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10 | 0x80 | 0x100 | 0x200);
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = red;
    desc.green = green;
    desc.blue = blue;
    desc.alpha = 0x82;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.playerNum = player->unk23D;
    desc.unk48 = 0.200000003f;
    desc.unk4C = 1.0f;
    desc.width = 28.0f;
    desc.height = 4.199999809f;
    desc.y = -37.0f;
    desc.texture = 0x9A;
    desc.x = 0.0f;
    sprite = func_1515548C(&desc, 9, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9B;
    desc.y = 0.0f;
    desc.unk48 = 1.0f;
    desc.unk4C = 0.200000003f;
    desc.width = 4.199999809f;
    desc.height = 28.0f;
    desc.x = -37.0f;
    sprite = func_1515548C(&desc, 9, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* The part of the struct126 at struct127.unk31C read here (struct126 does not declare it). */
typedef struct {
    u8 pad0[0x130];
    vertex aimDir;
} SightAimer;

/* Cross-bar update: shifts the bar sideways (texture 0x9A) or vertically by the aim angles. */
s32 func_151CC77C(SightSprite *sprite) {
    // matching: the angle pointers are taken (in registers) before the NULL check
    register f32 *pitchp;
    register f32 *yawp;
    struct {
        f32 yaw;
        f32 pitch;
        s32 pad[2];
    } angles;
    SightAimer *aimer;
    struct127 *state;

    // matching: one variable first holds the player, then the aimer (as IDO allocated it)
    aimer = (SightAimer *)sprite->payload.player;
    state = ((struct108 *)aimer)->unk3D0;
    if (state == NULL) {
        return 0;
    }
    aimer = (SightAimer *)state->unk31C;
    pitchp = &angles.pitch;
    yawp = &angles.yaw;
    if (aimer == NULL) {
        return 0;
    }
    func_15145974(&aimer->aimDir, pitchp, yawp);
    if (sprite->desc.texture == 0x9A) {
        sprite->desc.unk50 = -((angles.pitch * 0.002777777845f) * 2560.0f) * 2.0f;
    } else {
        sprite->desc.unk54 = ((angles.yaw * 0.002777777845f) * 2560.0f) * 2.0f;
    }
    return 1;
}

/* Spawns the centre dot, side ticks and diagonal corners of the cross sight for `player`. */
void func_151CC840(struct108 *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.lifetime = 300;
    desc.flags = (1 << (player->unk23D + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10);
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = red;
    desc.green = green;
    desc.blue = blue;
    desc.alpha = 0xB4;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.playerNum = player->unk23D;
    desc.texture = 0x64;
    desc.height = 12.0f;
    desc.width = 12.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9D;
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.height = 7.0f;
    desc.width = 7.0f;
    desc.x = 55.0f;
    desc.y = 0.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = -55.0f;
    desc.y = 0.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9C;
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = 0.0f;
    desc.y = 55.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = 0.0f;
    desc.y = -55.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9E;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.height = 6.0f;
    desc.width = 6.0f;
    desc.x = 44.0f;
    desc.y = 44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -44.0f;
    desc.y = 44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -44.0f;
    desc.y = -44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 44.0f;
    desc.y = -44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Spawns the large ring of the cross sight (used instead of the cross in multiplayer). */
void func_151CCD1C(struct108 *player, u8 red, u8 green, u8 blue) {
    SightSpriteDesc desc;
    s32 pad; // matching: unused stack slot
    SightSpritePayload payload;
    SightSprite *sprite;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.lifetime = 300;
    desc.flags = (1 << (player->unk23D + 0xB)) | 0x50;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = red;
    desc.green = green;
    desc.blue = blue;
    desc.alpha = 0xB4;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.playerNum = player->unk23D;
    desc.texture = 0xB4;
    desc.width = 65.0f;
    desc.height = 65.0f;

    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* When D_800BE616 is set, spawns the func_151A4FD0 effect for `player` (see func_151CBB6C). */
void func_151CCE94(struct108 *player) {
    SightOwnedEffect *effect;
    struct108 *owner;

    if (D_800BE616 != 0) {
        owner = player;
        effect = func_151A4FD0(0, 0, 0, 0xFF, 0, player->unk23D, 1, 4);
        if (effect != NULL) {
            memcpy(&effect->player, &owner, sizeof(owner));
        }
    }
}

extern f32 D_800AAD8C[];
extern f32 D_800AADB4[];
/* Texture ids of the four strips in a scope frame row. */
typedef struct { u8 b[4]; } SightTextureRow;
extern SightTextureRow D_8008FC28;

/* Spawns the two rows of four texture strips (top and bottom) of the scope frame for `player`. */
void func_151CCF08(struct108 *player, u8 arg1, s8 arg2, u8 arg3) {
    SightSpriteDesc desc;
    SightSprite *sprite;
    SightSpritePayload payload;
    u8 i;
    SightTextureRow textures;
    u8 j;

    payload.rangeMax = 0.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.rangeMin = D_800AAD8C[D_80082FA0 == 1];
    payload.rangeScale = D_800AADB4[D_80082FA0 == 1];
    payload.unk8 = 130.0f;
    payload.flags = (s32)((arg2 == -1) ? 0 : 4); // matching: see func_151C9F38
    payload.sourceIndex = arg2;

    desc.lifetime = 300;
    desc.flags = (arg1 ? 0x40 : 0) | 0x20 | 0x10;
    desc.unk16 = 1;
    desc.unk18 = 0xFF;
    desc.unk1A = 7;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.alpha = 0xFF;
    desc.unk1F = 0xFF;
    desc.unk20 = 0xFF;
    desc.unk21 = 0xFF;
    desc.unk22 = 0xFF;
    desc.unk23 = 0xFF;
    desc.unk24 = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk30 = 8;
    desc.unk34 = 0x13;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 0;
    desc.playerNum = player->unk23D;
    desc.unk50 = 0.0f;
    desc.unk54 = 0.0f;
    desc.unk48 = 1.0f;
    desc.unk4C = 1.0f;
    desc.width = 35.75f;
    desc.height = 16.0f;

    for (j = 0; j < 2; j++) {
        textures = D_8008FC28;
        desc.y = j ? 92.0f : -90.0f;
        desc.flags |= j ? 4 : 0;
        payload.kind = j ? 5 : 4;
        for (i = 0; i < 4; i++) {
            desc.x = ((i * 71.5f) + 35.75f) - 143.0f;
            desc.texture = textures.b[i];
            sprite = func_1515548C(&desc, 0xB, 0, 0, sizeof(SightSpritePayload), arg3, 1);
            if (sprite != NULL) {
                memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
            }
        }
    }
}



/* Scope frame strip update: the top (kind 5) and bottom (kind 4) rows slide with the sight value. */
void func_151CD224(SightSprite *sprite) {
    SightSpritePayload *payload = &sprite->payload;
    f32 value = func_151CC1D4(sprite);
    f32 offset = (1.0f - (value - payload->rangeMin) * payload->rangeScale) * 75.0f;
    if (payload->kind == 5) {
        sprite->desc.y = 92.0f + offset;
    } else if (payload->kind == 4) {
        sprite->desc.y = -92.0f - offset;
    }
}
