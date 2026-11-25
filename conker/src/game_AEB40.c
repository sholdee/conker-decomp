#include <ultra64.h>

// variables.h declares several of the symbols below with types that do not fit how this file uses
// them (model tables are declared as plain s32, ...). Their header declarations are renamed out of
// the way and redeclared further down.
#include "functions.h"

#define D_800D1C90 D_800D1C90_hdr
#include "variables.h"
#undef D_800D1C90

// One entry of a model's definition table D_800D1C90 (header struct124).
typedef struct ModelDef {
    /* 0x00 */ u8 pad0[0x2];
    /* 0x02 */ s16 unk2;
    /* 0x04 */ u8 subModelCount;
    /* 0x05 */ u8 subModelList;     // index into D_80086CAC
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 pad8[0x6];
    /* 0x0E */ u16 unkE;
    /* 0x10 */ u8 pad10[0x2];
    /* 0x12 */ u8 unk12;
    /* 0x13 */ u8 pad13[0x16];
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u16 unk2A;
    /* 0x2C */ u8 pad2C[0x7];
    /* 0x33 */ s8 unk33;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 extraModelCount;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 pad3A;
    /* 0x3B */ u8 unk3B;
    /* 0x3C */ u8 unk3C;
} ModelDef;

// One 0x10-byte part of a part set (D_80086CC4): a child object or an attachment.
typedef struct PartDef {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 unk2;
    /* 0x03 */ u8 type;     // 0: child object (func_15083AC8), otherwise func_15030AF4
    /* 0x04 */ u8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 data[6];
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
} PartDef;

typedef struct PartSet {
    /* 0x0 */ PartDef *parts;
    /* 0x4 */ u8 count;
} PartSet;

// Result of a ray cast (func_15081690), consumed by func_15081E78.
typedef struct RayHit {
    /* 0x00 */ Actor *obj;
    /* 0x04 */ f32 dist;
    /* 0x08 */ Vec3f hit;
    /* 0x14 */ Vec3f centre;
    /* 0x20 */ Vec3f far;
    /* 0x2C */ Vec3f origin;
    /* 0x38 */ Vec3f dir;
    /* 0x44 */ u8 unk44[0x12];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 hitType;      // 0 none, 1 world, 2 object cylinder, 3 object model
    /* 0x5A */ u8 unk5A;
    /* 0x5C */ WorldObject *surface;
    /* 0x60 */ s32 unk60;
} RayHit;

typedef struct {
    /* 0x0 */ u32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} ModelReloc;

extern ModelDef *D_800D1C90[187];
extern f32 D_8009CC3C;  // matching: 20000.0f, but as a literal IDO loads it into a different register
extern u8 *D_800D210C;
extern u8 D_800D2100;
extern u8 D_800D2101;
extern s8 D_800D2130;
extern u8 *D_80086CAC[];
extern PartSet D_80086CC4[];
extern s32 (*D_80086C90[])(Actor *, Actor *, s32);
extern s8 D_8008FE44[];
extern u8 D_80099AC4[];
extern u8 D_800A7230[][8];
extern u8 D_800BE590;
extern u16 D_800BE598[];
extern ModelReloc *D_800C5338[];
extern u16 D_800C5628[];
extern u16 D_800C57A0[];

extern void *allocate_memory(s32 size, s32, s32, s32);
extern s32 func_10004074(void *);
extern void func_100043B4(s32 *arg0, u32 arg1);
extern s32 func_1502B020(s32, s32, s32, s32);
extern void func_150302F0(Actor *obj, s32 setId);
extern Actor *func_15030AF4(Actor *, s32, s32, s32, s32, s32, s32, s32, s32, u8 *, s32, s32, s32);
extern void func_150335C8(Actor *, Actor *, s32, s32, s32, s32);
extern void func_15036C70(Actor *obj);
extern s32 func_1503D660(s32 modelId, s32 arg1);
extern s32 func_1503D774(s32 modelId, s32 arg1);
extern s32 func_1503DA9C(Actor *, s32, s32, s32);
extern s32 func_1505D024(Actor *, s32, u16, s32);
extern Actor *func_1505ED34(void);
extern Actor *func_1505EEF4(void);
extern void func_1505F188(Actor *obj);
extern void func_15060A30(s32, Actor *);
extern void func_15060A9C(s32, Actor *);
extern void func_1506AC8C(Actor *, s32, s32);
extern void func_1507DE4C(Actor *);
extern void func_1507DF10(Actor *obj, s32 arg1);
extern s32 func_1508114C(Actor *obj, Vec3f *rayOrigin, Vec3f *rayDir, Vec3f *outNear, Vec3f *outFar, Vec3f *outCentre, f32 *outDist, s32 *outIndex, s32 *outBone, s32 *outPart, s16 *outOffset);
extern s32 func_15084D00(Actor *);
extern s32 func_150A6360(void *arg0, MtxF arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, f32 *, f32 *, f32 *, f32 *, s32 *, void *, f32);
extern void func_150B5C38(void *, u8, s32);
extern void func_150B6754(u8, s32);
extern void func_150D7120(void *, u8, s32);
extern void func_150D728C(void *, void *, void *, u8, s32);
extern void func_150EB484(f32 *, f32 *, f32 *);
extern void func_15104608(f32, f32, f32, s32);
extern void func_1510D720(s32 arg0);
extern void func_15137610(Actor *, void *, void *, void *, u8, s32);
extern void func_15138C80(Actor *, s32, s32);
extern f32 func_15143E64(Vec3f *);
extern s32 func_15145128(Vec3f *, Vec3f *, f32 *, f32 *);
extern s32 func_151452C4(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2, f32 arg3, Vec3f *arg4, Vec3f *arg5, f32 *arg6, f32 *arg7);
extern void func_15145974(void *, f32 *, f32 *);
extern s32 func_1514D15C(Actor *obj);
extern void *func_1515D440(void);
extern s32 func_1515D480(s32);
extern s32 func_151D3480(void *, void *, void *, u8, u8, s32);
extern s32 func_151E5F64(s32 arg0);

s32 func_15081C20(Vec3f *origin, Vec3f *dir, Actor *obj, Vec3f *outNear, Vec3f *outFar, f32 *outNearDist, f32 *outFarDist);
void func_15081E0C(Actor *obj, u16 part, u8 skip);
s32 func_150825C0(s32 spawnIdx, s32 arg1);
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot);
void func_150829D8(Actor *obj);
void func_150832AC(Actor *obj);
void func_15083384(Actor *obj, s32 arg1);
Actor *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3);
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2);
s32 func_150838EC(Actor *obj, s32 arg1, s32 arg2, f32 arg3);
void func_150839B8(Actor *obj, s32 modelId, SpawnRecord *spawn);
Actor *func_15083AC8(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8);
Actor *func_15083E90(u8 id);
void func_150843AC(Actor *obj, s32 model);
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2);
u8 func_150849A0(Actor *obj);
void func_15084C30(Actor *obj);
s32 func_15084CB0(s32 modelId);

