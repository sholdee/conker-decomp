#include <ultra64.h>
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1513D4B8 func_1513D4B8_void_decl
#include "functions.h"
#undef func_150ADA20
#undef func_1513D4B8
s32 func_1513D4B8(s32 arg0, s32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, s32 arg9);
u32 func_150ADA20(void);
#include "variables.h"

extern f32 D_800AAED4;
extern f32 D_800AAED8;
extern f32 D_800AAEA8;
extern f32 D_800AAEAC;
extern f32 D_800AAEB0;
extern f32 D_800AAEB4;
extern f32 D_800AAEB8;
extern f32 func_151CC1D4(void *);
extern f32 sinf(f32);
extern void func_1514373C(f32, f32, f32 *, f32 *);
extern void func_15145974(struct17 *, f32 *, f32 *);
extern void func_1000FD38(void *, void *, s32);
extern void *func_1515548C(void *, u8, s32, s32, s32, u8, s32);
extern void *func_1515FF74(void *, s32, u8, s32);
extern void func_15160274(s32, s32, s32);
extern void func_1515572C(s32, s32);
extern void func_151A561C(s32, s32);
extern void (*D_8008FC10[])(struct102 *, void **, u8);
typedef struct {
    s32 field_0x0;
    u8 pad_0x4[0x8];
    u8 field_0xC;
    u8 field_0xD;
    u8 field_0xE;
    u8 pad_0xF[0x1];
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    u8 pad_0x1C[0x4];
    f32 field_0x20;
} Game1F4650EffectState;
typedef struct {
    u8 pad_0x00[0x14];
    f32 field_0x14;
    u8 pad_0x18[0x58];
    Game1F4650EffectState field_0x70;
} Game1F4650EffectObject;
typedef struct {
    u8 field_0x0;
    u8 pad_0x1[0x3];
    s32 field_0x4;
} Game1F4650EffectTriggerEvent;

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
    u8 *unk318;
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
        func_15164F0C(2, actor->unk318[0x23D], 0, obj->unkC, obj->unk1);
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
void func_151C9F38(void *, u8, u8, s8, u8);
void func_151CA6A0(void *, u8);
void func_151CAD28(void *, u8);
void func_151CAB78(void *arg0, u8 arg1);
void func_151CCE94(void *arg0);
void func_151C9DE8(struct108 *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4);
void func_151CC524(void *, u8, u8, u8, u8, u8);
void func_151CC840(void *, u8, u8, u8, u8, u8);
void func_151CCD1C(void *arg0, u8 arg1, u8 arg2, u8 arg3);
void func_151CB5FC(void *, u8, u8, s8, u8);

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
void func_151C9F38(void *, u8, u8, s8, u8);
void func_151CC524(void *, u8, u8, u8, u8, u8);
void func_151CC840(void *, u8, u8, u8, u8, u8);
void func_151CCF08(void *, u8, s8, u8);
void func_151CB5FC(void *, u8, u8, s8, u8);

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
} Col151C9F38;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
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
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} Emitter151C9F38;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 padE[0xA];
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ f32 unk28;
    /* 0x2C */ Col151C9F38 unk2C;
    /* 0x3C */ Col151C9F38 unk3C;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ u8 pad50[4];
    /* 0x54 */ s8 unk54;
    /* 0x55 */ u8 pad55[3];
} Payload151C9F38;

extern Col151C9F38 D_800AAC80;
extern Col151C9F38 D_800AAC90[];
extern Col151C9F38 D_800AACE0[];
extern f32 D_800AAD30[];
extern f32 D_800AAD44[];
extern f32 D_800AAD58[];
extern f32 D_800AAD6C[][2];
extern f32 D_800AAD94[][2];

void func_151C9F38(void *arg0, u8 arg1, u8 arg2, s8 arg3, u8 arg4) {
    Emitter151C9F38 spA0;
    void *temp;
    Payload151C9F38 sp44;

    sp44.unk2C = D_800AAC80;
    sp44.unk3C = D_800AAC90[arg1];
    sp44.unk4C = D_800AAD44[arg1] - 1.0f;
    sp44.unk4 = 0.0f;
    sp44.unkD = 0;
    sp44.unk8 = 130.0f;
    sp44.unk0 = arg0;
    sp44.unkC = (s32)((arg3 == -1) ? 0 : 4);
    sp44.unk54 = arg3;
    spA0.unk12 = 0x12C;
    spA0.unk16 = 1;
    spA0.unk18 = 0xFF;
    spA0.unk1A = 7;
    spA0.unk1F = 0xFF;
    spA0.unk20 = 0xFF;
    spA0.unk21 = 0xFF;
    spA0.unk22 = 0xFF;
    spA0.unk23 = 0xFF;
    spA0.unk24 = 0;
    spA0.unk28 = 0x200004;
    spA0.unk2C = 0x1F0601;
    spA0.unk38 = 0x80;
    spA0.unk3C = 0x20;
    spA0.unk40 = 0;
    spA0.unk41 = 0xA;
    spA0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA0.unk0 = D_800AAD58[arg1];
    spA0.unk4 = 0.0f;
    spA0.unk8 = D_800AAD30[arg1];
    spA0.unkC = D_800AAD44[arg1];
    spA0.unk10 = 0xFF;
    spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg2 ? 0x40 : 0) | 0x10);
    spA0.unk30 = 8;
    spA0.unk34 = 0x54;
    sp44.unk1C = 0.0f;
    sp44.unk28 = 99999.0f;
    sp44.unk18 = D_800AAD6C[arg1][D_80082FA0 == 1];
    sp44.unk20 = D_800AAD94[arg1][D_80082FA0 == 1];
    sp44.unkD = 3;

    spA0.unk1B = D_800AAC90[arg1].r;
    spA0.unk1C = D_800AAC90[arg1].g;
    spA0.unk1D = D_800AAC90[arg1].b;
    spA0.unk1E = D_800AAC90[arg1].a;
    temp = func_1515548C(&spA0, 8, 0, 0, 0x58, arg4, 1);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp44, 0x58);
    }

    sp44.unkD = 0;
    spA0.unk8 = D_800AAD30[arg1] + 2.0f;
    spA0.unkC = D_800AAD44[arg1] + 2.0f;
    spA0.unk1B = D_800AACE0[arg1].r;
    spA0.unk1C = D_800AACE0[arg1].g;
    spA0.unk1D = D_800AACE0[arg1].b;
    spA0.unk1E = D_800AACE0[arg1].a;
    temp = func_1515548C(&spA0, 0, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp44, 0x58);
    }
}

