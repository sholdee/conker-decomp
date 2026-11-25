#include <ultra64.h>
#define func_1502B7F0 func_1502B7F0__proto
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_1511FC2C func_1511FC2C__proto
#define func_1511FC44 func_1511FC44__proto
#define func_150A7CB0 func_150A7CB0__proto
#include "functions.h"
#undef func_1502B7F0
#undef func_150ADA20
#undef func_1511FC2C
#undef func_1511FC44
#undef func_150A7CB0
#include "variables.h"

u32 func_150ADA20(void);
void func_150A7CB0(f32 mtx[4][4], f32 scaleX, f32 scaleY, f32 scaleZ); // functions.h types the scales s32

extern u8 D_800DBEA8[];
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);

typedef struct {
    u8  pad0[0x24];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} Struct151103C8;

typedef struct Game13D350ViewAngles {
    u8 pad0[0xC];
    f32 pitch;
    f32 yaw;
    f32 roll;
} Game13D350ViewAngles;

typedef Gfx *(*Game13D350ViewDraw)(Gfx *, f32, f32, f32, f32, f32, f32, s32);

Gfx *func_151106A8(Gfx *);
s32 func_151108C4(f32, f32, f32, f32, f32);
extern s16 D_80082FA6;
extern u8 D_800D9B69;
extern u8 D_800D9B6A;
extern u8 D_800D9B6B;
extern u8 D_800D9B6C;
extern u8 D_800D9B6D;
extern u8 D_800D9B6E;
extern u8 D_800D9B6F;
extern u8 D_800D9B70;
extern u8 D_800D9B71;
extern u8 D_800D9B72;
extern u8 D_800D9B73;
extern u8 D_800D9B79;
extern u8 D_800D9B7A;
extern u8 D_800D9B7B;
extern u8 D_800D9B7C;
extern u8 D_800D9B7D;
extern u8 D_800D9B7E;
extern u8 D_800D9B7F;
extern u8 D_800D9B80;
extern u8 D_800D9B81;
extern u8 D_800D9B82;
extern u8 D_800D9B83;
extern Game13D350ViewAngles D_800DCDE0[];
extern Game13D350ViewDraw D_800891D8[];
Gfx *func_1510FEA0(Gfx *arg0, s32 arg1);

extern s32 D_800BE628;
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);
void func_151102CC(f32 a0[4][4], f32 a1, f32 a2, f32 a3);
void func_151102CC(f32 arg0[4][4], f32 arg1, f32 arg2, f32 arg3);
void func_15110360(s32 arg0, f32 arg1[4][4], f32 arg2, f32 arg3, f32 arg4);
Gfx *func_151103C8(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);

Gfx *func_1501A680(Gfx *arg0);
Gfx *func_1501A6CC(Gfx *arg0, s32 a, s32 b, s32 c, s32 d);
Gfx *func_15110544(Gfx *gfx, s32 a, s32 b, s32 c, s32 d, u8 r, u8 g, u8 bl);
Gfx *func_15110600(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3);

extern u8 D_800BEAC2;
Gfx *func_151106A8(Gfx *arg0);

extern s32 *D_800DBE80;
extern s32 *D_800891BC[];
s32 func_1502B7F0();
void func_10004074();
void func_1510D694(s32);
void func_15111858(void);
void func_1511172C(s32 arg0);

extern void *allocate_memory(s32, s32, s32, s32);

typedef struct Star15111858 {
    u16 x;
    u16 y;
    u16 z;
    u8 unk6;
    u8 unk7;
} Star15111858;
void func_15111858(void);

typedef struct Turret15112520Cfg {
    /* 0x00 */ f32 speedNear;
    /* 0x04 */ f32 speedFar;
    /* 0x08 */ f32 maxStep;
    /* 0x0C */ f32 yawMax;
    /* 0x10 */ f32 yawMin;
    /* 0x14 */ f32 yawMargin;
    /* 0x18 */ f32 pitchMax;
    /* 0x1C */ f32 pitchMin;
    /* 0x20 */ f32 pitchMargin;
    /* 0x24 */ f32 baseYaw;
    /* 0x28 */ f32 basePitch;
    /* 0x2C */ f32 range;
    /* 0x30 */ u16 sfx;
    /* 0x32 */ u16 timer;
} Turret15112520Cfg;

typedef struct Turret15112520 {
    /* 0x00 */ f32 pitch;
    /* 0x04 */ f32 yaw;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x26];
    /* 0x3C */ Turret15112520Cfg *cfg;
} Turret15112520;


extern f32 D_800DBEC0;