/* Casts a ray from origin along dir against the world and every active object (except self and those in
   ignoreMask or rejected by filter), recording the nearest hit closer than maxDist in out. */
void func_15081690(Actor *self, f32 originX, f32 originY, f32 originZ, f32 dirX, f32 dirY, f32 dirZ, RayHit *out, f32 maxDist, s32 skipWorld, s32 skipObjs, s32 skipModels, s8 filter, s32 filterArg, s32 ignoreMask) {
    s32 surf;
    f32 hitX;
    f32 hitY;
    f32 hitZ;
    f32 dist;
    f32 farDist;
    s32 part;
    s32 bone;
    s16 offset[4];
    s32 i;
    Actor *obj;
    s32 best;
    s32 skip;
    s32 ret;
    u8 childSlot[25];
    Vec3f near;
    Vec3f far;
    Vec3f centre;
    Vec3f origin;
    Vec3f dir;
    s32 index;
    s32 surfIdx;

    origin.x = originX;
    origin.y = originY;
    origin.z = originZ;
    dir.x = dirX;
    dir.y = dirY;
    dir.z = dirZ;
    out->dist = -1.0f;
    out->obj = NULL;
    out->hitType = 0;
    out->origin = origin;
    best = -1;
    out->dir = dir;
    out->surface = NULL;
    out->unk60 = 0;
    if (maxDist == 0.0f) {
        maxDist = 9999999.0f;
    }
    if (self != NULL) {
        skip = self->unk124 - 1;
    } else {
        skip = -1;
    }
    // childSlot[owner] = 1-based slot of an object owned by owner
    bzero(childSlot, sizeof(childSlot));
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        if (D_800CC2D0[i].unk65 != 0) {
            childSlot[D_800CC2D0[i].unk65 - 1] = i + 1;
        }
    }
    if (skipWorld == 0 &&
        func_150AC9C0(originX, originY, originZ, dirX, dirY, dirZ, &surf, out->unk44, &hitX, &hitY, &hitZ, &dist, &surfIdx, &out->unk60, 0.0f) != 0 &&
        dist < maxDist) {
        out->dist = dist;
        out->hitType = 1;
        out->hit.x = hitX;
        out->hit.y = hitY;
        out->hit.z = hitZ;
        out->far = out->hit;
        if (surfIdx != -1) {
            out->surface = &D_800DBEF4[surfIdx];
        } else {
            out->surface = NULL;
        }
    }
    if (skipObjs == 0) {
        for (i = 0; i < 25; i++) {
            if ((1 << i) & ignoreMask) {
                continue;
            }
            if (skip == i) {
                continue;
            }
            obj = &D_800CC2D0[i];
            if (obj->interaction_state == 0) {
                continue;
            }
            if (obj->unk5 == 3) {
                continue;
            }
            if (obj == self) {
                continue;
            }
            if (!(obj->unkF8 & 0x40)) {
                continue;
            }
            if (filter != -1 && D_80086C90[filter](obj, self, filterArg) == 0) {
                continue;
            }
            if (skipModels == 0) {
                ret = func_1508114C(obj, &origin, &dir, &near, &far, &centre, &dist, &index, &bone, &part, offset);
                if (ret > 0 && dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                    out->dist = dist;
                    out->obj = obj;
                    best = i + 1;
                    out->unk58 = part;
                    out->unk56 = bone;
                    out->hit = near;
                    out->centre = centre;
                    out->unk5A = index;
                    out->far = far;
                    out->hitType = 3;
                }
                if (ret != -1) {
                    continue;
                }
            }
            if (func_15081C20(&origin, &dir, obj, &near, &far, &dist, &farDist) != 0 &&
                dist < maxDist && (out->dist == -1.0f || dist < out->dist) && best != childSlot[i]) {
                out->dist = dist;
                out->obj = obj;
                best = i + 1;
                out->hit = near;
                out->far = far;
                out->hitType = 2;
            }
        }
    }
}

/* Intersects a ray with obj's collision cylinder (radius unkD2, centre raised by unkD6, y squashed by
   unkDC). Returns 1 with the entry/exit points and their distances from origin on a hit. */
s32 func_15081C20(Vec3f *origin, Vec3f *dir, Actor *obj, Vec3f *outNear, Vec3f *outFar, f32 *outNearDist, f32 *outFarDist) {
    Vec3f pos;

    pos.x = obj->x_position;
    pos.y = obj->y_position + obj->unkD6;
    pos.z = obj->z_position;
    {
        Vec3f start;
        Vec3f unitDir;
        Vec3f center;
        f32 yScale;
        f32 yUnscale;

        yScale = obj->unkDC;
        yUnscale = obj->unkE0;
        start.x = origin->x;
        start.y = origin->y * yScale;
        start.z = origin->z;
        unitDir.x = dir->x;
        unitDir.y = dir->y * yScale;
        unitDir.z = dir->z;
        if (func_15145128(&unitDir, &unitDir, NULL, NULL) == 0) {
            return 0;
        }
        center.x = pos.x;
        center.y = pos.y * yScale;
        center.z = pos.z;
        if (func_151452C4(&start, &unitDir, &center, obj->unkD2, outNear, outFar, outNearDist, outFarDist) == 0) {
            return 0;
        }
        outNear->y *= yUnscale;
        outFar->y *= yUnscale;
        {
            Vec3f diff;

            diff.x = outNear->x - origin->x;
            diff.y = outNear->y - origin->y;
            diff.z = outNear->z - origin->z;
            *outNearDist = func_15143E64(&diff);
            diff.x = outFar->x - origin->x;
            diff.y = outFar->y - origin->y;
            diff.z = outFar->z - origin->z;
            *outFarDist = func_15143E64(&diff);
        }
    }
    return 1;
}

void func_15081E0C(Actor *obj, u16 part, u8 skip) {
    if (D_800BE616 == 0) {
        if (obj->health != 0) {
            obj->health = 7;
        }
        if ((skip == 0) || (part != 10)) {
            func_1507DF10(obj, part);
        }
    }
}

/* Applies the effects of a ray hit of the given kind fired by self: world impact effects for a world
   hit, otherwise hit reactions, sounds and effects on the object that was hit. Returns 0 for no hit,
   1 for a world hit and 2 for an object hit. */