void func_151CA6A0(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CA6A0_spA0;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0xA];
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        f32 unk24;
        f32 unk28;
        u8 pad2C[0x2C];
    } struct_151CA6A0_sp44;
    struct_151CA6A0_spA0 spA0;
    void *temp_v0;
    struct_151CA6A0_sp44 sp44;
    s32 pad1;
    s32 pad2;

    sp44.unkD = 0;
    sp44.unkC = 0;
    spA0.unkC = 25.0f;
    spA0.unk8 = 25.0f;
    sp44.unk0 = arg0;
    sp44.unk4 = 0.0f;
    sp44.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        spA0.unk10 = 0x73;
    } else {
        spA0.unk10 = 0x5D;
    }
    spA0.unk12 = 0x12C;
    spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    spA0.unk16 = 1;
    spA0.unk18 = 0xFF;
    spA0.unk1A = 7;
    spA0.unk1B = 0;
    spA0.unk1C = 0xFF;
    spA0.unk1D = 0;
    spA0.unk1E = 0x82;
    spA0.unk1F = 0xFF;
    spA0.unk20 = 0xFF;
    spA0.unk21 = 0xFF;
    spA0.unk22 = 0xFF;
    spA0.unk23 = 0xFF;
    spA0.unk24 = 0;
    spA0.unk28 = 0x200004;
    spA0.unk2C = 0x1F0601;
    spA0.unk30 = 8;
    spA0.unk34 = 0x44;
    spA0.unk38 = 0x80;
    spA0.unk3C = 0x20;
    spA0.unk40 = 0;
    spA0.unk41 = 0xA;
    spA0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA0.unk14 &= ~6;
    spA0.unk14 |= 6;
    spA0.unk0 = 25.0f;
    spA0.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 4;
    spA0.unk0 = -25.0f;
    spA0.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 0;
    spA0.unk0 = -25.0f;
    spA0.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    spA0.unk14 &= ~6;
    spA0.unk14 |= 2;
    spA0.unk0 = 25.0f;
    spA0.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA0, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
    }
    {
        s32 i;

        spA0.unk0 = 0.0f;
        spA0.unk4 = 0.0f;
        spA0.unkC = 3.0f;
        spA0.unk8 = 3.0f;
        spA0.unk10 = 0x99;
        spA0.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
        spA0.unk30 = 8;
        spA0.unk34 = 0x44;
        sp44.unk28 = 99999.0f;
        if (D_80082FA0 == 1) {
            sp44.unk18 = -62.0f;
        } else {
            sp44.unk18 = -50.0f;
        }
        sp44.unk1C = 0.0f;
        if (D_80082FA0 == 1) {
            sp44.unk20 = 0.01612903178f;
        } else {
            sp44.unk20 = 0.01999999955f;
        }
        i = 0;
        sp44.unk24 = 0.0f;
        for (; i < 12; i++) {
            temp_v0 = func_1515548C(&spA0, 7, 0, 0, 0x58, 0xFF, 1);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0x70, &sp44, 0x58);
            }
            sp44.unk24 += 0.5235987902f;
        }
    }
}

void func_151CAACC(struct102 *arg0, void **arg1, u8 arg2) {
    void (*fn)(struct102 *, void **, u8);
    void *temp;

    if (arg2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x70) == *(s32 *)arg1) {
            func_1516972C(arg0);
        label_151CAACC:
            ;
        }
    } else if (arg2 == 0) {
        temp = *arg1;
        if (*(s32 *)((s32)arg0 + 0x70) == *(s32 *)((s32)temp + 0x318)) {
            func_1516972C(arg0);
        }
    }
    fn = D_8008FC10[*(u8 *)((s32)arg0 + 0x7D)];
    if (fn != NULL) {
        fn(arg0, arg1, arg2);
    }
}