/* Turns a turret toward the player (within its yaw/pitch limits and range) and plays its motor sound. */
void func_15112520(Turret15112520 *turret) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 yaw;
    f32 pitch;
    f32 delta;
    s32 volume;
    f32 speed;
    Turret15112520Cfg *cfg;
    s32 pad; // matching: unused stack slot
    static s16 D_80089234; // sound timer: 0 idle, -1 start the sound, >0 cooldown frames

    cfg = turret->cfg;
    dx = D_800CC2D0[0].x_position - turret->x;
    dy = (D_800CC2D0[0].y_position + 65.0f) - turret->y;
    dz = D_800CC2D0[0].z_position - turret->z;
    D_800DBEC0 = dx * dx + dy * dy + dz * dz;
    if (cfg->range * cfg->range < D_800DBEC0) {
        pitch = cfg->basePitch;
        yaw = cfg->baseYaw;
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
        speed = cfg->speedFar;
    } else {
        yaw = func_150484A0(dx, dz);
        pitch = func_150484A0(dy, sqrtf(dx * dx + dz * dz));
        yaw *= 57.29577637f;
        pitch *= 57.29577637f;
        if (pitch < 0.0f) {
            pitch += 360.0f;
        } else if (pitch >= 360.0f) {
            pitch -= 360.0f;
        }
        if (yaw < 0.0f) {
            yaw += 360.0f;
        } else if (yaw >= 360.0f) {
            yaw -= 360.0f;
        }
        delta = func_15048A70(cfg->baseYaw, yaw);
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        if (cfg->yawMax + cfg->yawMargin < delta || delta < cfg->yawMin - cfg->yawMargin) {
            yaw = cfg->baseYaw;
            pitch = cfg->basePitch;
            speed = cfg->speedFar;
        } else {
            speed = cfg->speedNear;
            delta = (delta < cfg->yawMin) ? cfg->yawMin : ((cfg->yawMax < delta) ? cfg->yawMax : delta);
            yaw = cfg->baseYaw + delta;
            if (yaw < 0.0f) {
                yaw += 360.0f;
            } else if (yaw >= 360.0f) {
                yaw -= 360.0f;
            }
            if (cfg->pitchMax + cfg->pitchMargin < pitch || pitch < cfg->pitchMin - cfg->pitchMargin) {
                pitch = 0.0f;
            } else {
                pitch = (pitch < cfg->pitchMin) ? cfg->pitchMin : ((cfg->pitchMax < pitch) ? cfg->pitchMax : pitch);
            }
        }
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
    }
    delta = func_15048A70(turret->yaw, yaw);
    yaw = fabsf(delta);
    if (cfg->maxStep < yaw) {
        delta = ((delta < 0.0f) ? -1.0f : 1.0f) * cfg->maxStep;
    }
    turret->yaw += delta * speed;
    if (D_80089234 == 0 && yaw > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->yaw < 0.0f) {
        turret->yaw += 360.0f;
    } else if (turret->yaw >= 360.0f) {
        turret->yaw -= 360.0f;
    }
    delta = func_15048A70(turret->pitch, 360.0f - pitch);
    turret->pitch += delta * speed;
    if (D_80089234 == 0 && fabsf(delta) > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->pitch < 0.0f) {
        turret->pitch += 360.0f;
    } else if (turret->pitch >= 360.0f) {
        turret->pitch -= 360.0f;
    }
    if (D_80089234 == -1 && cfg->timer != 0) {
        volume = 32000.0f - D_800DBEC0 * 0.1000000015f;
        D_80089234 = cfg->timer;
        if (volume > 0) {
            func_10010F30(cfg->sfx, 32000, 64, 0, 0);
        }
    } else if (D_80089234 > 0) {
        D_80089234--;
    }
}
/*
 * One entry of the 0xA0-byte object array at D_800DBEF4 (D_800DBEF0 entries), as used by this file.
 * include/structs.h's struct131 is the same object, declared only up to 0x6F.
 */