s32 func_15081E78(Actor *self, RayHit *hit, s32 kind) {
    s32 sound;
    f32 yaw;
    f32 pitch;
    u8 playImpact;
    Actor *obj;
    ActorCtl *ext;
    // matching: volatile keeps IDO from hoisting the li/lw of noHealth out of the if/else
    volatile s32 noHealth;
    s32 longShot;
    s32 effect;
    s32 spawnEffect;
    s32 pad;

    playImpact = 1;
    ext = self->unk31C;
    if (ext != NULL) {
        ext->unk1AA++;
    }
    if (hit->hitType == 0) {
        return 0;
    }
    if (hit->hitType == 1) {
        switch (kind) {
        case 18:
            effect = 4;
            break;
        case 6:
            effect = 0;
            break;
        case 2:
            effect = 1;
            break;
        case 7:
            effect = 5;
            break;
        case 11:
            effect = 3;
            break;
        case 12:
            effect = 2;
            break;
        case 13:
            effect = -1;
            func_150D728C(hit->unk44, &hit->hit, &hit->dir, 0xFF, 1);
            break;
        case 5:
            if (D_800BE9F0 == 14) {
                f32 vc;
                f32 front[3];
                f32 back[3];

                front[0] = hit->hit.x + hit->dir.x * 5.0f;
                front[1] = hit->hit.y + hit->dir.y * 5.0f;
                front[2] = hit->hit.z + hit->dir.z * 5.0f;
                back[0] = hit->hit.x - hit->dir.x * 5.0f;
                back[1] = hit->hit.y - hit->dir.y * 5.0f;
                back[2] = hit->hit.z - hit->dir.z * 5.0f;
                func_150EB484(front, back, &vc);
            }
            // fallthrough
        case 9:
        case 10:
            effect = -1;
            func_15104608(hit->hit.x, hit->hit.y, hit->hit.z, self->unk124);
            func_1506AC8C(self, 2, 0);
            break;
        default:
            effect = 5;
            break;
        }
        if (effect != -1) {
            func_151D3480(&hit->hit, &hit->dir, hit->unk44, effect, 0xFF, 1);
        }
        return 1;
    }
    obj = hit->obj;
    if (kind == 5 || kind == 9 || kind == 10) {
        func_1506AC8C(self, 3, 0);
    }
    if (obj->health == 0) {
    // matching: an (unused) label here stops IDO hoisting the store out of the then-block
    noHealthLabel:
        noHealth = 1;
    } else {
        noHealth = 0;
    }
    if ((obj->immune != 0 || obj->stunned != 0) && noHealth == 0) {
        return 2;
    }
    longShot = 0;
    sound = D_80099AC4[kind];
    if (kind == 7) {
        if (D_800BE9F0 == 43) {
            sound = 0x27;
        }
    }
    func_15145974(&hit->dir, &yaw, &pitch);
    if (hit->hitType == 2) {
        if (obj->id == 0x94) {
            playImpact = 0;
            func_150B5C38(&hit->hit, 0xFF, 1);
            func_150B6754(0xFF, 1);
        }
    } else if (hit->hitType == 3) {
        if (D_800BE616 == 0) {
            if (obj->id == 0x9F || obj->id == 0xA0) {
                if (1000.0f < hit->dist) {
                    longShot = 1;
                }
            }
        }
        if (hit->unk58 == 10 && (obj->unk31C == NULL || obj->unk31C->unk1AD == 0) && longShot == 0) {
            // matching: unsigned compares keep the constants as immediates
            if ((u32)kind != 2 && (u32)kind != 18 && (u32)kind != 11 && (u32)kind != 12) {
                func_1507DE4C(obj);
                if (obj->unk31C != NULL) {
                    obj->unk31C->unk1AD = 1;
                }
                if (obj->id == 0x80 && hit->unk5A == 0 && hit->hit.y - 5.5f > hit->centre.y) {
                    func_15060A9C((func_150ADA20() & 7) + 0x2E3, obj);
                }
                func_15138C80(obj, 0xFF, 1);
                sound = 0x20;
                if (obj->id == 0x9F || obj->id == 0xA0) {
                    if (obj->unk2EC != 0) {
                        sound = 0x3D;
                    }
                }
                func_15060A30(0xAA, obj);
            } else {
                func_15060A30(0x107, obj);
            }
            if (kind != 11 && kind != 12 && obj->health != 0) {
                obj->health = 1;
            }
        } else if (obj->id == 0x9F || obj->id == 0xA0) {
            func_15081E0C(obj, hit->unk58, longShot);
        }
        switch (kind) {
        case 2:
            spawnEffect = 0x2A;
            break;
        case 18:
            spawnEffect = 0x22;
            break;
        case 12:
            spawnEffect = 0x6B;
            break;
        case 11:
            spawnEffect = 0x6D;
            break;
        default:
            spawnEffect = 0;
            break;
        }
        if (spawnEffect != 0 && obj->health < 2) {
            Actor source;

            source.x_position = hit->hit.x;
            source.y_position = hit->hit.y;
            source.z_position = hit->hit.z;
            source.unkB8 = pitch;
            source.unk40 = yaw + 180.0f;
            source.unkC4 = 0.0f;
            source.xz_scale = obj->xz_scale;
            source.y_scale = obj->y_scale;
            func_150335C8(&source, obj, spawnEffect, hit->unk56, 1, 0);
            if (hit->unk58 == 10) {
                sound = 0x20;
            }
        }
    }
    if (playImpact) {
        func_15137610(hit->obj, &hit->hit, &hit->far, &hit->dir, 0xFF, 1);
    }
    if (kind == 13) {
        func_150D7120(&hit->hit, 0xFF, 1);
    }
    if (noHealth == 0) {
        func_1505D024(obj, sound | 0x80000, (yaw - 90.0f) * 182.04444885f, self - D_800CC2D0);
    }
    return 2;
}

/* Spawns the object for spawn record spawnIdx once it is due: skips records that are in use,
   disabled (x == 0x7FFF) or still counting down their D_800D2110 delay, and records whose flags
   ask for no player nearby (radius by flags & 6) or for being off-screen (flags & 8). */