void func_151CAB78(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        s16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CAB78_sp90;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4E];
    } struct_151CAB78_sp34;
    struct_151CAB78_sp90 sp90;
    struct_151CAB78_sp34 sp34;
    void *temp_v0;
    s32 temp_type;
    f32 temp_f0;

    sp34.unk0 = arg0;
    sp34.unk4 = 0.0f;
    sp34.unkD = 0;
    sp34.unkC = 0;
    sp34.unk8 = 130.0f;

    if (D_80082FA0 > 0) {
        temp_f0 = 2.0f;
    } else {
        temp_f0 = 1.0f;
    }

    sp90.unk10 = 0x64;
    sp90.unk12 = 0x12C;
    sp90.unkC = temp_f0 * 12.0f;
    sp90.unk8 = sp90.unkC;
    sp90.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    sp90.unk16 = 1;
    sp90.unk18 = 0xFF;
    sp90.unk1A = 7;
    sp90.unk1B = 0xFF;
    sp90.unk1C = 0xC8;
    sp90.unk1D = 0;
    sp90.unk1E = 0x82;
    sp90.unk1F = 0xFF;
    sp90.unk20 = 0xFF;
    sp90.unk21 = 0xFF;
    sp90.unk22 = 0xFF;
    sp90.unk23 = 0xFF;
    sp90.unk24 = 0;
    sp90.unk28 = 0x200004;
    sp90.unk2C = 0x1F0601;
    sp90.unk30 = 8;
    sp90.unk34 = 0x44;
    sp90.unk38 = 0x80;
    sp90.unk3C = 0x20;
    sp90.unk40 = 0;
    sp90.unk41 = 0xA;
    sp90.unk0 = 0.0f;
    sp90.unk4 = 0.0f;
    sp90.unk44 = *(u8 *)((s32)arg0 + 0x23D);

    if (arg1 != 0) {
        temp_type = 4;
    } else {
        temp_type = 0;
    }

    temp_v0 = func_1515548C(&sp90, temp_type, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp34, 0x58);
    }
}

void func_151CAD28(void *arg0, u8 arg1) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CAD28_spA8;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CAD28_sp4C;
    typedef struct {
        s8 unk0;
        s8 unk1;
        s8 unk2;
        s16 unk4;
        s8 unk6;
    } struct_151CAD28_sp44;
    typedef struct {
        void *unk0;
        void *unk4;
        void *unk8;
        s32 unkC;
        u32 unk10;
    } struct_151CAD28_sp30;
    struct_151CAD28_spA8 spA8;
    void *temp_v0;
    struct_151CAD28_sp4C sp4C;

    sp4C.unkD = 0;
    sp4C.unkC = 0;
    spA8.unkC = 25.0f;
    spA8.unk8 = 25.0f;
    sp4C.unk0 = arg0;
    sp4C.unk4 = 0.0f;
    sp4C.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        spA8.unk10 = 0x72;
    } else {
        spA8.unk10 = 0x65;
    }
    spA8.unk12 = 0x12C;
    spA8.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    spA8.unk16 = 1;
    spA8.unk18 = 0xFF;
    spA8.unk1A = 7;
    spA8.unk1B = 0;
    spA8.unk1C = 0xFF;
    spA8.unk1D = 0;
    spA8.unk1E = 0x82;
    spA8.unk1F = 0xFF;
    spA8.unk20 = 0xFF;
    spA8.unk21 = 0xFF;
    spA8.unk22 = 0xFF;
    spA8.unk23 = 0xFF;
    spA8.unk24 = 0;
    spA8.unk28 = 0x200004;
    spA8.unk2C = 0x1F0601;
    spA8.unk30 = 8;
    spA8.unk34 = 0x44;
    spA8.unk38 = 0x80;
    spA8.unk3C = 0x20;
    spA8.unk40 = 0;
    spA8.unk41 = 0xA;
    spA8.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spA8.unk14 &= ~6;
    spA8.unk14 |= 6;
    spA8.unk0 = 25.0f;
    spA8.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 4;
    spA8.unk0 = -25.0f;
    spA8.unk4 = 25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 0;
    spA8.unk0 = -25.0f;
    spA8.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    spA8.unk14 &= ~6;
    spA8.unk14 |= 2;
    spA8.unk0 = 25.0f;
    spA8.unk4 = -25.0f;
    temp_v0 = func_1515548C(&spA8, arg1 ? 4 : 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp4C, 0x58);
    }
    if (D_800BE616 == 0) {
    struct_151CAD28_sp44 sp44;
    struct_151CAD28_sp30 sp30;

        sp30.unk0 = arg0;
        sp30.unk4 = (void *)(*(u8 *)((s32)arg0 + 0x23D) * 0x180 + D_800BE628);
        sp30.unkC = *(f32 *)((s32)sp30.unk4 + 0x34) - 12.0f;
        sp30.unk10 = (u32)(s32)(*(f32 *)((s32)sp30.unk4 + 0x38) - 12.0f) * D_800BE620;
        sp44.unk0 = 0xE;
        sp44.unk1 = -1;
        sp44.unk2 = 1;
        sp44.unk4 = 0x12C;
        sp44.unk6 = 1;
        temp_v0 = func_1515FF74(&sp44, 0x498, 0xFF, 1);
        if (temp_v0 != NULL) {
            sp30.unk8 = (u8 *)temp_v0 + 0x30;
            memcpy((u8 *)temp_v0 + 0x18, &sp30, 0x14);
        }
    }
}