typedef struct Game13F9D0Obj {
    /* 0x00 */ f32 rotX;
    /* 0x04 */ f32 rotY;
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ u8 pad0C[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ f32 yOffset;
    /* 0x1C */ Gfx *displayList;
    /* 0x20 */ s32 matrices[2]; // segment 6 address, one per frame buffer
    /* 0x28 */ u8 pad28[4];
    /* 0x2C */ f32 scaleX;
    /* 0x30 */ f32 scaleY;
    /* 0x34 */ f32 scaleZ;
    /* 0x38 */ void (*update)(struct Game13F9D0Obj *);
    /* 0x3C */ u8 pad3C[0x10];
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 pad4D;
    /* 0x4E */ u8 attach; // < 3: own transform, 3: none, 101..124: rides actor D_800CC2D0[attach - 100]
    /* 0x4F */ u8 flags;
    /* 0x50 */ u16 unk50;
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u8 pad54[6];
    /* 0x5A */ s16 moveX; // translation this frame
    /* 0x5C */ s16 moveY;
    /* 0x5E */ s16 moveZ;
    /* 0x60 */ f32 spinX; // rotation this frame (degrees)
    /* 0x64 */ f32 spinY;
    /* 0x68 */ f32 spinZ;
    /* 0x6C */ u16 effectFlags;
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 viewFlags; // bits 0-3: visible in view N, 0x40: update this frame
    /* 0x70 */ u8 state; // 1: static matrix, 8: removed, high nibble: objects following in its group
    /* 0x71 */ u8 pad71;
    /* 0x72 */ u8 id;
    /* 0x73 */ u8 pad73[5];
    /* 0x78 */ void (*onContact)(struct Game13F9D0Obj *, struct127 *);
    /* 0x7C */ s32 unk7C;
    /* 0x80 */ s32 unk80;
    /* 0x84 */ u8 pad84[4];
    /* 0x88 */ u16 fadeDistance; // fades out when the camera is closer than this
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 viewAlpha[5];
    /* 0x90 */ u16 drawDistance; // 0: default (D_800D9B1C), 1: 40000
    /* 0x92 */ u8 unk92;
    /* 0x93 */ u8 pad93;
    /* 0x94 */ struct102 *unk94;
    /* 0x98 */ u8 unk98[8];
} Game13F9D0Obj; // size 0xA0

#define OBJECTS ((Game13F9D0Obj *)D_800DBEF4)

/* Per-view list of the objects to draw this frame. */
typedef struct DrawEntry {
    u16 index;    // into OBJECTS
    u16 distance; // from func_150A6360
} DrawEntry;

extern DrawEntry **D_80089240[]; // per view
extern Mtx **D_80089250[];       // per frame buffer: one Mtx per object
extern u16 D_800DBEE8[];         // per view: entries in D_80089240
extern f32 D_800D9C10[][4][4];   // per view
extern u8 D_800C3658[];

extern s32 func_15022B08(s32, s32);
extern s32 func_1503F800(u8 *, s16, s16, s32, s32);
extern s32 func_150A6360(s32, void *, f32, f32, f32, f32, f32, f32);

/* Builds each view's draw list: distance-culls and fades every object group, then appends visible groups. */
void func_15112A80(s32 player) {
    s32 drawDist;
    s32 alpha;
    Game13F9D0Obj *obj;
    s32 i;
    s32 dist[16];
    s32 step;
    s32 mask;
    s32 end;
    s32 start;
    s32 view;
    u16 *count;
    s32 groupSize;
    s32 *dp;

    for (view = 0; view <= D_80082FA0; view++) {
        s32 recp;

        count = &D_800DBEE8[view];
        recp = view * 0x180 + D_800BE628;
        if (func_150859AC(view, 0) != 0) {
            D_800DBEE8[view] = 0;
            for (i = 0; i < D_800DBEF0;) {
                groupSize = OBJECTS[i].state >> 4;
                end = i + groupSize;
                mask = 0;
                start = i;
                for (; i <= end; i++) {
                    obj = i + OBJECTS; // matching: &OBJECTS[i] swaps the addu operands
                    obj->viewFlags &= ~(1 << view);
                    // matching: empty then-branch orders uopt's live ranges (see func_15112A80 notes)
                    if (!(obj->unk6E != 1 && !(obj->state & 8) && (obj->flags & 1) && obj->alpha != 0 &&
                          (D_800C3658[player] != 0 || (&D_800C35EA)[player] != 1 || func_15022B08(i, player) != 0) &&
                          (((&D_800C35EA)[player] == 1 && func_15022B08(i, player) != 0) ||
                           !(obj->effectFlags & 0x100) || func_1503F800(obj->unk98, obj->x, obj->z, view, 0) == 0))) {
                    } else {
                        f32 (*mtx)[4];

                        mtx = D_800D9C10[view];
                        dp = (s32 *)((u8 *)dist + (i - start) * 4); // matching: &dist[i - start] scales i and start separately
                        if (D_800DBFF0[view].unk3D4->unk197 != 0) {
                            drawDist = D_800D9B1C;
                        } else if (obj->drawDistance == 0) {
                            drawDist = D_800D9B1C;
                        } else if (obj->drawDistance == 1) {
                            drawDist = 40000;
                        } else {
                            drawDist = obj->drawDistance;
                        }
                        *dp = func_150A6360(recp, mtx, obj->x, obj->y + obj->yOffset, obj->z,
                                            (f32)(u32)obj->unk50, (f32)(u32)obj->unk52, drawDist);
                        if (*dp < 0) {
                            *dp = -*dp;
                        }
                        if (*dp == 0) {
                            obj->viewAlpha[view] = 0;
                        } else if (drawDist < *dp) {
                            obj->viewAlpha[view] = 0;
                        } else {
                            if (*dp < drawDist - 200) {
                                step = D_800BE9E4 * 10;
                                if (obj->fadeDistance != 0) {
                                    if (*dp < obj->fadeDistance) {
                                        if (step < obj->viewAlpha[view]) {
                                            obj->viewAlpha[view] -= step;
                                        } else {
                                            obj->viewAlpha[view] = 0;
                                        }
                                    } else {
                                        if (obj->viewAlpha[view] < 255 - step) {
                                            obj->viewAlpha[view] += step;
                                        } else {
                                            obj->viewAlpha[view] = 255;
                                        }
                                    }
                                    if (obj->viewAlpha[view] == 0) {
                                        continue;
                                    }
                                } else {
                                    obj->viewAlpha[view] = 255;
                                // matching: this label splits the basic block, changing as1's scheduling
                                fence:;
                                }
                            } else {
                                alpha = ((drawDist - *dp) * 255) / 200;
                                if (alpha >= 256) {
                                    alpha = 255;
                                }
                                obj->viewAlpha[view] = alpha;
                            }
                            mask |= 1 << view;
                        }
                    }
                }
                if (mask != 0) {
                    for (i = start; i <= end; i++) {
                        OBJECTS[i].viewFlags |= 1 << view;
                        (*D_80089240[view])[*count].index = i;
                        (*D_80089240[view])[*count].distance = *(s32 *)((u8 *)dist + (i - start) * 4);
                        (*count)++;
                    }
                }
            }
        }
    }
}
/* Flags every object that is visible in some view (or always-update, state bit 4) and has an update callback. */
void func_15113180(void) {
    s32 i;

    for (i = 0; i < D_800DBEF0; i++) {
        OBJECTS[i].viewFlags &= ~0x40;
        if (((OBJECTS[i].viewFlags & 0xF) != 0 || (OBJECTS[i].state & 4) == 4) && OBJECTS[i].update != NULL) {
            OBJECTS[i].viewFlags |= 0x40;
        }
    }
}
/* The part of the D_800CC2D0 actor (struct127) this file uses. */
typedef struct Game13F9D0Actor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ f32 pos[3];
    /* 0x020 */ u8 pad20[0x56];
    /* 0x076 */ u16 movingAngle;
    /* 0x078 */ u8 pad78[2];
    /* 0x07A */ s16 facingAngle;
    /* 0x07C */ u8 pad7C[0x14];
    /* 0x090 */ s16 unk90;
    /* 0x092 */ u8 pad92[0xA5];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 pad138[0x9C];
    /* 0x1D4 */ struct { f32 mtx[4][4]; } *unk1D4; // world matrix
    /* 0x1D8 */ u8 pad1D8[0x154];
} Game13F9D0Actor; // size 0x32C