s32 func_150825C0(s32 spawnIdx, s32 arg1) {
    f32 radiusSq;
    s32 i;
    u16 range;
    u8 clear;
    u8 flags;
    s32 j;
    f32 dx;
    f32 dz;

    if (D_800D210C[spawnIdx] != 0) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].unk2 == 1) {
        return 0;
    }
    if (D_800D20FC[spawnIdx].x == 0x7FFF) {
        return 0;
    }
    if (D_800D2110[spawnIdx]) {
        if (D_800D2110[spawnIdx] <= D_800BE9A0) {
            D_800D2110[spawnIdx] = 0;
        } else {
            D_800D2110[spawnIdx] -= D_800BE9A0;
        }
        return 0;
    }
    flags = D_800D20FC[spawnIdx].flags;
    if (spawnIdx >= 5) {
        range = flags & 6;
        if (range != 0) {
            if (range == 2) {
                radiusSq = 422500.0f;
            } else if (range == 4) {
                radiusSq = 1690000.0f;
            } else {
                radiusSq = 7562500.0f;
            }
            clear = 1;
            for (j = 0; j < D_8008FD8C; j++) {
                if (D_800CC2D0[j].interaction_state == 0) {
                    continue;
                }
                if (D_800CC2D0[j].unk127 == 0xFF) {
                    continue;
                }
                if (D_800D2138.unk208 & (1 << D_800CC2D0[j].unk127)) {
                    // matching: squaring in place (d = d * d) reuses the operand's register
                    dx = D_800CC2D0[j].x_position - D_800D20FC[spawnIdx].x;
                    dx = dx * dx;
                    dz = D_800CC2D0[j].z_position - D_800D20FC[spawnIdx].z;
                    dz = dz * dz;
                    if (dx + dz <= radiusSq) {
                        clear = 0;
                        break;
                    }
                }
            }
            if (clear) {
                return 0;
            }
        }
    }
    if (flags & 8) {
        clear = 1;
        for (i = 0; i <= D_80082FA0; i++) {
            // matching: offset + base operand order
            if (func_150A6360(i + D_800BE628, D_800D9C10[i],
                              D_800D20FC[spawnIdx].x,
                              D_800D20FC[spawnIdx].y + 150,
                              D_800D20FC[spawnIdx].z,
                              150.0f, 150.0f, D_8009CC3C) != 0) {
                clear = 0;
            }
        }
        if (!clear) {
            return 0;
        }
    }
    return func_15082A44(&D_800D20FC[spawnIdx], spawnIdx, 0, 0, 0);
}

/* Runs func_150825C0 over the spawn records: [0, D_800D2101) when firstRange is set, else
   [D_800D2101, D_800D2100). */
void func_1508295C(s32 arg0, s32 arg1, s32 firstRange) {
    s32 start;
    s32 end;
    s32 i;

    if (firstRange != 0) {
        start = 0;
        end = D_800D2101;
    } else {
        start = D_800D2101;
        end = D_800D2100;
    }

    for (i = start; i < end; i++) {
        func_150825C0(i, arg1);
    }
}

void func_150829D8(Actor *obj) {
    obj->unkF8 &= 0xF7FFFFFD;
    obj->unkF8 |= 4;
    if ((obj->unk31C != NULL) && (obj->unk31C->unk56 == 0)) {
        switch (D_800BE9F0) {
        case 0:
        case 10:
        case 14:
        case 18:
        case 19:
        case 25:
        case 27:
        case 44:
        case 60:
        case 61:
        case 66:
        case 67:
        case 68:
            break;
        default:
            obj->unkF8 |= 2;
            break;
        }
    }
}

/* Creates and initialises a game object from spawn record spawnIdx (slot: 1-based object slot to
   reuse, 0 for the default). Returns the 1-based slot, or 0 on failure. */
s32 func_15082A44(SpawnRecord *spawn, s32 spawnIdx, s32 arg2, s32 arg3, s32 slot) {
    s32 type;
    Actor *obj;
    s32 idx;
    s32 ret;
    s32 t;
    s32 i;
    s32 found;
    u32 model;
    VtxMorphState *mem;
    u16 angle;

    if (D_800D210C[spawnIdx] & ~0x80) {
        return 0;
    }
    type = spawn->type;
    if (slot != 0) {
        obj = &D_800CC2D0[slot - 1];
        func_1505F188(obj);
    } else if ((spawnIdx != -1) && (spawnIdx < D_8008FD8C)) {
        obj = &D_800CC2D0[spawnIdx];
        func_1505F188(obj);
    } else {
        obj = func_1505ED34();
    }
    idx = obj - D_800CC2D0;
    if (idx == 25) {
        return 0;
    }
    if (spawnIdx != -1) {
        if (spawn->flags & 0x21) {
            D_800D210C[spawnIdx] = idx + 1;
        } else {
            D_800D210C[spawnIdx] = idx + 0x81;
        }
        obj->unk13F = spawnIdx;
        if (spawnIdx < D_8008FD8C) {
            obj->unk127 = spawnIdx;
        } else {
            obj->unk127 = 0xFF;
        }
    }
    obj->spawn = spawn;
    ret = idx + 1;
    func_15084488(spawn, obj->unk13F, arg3);
    if ((idx < D_8008FD8C) || (type == 0x98) || (type == 0x28)) {
        obj->unk31C = allocate_memory(sizeof(ActorCtl), 1, 0, 0);
        bzero(obj->unk31C, sizeof(ActorCtl));
        obj->unk31C->unk1B8 = 5.0f;
    }
    obj->unique_id = spawn->unique_id;
    obj->unk1D0 = 2;
    obj->xz_scale = spawn->xz_scale;
    obj->y_scale = spawn->y_scale;
    obj->unk154 = spawn->xz_scale;
    obj->unk158 = spawn->y_scale;
    obj->interaction_state = spawn->interaction_state;
    obj->x_position = spawn->x;
    obj->y_position = spawn->y;
    obj->z_position = spawn->z;
    obj->unk212[0] = 0xFF;
    obj->unk20F = 0xFF;
    obj->unk210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->team = 0xFF;
    obj->old_x_position = obj->x_position;
    obj->old_y_position = obj->y_position;
    obj->unk1CC = obj->y_position;
    obj->old_z_position = obj->z_position;
    if ((spawn->flags & 6) || (spawn->flags & 4) || (spawn->flags & 2)) {
        func_1506160C(obj, 4, 0, 0, 0);
    }
    obj->unk232 = spawn->unkD;
    if ((spawnIdx < D_8008FD8C) && (D_8008FE44[spawnIdx] == -2)) {
        obj->unk31C->unk84 = 1;
        obj->unk232 = 1;
    }
    {
        // matching: unused stack slot
        u8 pad;

        if (obj->unk127 != 0xFF) {
            obj->team = func_151E5F64(obj->unk127);
            obj->unk31C->grenade_count = D_800D2130;
            obj->unk31C->unk1AF = 0x40;
            obj->unk31C->lastSpawnIndex = -1;
        }
    }
    obj->health = spawn->unkE & 0xF;
    obj->unk1E5 = spawn->unkF;
    obj->unk1E6 = obj->unk1E5;
    obj->unk221 = 1;
    obj->unk23C = spawn->unk1C;
    obj->unk23D = spawn->unk1D;
    obj->unk76 = spawn->angle << 8;
    angle = obj->unk76;
    obj->unk7A = angle;
    obj->unk78 = angle;
    obj->unk40 = (angle + 0x4000) * (360.0f / 65536.0f);
    obj->unk140 = arg2;
    obj->gravity = 6.0f;
    obj->unkF8 = spawn->objFlags;
    if (obj->unkF8 & 0x200000) {
        obj->unk224 = obj->y_position;
    }
    if (obj->unkF8 & 0x10000000) {
        obj->unk38 = obj->y_position - 500.0f;
    }
    if (obj->unkF8 & 0x800000) {
        D_800C67F0 = 1;
    }
    obj->unk1E4 = spawn->unk11;
    obj->unk66 = spawn->unk10;
    obj->unk1DD[0] = 120;
    obj->unk1DD[1] = 120;
    obj->unk1DD[2] = 120;
    obj->unk1E0[0] = 255;
    obj->unk1E0[1] = 255;
    obj->unk1E0[2] = 255;
    obj->unk23E = 4;
    obj->unk246 = 1;
    obj->unk247 = 100;
    obj->unk248 = 100;
    obj->unk1FC |= 2;
    func_150839B8(obj, type, spawn);
    t = spawn->unk13 & 0x7F;
    obj->unk92 = ((spawn->unk13 & 0x80) << 8) | t;
    obj->unk90 = spawn->unk14;
    func_150837D4(idx, type, 1);
    if (type != 0xFF) {
        func_150838EC(obj, 0, 0, 1.0f);
    } else {
        obj->unk5 = 3;
    }
    if (obj->unkF8 & 0x4000) {
        func_1503DA9C(obj, obj->id, 0, 0);
    }
    if (spawn->partSet) {
        func_15083568(obj, spawn->partSet, 1.0f, 0);
    }
    if (obj->unk127 != 0xFF) {
        obj->health = func_150859AC(obj->unk127, 4);
        obj->unkB2 = D_800D2132;
        func_150829D8(obj);
        func_15083384(obj, func_150859AC(idx, 1));
    }
    if (((obj->unk66 & 0xC) == 8) || ((obj->unk66 & 0xC) == 0xC)) {
        func_150832AC(obj);
    } else {
        obj->unk314 = NULL;
    }
    obj->unk288 = func_15084CB0(type);
    if (D_800BE616 != 0) {
        t = func_151E5F64(obj->unk127);
        if (t >= 4) {
            t = 3;
        }
        if (type == 0x3B) {
            obj->unk68[0] = t + 1;
        } else if (type == 0x77) {
            obj->unk68[0] = t + 0x10;
        }
    }
    if ((D_80038080 == 0) && (D_800BE9F0 != 29) && (spawn->keepTime == 0xFF)) {
        found = obj->modelCount;
        if (found == 0) {
            found = 1;
        }
        for (i = 0; i < found; i++) {
            D_800D2040[obj->modelIds[i]] = 0xFF;
        }
    }
    if (D_800BE9F0 == 29) {
        obj->unk66 &= ~0x20;
    }
    func_1514D15C(obj);
    model = func_150849A0(obj);
    found = 0;
    for (i = 0; i < 2; i++) {
        if (model == D_800A7230[i][0]) {
            found = 1;
            break;
        }
    }
    if (found) {
        mem = allocate_memory(sizeof(VtxMorphState), 1, 0, 2);
        obj->morph = mem;
        if (mem != NULL) {
            mem->prev = 0;
            mem->cur = 0xFF;
            mem->blend = 0.0f;
            mem->rate = 1.0f;
        }
    }
    func_15084C30(obj);
    return ret;
}