typedef struct {
    u8 pad0[0x34];
    f32 unk34;
    f32 unk38;
} struct_func_151CB110_s2;

typedef struct {
    s32 unk0;
    struct_func_151CB110_s2 *unk4;
    u16 *unk8;
    s32 unkC;
    s32 unk10;
} struct_func_151CB110_s0;

typedef struct {
    u8 pad0[0x18];
    struct_func_151CB110_s0 sub;
} struct_func_151CB110_arg0;

extern u8 D_800BEAC2;

void func_151CB110(struct_func_151CB110_arg0 *arg0, u16 *arg1) {
    struct_func_151CB110_s2 *s2;
    s32 i;
    s32 row;
    s32 src;
    f32 x;
    f32 y;
    f32 w;
    f32 invw;
    f32 k;
    s32 dst[2][2];
    s32 pad;
    s32 srcp[2][2];

    if (D_800BEAC0 != 0 || D_800BEAC1 != 0) {
        return;
    }
    if (D_800BEAC2 != 0) {
        return;
    }
    i = 0;
    s2 = arg0->sub.unk4;
    {
    struct_func_151CB110_s0 *s0 = &arg0->sub;
    src = arg0->sub.unkC + arg0->sub.unk10;
    row = 0;
    do {
        memcpy(s0->unk8 + row, (u8 *)arg1 + src * 2, 0x30);
        i++;
        row += 0x18;
        src += D_800BE620;
    } while (i != 0x18);

    {
    s32 q;
    for (y = -1.0f; y < 0.0f; y += 0.08333333582f) {
        w = func_15145A0C(fabsf(y), 1.0f, 1.0f);
        if (w) {
        dst[0][1] = (s32)(s2->unk38 + y * 12.0f) * D_800BE620;
        dst[1][1] = (s32)(s2->unk38 - y * 12.0f) * D_800BE620;
        invw = 1.0f / w;
        k = 1.0f - (1.0f - w * 0.6693121791f);
        x = -w;
        while (x < 0.0f) {
            f32 t;
            t = func_15145A0C(fabsf(x), w, invw);
            dst[0][0] = (s32)(s2->unk34 + x * 12.0f);
            dst[1][0] = (s32)(s2->unk34 - x * 12.0f);
            t = 1.0f - t * k;
            srcp[0][0] = (s32)(x * t * 12.0f + 12.0f);
            srcp[0][1] = (s32)(y * t * 12.0f + 12.0f) * 0x18;
            srcp[1][0] = (s32)(12.0f - x * t * 12.0f);
            srcp[1][1] = (s32)(12.0f - y * t * 12.0f) * 0x18;
            *(arg1 + dst[0][1] + dst[0][0]) = *(s0->unk8 + srcp[0][0] + srcp[0][1]);
            *(arg1 + dst[0][1] + dst[1][0]) = *(s0->unk8 + srcp[1][0] + srcp[0][1]);
            *(arg1 + dst[1][1] + dst[1][0]) = *(s0->unk8 + srcp[1][0] + srcp[1][1]);
            *(arg1 + dst[1][1] + dst[0][0]) = *(s0->unk8 + srcp[0][0] + srcp[1][1]);
            x += 0.08333333582f;
        }
        }
    }
    }
    }
}

void func_151CB49C(struct102 *arg0, void **arg1, u8 arg2) {
    void *temp_v0;
    s32 temp_a2;

    temp_a2 = arg2;
    if (temp_a2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x18) == *(s32 *)arg1) {
            func_1516972C(arg0);
        }
    } else if (temp_a2 == 0) {
        temp_v0 = *arg1;
        if (*(s32 *)((s32)arg0 + 0x18) == *(s32 *)((s32)temp_v0 + 0x318)) {
            func_1516972C(arg0);
        }
    }
}

void func_151CB510(void *arg0) {
    void *temp_v0;
    s32 base;
    f32 target;

    temp_v0 = *(void **)((s32)arg0 + 0x70);
    if ((*(u16 *)*(s32 *)((s32)temp_v0 + 0x36C) & 0x2000) && (base = (s32)arg0 + 0x70, *(u16 *)(*(s32 *)((s32)temp_v0 + 0x3D4) + 0x19E) == 0)) {
        target = *(f32 *)(base + 8);
    } else {
        target = 0.0f;
        base = (s32)arg0 + 0x70;
    }
    *(f32 *)(base + 4) += (target - *(f32 *)(base + 4)) * D_800AAEA8;
    *(u8 *)((s32)arg0 + 0x2E) = (u32)*(f32 *)(base + 4);
}

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  pad11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
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
    /* 0x44 */ u8  unk44;
    /* 0x45 */ u8  pad45[3];
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
} Emitter151CB5FC;

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 padF;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ u8 pad24[0x30];
    /* 0x54 */ s8 unk54;
    /* 0x55 */ u8 pad55[3];
} Payload151CB5FC;

extern f32 D_800AAD84[];
extern f32 D_800AADAC[];

