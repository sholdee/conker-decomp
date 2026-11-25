#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60(Vtx **bufs, s32 index, s32 size, Vtx **out, u8 *isNew);
void func_1513173C(struct210 *);
void func_1513175C(struct210 *);
void func_151617C4(struct210 *arg0);
void func_151617E4(struct210 *arg0);

typedef struct Beam Beam;

/* Collision record filled in by func_150AC9C0 (0x12 bytes, halfword aligned). */
typedef struct {
    s16 unk0[9];
} HitInfo;

/* Aim state of the beam's owner (owner->aim). */
typedef struct {
    u8 pad0[0xB4];
    f32 targetDist;
    struct17 targetPos;
    u8 padC4[0x24];
    struct17 targetDir;
    HitInfo targetHit;
    u8 pad106[3];
    u8 targetType; // 0 = no target
    u8 pad10A[0x8D];
    u8 unk197;
} BeamOwnerAim;

/* The actor the beam is attached to. */
typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x38];
    u8 unk74;
    u8 pad75[0x15F];
    f32 (*boneMtx)[4][4];
    u8 pad1D8[0x144];
    BeamOwnerAim *aim;
} BeamOwner;

/* How a child object refers back to its beam. */
typedef struct {
    Beam *beam;
    u8 flags; // 1 = visible
} BeamLink;

/* beam->data.startFx: placed at the beam's start, sized by the beam's width. */
typedef struct {
    u8 pad0[0x38];
    f32 sizeX;
    f32 sizeY;
    struct17 pos;
    u8 pad4C[0x5C];
    f32 baseSizeX;
    f32 baseSizeY;
    u8 padB0[0x20];
    BeamLink link;
} BeamStartFx;

typedef struct {
    u8 flags; // 1 = visible
    u8 pad1;
    HitInfo hit;
    Beam *beam;
} BeamHitFxData;

/* beam->data.hitFx: placed just short of the hit point on type 1 hits. */
typedef struct {
    u8 pad0[0x34];
    struct17 pos;
    u8 pad40[0xD0];
    BeamHitFxData data;
} BeamHitFx;

typedef struct {
    u8 pad0[0x9];
    u8 disabled;
    u8 padA[4];
    s16 pos[3];
} BeamEndPointData;

/* beam->data.endPoint: tracks the beam's end in s16 coordinates. */
typedef struct {
    u8 pad0[0x14];
    BeamEndPointData *data;
    Beam *beam;
} BeamEndPoint;

/* beam->data.line: spans the beam from start to end. */
typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x114];
    BeamLink link;
} BeamLine;

/* beam->data.endFx: placed at the beam's end on type 2 and 3 hits. */
typedef struct {
    u8 pad0[0x40];
    struct17 pos;
    u8 pad4C[0x5C];
    BeamLink link;
} BeamEndFx;

typedef struct {
    BeamOwner *owner;
    u8 unk4; // must match owner->unk3B
    u8 pad5[3];
    struct17 localStart;  // in bone space
    struct17 localTarget; // in bone space
    u8 bone;
    u8 pad21;
    u8 unk22;
    u8 flags; // 1 = drawn, 2 = aimed by the owner
    BeamStartFx *startFx;
    BeamHitFx *hitFx;
    BeamEndPoint *endPoint;
    BeamLine *line;
    BeamEndFx *endFx;
    f32 width;
    u8 pad3C[8];
    f32 length;
    f32 scrollBase;  // texture scroll speed = scrollBase + scrollRange * sin(scrollPhase)
    f32 scrollRange;
    f32 scrollPhase;
    u8 pad54[4];
    f32 texOffset;
    u8 pad5C[0x34];
    s16 unk90;
} BeamData;

struct Beam {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x74];
    Vtx vtxTemplate[4];
    Vtx *vtxBuf[4];
    BeamData data;
};