void func_150832AC(Actor *obj) {
    SpawnRecord *spawn;
    u8 n;
    s32 i;
    s32 count;

    spawn = obj->spawn;
    if (spawn != NULL) {
        n = spawn->unk2F;
        if (n == 0) {
            if (obj->unk127 == 0) {
                count = 5;
            } else {
                count = 3;
            }
        } else {
            count = n;
        }
        for (i = 0; i <= D_80082FA0; i++) {
            obj->unk304[i] = func_1515D480(count);
        }
        if (obj->unk304[i - 1] != 0) {
            obj->unk301 = count;
        }
        obj->unk314 = func_1515D440();
    }
}

void func_15083384(Actor *obj, s32 arg1) {
    s32 partSet;
    s32 group;
    ActorCtl *ext;

    partSet = -1;
    switch (arg1) {
    case 37:
    case 38:
        ext = obj->unk31C;
        if (ext != NULL) {
            ext->unk1B2 = 1;
        }
        break;
    case 9:
        obj->unk94 |= 2;
        break;
    case 13:
        partSet = 0x76;
        break;
    case 14:
        partSet = 0x78;
        break;
    case 15:
        partSet = 0x77;
        obj->unk94 |= 2;
        break;
    case 29:
    case 30:
    case 31:
    case 32:
        {
            s32 n;

            n = arg1 - 29;
            obj->unk94 = 0x1E;
            if (n < 2) {
                obj->unk94 &= ~0x14;
            } else {
                obj->unk94 &= ~8;
            }
            if (n >> 1) {
                obj->unk94 &= ~2;
            }
        }
        break;
    case 17: case 18: case 19: case 20: case 21: case 22:
    case 23: case 24: case 25: case 26: case 27: case 28:
        group = (arg1 - 17) >> 2;
        obj->unk94 = 0x3E;
        switch (group) {
        case 0:
            obj->unk94 &= ~4;
            break;
        case 1:
            obj->unk94 &= ~8;
            break;
        case 2:
            obj->unk94 &= ~0x10;
            break;
        }
        switch (arg1 - group * 4 - 17) {
        case 1:
            obj->unk94 &= ~2;
            break;
        case 2:
            obj->unk94 &= ~0x20;
            break;
        case 3:
            obj->unk94 &= ~0x22;
            break;
        }
        break;
    }
    if (partSet != -1) {
        func_15083568(obj, partSet, 0.0f, 0);
    }
}

/* Creates every part of part set setId (1-based) on parent; returns the last part created. */
Actor *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3) {
    PartDef *parts;
    Actor *ret;
    s32 i;

    setId--;
    ret = NULL;
    parts = D_80086CC4[setId].parts;
    for (i = 0; i < D_80086CC4[setId].count; i++) {
        if (parts[i].type == 0) {
            ret = func_15083AC8(parent, parts[i].unk1, parts[i].unk0, 0, parts[i].unk2, parts[i].unk4, parts[i].unk5, parts[i].unk6, arg2);
        } else {
            ret = func_15030AF4(parent, parts[i].unk0, parts[i].unk1, parts[i].unk7, parts[i].unk4, parts[i].unk5, arg3, setId + 1, parts[i].unk2,
                                parts[i].data, (parts[i].type == 2) ? parts[i].unk6 : -1, parts[i].unkE, parts[i].unkF);
        }
    }
    return ret;
}