void func_151CB5FC(void *arg0, u8 arg1, u8 arg2, s8 arg3, u8 arg4) {
    Emitter151CB5FC sp90;
    void *temp;
    Payload151CB5FC sp34;

    sp34.unk4 = 255.0f;
    sp34.unk8 = 255.0f;
    sp34.unkD = arg1;
    sp34.unk0 = arg0;
    sp34.unkC = (s32)((arg3 == -1) ? 0 : 4);
    sp34.unk54 = arg3;
    sp34.unk14 = 255.0f;
    sp90.unk12 = 0x12C;
    sp90.unk8 = 49.0f;
    sp34.unk10 = 0.0f;
    sp90.unkC = 18.0f;
    sp90.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg2 ? 0x40 : 0) | 0x32);
    sp90.unk16 = 1;
    sp90.unk18 = 0xFF;
    sp90.unk1A = 7;
    sp90.unk1B = 0xFF;
    sp90.unk1C = 0xFF;
    sp90.unk1D = 0xFF;
    sp90.unk1F = 0xFF;
    sp90.unk20 = 0xFF;
    sp90.unk21 = 0xFF;
    sp90.unk22 = 0xFF;
    sp90.unk23 = 0xFF;
    sp90.unk24 = 0;
    sp90.unk28 = 0x200004;
    sp90.unk2C = 0x1F0601;
    sp90.unk30 = 7;
    sp90.unk34 = 0x22;
    sp90.unk38 = 0x80;
    sp90.unk3C = 0x20;
    sp90.unk40 = 0;
    sp90.unk41 = 7;
    sp90.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp34.unkE = 0;
    sp90.unk1E = 0xFF;
    sp90.unk10 = 0x7B;
    sp34.unk1C = 0.0f;
    sp90.unk0 = 88.0f;
    sp90.unk4 = 91.0f;
    sp34.unk18 = D_800AAD84[D_80082FA0 == 1];
    sp34.unk20 = D_800AADAC[D_80082FA0 == 1];
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk1E = 0;
    sp90.unk10 = 0x7C;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk14 &= 0xFFFD;
    sp90.unk1E = 0xFF;
    sp90.unk10 = 0x7B;
    sp34.unkE = 1;
    sp90.unk0 = -88.0f;
    sp90.unk4 = 91.0f;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
    sp90.unk1E = 0;
    sp90.unk10 = 0x7C;
    temp = func_1515548C(&sp90, 5, 0, 0, 0x58, arg4, 0);
    if (temp != NULL) {
        memcpy((u8 *)temp + 0x70, &sp34, 0x58);
    }
}

void func_151CB918(Game1F4650EffectObject *arg0, Game1F4650EffectTriggerEvent *arg1, u8 arg2) {
    Game1F4650EffectState *base = &arg0->field_0x70;

    if (arg2 == 0x37) {
        if (base->field_0xE == arg1->field_0x0) {
            if (base->field_0x0 == arg1->field_0x4) {
                base->field_0x10 = 0.0f;
                base->field_0xC |= 0x1;
                base->field_0x14 = 0.0f;
            }
        }
    }
}

s32 func_151CB970(void *arg0) {
    typedef struct {
        u8 pad0[0xC];
        u8 unkC;
        u8 unkD;
        u8 padE[2];
        f32 unk10;
        f32 unk14;
        f32 unk18;
        u8 pad1C[4];
        f32 unk20;
    } Struct151CB970;
    register Struct151CB970 *base;
    f32 temp_f0;
    f32 pad0;
    f32 temp_f2;
    f32 temp_f12;
    u8 temp_v0;

    base = (Struct151CB970 *)((s32)arg0 + 0x70);
    temp_f0 = func_151CC1D4(arg0);
    temp_f2 = (1.0f - ((temp_f0 - base->unk18) * base->unk20)) * 75.0f;

    if (base->unkC & 1) {
        temp_f0 = sinf(base->unk10);
        temp_f12 = 91.0f + temp_f2;
        pad0 = (112.0f + temp_f2) - temp_f12;
        *(f32 *)((s32)arg0 + 0x14) = (temp_f0 * pad0) + temp_f12;
        base->unk10 += D_800AAEAC * D_800BE9A4;
        if (D_800AAEB0 <= base->unk10) {
            *(f32 *)((s32)arg0 + 0x14) = temp_f12;
            base->unkC &= ~1;
        }
    } else {
        *(f32 *)((s32)arg0 + 0x14) = 91.0f + temp_f2;
    }

    if (base->unkD == 2) {
        *(f32 *)((s32)arg0 + 0x14) -= 20.0f;
    }

    temp_v0 = (u32)base->unk14;
    if (*(u8 *)((s32)arg0 + 0x20) == 0x7C) {
        *(u8 *)((s32)arg0 + 0x2E) = 0xFF - temp_v0;
    } else {
        *(u8 *)((s32)arg0 + 0x2E) = temp_v0;
    }

    base->unk14 += (255.0f - base->unk14) * D_800AAEB4;
    return 1;
}

