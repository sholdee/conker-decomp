#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Releases the beam's child objects, then destroys it. */
void func_15101090(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    // matching: tested as u16 but passed as s16, so IDO reuses the lhu without an andi
    if (*(u16 *)&data->unk90) {
        func_100111C8(data->unk90);
    }
    if (data->hitFx != NULL) {
        func_1516972C(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        func_1516972C(data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516972C(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        func_1516972C(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        func_1516972C(&data->endFx->head.header);
    }
    func_1513CA6C(&beam->head);
}

/* Counterpart of func_15101090 for the beam's child objects and func_1513CAA0. */
void func_15101148(RayBeam *beam) {
    RayBeamData *data = &beam->data;

    if (data->hitFx != NULL) {
        func_1516979C(&data->hitFx->head.header);
    }
    if (data->endPoint != NULL) {
        func_1516979C(data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516979C(&data->startFx->head.header);
    }
    if (data->line != NULL) {
        func_1516979C(&data->line->head.header);
    }
    if (data->endFx != NULL) {
        func_1516979C(&data->endFx->head.header);
    }
    func_1513CAA0(&beam->head);
}

/* The functions below detach a child object from its beam before handing it on. */

void func_151011E8(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CA6C(&fx->head);
}

void func_15101210(BeamHitFx *fx) {
    RayBeamData *data = &fx->data.beam->data;

    data->hitFx = NULL;
    func_1513CAA0(&fx->head);
}

void func_15101238(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    func_151617C4(endPoint);
}

void func_15101260(LightEffect *endPoint) {
    RayBeamData *data = &((RayBeam *)endPoint->data.object)->data;

    data->endPoint = NULL;
    func_151617E4(endPoint);
}

void func_15101288(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513173C(&fx->head);
}

void func_151012B0(BeamStartFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->startFx = NULL;
    func_1513175C(&fx->head);
}

void func_151012D8(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411A4(&line->head);
}

void func_15101300(BeamLine *line) {
    RayBeamData *data = &line->link.beam->data;

    data->line = NULL;
    func_151411C4(&line->head);
}

void func_15101328(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513173C(&fx->head);
}

void func_15101350(BeamEndFx *fx) {
    RayBeamData *data = &fx->link.beam->data;

    data->endFx = NULL;
    func_1513175C(&fx->head);
}

/*
 * Places the beam: start at the owner's bone, end at the owner's aim target or at the first
 * collision along the beam, then moves or hides the child objects to match.
 * Returns 0 when the owner is gone.
 */
s8 func_15101378(RayBeam *beam) {
    s8 result;
    BeamOwnerAim *aim;
    BeamOwner *owner;
    u8 hit;
    CollisionTri hitInfo;
    Vec3f dir;
    u8 endIsHit; // matching: always tested as endIsHit != 0; a bare test changes the frame size
    f32 (*boneMtx)[4];
    Vec3f target;
    s32 unkOut;
    s32 hitType;
    s32 pad; // matching: stack slot needed for the frame size
    RayBeamData *data;
    MtxF *bones;

    result = 1;
    owner = beam->data.owner;
    data = &beam->data;
    if (owner->unk0 == 0 || owner->unk3B != data->unk4) {
        result = 0;
    } else {
        endIsHit = 1;
        bones = owner->boneMtx;
        if (bones != NULL && (D_800C35EA[0] != 1 || (owner->unk74 & 0xF) != 0xF)) {
            boneMtx = bones[data->bone];
            func_15143134(&data->localStart, &beam->head.def.pos, boneMtx);
            if (data->flags & 2) {
                aim = owner->aim;
                hitType = aim->targetType;
                if (hitType != 0) {
                    beam->head.def.rot = owner->aim->targetPos;
                    data->length = owner->aim->targetDist;
                    hitInfo = owner->aim->targetHit;
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
                } else {
                    endIsHit = 0;
                    hitType = 2;
                    data->length = 2000.0f;
                    dir.x = owner->aim->targetDir.x * 2000.0f;
                    dir.y = owner->aim->targetDir.y * 2000.0f;
                    dir.z = owner->aim->targetDir.z * 2000.0f;
                    beam->head.def.rot.x = beam->head.def.pos.x + dir.x;
                    beam->head.def.rot.y = beam->head.def.pos.y + dir.y;
                    beam->head.def.rot.z = beam->head.def.pos.z + dir.z;
                }
            } else {
                func_15143134(&data->localTarget, &target, boneMtx);
                target.x = target.x - beam->head.def.pos.x;
                target.y = target.y - beam->head.def.pos.y;
                target.z = target.z - beam->head.def.pos.z;
                if (func_150AC9C0(beam->head.def.pos.x, beam->head.def.pos.y, beam->head.def.pos.z, target.x, target.y,
                                  target.z, &unkOut, &hitInfo, &beam->head.def.rot.x, &beam->head.def.rot.y, &beam->head.def.rot.z, 0,
                                  0, 0, 0.0f) != hit * 0) { // matching: reading hit here makes IDO keep it in v1
                    dir.x = beam->head.def.rot.x - beam->head.def.pos.x;
                    dir.y = beam->head.def.rot.y - beam->head.def.pos.y;
                    dir.z = beam->head.def.rot.z - beam->head.def.pos.z;
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
            RayBeamLink *endFxLink;
            BeamEndFx *endFx;
            LightEffect *endPoint;
            BeamStartFx *startFx;
            BeamLine *line;

            data->flags |= 1;
            if (data->hitFx != NULL) {
                hitFx = data->hitFx;
                hitFxData = &hitFx->data;
                if (hitType == 1 && endIsHit != 0) {
                    if (data->length > 40.0f) {
                        f32 invLength = 1.0f / data->length;

                        hitFx->head.def.pos.x = beam->head.def.rot.x - dir.x * invLength * 20.0f;
                        hitFx->head.def.pos.y = beam->head.def.rot.y - dir.y * invLength * 20.0f;
                        hitFx->head.def.pos.z = beam->head.def.rot.z - dir.z * invLength * 20.0f;
                    } else {
                        hitFx->head.def.pos = beam->head.def.rot;
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
                    endFx->head.desc.pos = beam->head.def.rot;
                    endFxLink->flags |= 1;
                } else {
                    endFxLink->flags &= ~1;
                }
            }
            if (data->endPoint != NULL) {
                endPoint = data->endPoint;
                do {} while (0); // matching: adds the uopt temp that puts the t0 spill at 0x5C
                endPoint->light->x = beam->head.def.rot.x;
                endPoint->light->y = beam->head.def.rot.y;
                endPoint->light->z = beam->head.def.rot.z;
                if (endIsHit != 0) {
                    endPoint->light->disabled = 0;
                } else {
                    endPoint->light->disabled = 1;
                }
            }
            if (data->startFx != NULL) {
                startFx = data->startFx;
                startFx->head.desc.pos = beam->head.def.pos;
                startFx->head.desc.width = startFx->baseSizeX * data->width;
                startFx->head.desc.height = startFx->baseSizeY * data->width;
                startFx->link.flags |= 1;
            }
            if (data->line != NULL) {
                line = data->line;
                line->head.def.pos = beam->head.def.pos;
                line->head.def.rot = beam->head.def.rot;
                line->head.def.flags &= ~4;
                line->link.flags |= 1;
            }
        } else {
            BeamHitFx *hitFx;
            BeamHitFxData *hitFxData;
            RayBeamLink *endFxLink;
            BeamEndFx *endFx;
            LightEffect *endPoint;
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
                endPoint->light->disabled = 1;
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

/*
 * Builds this frame's quad for the beam as seen from viewport `viewport`: four vertices from start to
 * end, offset sideways by the beam's width (facing the eye point), with a scrolling texture.
 * Returns the vertices, or NULL when the beam is not drawn.
 */
Vtx *func_151019C4(RayBeam *beam, s16 viewport) {
    RayBeamData *data = &beam->data;
    Vtx *vtx;
    Vtx *result;
    Vec3f *eye;
    Vec3f dir;
    Vec3f closest;
    f32 dx, dy, dz;
    f32 sideZ, sideY, sideX;
    f32 sideLenSq;
    f32 scale;
    f32 scrollSpeed;
    Vec3f start;
    Vec3f end;
    f32 drawLen;
    u8 isNew;
    s32 texStart, texEnd, texSpan;
    f32 maxLen;

    if (viewport == data->unk22 && (data->owner == NULL || data->owner->aim->unk197 != 0)) {
        return NULL;
    }
    if (data->flags & 1) {
        func_151D5D60(beam->head.vtxBuffers, viewport, sizeof(beam->head.vtxTemplate), &vtx, &isNew);
        result = vtx;
        if (vtx != NULL) {
            if (isNew) {
                memcpy(beam->head.vtxBuffers[viewport], beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
                memcpy(beam->head.vtxBuffers[viewport] + 4, beam->head.vtxTemplate, sizeof(beam->head.vtxTemplate));
            }
        } else {
            return NULL;
        }
        eye = func_15144B34(viewport);
        maxLen = 3508.0f;
        start = beam->head.def.pos;
        scale = data->length;
        if (maxLen < scale) {
            f32 ratio = maxLen / scale;

            dir.x = (beam->head.def.rot.x - start.x) * ratio;
            dir.y = (beam->head.def.rot.y - start.y) * ratio;
            dir.z = (beam->head.def.rot.z - start.z) * ratio;
            end.x = start.x + dir.x;
            end.y = start.y + dir.y;
            end.z = start.z + dir.z;
            drawLen = maxLen;
        } else {
            end = beam->head.def.rot;
            dir.x = end.x - start.x;
            dir.y = end.y - start.y;
            dir.z = end.z - start.z;
            drawLen = data->length;
        }
        func_15145548(&start, &dir, eye, &closest, 0);
        dx = closest.x - eye->x;
        dy = closest.y - eye->y;
        dz = closest.z - eye->z;
        sideX = dir.y * dz - dy * dir.z;
        sideY = dir.z * dx - dz * dir.x;
        sideZ = dir.x * dy - dx * dir.y;
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
        vtx->v.ob[0] = end.x + sideX;
        vtx->v.ob[1] = end.y + sideY;
        vtx->v.ob[2] = end.z + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = end.x - sideX;
        vtx->v.ob[1] = end.y - sideY;
        vtx->v.ob[2] = end.z - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texEnd;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.x - sideX;
        vtx->v.ob[1] = start.y - sideY;
        vtx->v.ob[2] = start.z - sideZ;
        vtx->v.tc[1] = 0x7C0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        vtx++;
        vtx->v.ob[0] = start.x + sideX;
        vtx->v.ob[1] = start.y + sideY;
        vtx->v.ob[2] = start.z + sideZ;
        vtx->v.tc[1] = 0;
        vtx->v.tc[0] = texStart;
        vtx->v.flag = 0;
        return result;
    }
end: // matching: an unused label here keeps IDO's block layout
    return NULL;
}