/* Releases the beam's child objects, then destroys it. */
void func_15101090(Beam *beam) {
    BeamData *data = &beam->data;

    // matching: tested as u16 but passed as s16, so IDO reuses the lhu without an andi
    if (*(u16 *)&data->unk90) {
        func_100111C8(data->unk90);
    }
    if (data->hitFx != NULL) {
        func_1516972C((struct102 *)data->hitFx);
    }
    if (data->endPoint != NULL) {
        func_1516972C((struct102 *)data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516972C((struct102 *)data->startFx);
    }
    if (data->line != NULL) {
        func_1516972C((struct102 *)data->line);
    }
    if (data->endFx != NULL) {
        func_1516972C((struct102 *)data->endFx);
    }
    func_1513CA6C((struct210 *)beam);
}

/* Counterpart of func_15101090 for the beam's child objects and func_1513CAA0. */
void func_15101148(Beam *beam) {
    BeamData *data = &beam->data;

    if (data->hitFx != NULL) {
        func_1516979C((struct102 *)data->hitFx);
    }
    if (data->endPoint != NULL) {
        func_1516979C((struct102 *)data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516979C((struct102 *)data->startFx);
    }
    if (data->line != NULL) {
        func_1516979C((struct102 *)data->line);
    }
    if (data->endFx != NULL) {
        func_1516979C((struct102 *)data->endFx);
    }
    func_1513CAA0((struct210 *)beam);
}

/* The functions below detach a child object from its beam before handing it on. */

void func_151011E8(BeamHitFx *fx) {
    BeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CA6C((struct210 *)fx);
}

void func_15101210(BeamHitFx *fx) {
    BeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CAA0((struct210 *)fx);
}

void func_15101238(BeamEndPoint *endPoint) {
    BeamData *data = &endPoint->beam->data;

    data->endPoint = NULL;
    func_151617C4((struct210 *)endPoint);
}

void func_15101260(BeamEndPoint *endPoint) {
    BeamData *data = &endPoint->beam->data;

    data->endPoint = NULL;
    func_151617E4((struct210 *)endPoint);
}

void func_15101288(BeamStartFx *fx) {
    BeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513173C((struct210 *)fx);
}

void func_151012B0(BeamStartFx *fx) {
    BeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513175C((struct210 *)fx);
}

void func_151012D8(BeamLine *line) {
    BeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411A4((struct210 *)line);
}

void func_15101300(BeamLine *line) {
    BeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411C4((struct210 *)line);
}

void func_15101328(BeamEndFx *fx) {
    BeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513173C((struct210 *)fx);
}

void func_15101350(BeamEndFx *fx) {
    BeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513175C((struct210 *)fx);
}

extern void func_15143134(struct17 *src, struct17 *dst, f32 (*mtx)[4]);
extern f32 func_15143E64(struct17 *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, s32 *, HitInfo *, f32 *, f32 *, f32 *, s32, s32, s32, f32);

/*
 * Places the beam: start at the owner's bone, end at the owner's aim target or at the first
 * collision along the beam, then moves or hides the child objects to match.
 * Returns 0 when the owner is gone.
 */
s8 func_15101378(Beam *beam) {
    s8 result;
    BeamOwnerAim *aim;
    BeamOwner *owner;
    u8 hit;
    HitInfo hitInfo;
    struct17 dir;
    u8 endIsHit;
    f32 (*boneMtx)[4];
    struct17 target;
    s32 unkOut;
    s32 hitType;
    s32 pad; // matching: stack slot needed for the frame size
    BeamData *data;
    f32 (*bones)[4][4];

    result = 1;
    owner = beam->data.owner;
    data = &beam->data;
    if (owner->unk0 == 0 || owner->unk3B != data->unk4) {
        result = 0;
    } else {
        endIsHit = 1;
        bones = owner->boneMtx;
        if (bones != NULL && (D_800C35EA != 1 || (owner->unk74 & 0xF) != 0xF)) {
            boneMtx = bones[data->bone];
            func_15143134(&data->localStart, &beam->start, boneMtx);
            if (data->flags & 2) {
                aim = owner->aim;
                hitType = aim->targetType;
                if (hitType != 0) {
                    beam->end = owner->aim->targetPos;
                    data->length = owner->aim->targetDist;
                    hitInfo = owner->aim->targetHit;
                    dir.unk0 = beam->end.unk0 - beam->start.unk0;
                    dir.unk4 = beam->end.unk4 - beam->start.unk4;
                    dir.unk8 = beam->end.unk8 - beam->start.unk8;
                } else {
                    endIsHit = 0;
                    hitType = 2;
                    data->length = 2000.0f;
                    dir.unk0 = owner->aim->targetDir.unk0 * 2000.0f;
                    dir.unk4 = owner->aim->targetDir.unk4 * 2000.0f;
                    dir.unk8 = owner->aim->targetDir.unk8 * 2000.0f;
                    beam->end.unk0 = beam->start.unk0 + dir.unk0;
                    beam->end.unk4 = beam->start.unk4 + dir.unk4;
                    beam->end.unk8 = beam->start.unk8 + dir.unk8;
                }
            } else {
                func_15143134(&data->localTarget, &target, boneMtx);
                target.unk0 = target.unk0 - beam->start.unk0;
                target.unk4 = target.unk4 - beam->start.unk4;
                target.unk8 = target.unk8 - beam->start.unk8;
                if (func_150AC9C0(beam->start.unk0, beam->start.unk4, beam->start.unk8, target.unk0, target.unk4,
                                  target.unk8, &unkOut, &hitInfo, &beam->end.unk0, &beam->end.unk4, &beam->end.unk8, 0,
                                  0, 0, 0.0f) != hit * 0) { // matching: reading hit here makes IDO keep it in v1
                    dir.unk0 = beam->end.unk0 - beam->start.unk0;
                    dir.unk4 = beam->end.unk4 - beam->start.unk4;
                    dir.unk8 = beam->end.unk8 - beam->start.unk8;
                    hit = 1;
                    data->length = func_15143E64(&dir);
                } else {
                    hit = 0;
                }
                hitType = hit;
            lbl1:; // matching: unused labels split the basic blocks the way IDO needs
            }
        } else {
            hitType = 0;
        }
        if (hitType != 0) {
            BeamHitFx *hitFx;
            BeamHitFxData *hitFxData;
            BeamLink *endFxLink;
            BeamEndFx *endFx;
            BeamEndPoint *endPoint;
            BeamStartFx *startFx;
            BeamLine *line;

            data->flags |= 1;
            if (data->hitFx != NULL) {
                hitFx = data->hitFx;
                hitFxData = &hitFx->data;
                if (hitType == 1 && endIsHit != 0) {
                    if (data->length > 40.0f) {
                        f32 invLength = 1.0f / data->length;

                        hitFx->pos.unk0 = beam->end.unk0 - dir.unk0 * invLength * 20.0f;
                        hitFx->pos.unk4 = beam->end.unk4 - dir.unk4 * invLength * 20.0f;
                        hitFx->pos.unk8 = beam->end.unk8 - dir.unk8 * invLength * 20.0f;
                    } else {
                        hitFx->pos = beam->end;
                    }
                    hitFxData->hit = hitInfo;
                    hitFxData->flags |= 1;
                } else {
                    hitFxData->flags &= ~1;
                }
            }
            if (data->endFx != NULL) {
                endFx = data->endFx;
                endFxLink = &endFx->link;
                if ((hitType == 2 || hitType == 3) && endIsHit != 0) {
                    endFx->pos = beam->end;
                    endFxLink->flags |= 1;
                } else {
                    endFxLink->flags &= ~1;
                }
            }
            if (data->endPoint != NULL) {
                endPoint = data->endPoint;
                do {} while (0); // matching: adds the uopt temp that puts the t0 spill at 0x5C
                endPoint->data->pos[0] = beam->end.unk0;
                endPoint->data->pos[1] = beam->end.unk4;
                endPoint->data->pos[2] = beam->end.unk8;
                if (endIsHit != 0) {
                    endPoint->data->disabled = 0;
                } else {
                    endPoint->data->disabled = 1;
                }
            }
            if (data->startFx != NULL) {
                startFx = data->startFx;
                startFx->pos = beam->start;
                startFx->sizeX = startFx->baseSizeX * data->width;
                startFx->sizeY = startFx->baseSizeY * data->width;
                startFx->link.flags |= 1;
            }
            if (data->line != NULL) {
                line = data->line;
                line->start = beam->start;
                line->end = beam->end;
                line->unk58 &= ~4;
                line->link.flags |= 1;
            }
        } else {
            BeamHitFx *hitFx;
            BeamHitFxData *hitFxData;
            BeamLink *endFxLink;
            BeamEndFx *endFx;
            BeamEndPoint *endPoint;
            BeamStartFx *startFx;
            BeamLine *line;

        lbl2:;
            data->flags &= ~1;
            if (data->hitFx != NULL) {
                hitFx = data->hitFx;
                hitFx->data.flags &= ~1;
            }
            if (data->endPoint != NULL) {
                endPoint = data->endPoint;
                endPoint->data->disabled = 1;
            }
            if (data->startFx != NULL) {
                startFx = data->startFx;
                startFx->link.flags &= ~1;
            }
            if (data->line != NULL) {
                line = data->line;
                line->link.flags &= ~1;
            }
            if (data->endFx != NULL) {
                endFx = data->endFx;
                endFx->link.flags &= ~1;
            }
        }
    }
    return result;
}



extern f32 *func_15144B34(s32);
extern void func_15145548(struct17 *, struct17 *, f32 *, struct17 *, s32);

/*
 * Builds this frame's quad for the beam as seen from viewport `viewport`: four vertices from start to
 * end, offset sideways by the beam's width (facing the eye point), with a scrolling texture.
 * Returns the vertices, or NULL when the beam is not drawn.
 */
Vtx *func_151019C4(Beam *beam, s16 viewport) {
    BeamData *data = &beam->data;
    Vtx *vtx;
    Vtx *result;
    f32 *eye;
    struct17 dir;
    struct17 closest;
    f32 dx, dy, dz;
    f32 sideZ, sideY, sideX;
    f32 sideLenSq;
    f32 scale;
    f32 scrollSpeed;
    struct17 start;
    struct17 end;
    f32 drawLen;
    u8 isNew;
    s32 texStart, texEnd, texSpan;
    f32 maxLen;

    if (viewport == data->unk22 && (data->owner == NULL || data->owner->aim->unk197 != 0)) {
        return NULL;
    }
    if (data->flags & 1) {
        func_151D5D60(beam->vtxBuf, viewport, sizeof(beam->vtxTemplate), &vtx, &isNew);
        result = vtx;
        if (vtx != NULL) {
            if (isNew) {
                memcpy(beam->vtxBuf[viewport], beam->vtxTemplate, sizeof(beam->vtxTemplate));
                memcpy(beam->vtxBuf[viewport] + 4, beam->vtxTemplate, sizeof(beam->vtxTemplate));
            }
        } else {
            return NULL;
        }
        eye = func_15144B34(viewport);
        maxLen = 3508.0f;
        start = beam->start;
        scale = data->length;
        if (maxLen < scale) {
            f32 ratio = maxLen / scale;

            dir.unk0 = (beam->end.unk0 - start.unk0) * ratio;
            dir.unk4 = (beam->end.unk4 - start.unk4) * ratio;
            dir.unk8 = (beam->end.unk8 - start.unk8) * ratio;
            end.unk0 = start.unk0 + dir.unk0;
            end.unk4 = start.unk4 + dir.unk4;
            end.unk8 = start.unk8 + dir.unk8;
            drawLen = maxLen;
        } else {
            end = beam->end;
            dir.unk0 = end.unk0 - start.unk0;
            dir.unk4 = end.unk4 - start.unk4;
            dir.unk8 = end.unk8 - start.unk8;
            drawLen = data->length;
        }
        func_15145548(&start, &dir, eye, &closest, 0);
        dx = closest.unk0 - eye[0];
        dy = closest.unk4 - eye[1];
        dz = closest.unk8 - eye[2];
        sideX = dir.unk4 * dz - dy * dir.unk8;
        sideY = dir.unk8 * dx - dz * dir.unk0;
        sideZ = dir.unk0 * dy - dx * dir.unk4;
        sideLenSq = sideX * sideX + sideY * sideY + sideZ * sideZ;
        if (sideLenSq == 0.0f) {
            sideZ = sideY = sideX = 0.0f;
        } else {
            scale = data->width / sqrtf(sideLenSq);
            sideX *= scale;
            sideY *= scale;
            sideZ *= scale;
        }
        scrollSpeed = sinf(data->scrollPhase);
        scrollSpeed = data->scrollBase + data->scrollRange * scrollSpeed;
        texStart = data->texOffset;
        texEnd = data->texOffset + drawLen * scrollSpeed;
        texSpan = texEnd - texStart;
        {
            s32 wrapped;

            // keep the texture coordinates within s16 range
            if (texEnd >= 0x8000 || texStart < -0x8000) {
                wrapped = texStart;
                while (wrapped > 0x2000) {
                    wrapped -= 0x2000;
                }
                while (wrapped < 0) {
                    wrapped += 0x2000;
                }
                texStart = wrapped - 0x8000;
                texEnd = texStart + texSpan;
            }
        }
        vtx->v.ob[0] = end.unk0 + sideX;
        vtx->v.ob[1] = end.unk4 + sideY;
        vtx->v.ob[2] = end.unk8 + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = end.unk0 - sideX;
        vtx->v.ob[1] = end.unk4 - sideY;
        vtx->v.ob[2] = end.unk8 - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.unk0 - sideX;
        vtx->v.ob[1] = start.unk4 - sideY;
        vtx->v.ob[2] = start.unk8 - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.unk0 + sideX;
        vtx->v.ob[1] = start.unk4 + sideY;
        vtx->v.ob[2] = start.unk8 + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        return result;
    }
end: // matching: an unused label here keeps IDO's block layout
    return NULL;
}