void func_151CBB6C(struct102 *arg0, void **arg1, u8 arg2) {
    void *temp_v0;
    s32 temp_a2;

    temp_a2 = arg2;
    if (temp_a2 == 0x21) {
        if (*(s32 *)((s32)arg0 + 0x20) == *(s32 *)arg1) {
            func_1516972C(arg0);
        }
    } else if (temp_a2 == 0) {
        temp_v0 = *arg1;
        if (*(s32 *)((s32)arg0 + 0x20) == *(s32 *)((s32)temp_v0 + 0x318)) {
            func_1516972C(arg0);
        }
    }
}

s32 func_151CBBE0(void *arg0) {
    typedef struct {
        u8 pad0[0x18];
        f32 unk18;
        u8 pad1C[4];
        f32 unk20;
        f32 unk24;
        f32 unk28;
    } Struct151CBBE0;
    register Struct151CBBE0 *base;
    f32 scaled;
    f32 temp_f0;

    base = (Struct151CBBE0 *)((s32)arg0 + 0x70);
    temp_f0 = func_151CC1D4(arg0);
    if (temp_f0 != base->unk28) {
        base->unk28 = temp_f0;
        scaled = ((temp_f0 - base->unk18) * base->unk20) * D_800AAEB8;
        func_1514373C(base->unk24 + scaled,
            55.0f, (f32 *)((s32)arg0 + 0x10), (f32 *)((s32)arg0 + 0x14));
    }
    return 1;
}

s32 func_151CBC60(u8 *arg0) {
    u8 *state = arg0 + 0x70;
    f32 value;
    f32 base;

    value = func_151CC1D4(arg0);
    if (value != *(f32 *)(state + 0x28)) {
        base = *(f32 *)(state + 0x4C);
        *(f32 *)(state + 0x28) = value;
        *(f32 *)(arg0 + 0x1C) = base - *(f32 *)(state + 0x20) * (value - *(f32 *)(state + 0x18)) * base + 1.0f;
    }
    if (*(state + 0xC) & 2) {
        *(f32 *)(state + 0x50) -= D_800BE9A4;
        value = *(f32 *)(state + 0x50);
        if (value <= 0.0f) {
            arg0[0x2B] = *(f32 *)(state + 0x3C);
            arg0[0x2C] = *(f32 *)(state + 0x40);
            arg0[0x2D] = *(f32 *)(state + 0x44);
            arg0[0x2E] = *(f32 *)(state + 0x48);
            *(state + 0xC) &= ~2;
        } else {
            arg0[0x2B] = (*(f32 *)(state + 0x2C) - *(f32 *)(state + 0x3C)) * (value * 0.028571429f) + *(f32 *)(state + 0x3C);
            arg0[0x2C] = (*(f32 *)(state + 0x30) - *(f32 *)(state + 0x40)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x40);
            arg0[0x2D] = (*(f32 *)(state + 0x34) - *(f32 *)(state + 0x44)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x44);
            arg0[0x2E] = (*(f32 *)(state + 0x38) - *(f32 *)(state + 0x48)) * (*(f32 *)(state + 0x50) * 0.028571429f) + *(f32 *)(state + 0x48);
        }
    }
    return 1;
}

extern f32 D_800AAEC0;

f32 func_151CC1D4(void *arg0) {
    typedef struct {
        u8 *unk0;
        u8 pad4[0x8];
        u8 unkC;
        u8 padD[0xB];
        f32 unk18;
        f32 unk1C;
        u8 pad20[0x34];
        s8 unk54;
    } Struct151CC1D4;
    Struct151CC1D4 *base;
    f32 v;

    base = (Struct151CC1D4 *)((s32)arg0 + 0x70);
    if (base->unkC & 4) {
        f32 t = D_800C3958[base->unk54].unk28 * D_800AAEC0;

        v = base->unk18 + (base->unk1C - base->unk18) * t;
    } else {
        v = (*(f32 *)(base->unk0 + 0x1A0) + *(f32 *)(base->unk0 + 0x19C)) * 0.5f;
    }
    if (v < base->unk18) {
        v = base->unk18;
    } else if (base->unk1C < v) {
        v = base->unk1C;
    }
    return v;
}


extern void func_1515572C(s32, s32);

void func_151CC290(s32 arg0) {
    s32 sp1C;

    sp1C = arg0;
    func_1515572C((s32)&sp1C, 0x46);
}

void func_151CC2BC(void *arg0, s32 *arg1, u8 arg2) {
    typedef struct {
        s32 field_0x0;
        u8 pad_0x4[0x8];
        u8 field_0xC;
        u8 pad_0xD[0x1F];
        f32 field_0x2C;
        f32 field_0x30;
        f32 field_0x34;
        f32 field_0x38;
        u8 pad_0x3C[0x14];
        f32 field_0x50;
    } State;
    State *base;

    base = (State *)((s32)arg0 + 0x70);
    if ((arg2 == 0x46) && (*arg1 == *(s32 *)((u8 *)base + 0))) {
        base->field_0xC |= 2;
        base->field_0x50 = 35.0f;
        *(u8 *)((s32)arg0 + 0x2B) = (u32)base->field_0x2C;
        *(u8 *)((s32)arg0 + 0x2C) = (u32)base->field_0x30;
        *(u8 *)((s32)arg0 + 0x2D) = (u32)base->field_0x34;
        *(u8 *)((s32)arg0 + 0x2E) = (u32)base->field_0x38;
    }
}