/* Removes the child objects of part set setId (1-based) that belong to parent. */
void func_150836CC(Actor *parent, s32 setId) {
    PartSet *set;
    s32 i;
    PartDef *parts;

    func_150302F0(parent, setId);
    setId--;
    set = &D_80086CC4[setId];
    parts = set->parts;
    for (i = 0; i < set->count; i++) {
        Actor *child;

        if (parts[i].type == 0) {
            child = func_1505F0AC(parts[i].unk0);
            if (child != NULL) {
                if (child->unk65 == ((parent - D_800CC2D0) + 1)) {
                    func_15060F28(child, 0);
                }
            }
        }
    }
}

/* Sets object objIdx's model to modelId (0xFF for none) and loads its per-model state. */
void func_150837D4(s32 objIdx, s32 modelId, s32 arg2) {
    Actor *obj = &D_800CC2D0[objIdx];
    ModelDef *def;

    obj->id = modelId;
    if (modelId == 0xFF) {
        obj->unkE4 = 0;
        obj->unkE6 = 0;
        obj->unkE8 = 0;
        obj->unkD2 = 70;
        obj->unkD4 = 70;
        obj->unkD6 = 0;
        obj->unk160 = 0;
        obj->unkC8 = 0.0f;
    } else {
        obj->unkC8 = D_800D1C90[modelId]->unk6;
        func_15062BDC(obj, obj->xz_scale, obj->y_scale);
        def = D_800D1C90[modelId];
        obj->unk5 = def->unk12;
    }
    obj->unk6 = func_15084D00(obj);
    if (D_800C5A90[obj->unk6] != 0) {
        obj->unk58 = func_1502B020(0, 2, 2, obj->unk6);
    }
}

/* Allocates the animation state block of obj's model. Returns 1 only if the allocation fails. */
s32 func_150838EC(Actor *obj, s32 arg1, s32 arg2, f32 arg3) {
    s32 modelId;

    modelId = obj->id;
    if (D_800D1588[modelId] == 0) {
        return 0;
    }
    if (D_800C5A90[modelId] == 0) {
        return 0;
    }
    obj->unk2D0 = allocate_memory(0x3E0, 1, 2, 0);
    if (obj->unk2D0 == NULL) {
        return 1;
    }
    bzero(obj->unk2D0, 0x40);
    func_1505E650(obj, arg1, arg3, 0.0f, 0.0f, 0.0f, arg2);
    return 0;
}

/* Copies model modelId's defaults from its ModelDef into obj; spawn (may be NULL) overrides unk10. */
void func_150839B8(Actor *obj, s32 modelId, SpawnRecord *spawn) {
    ModelDef *def;
    u16 override;

    if (modelId != 0xFF) {
        def = D_800D1C90[modelId];
        if (spawn != NULL) {
            override = spawn->unk2C;
            if (override == 0) {
                obj->unk10 = def->unk2A;
            } else {
                if (override == 1) {
                    obj->unk10 = 0;
                } else {
                    obj->unk10 = override;
                }
            }
            if (D_800BE9F0 == 29) {
                obj->unk10 = 1000;
            }
        } else {
            obj->unk10 = 0;
        }
        obj->unk13B = def->unk39;
        obj->unk2CB = def->unk33;
        obj->unk2CC = def->unk34;
        obj->unk5 = def->unk12;
        obj->unk68[0] = def->unk3B;
        obj->unk68[1] = def->unk3C;
        obj->unk160 = def->unk2;
        if (def->subModelCount != 0) {
            obj->modelIds = D_80086CAC[def->subModelList];
            obj->lodCount = def->subModelCount;
        }
        obj->modelCount = obj->lodCount + def->extraModelCount;
        if (def->unk29 != 0) {
            func_15036C70(obj);
        }
    }
}

/* Creates a child object of parent with model modelId (a part from func_15083568). */
Actor *func_15083AC8(Actor *parent, s32 arg1, s32 modelId, s32 arg3, s32 state, s32 arg5, s32 inheritScale, s32 arg7, f32 arg8) {
    Actor *obj;
    // matching: unused stack slot
    s32 pad;
    s32 animFlag;
    s32 blocked;
    s32 i;

    func_1503D774(modelId, 0);
    func_1503D660(modelId, 0);
    obj = func_1505ED34();
    obj->unique_id = 0xFF - (obj - D_800CC2D0);
    obj->unkAB = arg3;
    obj->interaction_state = state;
    func_150615DC(obj);
    obj->unk232 = 1;
    obj->health = 1;
    obj->immune = 0xFF;
    obj->unk65 = (parent - D_800CC2D0) + 1;
    obj->unk5C = arg1;
    obj->unk123 = 1;
    obj->id = modelId;
    obj->unk66 = parent->unk66;
    obj->unk66 &= ~0x10;
    // matching: a no-op |= 0 (a constant flag field upstream) keeps the reload of unk66
    obj->unk66 |= 0;
    obj->unk1DD[0] = parent->unk1DD[0];
    obj->unk1DD[1] = parent->unk1DD[1];
    obj->unk1DD[2] = parent->unk1DD[1];
    obj->unk1E0[0] = parent->unk1E0[0];
    obj->unk1E0[1] = parent->unk1E0[1];
    obj->unk1E0[2] = parent->unk1E0[2];
    if (inheritScale == 0) {
        obj->xz_scale = 1.0f;
        obj->y_scale = 1.0f;
        obj->unk154 = 1.0f;
        obj->unk158 = 1.0f;
    } else {
        obj->xz_scale = parent->xz_scale;
        obj->y_scale = parent->y_scale;
        obj->unk154 = parent->unk154;
        obj->unk158 = parent->unk158;
    }
    obj->unk101 = inheritScale;
    obj->unkAC = arg5;
    func_1503D774(modelId, 0);
    if (arg5 & 1) {
        obj->unkF8 |= 0x4000;
        obj->unk270 = D_800D1C90[modelId]->unkE * obj->xz_scale;
    }
    animFlag = 0;
    if (arg5 & 2) {
        animFlag = 1;
    }
    for (i = 0; i <= D_80082FA0; i++) {
        obj->unk304[i] = func_1515D480(1);
    }
    if (obj->unk304[i - 1] != 0) {
        obj->unk301 = 1;
    }
    obj->unk314 = func_1515D440();
    if (D_800C35EA[0] == 1) {
        blocked = 1;
    } else {
        blocked = 0;
    }
    if (blocked) {
        D_800C3638 = 0;
    }
    func_150839B8(obj, modelId, NULL);
    func_150837D4(obj - D_800CC2D0, modelId, 1);
    func_150838EC(obj, arg7, animFlag, arg8);
    if (blocked) {
        D_800C3638 = 1;
    }
    return obj;
}