#define ACTORS ((Game13F9D0Actor *)D_800CC2D0)

void func_1511490C(f32 mtx[4][4], Game13F9D0Obj *obj);
void func_150A7B80(void *);
extern f32 D_800DBF08[][4][4];

/* Writes this frame's model matrix for every object in each view's draw list. */
void func_15113218(void) {
    Game13F9D0Obj *obj;
    Game13F9D0Actor *actor;
    f32 (*parent)[4];
    f32 mf[4][4];
    s32 j;
    s32 view;
    s32 pad1[2]; // matching: unused stack slots
    f32 rot[3];
    s32 pad2[3];

    for (view = 0; view <= D_80082FA0; view++) {
        if (func_150859AC(view, 0) == 0) {
            continue;
        }
        for (j = 0; j < D_800DBEE8[view]; j++) {
                obj = &OBJECTS[(*D_80089240[view])[j].index];
                if ((obj->state & 1) == 1) {
                    func_150A7B80(&(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                    continue;
                }
                if (obj->attach == 3) {
                    continue;
                }
                if (obj->attach < 3) {
                    func_1511490C(mf, obj);
                    guMtxF2L(mf, &(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                } else if (obj->attach > 100 && obj->attach < 125) {
                    actor = &ACTORS[obj->attach - 100];
                    if (actor->unk1D4 == NULL) {
                        func_150A7CB0(mf, obj->scaleX, obj->scaleY, obj->scaleZ);
                        mf[3][0] = actor->pos[0];
                        mf[3][1] = actor->pos[1];
                        mf[3][2] = actor->pos[2];
                        guMtxF2L(mf, &(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                    } else {
                        rot[2] = 0.0f;
                        rot[0] = rot[2];
                        rot[1] = actor->unk90;
                        parent = actor->unk1D4->mtx;
                        func_150A7CB0(mf, obj->scaleX, obj->scaleY, obj->scaleZ);
                        mf[3][0] = parent[3][0];
                        mf[3][1] = parent[3][1];
                        mf[3][2] = parent[3][2];
                        func_150442C0(mf, rot[0], rot[1], rot[2]);
                        obj->x = mf[3][0];
                        obj->y = mf[3][1];
                        obj->z = mf[3][2];
                        guMtxF2L(mf, &(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                    }
                } else {
                    guMtxF2L(D_800DBF08[D_800BE9C0], &(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                }
            }
    }
}
Gfx *func_151733D8(Gfx *gfx, s32);
Gfx *func_151137D4(Gfx *gfx, Game13F9D0Obj *obj, Mtx *mtx, s32 view, s32 distance, s32 pass);
Gfx *func_1515E544(Gfx *gfx, s32, s32, s32, void *);
extern Mtx D_80089470;
extern u8 D_800D9E21;

/* Draws one view's object list; translucent selects the pass (objects with flags bit 0x10 or alpha < 255). */
Gfx *func_151135C4(Gfx *gfx, s32 translucent, s16 view) {
    s32 viewOffset;
    s32 pass;
    s32 i;
    DrawEntry ***list;

    if (translucent == 0) {
        pass = 0;
    } else {
        pass = 0x10;
    }
    gfx = func_151733D8(gfx, 0);
    gSPClearGeometryMode(gfx++, G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | 0x400000);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    viewOffset = view * 4; // matching: indexing both tables by view computes view * 4 twice
    list = (DrawEntry ***)((u8 *)D_80089240 + viewOffset);
    gfx = func_1515E544(gfx, *(s32 *)((u8 *)D_800D9E10 + viewOffset), D_800D9E20, D_800D9E21,
                        D_800D9BD0[view][D_800BE9C0]);
    for (i = 0; i < D_800DBEE8[view]; i++) {
        gfx = func_151137D4(gfx, &OBJECTS[(**list)[i].index], &(*D_80089250[D_800BE9C0])[(**list)[i].index], view,
                            (**list)[i].distance, pass);
    }
    gSPMatrix(gfx++, &D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gfx++, G_CULL_FRONT | G_LOD | 0x400000);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    return gfx;
}

Gfx *func_1511FC2C(Gfx *gfx, f32 x, f32 y, f32 z, f32, s32 view);
Gfx *func_1511FC44(Gfx *gfx, f32 x, f32 y, f32 z, s32 view);
Gfx *func_1510CDB8(Gfx *gfx, s32, s32 alpha, s32 view);
Gfx *func_1510D8C0(Gfx *gfx, Game13F9D0Obj *obj);
Gfx *func_150CF5E8(Gfx *gfx);
Gfx *func_150A50C0(Gfx *dl, Gfx *gfx, f32 mtx[4][4], s32, s32, s32);
extern u8 D_80083D40[];
extern u8 D_80083EC0[];
extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;

/*
 * Draws one object in one view, if it belongs to this pass (opaque pass: alpha 255 and no flags bit 0x10).
 * Returns gfx unchanged if nothing was drawn or the display list would overflow.
 */
Gfx *func_151137D4(Gfx *gfx, Game13F9D0Obj *obj, Mtx *mtx, s32 view, s32 distance, s32 pass) {
    s32 alpha;
    Gfx *start;
    s32 overflow;
    u16 effects;

    start = gfx;
    if (obj->viewAlpha[view] == 255 && obj->alpha == 255) {
        alpha = 255;
    } else {
        alpha = (obj->viewAlpha[view] * obj->alpha) / 255;
        if (alpha == 0) {
            return gfx;
        }
    }
    if (alpha == 255) {
        if (pass == (obj->flags & 0x10)) {
            return gfx;
        }
    } else if (pass != 0) {
        return gfx;
    }
    effects = obj->effectFlags;
    if (obj->effectFlags != 0) {
        if (effects & 1) {
            gfx = func_1511FC2C(gfx, obj->x, obj->y + obj->yOffset, obj->z, obj->rotY, view);
        } else if (effects & 0x10) {
            gfx = func_1511FC2C(gfx, obj->x, obj->y + obj->yOffset, obj->z, obj->unk7C, view);
        } else if (effects & 0x20) {
            gfx = func_1511FC44(gfx, obj->x, obj->y + obj->yOffset, obj->z, view);
        }
    }
    gDPPipeSync(gfx++);
    if (alpha < 255) {
        gSPSegment(gfx++, 8, D_80083D40);
    } else {
        gSPSegment(gfx++, 8, D_80083EC0);
    }
    if ((obj->flags & 2) == 2) {
        gSPSetGeometryMode(gfx++, 0x400000);
    } else {
        gSPClearGeometryMode(gfx++, 0x400000);
    }
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    gfx = func_1510D8C0(func_1510CDB8(gfx, 255, alpha, view), obj);
    if (pass == 0) {
        gSPSetGeometryMode(gfx++, G_FOG | G_LOD);
    }
    gSPSegment(gfx++, 1, obj->matrices[D_800BE9C0]);
    gSPMatrix(gfx++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gfx++, G_TEXTURE_GEN_LINEAR);
    if (obj->effectFlags & 0x200) {
        gfx = func_150CF5E8(gfx);
    }
    if ((obj->state & 2) == 2) {
        gfx = func_150A50C0(obj->displayList, gfx, D_800D9C10[view], 0, 0, obj->matrices[D_800BE9C0] + 0xFF000000);
    } else {
        gSPDisplayList(gfx++, obj->displayList);
    }
    { // matching: hand-written through one pointer; the GBI macros' own _g copy reorders the stores
        Gfx *cmd = gfx++; cmd->words.w0 = 0xD9EEFFFF; cmd->words.w1 = 0; // gSPClearGeometryMode(G_FOG | G_LOD)
        cmd = gfx++; cmd->words.w0 = 0xE7000000; cmd->words.w1 = 0;      // gDPPipeSync
    }
    overflow = (D_800BEBA4 < gfx - (Gfx *)D_800BE9C8[D_800BE9C0]) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    return gfx;
}

extern struct102 *func_1510D970(s32, void *, s32, s32, s32);

/* Creates or frees each object's unk94 handle depending on whether the object is shown (in every active view). */
void func_15113C88(void) {
    s32 i;
    s32 j;
    s32 hidden;
    s32 show;
    Game13F9D0Obj *obj;

    for (i = 0; i < D_800DBEF0; i++) {
        hidden = FALSE;
        obj = i + OBJECTS; // matching: &OBJECTS[i] swaps the addu operands
        for (j = 0; j < D_80082FA0; j++) {
            if (func_150859AC(j, 0) != 0 && obj->viewAlpha[j] == 0) {
                hidden = TRUE;
                break;
            }
        }
        do { // matching: without this wrapper uopt stops keeping &D_800C3658 in a saved register
            if ((obj->state & 8) || obj->unk4C == 0 || (obj->flags & 0x10) == 0x10 || hidden ||
                (D_800C3658[0] == 0 && D_800C35EA == 1 && func_15022B08(i, 0) == 0) || !(obj->flags & 1)) {
                show = FALSE;
            } else {
                show = TRUE;
            }
        } while (0);
        if (show) {
            if (obj->unk94 == NULL) {
                obj->unk94 = func_1510D970(1, obj, 0, 1, 0);
            }
        } else if (obj->unk94 != NULL) {
            func_1516972C(obj->unk94);
            obj->unk94 = NULL;
        }
    }
}
extern s32 D_800DBF98; // grouped objects, kept at the end of OBJECTS

/* A group of objects that move together; the first member leads. */
typedef struct ObjGroup {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u16 count;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u16 members[8]; // high nibble: kind (2 = object), low 12 bits: index into OBJECTS
} ObjGroup; // size 0x18

#define GROUPS (*(ObjGroup **)&D_800D23C0) // D_80087380 entries

/* Runs the update callback of every flagged object: first the ungrouped ones, then each group's members,
   where followers take the leader's move delta and unk80. */
void func_15113E54(s32 arg0) {
    s16 moveX;
    s16 moveY;
    s16 moveZ;
    s32 leaderUnk80;
    s32 i;
    s32 j;
    s32 count;
    s32 member;
    Game13F9D0Obj *obj;

    moveX = 0;
    moveY = 0;
    moveZ = 0;
    leaderUnk80 = 0;
    count = D_800DBEF0 - D_800DBF98;
    for (i = 0; i < count; i++) {
        obj = i + OBJECTS; // matching: operand order of the addu
        if ((obj->viewFlags & 0x40) == 0x40 && obj->unk6E == 0) {
            obj->update(obj);
        }
    }

    for (i = 0; i < (s32)D_80087380; i++) {
        if ((GROUPS[i].members[0] >> 12) == 2) {
            for (j = 0; j < GROUPS[i].count; j++) {
                member = GROUPS[i].members[j];
                if ((member >> 12) == 2) {
                    obj = (member & 0xFFF) + OBJECTS; // matching: operand order of the addu
                    if ((obj->viewFlags & 0x40) == 0x40 && obj->unk6E == 0) {
                        if (j != 0) {
                            obj->moveX = moveX;
                            obj->moveY = moveY;
                            obj->moveZ = moveZ;
                            obj->unk80 = leaderUnk80;
                        }
                        obj->update(obj);
                    }
                    if (j == 0) {
                        moveX = obj->moveX;
                        moveY = obj->moveY;
                        moveZ = obj->moveZ;
                        leaderUnk80 = obj->unk80;
                    }
                }
            }
        }
    }
}
extern s32 *D_800DBF94; // per object: bit N set when actor D_800CC2D0[N] touches it

/* Whether actor `actor` (any actor if -1) touches obj; only objects with flags bit 0x80 track contacts. */
s32 func_15114050(Game13F9D0Obj *obj, s32 actor) {
    if (obj->flags & 0x80) {
        if (actor == -1) {
            return TRUE;
        }
        if (D_800DBF94[obj - OBJECTS] & (1 << actor)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* The lowest-numbered actor touching obj, or 0. */
s32 func_151140C4(Game13F9D0Obj *obj) {
    s32 i;

    i = 0;
    if (obj->flags & 0x80) {
        for (; i != 32; i++) {
            if (D_800DBF94[obj - OBJECTS] & (1 << i)) {
                return i;
            }
        }
    }
    return 0;
}

void func_1511473C(Game13F9D0Actor *actor, s32 objIndex);

/* Dispatches this frame's actor contacts to each object's onContact callback and carries riding actors. */
void func_15114188(void) {
    s32 i;
    s32 j;

    for (i = 0; i < D_800DBEF0; i++) {
        if (D_800DBF94[i] != 0) {
            for (j = 0; j < 25 && D_800DBF94[i] != 0; j++) {
                if (D_800DBF94[i] & (1 << j)) {
                    D_800DBF94[i] ^= 1 << j;
                    if (OBJECTS[i].onContact != NULL && (OBJECTS[i].unk92 != 0 || D_800CC2D0[j].unk127 != 0xFF)) {
                        OBJECTS[i].onContact(i + OBJECTS, &D_800CC2D0[j]); // matching: addu operand order
                    }
                    if ((OBJECTS[i].flags & 4) == 4 && !(OBJECTS[i].flags & 8)) {
                        func_1511473C(&ACTORS[j], i);
                    }
                }
            }
            OBJECTS[i].flags &= ~0x8C;
        }
    }
}

void func_150A7960(f32 m[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);

/*
 * Moves the point (*x, *y, *z) along with object objIndex: rotates it about the object by this frame's spin
 * (around the object's own axes) and adds the object's move delta.
 */
void func_15114348(s32 objIndex, f32 *x, f32 *y, f32 *z) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 m[4][4];
    f32 m2[4][4];
    f32 zaxis[3];
    f32 yaxis[3];
    f32 xaxis[3];

    xaxis[0] = 1.0f;
    xaxis[1] = 0.0f;
    xaxis[2] = 0.0f;
    yaxis[0] = 0.0f;
    yaxis[1] = 1.0f;
    yaxis[2] = 0.0f;
    zaxis[0] = 0.0f;
    zaxis[1] = 0.0f;
    zaxis[2] = 1.0f;
    dx = *x - OBJECTS[objIndex].x;
    dy = *y - OBJECTS[objIndex].y;
    dz = *z - OBJECTS[objIndex].z;
    if (OBJECTS[objIndex].rotX != 0.0f) {
        guRotateF(m, OBJECTS[objIndex].rotX, 1, 0, 0);
        func_150A7960(m, yaxis[0], yaxis[1], yaxis[2], &yaxis[0], &yaxis[1], &yaxis[2]);
        func_150A7960(m, zaxis[0], zaxis[1], zaxis[2], &zaxis[0], &zaxis[1], &zaxis[2]);
    }
    if (OBJECTS[objIndex].rotZ != 0.0f) {
        guRotateF(m, OBJECTS[objIndex].rotZ, zaxis[0], zaxis[1], zaxis[2]);
        func_150A7960(m, yaxis[0], yaxis[1], yaxis[2], &yaxis[0], &yaxis[1], &yaxis[2]);
        func_150A7960(m, xaxis[0], xaxis[1], xaxis[2], &xaxis[0], &xaxis[1], &xaxis[2]);
    }
    if (OBJECTS[objIndex].rotY != 0.0f) {
        guRotateF(m, OBJECTS[objIndex].rotY, yaxis[0], yaxis[1], yaxis[2]);
        func_150A7960(m, xaxis[0], xaxis[1], xaxis[2], &xaxis[0], &xaxis[1], &xaxis[2]);
        func_150A7960(m, zaxis[0], zaxis[1], zaxis[2], &zaxis[0], &zaxis[1], &zaxis[2]);
    }
    guRotateF(m, OBJECTS[objIndex].spinX, xaxis[0], xaxis[1], xaxis[2]);
    guRotateF(m2, OBJECTS[objIndex].spinZ, zaxis[0], zaxis[1], zaxis[2]);
    func_150A7A48(m, m2, m);
    guRotateF(m2, OBJECTS[objIndex].spinY, yaxis[0], yaxis[1], yaxis[2]);
    func_150A7A48(m, m2, m);
    func_150A7960(m, dx, dy, dz, &rx, &ry, &rz);
    rx = OBJECTS[objIndex].moveX + (rx - dx);
    ry = OBJECTS[objIndex].moveY + (ry - dy);
    rz = OBJECTS[objIndex].moveZ + (rz - dz);
    *x += rx;
    *y += ry;
    *z += rz;
}

extern f32 D_800A2F60;
extern u16 D_800CBDA0[];

/* Carries an actor standing on object objIndex: moves it with the object and turns it by the object's yaw spin. */
void func_1511473C(Game13F9D0Actor *actor, s32 objIndex) {
    if (actor == NULL || objIndex >= D_800DBEF0) {
        return;
    }
    func_15114348(objIndex, &actor->pos[0], &actor->pos[1], &actor->pos[2]);
    actor->movingAngle += OBJECTS[objIndex].spinY * D_800A2F60;
    if (actor->unk137 != 0 && objIndex + 1 == D_800CBDA0[actor - ACTORS]) {
        actor->facingAngle = actor->movingAngle;
    }
}
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

/* Builds an object's rotation matrix: yaw first, then pitch/roll (func_150A8050 makes Euler rotations). */
void func_151148A8(f32 mtx[4][4], Game13F9D0Obj *obj) {
    f32 pitchRoll[4][4];

    func_150A8050(mtx, 0.0f, obj->rotY, 0.0f);
    func_150A8050(pitchRoll, obj->rotX, 0.0f, obj->rotZ);
    func_150A7A48(pitchRoll, mtx, mtx);
}

/* Builds an object's model matrix: scale, then rotation, then translation to (x, y + yOffset, z). */
void func_1511490C(f32 mtx[4][4], Game13F9D0Obj *obj) {
    f32 scale[4][4];

    func_151148A8(mtx, obj);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y + obj->yOffset;
    mtx[3][2] = obj->z;
    func_150A7CB0(scale, obj->scaleX, obj->scaleY, obj->scaleZ);
    func_150A7A48(scale, mtx, mtx);
}
/* The object whose id is `id`, or NULL (id 0 never matches). */
Game13F9D0Obj *func_151149AC(u8 id) {
    s32 i;

    if (id == 0) {
        return NULL;
    }
    for (i = 0; i < D_800DBEF0; i++) {
        if (OBJECTS[i].id == id) {
            return i + OBJECTS;
        }
    }
    return NULL;
}

#define HANDLES (*(s32 **)&D_800DBEF8)      // per object: handle released with func_10004074
#define RELEASE_DELAY (*(u8 **)&D_800DBEFC) // per object: frames to keep the handle once hidden

/* Releases the handle of each object that is no longer drawn (after its delay, if it is unattached). */
void func_15114A1C(void) {
    s32 i;
    s32 handle;

    for (i = 0; i < D_800DBEF0; i++) {
        if ((OBJECTS[i].state & 1) != 1) {
            // matching: the dead copies (handle, j) and empty ifs steer uopt's register choices (see notes)
            handle = HANDLES[i];
            if (&D_800DBEF8) {}
            if (HANDLES[i] != 0) {
                if (OBJECTS[i].attach != 3 && !(OBJECTS[i].viewFlags & 0x80)) {
                    s32 j;

                    j = i;
                    if (j) {}
                    func_10004074(HANDLES[j]);
                    if (handle) {}
                    HANDLES[j] = 0;
                } else if (RELEASE_DELAY[i] != 0) {
                    RELEASE_DELAY[i]--;
                } else {
                    s32 j;

                    j = i;
                    if (j) {}
                    func_10004074(HANDLES[j]);
                    HANDLES[j] = 0;
                }
            }
        } else if (!(OBJECTS[i].viewFlags & 0x80)) {
            HANDLES[i] = 0;
        }
    }
}
#undef HANDLES
#undef RELEASE_DELAY
typedef struct Struct15114B94 {
    u8 pad0[0x4C];
    u8 enabled;
    u8 pad4D[2];
    u8 flags;
    u8 pad50[0x1E];
    u8 unk6E;
    u8 pad6F;
    u8 state;
    u8 pad71[0x1A];
    u8 players[9];
    struct102 *callback;
    u8 pad98[8];
} Struct15114B94;

void func_15114B94(u32 arg0) {
    u32 threshold;

    if (arg0 < D_800DBEF0) {
        if (((Struct15114B94 *)D_800DBEF4)[arg0].callback != NULL) {
            func_1516972C(((Struct15114B94 *)D_800DBEF4)[arg0].callback);
            ((Struct15114B94 *)D_800DBEF4)[arg0].callback = NULL;
        }
        ((Struct15114B94 *)D_800DBEF4)[arg0].state |= 8;
        ((Struct15114B94 *)D_800DBEF4)[arg0].unk6E = 1;
    }
    arg0 = D_800DBEF0 - 1;
    threshold = D_800DBEF0 - D_800DBF98;
    while (D_800DBEF0 != 0 && (((Struct15114B94 *)D_800DBEF4)[arg0].state & 8)) {
        if (arg0 >= threshold && D_800DBF98 != 0) {
            D_800DBF98--;
        } else {
            threshold--;
        }
        D_800DBEF0--;
        arg0--;
    }
}

s32 func_15114CC4(void *a0, s32 a1, s32 *a2, s32 a3) {
    s16 *v1 = *(s16 **)((u8 *)a0 + 0x18);

    if (v1 != 0 && (*a2 != 0 || *(s32 *)((u8 *)a0 + 0x1C) == 0)) {
        *(s32 *)((u8 *)a0 + 0x1C) = 1;
        *(s16 *)((u8 *)a0 + 0x2) = v1[8];
        *(s16 *)((u8 *)a0 + 0x4) = v1[9];
        *(s16 *)((u8 *)a0 + 0x6) = v1[10];
        return 0;
    }
    return 1;
}

typedef struct {
    char pad0[0x10];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    char pad16[0x5E];
    u16 unk74;
} Struct15114D24;

s32 func_10010E78(u16 arg0, s32 arg1, u16 arg2, s16 arg3, u8 arg4, s32 arg5,
                  s16 arg6, s16 arg7, s16 arg8, s16 arg9, s16 argA);

void func_15114D24(Struct15114D24 *arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s32 arg5) {
    u16 sfx;

    if (arg5 & 1) {
        if (arg0->unk74 != 0) {
            if (func_1001147C(arg0->unk74) == arg1) {
                return;
            }
        }
    }

    if (arg1 == -1) {
        if (arg0->unk74 != 0) {
            func_100111C8(arg0->unk74);
            arg0->unk74 = 0;
        }
        return;
    }

    if (arg5 & 8) {
        sfx = 0;
    } else {
        sfx = arg0->unk74;
    }

    if (arg5 & 4) {
        func_10010E78(sfx, arg1, arg2, 0, 0, -1, arg0->unk10, arg0->unk12,
                      arg0->unk14, arg3, arg4);
        if ((arg5 & 8) == 0) {
            arg0->unk74 = 0;
        }
    } else if (arg5 & 2) {
        if (sfx != 0) {
            func_1001123C(sfx);
            sfx = 0;
        }
        arg0->unk74 = func_10010E78(sfx, arg1, arg2, 0, 0, -1, arg0->unk10,
                                    arg0->unk12, arg0->unk14, arg3, arg4);
    } else {
        if (sfx != 0) {
            func_1001123C(sfx);
        }
        arg0->unk74 = func_1000FA64(arg1, arg0->unk10, arg0->unk12, arg0->unk14,
                                    arg2, arg4, arg3, (s32)func_15114CC4, arg0,
                                    0, 0, 0);
    }
}

void func_15114F04(s32 arg0, s32 arg1, s32 arg2) {
    func_1001001C(func_15114CC4, arg0, 0, arg1, arg2);
}

extern s32 D_800DBF9C;
extern s32 D_800DBFA0;
extern s32 D_800DBFA4;

void func_15114F44(s32 arg0, s32 arg1, s32 arg2) {
    s32 j;
    s32 i;
    u32 (*list)[3];

    if ((D_800BE9F0 == 0x12) || (D_800BE9F0 == 0x36) || (D_800BE9F0 == 0x13)) {
        for (j = 0; j < 2; j++) {
            list = (u32 (*)[3])D_800DBDD8[j];
            for (i = 0; i < D_800DBE18[j]; i++) {
                /* every command word must be a KSEG0 address, 4-byte aligned */
                if (((list[i][0] & 0xF0000000) != 0x80000000) || ((list[i][1] & 0xF0000000) != 0x80000000) ||
                    ((list[i][2] & 0xF0000000) != 0x80000000) ||
                    (list[i][0] & 3) || (list[i][1] & 3) || (list[i][2] & 3)) {
                    D_8003C8E0 = 0xC00005A;
                    func_150AD770();
                }
            }
        }
        D_800DBF9C = arg0;
        D_800DBFA0 = arg1;
        D_800DBFA4 = arg2;
    }
}