void func_151CC524(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CC524_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CC524_sp2C;
    struct_151CC524_sp88 sp88;
    void *temp_v0;
    struct_151CC524_sp2C sp2C;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk50 = 0.0f;
    sp88.unk54 = 0.0f;
    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10 | 0x80 | 0x100 | 0x200);
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0x82;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk48 = 0.200000003f;
    sp88.unk4C = 1.0f;
    sp88.unk8 = 28.0f;
    sp88.unkC = 4.199999809f;
    sp88.unk4 = -37.0f;
    sp88.unk10 = 0x9A;
    sp88.unk0 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 9, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9B;
    sp88.unk4 = 0.0f;
    sp88.unk48 = 1.0f;
    sp88.unk4C = 0.200000003f;
    sp88.unk8 = 4.199999809f;
    sp88.unkC = 28.0f;
    sp88.unk0 = -37.0f;
    temp_v0 = func_1515548C(&sp88, 9, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

s32 func_151CC77C(void *arg0) {
    register f32 *temp_a1;
    register f32 *temp_a2;
    struct {
        f32 sp20;
        f32 sp24;
        s32 pad28;
        s32 pad2C;
    } sp20;
    void *temp_v0;
    void *temp_v1;

    temp_v0 = *(void **)((s32)arg0 + 0x70);
    temp_v1 = *(void **)((s32)temp_v0 + 0x3D0);
    if (temp_v1 == NULL) {
        return 0;
    }
    temp_v0 = *(void **)((s32)temp_v1 + 0x31C);
    temp_a1 = &sp20.sp24;
    temp_a2 = &sp20.sp20;
    if (temp_v0 == NULL) {
        return 0;
    }
    func_15145974((struct17 *)((s32)temp_v0 + 0x130), temp_a1, temp_a2);
    if (*(u8 *)((s32)arg0 + 0x20) == 0x9A) {
        *(f32 *)((s32)arg0 + 0x60) = -((sp20.sp24 * D_800AAED4) * 2560.0f) * 2.0f;
    } else {
        *(f32 *)((s32)arg0 + 0x64) = ((sp20.sp20 * D_800AAED8) * 2560.0f) * 2.0f;
    }
    return 1;
}

void func_151CC840(void *arg0, u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CC840_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4A];
    } struct_151CC840_sp2C;
    struct_151CC840_sp88 sp88;
    void *temp_v0;
    struct_151CC840_sp2C sp2C;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | ((arg4 ? 0x40 : 0) | 0x10);
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0xB4;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 0.0f;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk10 = 0x64;
    sp88.unkC = 12.0f;
    sp88.unk8 = 12.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9D;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unkC = 7.0f;
    sp88.unk8 = 7.0f;
    sp88.unk0 = 55.0f;
    sp88.unk4 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 2;
    sp88.unk0 = -55.0f;
    sp88.unk4 = 0.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9C;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 4;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 55.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unk0 = 0.0f;
    sp88.unk4 = -55.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk10 = 0x9E;
    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 6;
    sp88.unkC = 6.0f;
    sp88.unk8 = 6.0f;
    sp88.unk0 = 44.0f;
    sp88.unk4 = 44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 4;
    sp88.unk0 = -44.0f;
    sp88.unk4 = 44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 0;
    sp88.unk0 = -44.0f;
    sp88.unk4 = -44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }

    sp88.unk14 &= 0xFFF9;
    sp88.unk14 |= 2;
    sp88.unk0 = 44.0f;
    sp88.unk4 = -44.0f;
    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, arg5, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

void func_151CCD1C(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        s16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CCD1C_sp88;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0x4E];
    } struct_151CCD1C_sp2C;
    struct_151CCD1C_sp88 sp88;
    struct_151CCD1C_sp2C sp2C;
    void *temp_v0;

    sp2C.unk0 = arg0;
    sp2C.unk4 = 0.0f;
    sp2C.unkD = 0;
    sp2C.unkC = 0;
    sp2C.unk8 = 130.0f;

    sp88.unk12 = 0x12C;
    sp88.unk14 = (1 << (*(u8 *)((s32)arg0 + 0x23D) + 0xB)) | 0x50;
    sp88.unk16 = 1;
    sp88.unk18 = 0xFF;
    sp88.unk1A = 7;
    sp88.unk1B = arg1;
    sp88.unk1C = arg2;
    sp88.unk1D = arg3;
    sp88.unk1E = 0xB4;
    sp88.unk1F = 0xFF;
    sp88.unk20 = 0xFF;
    sp88.unk21 = 0xFF;
    sp88.unk22 = 0xFF;
    sp88.unk23 = 0xFF;
    sp88.unk24 = 0;
    sp88.unk28 = 0x200004;
    sp88.unk2C = 0x1F0601;
    sp88.unk30 = 8;
    sp88.unk34 = 0x44;
    sp88.unk38 = 0x80;
    sp88.unk3C = 0x20;
    sp88.unk40 = 0;
    sp88.unk41 = 0xA;
    sp88.unk0 = 0.0f;
    sp88.unk4 = 0.0f;
    sp88.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    sp88.unk10 = 0xB4;
    sp88.unk8 = 65.0f;
    sp88.unkC = 65.0f;

    temp_v0 = func_1515548C(&sp88, 0, 0, 0, 0x58, 0xFF, 1);
    if (temp_v0 != NULL) {
        memcpy((u8 *)temp_v0 + 0x70, &sp2C, 0x58);
    }
}