/* Returns the lowest unique id (1..255) that no spawn record uses. */
s32 func_15083DA0(void) {
    s32 id;
    s32 i;

    id = 1;
    for (i = 0; i < D_800D2100; i++) {
        if (id == D_800D20FC[i].unique_id) {
            id++;
            i = 0;
        }
    }
    if (id >= 0x100) {
        id = 0xFF;
    }
    return id;
}

/* Returns the index of the spawn record with unique id id, or -1. */
s32 func_15083E0C(u8 id) {
    s32 i;

    if (id == 0) {
        return -1;
    }
    for (i = 0; i < D_800D2100; i++) {
        if (D_800D20FC[i].unique_id == id) {
            return &D_800D20FC[i] - D_800D20FC;
        }
    }
    return -1;
}

/* Returns the active object with unique id id, or NULL. */
Actor *func_15083E90(u8 id) {
    Actor *obj;
    s32 i;

    if (id == 0) {
        return NULL;
    }
    // matching: a walking pointer; indexing D_800CC2D0[i] makes IDO unroll the loop differently
    obj = D_800CC2D0;
    for (i = 0; i < 26; i++) {
        if (obj->interaction_state != 0 && obj->unique_id == id) {
            return obj;
        }
        obj++;
    }
    return NULL;
}

/* Returns the slot of the active object with unique id id, or -1. */
s32 func_15083FB0(u8 id) {
    Actor *obj;

    obj = func_15083E90(id);
    if (obj != NULL) {
        return obj - D_800CC2D0;
    }
    return -1;
}

s32 func_15084000(void) {
    Actor *obj = func_1505EEF4();

    if (obj != NULL) {
        return obj - D_800CC2D0;
    }
    return -1;
}

/* Sets up obj's two data pointers for its model'th model: shared with D_800D19A0[id] or private
   copies, depending on the owning spawn record's mode (unk10 & 3). Returns 1 if an allocation failed. */
s32 func_15084044(Actor *obj, s32 model) {
    u8 *slot;
    SpawnRecord *spawn;
    s32 mode;
    u8 id;
    s32 val;
    s32 ret;
    s32 i;
    s32 size;

    // matching: every access to modelData[model] goes through this byte pointer (slot + 0x28C/0x290) or
    // an s32 row view, as s32 lvalues; typed field access reallocates registers (see the func_15084044 ledger)
    slot = (u8 *)((s32)obj + model * 8);
    if (obj->unk65 != 0) {
        spawn = D_800CC2D0[obj->unk65 - 1].spawn;
    } else {
        spawn = obj->spawn;
    }
    id = obj->modelIds[model];
    mode = (spawn != NULL) ? (spawn->unk10 & 3) : 0;
    if (id == 0) {
        mode = 2;
    }
    if (*(s32 *)(slot + 0x28C) != 0 && *(s32 *)(slot + 0x290) != 0) {
        return 0;
    }
    val = (s32)D_800D19A0[id];
    if ((s32)D_800D19A0[id] == 0) {
        return 0;
    }
    ret = 0;
    if (mode == 0) {
        val = (s32)D_800D19A0[id];
        *(s32 *)(slot + 0x290) = val;
        *(s32 *)(slot + 0x28C) = val;
        D_800D1F80[id] += 2;
    } else if (mode == 1) {
        if (*(s32 *)(slot + 0x28C) == 0 || *(s32 *)(slot + 0x28C) == (s32)D_800D19A0[id]) {
            if (D_800D1F80[id] != 0) {
                size = D_800C57A0[id] * 16;
                *(s32 *)(slot + 0x28C) = (s32)allocate_memory(size, 1, 2, 2);
                if (*(s32 *)(slot + 0x28C) == 0) {
                    ret = 1;
                } else if (*(s32 *)(slot + 0x28C) != 0) {
                    bcopy((void *)(s32)D_800D19A0[id], (void *)*(s32 *)(slot + 0x28C), size);
                } else {
                    *(s32 *)(slot + 0x28C) = (s32)D_800D19A0[id];
                }
            } else {
                val = (s32)D_800D19A0[id];
                *(s32 *)(slot + 0x290) = val;
                *(s32 *)(slot + 0x28C) = val;
                D_800D1F80[id] += 2;
            }
        }
        *(s32 *)(slot + 0x290) = *(s32 *)(slot + 0x28C);
    } else if (mode == 2) {
        for (i = 0; i < 2; i++) {
            if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0 || ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == (s32)D_800D19A0[id] || (i == 1 && *(s32 *)(slot + 0x28C) == *(s32 *)(slot + 0x290))) {
                if (D_800D1F80[id] != 0) {
                    size = D_800C57A0[id] * 16;
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)allocate_memory(size, 1, 2, 2);
                    if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] == 0) {
                        ret = 1;
                        if (i == 1) {
                            if (*(s32 *)(slot + 0x28C) != (s32)D_800D19A0[id]) {
                                func_10004074((void *)*(s32 *)(slot + 0x28C));
                            } else {
                                D_800D1F80[id] -= 1;
                            }
                            *(s32 *)(slot + 0x28C) = 0;
                        }
                        break;
                    } else if (((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] != 0) {
                        bcopy((void *)(s32)D_800D19A0[id], (void *)((s32 (*)[2])((u8 *)obj + 0x28C))[model][i], size);
                    } else {
                        ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    }
                } else {
                    ((s32 (*)[2])((u8 *)obj + 0x28C))[model][i] = (s32)D_800D19A0[id];
                    D_800D1F80[id] += 1;
                }
            }
        }
    }
    return ret;
}

/* Releases the data pointers of all of obj's models. */
void func_1508434C(Actor *obj) {
    s32 count;
    s32 i;

    count = obj->modelCount;
    if (count == 0) {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        func_150843AC(obj, i);
    }
}

/* Releases the data pointers of obj's model'th model: frees private copies, drops the
   reference count of shared ones. */
void func_150843AC(Actor *obj, s32 model) {
    // matching: a named trip count takes its saved register before the D_800D19A0 base
    u8 limit;
    u8 *data;
    s32 i;
    u8 id;

    if (obj->modelData[model][0] == NULL && obj->modelData[model][1] == NULL) {
        return;
    }
    limit = 2;
    id = obj->modelIds[model];
    for (i = 0; i != limit; i++) {
        data = obj->modelData[model][i];
        if (data != NULL) {
            if (data != D_800D19A0[id]) {
                func_100043B4((s32 *)data, 3);
            } else {
                D_800D1F80[id] -= 1;
            }
            obj->modelData[model][i] = NULL;
        }
    }
}