void func_151CCE94(void *arg0) {
    void *temp_v0;
    void *sp28;

    if (D_800BE616 != 0) {
        sp28 = arg0;
        temp_v0 = func_151A4FD0(0, 0, 0, 0xFF, 0, *(u8 *)((s32)arg0 + 0x23D), 1, 4);
        if (temp_v0 != NULL) {
            memcpy((void *)((s32)temp_v0 + 0x20), &sp28, 4);
        }
    }
}

extern f32 D_800AAD8C[];
extern f32 D_800AADB4[];
typedef struct { u8 b[4]; } Colors151CCF08;
extern Colors151CCF08 D_8008FC28;

void func_151CCF08(void *arg0, u8 arg1, s8 arg2, u8 arg3) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
        u8  unk10;
        u8  pad11;
        s16 unk12;
        u16 unk14;
        s16 unk16;
        s16 unk18;
        u8  unk1A;
        u8  unk1B;
        u8  unk1C;
        u8  unk1D;
        u8  unk1E;
        u8  unk1F;
        u8  unk20;
        u8  unk21;
        u8  unk22;
        u8  unk23;
        s32 unk24;
        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        s32 unk38;
        s32 unk3C;
        u8  unk40;
        u8  unk41;
        u8  pad42[2];
        u8  unk44;
        u8  pad45[3];
        f32 unk48;
        f32 unk4C;
        f32 unk50;
        f32 unk54;
    } struct_151CCF08_spE0;
    typedef struct {
        void *unk0;
        f32 unk4;
        f32 unk8;
        u8 unkC;
        u8 unkD;
        u8 padE[0xA];
        f32 unk18;
        f32 unk1C;
        f32 unk20;
        u8 pad24[0x30];
        s8 unk54;
        u8 pad55[3];
    } struct_151CCF08_sp84;
    struct_151CCF08_spE0 spE0;
    void *temp_v0;
    struct_151CCF08_sp84 sp84;
    u8 i;
    Colors151CCF08 sp7C;
    u8 j;

    sp84.unk1C = 0.0f;
    sp84.unk0 = arg0;
    sp84.unk4 = 0.0f;
    sp84.unk18 = D_800AAD8C[D_80082FA0 == 1];
    sp84.unk20 = D_800AADB4[D_80082FA0 == 1];
    sp84.unk8 = 130.0f;
    sp84.unkC = (s32)((arg2 == -1) ? 0 : 4);
    sp84.unk54 = arg2;

    spE0.unk12 = 0x12C;
    spE0.unk14 = (arg1 ? 0x40 : 0) | 0x20 | 0x10;
    spE0.unk16 = 1;
    spE0.unk18 = 0xFF;
    spE0.unk1A = 7;
    spE0.unk1B = 0xFF;
    spE0.unk1C = 0xFF;
    spE0.unk1D = 0xFF;
    spE0.unk1E = 0xFF;
    spE0.unk1F = 0xFF;
    spE0.unk20 = 0xFF;
    spE0.unk21 = 0xFF;
    spE0.unk22 = 0xFF;
    spE0.unk23 = 0xFF;
    spE0.unk24 = 0;
    spE0.unk28 = 0x200004;
    spE0.unk2C = 0x1F0601;
    spE0.unk30 = 8;
    spE0.unk34 = 0x13;
    spE0.unk38 = 0x80;
    spE0.unk3C = 0x20;
    spE0.unk40 = 0;
    spE0.unk41 = 0;
    spE0.unk44 = *(u8 *)((s32)arg0 + 0x23D);
    spE0.unk50 = 0.0f;
    spE0.unk54 = 0.0f;
    spE0.unk48 = 1.0f;
    spE0.unk4C = 1.0f;
    spE0.unk8 = 35.75f;
    spE0.unkC = 16.0f;

    for (j = 0; j < 2; j++) {
        sp7C = D_8008FC28;
        spE0.unk4 = j ? 92.0f : -90.0f;
        spE0.unk14 |= j ? 4 : 0;
        sp84.unkD = j ? 5 : 4;
        for (i = 0; i < 4; i++) {
            spE0.unk0 = ((i * 71.5f) + 35.75f) - 143.0f;
            spE0.unk10 = sp7C.b[i];
            temp_v0 = func_1515548C(&spE0, 0xB, 0, 0, 0x58, arg3, 1);
            if (temp_v0 != NULL) {
                memcpy((u8 *)temp_v0 + 0x70, &sp84, 0x58);
            }
        }
    }
}

extern f32 func_151CC1D4(void *);

void func_151CD224(Game1F4650EffectObject *arg0) {
    Game1F4650EffectState *base = &arg0->field_0x70;
    f32 ret = func_151CC1D4(arg0);
    f32 temp = (1.0f - (ret - base->field_0x18) * base->field_0x20) * 75.0f;
    if (base->field_0xD == 5) {
        arg0->field_0x14 = 92.0f + temp;
    } else if (base->field_0xD == 4) {
        arg0->field_0x14 = -92.0f - temp;
    }
}