/* Loads model spawn->type and all of its sub-models. */
void func_15084488(SpawnRecord *spawn, s32 arg1, s32 arg2) {
    s32 count;
    s32 i;
    ModelDef *def;
    u8 *list;
    s32 modelId;

    modelId = spawn->type;
    if (modelId != 0xFF) {
        func_1503D774(modelId, arg2);
        def = D_800D1C90[modelId];
        list = &spawn->type;
        count = def->subModelCount;
        if (count == 0) {
            count = 1;
        } else {
            list = D_80086CAC[def->subModelList];
        }
        count += def->extraModelCount;
        for (i = 0; i < count; i++) {
            func_1503D774(list[i], arg2);
            func_1503D660(list[i], arg2);
        }
    }
}

/* Releases obj's model data, and frees each of its models (and their tables) that no other
   active object still uses. */
void func_15084558(Actor *self) {
    s32 id;
    u8 *list;
    s32 i;
    s32 j;
    s32 count;
    s32 k;
    s32 unique;
    s32 freeData;
    s32 freeDef;
    Actor *obj;
    u8 otherCount;
    u8 *otherList;
    // matching: unused stack slot
    s32 pad;
    s32 data;
    u8 *block;
    s32 table;

    id = self->id;
    if (id != 0xFF && D_800D19A0[id] != NULL) {
        count = self->modelCount;
        list = self->modelIds;
        func_1508434C(self);
        for (i = 0; i < count; i++) {
            unique = 1;
            freeData = 1;
            freeDef = 1;
            // matching: IDO emits this empty duplicate scan; it must stay
            for (j = 0; j < i; j++) {
                if (list[j] == list[i]) {
                }
            }
            for (j = 0; j < 25; j++) {
                if (j != (self - D_800CC2D0)) {
                    obj = &D_800CC2D0[j];
                    if (obj->interaction_state != 0 && obj->id != 0xFF && obj->unk5 != 2) {
                        otherCount = obj->modelCount;
                        otherList = obj->modelIds;
                        for (k = 0; k < otherCount; k++) {
                            if (otherList[k] == list[i]) {
                                unique = 0;
                            }
                            if (D_800D1588[otherList[k]] == D_800D1588[list[i]] && D_800D1588[otherList[k]] != 0) {
                                freeData = 0;
                            }
                            if (D_800D1C90[otherList[k]] == D_800D1C90[list[i]] && D_800D1C90[otherList[k]] != NULL) {
                                freeDef = 0;
                            }
                        }
                    }
                }
            }
            if (freeData && D_800D1588[list[i]] != 0) {
                data = D_800D1588[list[i]];
                {
                    func_100043B4((s32 *)(data - 0x10), 3);
                    for (k = 0; k < 187; k++) {
                        if (data == D_800D1588[k]) {
                            D_800D1588[k] = 0;
                        }
                    }
                }
            }
            if (freeDef) {
                block = (u8 *)D_800D1C90[list[i]];
                if (block != NULL) {
                    block -= 0x10;  // the allocation starts 0x10 bytes before the data
                    func_100043B4((s32 *)block, 3);
                    D_800D1C90[list[i]] = NULL;
                }
            }
            if (unique) {
                if (D_800D19A0[list[i]] != NULL) {
                    if (D_800C5C08[id] != 0) {
                        func_10004074((void *)D_800C5C08[id]);
                        D_800C5C08[id] = 0;
                        func_10004074((void *)D_800C6070[id]);
                        D_800C6070[id] = 0;
                    }
                    table = D_800C6360[id];
                    if (table != 0) {
                        func_10004074((void *)table);
                        D_800C6360[id] = 0;
                    }
                }
            }
        }
    }
}

/* Returns obj's current model id (modelIds[lodRequest - 1], or the first). */
u8 func_150849A0(Actor *obj) {
    s32 current = obj->lodRequest;

    if (current != 0) {
        return obj->modelIds[current - 1];
    }
    return obj->modelIds[0];
}

/* Like func_150849A0, falling back to modelIds[lodCount - 1]; stores the index in *outIndex. */
u8 func_150849CC(Actor *obj, s32 *outIndex) {
    s32 index;
    s32 n;

    n = obj->lodRequest;
    if (n != 0) {
        index = n - 1;
    } else {
        n = obj->lodCount;
        if (n != 0) {
            index = n - 1;
        } else {
            index = 0;
        }
    }
    if (outIndex != NULL) {
        *outIndex = index;
    }
    return obj->modelIds[index];
}

/* Ticks the D_800D2040 countdowns; a model whose countdown expires while it has no D_800C5C08
   table is unloaded and every object's references to it are dropped. */
void func_15084A18(void) {
    s32 i;
    s32 m;
    Actor *obj;
    s32 j;
    s32 k;
    ModelReloc *relocs;
    u8 *timer;

    timer = D_800D2040;
    for (i = 0; i < 187; i++, timer++) {
        if (*timer == 0xFF || *timer == 0) {
            continue;
        }
        if (*timer == 1) {
            if (D_800D19A0[i] != NULL && D_800C5C08[i] == 0) {
                for (m = 0; m < 25; m++) {
                    obj = &D_800CC2D0[m];
                    if (obj->interaction_state != 0) {
                        for (j = 0; j < obj->modelCount; j++) {
                            for (k = 0; k < 2; k++) {
                                if (obj->modelData[j][k] == D_800D19A0[i]) {
                                    obj->modelData[j][k] = NULL;
                                }
                            }
                            if (i == obj->modelIds[j]) {
                                func_150843AC(obj, j);
                            }
                        }
                    }
                }
                relocs = D_800C5338[i];
                for (k = 0; k < D_800C5628[i]; k++) {
                    if (relocs[k].unk0 >= 0x10000000U) {
                        func_1510D720(relocs[k].unk4);
                    }
                }
                func_10004074(D_800D19A0[i] - 0x38);
                D_800D1F80[i] = 0;
                D_800D19A0[i] = NULL;
            }
        }
        (*timer)--;
    }
}

void func_15084C30(Actor *obj) {
    struct197 *anim;
    f32 phase;

    if (obj->id == 0x94) {
        anim = obj->unk2D0;
        phase = func_150ADA20() % (s32)anim->unk18;
        anim->unk8 = phase;
    }
}

/* Returns the index of modelId in D_800BE598, or 0. */
s32 func_15084CB0(s32 modelId) {
    s32 ret;
    s32 i;

    ret = 0;
    for (i = 0; i < D_800BE590; i++) {
        if (modelId == D_800BE598[i]) {
            ret = i;
            break;
        }
    }
    return ret;
}
