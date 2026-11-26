#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800DBEC0; // turret's squared distance to the player

/* Turret settings in the data (no code reads this name; a turret object's moveData can point here). */
Turret15112520Cfg D_80089200 = {
    0.1f, 0.05f, 15.0f, 90.0f, -90.0f, 30.0f, 40.0f, -20.0f, 20.0f, 145.0f, -10.0f, 1000.0f, 27, 0,
};

/* Turns a turret toward the player (within its yaw/pitch limits and range) and plays its motor sound. */
void func_15112520(WorldObject *turret) {
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
    static s16 D_80089234 = 0; // sound timer: 0 idle, -1 start the sound, >0 cooldown frames

    cfg = turret->moveData.data;
    dx = movement_obj[0].xpos - turret->x;
    dy = (movement_obj[0].ypos + 65.0f) - turret->y;
    dz = movement_obj[0].zpos - turret->z;
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
    delta = func_15048A70(turret->u, yaw);
    yaw = fabsf(delta);
    if (cfg->maxStep < yaw) {
        delta = ((delta < 0.0f) ? -1.0f : 1.0f) * cfg->maxStep;
    }
    turret->u += delta * speed;
    if (D_80089234 == 0 && yaw > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->u < 0.0f) {
        turret->u += 360.0f;
    } else if (turret->u >= 360.0f) {
        turret->u -= 360.0f;
    }
    delta = func_15048A70(turret->t, 360.0f - pitch);
    turret->t += delta * speed;
    if (D_80089234 == 0 && fabsf(delta) > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->t < 0.0f) {
        turret->t += 360.0f;
    } else if (turret->t >= 360.0f) {
        turret->t -= 360.0f;
    }
    if (D_80089234 == -1 && cfg->sfxCooldown != 0) {
        volume = 32000.0f - D_800DBEC0 * 0.1000000015f;
        D_80089234 = cfg->sfxCooldown;
        if (volume > 0) {
            func_10010F30(cfg->sfx, 32000, 64, 0, 0);
        }
    } else if (D_80089234 > 0) {
        D_80089234--;
    }
}


/* Builds each view's draw list: distance-culls and fades every object group, then appends visible groups. */
void func_15112A80(s32 player) {
    s32 drawDist;
    s32 alpha;
    WorldObject *obj;
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
        RenderView *recp;

        count = &D_800DBEE8[view];
        recp = &D_800BE628[view];
        if (func_150859AC(view, 0) != 0) {
            D_800DBEE8[view] = 0;
            for (i = 0; i < gWorldObjectCount;) {
                groupSize = db_Object_array[i].state >> 4;
                end = i + groupSize;
                mask = 0;
                start = i;
                for (; i <= end; i++) {
                    obj = i + db_Object_array; // matching: &db_Object_array[i] swaps the addu operands
                    obj->viewFlags &= ~(1 << view);
                    // matching: empty then-branch orders uopt's live ranges (see func_15112A80 notes)
                    if (!(obj->inactive != 1 && !(obj->state & 8) && (obj->object_status & 1) && obj->alpha != 0 &&
                          (D_800C3658[player] != 0 || D_800C35EA[player] != 1 || func_15022B08(i, player) != 0) &&
                          ((D_800C35EA[player] == 1 && func_15022B08(i, player) != 0) ||
                           !(obj->effectFlags & 0x100) || func_1503F800(&obj->unk98, obj->x, obj->z, view, 0) == 0))) {
                    } else {
                        f32 (*mtx)[4];

                        mtx = D_800D9C10[view];
                        dp = (s32 *)((u8 *)dist + (i - start) * 4); // matching: &dist[i - start] scales i and start separately
                        if (camera_global_Cameras[view].mv->SniperState != 0) {
                            drawDist = D_800D9B1C;
                        } else if (obj->drawDistance == 0) {
                            drawDist = D_800D9B1C;
                        } else if (obj->drawDistance == 1) {
                            drawDist = 40000;
                        } else {
                            drawDist = obj->drawDistance;
                        }
                        *dp = func_150A6360(recp, mtx, obj->x, obj->y + obj->yOffset, obj->z,
                                            (f32)(u32)obj->cullRadiusXZ, (f32)(u32)obj->cullRadiusY, drawDist);
                        if (*dp < 0) {
                            *dp = -*dp;
                        }
                        if (*dp == 0) {
                            obj->dst_fade_alpha[view] = 0;
                        } else if (drawDist < *dp) {
                            obj->dst_fade_alpha[view] = 0;
                        } else {
                            if (*dp < drawDist - 200) {
                                step = D_800BE9E4 * 10;
                                if (obj->fadeDistance != 0) {
                                    if (*dp < obj->fadeDistance) {
                                        if (step < obj->dst_fade_alpha[view]) {
                                            obj->dst_fade_alpha[view] -= step;
                                        } else {
                                            obj->dst_fade_alpha[view] = 0;
                                        }
                                    } else {
                                        if (obj->dst_fade_alpha[view] < 255 - step) {
                                            obj->dst_fade_alpha[view] += step;
                                        } else {
                                            obj->dst_fade_alpha[view] = 255;
                                        }
                                    }
                                    if (obj->dst_fade_alpha[view] == 0) {
                                        continue;
                                    }
                                } else {
                                    obj->dst_fade_alpha[view] = 255;
                                // matching: this label splits the basic block, changing as1's scheduling
                                fence:;
                                }
                            } else {
                                alpha = ((drawDist - *dp) * 255) / 200;
                                if (alpha >= 256) {
                                    alpha = 255;
                                }
                                obj->dst_fade_alpha[view] = alpha;
                            }
                            mask |= 1 << view;
                        }
                    }
                }
                if (mask != 0) {
                    for (i = start; i <= end; i++) {
                        db_Object_array[i].viewFlags |= 1 << view;
                        (*D_80089240[view])[*count].index = i;
                        (*D_80089240[view])[*count].distance = *(s32 *)((u8 *)dist + (i - start) * 4); // matching: as above
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

    for (i = 0; i < gWorldObjectCount; i++) {
        db_Object_array[i].viewFlags &= ~0x40;
        if (((db_Object_array[i].viewFlags & 0xF) != 0 || (db_Object_array[i].state & 4) == 4) && db_Object_array[i].moveProc != NULL) {
            db_Object_array[i].viewFlags |= 0x40;
        }
    }
}

/* Writes this frame's model matrix for every object in each view's draw list. */
void func_15113218(void) {
    WorldObject *obj;
    Actor *actor;
    f32 (*parent)[4];
    MtxF mf;
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
                obj = &db_Object_array[(*D_80089240[view])[j].index];
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
                    actor = &movement_obj[obj->attach - 100];
                    if (actor->ob_matrix_ptr == NULL) {
                        func_150A7CB0(mf, obj->xScale, obj->yScale, obj->zScale);
                        mf[3][0] = actor->xpos;
                        mf[3][1] = actor->ypos;
                        mf[3][2] = actor->zpos;
                        guMtxF2L(mf, &(*D_80089250[D_800BE9C0])[(*D_80089240[view])[j].index]);
                    } else {
                        rot[2] = 0.0f;
                        rot[0] = rot[2];
                        rot[1] = actor->unk90;
                        parent = actor->ob_matrix_ptr[0];
                        func_150A7CB0(mf, obj->xScale, obj->yScale, obj->zScale);
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
    gfx = func_1515E544(gfx, *(PointLightGfx **)((u8 *)D_800D9E10 + viewOffset), D_800D9E20, D_800D9E21,
                        &D_800D9BD0[view][D_800BE9C0]);
    for (i = 0; i < D_800DBEE8[view]; i++) {
        gfx = func_151137D4(gfx, &db_Object_array[(**list)[i].index], &(*D_80089250[D_800BE9C0])[(**list)[i].index], view,
                            (**list)[i].distance, pass);
    }
    gSPMatrix(gfx++, &D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gfx++, G_CULL_FRONT | G_LOD | 0x400000);
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    return gfx;
}

extern u8 D_80083D40[];
extern u8 D_80083EC0[];

/*
 * Draws one object in one view, if it belongs to this pass (opaque pass: alpha 255 and no flags bit 0x10).
 * Returns gfx unchanged if nothing was drawn or the display list would overflow.
 */
Gfx *func_151137D4(Gfx *gfx, WorldObject *obj, Mtx *mtx, s32 view, s32 distance, s32 pass) {
    s32 alpha;
    Gfx *start;
    s32 overflow;
    u16 effects;

    start = gfx;
    if (obj->dst_fade_alpha[view] == 255 && obj->alpha == 255) {
        alpha = 255;
    } else {
        alpha = (obj->dst_fade_alpha[view] * obj->alpha) / 255;
        if (alpha == 0) {
            return gfx;
        }
    }
    if (alpha == 255) {
        if (pass == (obj->object_status & 0x10)) {
            return gfx;
        }
    } else if (pass != 0) {
        return gfx;
    }
    effects = obj->effectFlags;
    if (obj->effectFlags != 0) {
        if (effects & 1) {
            gfx = func_1511FC2C(gfx, obj->x, obj->y + obj->yOffset, obj->z, obj->u, view);
        } else if (effects & 0x10) {
            gfx = func_1511FC2C(gfx, obj->x, obj->y + obj->yOffset, obj->z, obj->touch.floor.unk7C, view);
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
    if ((obj->object_status & 2) == 2) {
        gSPSetGeometryMode(gfx++, 0x400000);
    } else {
        gSPClearGeometryMode(gfx++, 0x400000);
    }
    gSPSetGeometryMode(gfx++, G_LIGHTING);
    gfx = func_1510D8C0(func_1510CDB8(gfx, 255, alpha, view), obj);
    if (pass == 0) {
        gSPSetGeometryMode(gfx++, G_FOG | G_LOD);
    }
    gSPSegment(gfx++, 1, obj->vertices[D_800BE9C0]);
    gSPMatrix(gfx++, mtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPClearGeometryMode(gfx++, G_TEXTURE_GEN_LINEAR);
    if (obj->effectFlags & 0x200) {
        gfx = func_150CF5E8(gfx);
    }
    if ((obj->state & 2) == 2) {
        gfx = func_150A50C0(obj->displayList, gfx, D_800D9C10[view], 0, 0, (s32)obj->vertices[D_800BE9C0] + 0xFF000000);
    } else {
        gSPDisplayList(gfx++, obj->displayList);
    }
    { // matching: hand-written through one pointer; the GBI macros' own _g copy reorders the stores
        Gfx *cmd = gfx++; cmd->words.w0 = 0xD9EEFFFF; cmd->words.w1 = 0; // gSPClearGeometryMode(G_FOG | G_LOD)
        cmd = gfx++; cmd->words.w0 = 0xE7000000; cmd->words.w1 = 0;      // gDPPipeSync
    }
    overflow = (D_800BEBA4 < gfx - D_800BE9C8[D_800BE9C0]) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    return gfx;
}

/* Creates or frees each object's shadow depending on whether the object is shown (in every active view). */
void func_15113C88(void) {
    s32 i;
    s32 j;
    s32 hidden;
    s32 show;
    WorldObject *obj;

    for (i = 0; i < gWorldObjectCount; i++) {
        hidden = FALSE;
        obj = i + db_Object_array; // matching: &db_Object_array[i] swaps the addu operands
        for (j = 0; j < D_80082FA0; j++) {
            if (func_150859AC(j, 0) != 0 && obj->dst_fade_alpha[j] == 0) {
                hidden = TRUE;
                break;
            }
        }
        do { // matching: without this wrapper uopt stops keeping &D_800C3658 in a saved register
            if ((obj->state & 8) || obj->unk4C == 0 || (obj->object_status & 0x10) == 0x10 || hidden ||
                (D_800C3658[0] == 0 && D_800C35EA[0] == 1 && func_15022B08(i, 0) == 0) || !(obj->object_status & 1)) {
                show = FALSE;
            } else {
                show = TRUE;
            }
        } while (0);
        if (show) {
            if (obj->shadow == NULL) {
                obj->shadow = func_1510D970(1, obj, 0, 1, 0);
            }
        } else if (obj->shadow != NULL) {
            func_1516972C(obj->shadow);
            obj->shadow = NULL;
        }
    }
}

/* A group of objects that move together; the first member leads. */

/* Runs the update callback of every flagged object: first the ungrouped ones, then each group's members,
   where followers take the leader's move delta and floor triangle. */
void func_15113E54(s32 arg0) {
    s16 moveX;
    s16 moveY;
    s16 moveZ;
    void *leaderFloorTri;
    s32 i;
    s32 j;
    s32 count;
    s32 member;
    WorldObject *obj;

    moveX = 0;
    moveY = 0;
    moveZ = 0;
    leaderFloorTri = 0;
    count = gWorldObjectCount - D_800DBF98;
    for (i = 0; i < count; i++) {
        obj = i + db_Object_array; // matching: operand order of the addu
        if ((obj->viewFlags & 0x40) == 0x40 && obj->inactive == 0) {
            obj->moveProc(obj);
        }
    }

    for (i = 0; i < (s32)D_80087380; i++) {
        if ((D_800D23C0[i].members[0] >> 12) == 2) {
            for (j = 0; j < D_800D23C0[i].count; j++) {
                member = D_800D23C0[i].members[j];
                if ((member >> 12) == 2) {
                    obj = (member & 0xFFF) + db_Object_array; // matching: operand order of the addu
                    if ((obj->viewFlags & 0x40) == 0x40 && obj->inactive == 0) {
                        if (j != 0) {
                            obj->moveX = moveX;
                            obj->moveY = moveY;
                            obj->moveZ = moveZ;
                            obj->touch.floor.tri = leaderFloorTri;
                        }
                        obj->moveProc(obj);
                    }
                    if (j == 0) {
                        moveX = obj->moveX;
                        moveY = obj->moveY;
                        moveZ = obj->moveZ;
                        leaderFloorTri = obj->touch.floor.tri;
                    }
                }
            }
        }
    }
}

/* Whether actor `actor` (any actor if -1) touches obj; only objects with flags bit 0x80 track contacts. */
s32 func_15114050(WorldObject *obj, s32 actor) {
    if (obj->object_status & 0x80) {
        if (actor == -1) {
            return TRUE;
        }
        if (gWorldObjectContacts[obj - db_Object_array] & (1 << actor)) {
            return TRUE;
        }
    }
    return FALSE;
}

/* The lowest-numbered actor touching obj, or 0. */
s32 func_151140C4(WorldObject *obj) {
    s32 i;

    i = 0;
    if (obj->object_status & 0x80) {
        for (; i != 32; i++) {
            if (gWorldObjectContacts[obj - db_Object_array] & (1 << i)) {
                return i;
            }
        }
    }
    return 0;
}

/* Dispatches this frame's actor contacts to each object's onContact callback and carries riding actors. */
void func_15114188(void) {
    s32 i;
    s32 j;

    for (i = 0; i < gWorldObjectCount; i++) {
        if (gWorldObjectContacts[i] != 0) {
            for (j = 0; j < 25 && gWorldObjectContacts[i] != 0; j++) {
                if (gWorldObjectContacts[i] & (1 << j)) {
                    gWorldObjectContacts[i] ^= 1 << j;
                    if (db_Object_array[i].onContact != NULL && (db_Object_array[i].contactNonPlayers != 0 || movement_obj[j].playernumber != 0xFF)) {
                        db_Object_array[i].onContact(i + db_Object_array, &movement_obj[j]); // matching: addu operand order
                    }
                    if ((db_Object_array[i].object_status & 4) == 4 && !(db_Object_array[i].object_status & 8)) {
                        func_1511473C(&movement_obj[j], i);
                    }
                }
            }
            db_Object_array[i].object_status &= ~0x8C;
        }
    }
}

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
    MtxF m;
    MtxF m2;
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
    dx = *x - db_Object_array[objIndex].x;
    dy = *y - db_Object_array[objIndex].y;
    dz = *z - db_Object_array[objIndex].z;
    if (db_Object_array[objIndex].t != 0.0f) {
        guRotateF(m, db_Object_array[objIndex].t, 1, 0, 0);
        func_150A7960(m, yaxis[0], yaxis[1], yaxis[2], &yaxis[0], &yaxis[1], &yaxis[2]);
        func_150A7960(m, zaxis[0], zaxis[1], zaxis[2], &zaxis[0], &zaxis[1], &zaxis[2]);
    }
    if (db_Object_array[objIndex].v != 0.0f) {
        guRotateF(m, db_Object_array[objIndex].v, zaxis[0], zaxis[1], zaxis[2]);
        func_150A7960(m, yaxis[0], yaxis[1], yaxis[2], &yaxis[0], &yaxis[1], &yaxis[2]);
        func_150A7960(m, xaxis[0], xaxis[1], xaxis[2], &xaxis[0], &xaxis[1], &xaxis[2]);
    }
    if (db_Object_array[objIndex].u != 0.0f) {
        guRotateF(m, db_Object_array[objIndex].u, yaxis[0], yaxis[1], yaxis[2]);
        func_150A7960(m, xaxis[0], xaxis[1], xaxis[2], &xaxis[0], &xaxis[1], &xaxis[2]);
        func_150A7960(m, zaxis[0], zaxis[1], zaxis[2], &zaxis[0], &zaxis[1], &zaxis[2]);
    }
    guRotateF(m, db_Object_array[objIndex].spinX, xaxis[0], xaxis[1], xaxis[2]);
    guRotateF(m2, db_Object_array[objIndex].spinZ, zaxis[0], zaxis[1], zaxis[2]);
    func_150A7A48(m, m2, m);
    guRotateF(m2, db_Object_array[objIndex].spinY, yaxis[0], yaxis[1], yaxis[2]);
    func_150A7A48(m, m2, m);
    func_150A7960(m, dx, dy, dz, &rx, &ry, &rz);
    rx = db_Object_array[objIndex].moveX + (rx - dx);
    ry = db_Object_array[objIndex].moveY + (ry - dy);
    rz = db_Object_array[objIndex].moveZ + (rz - dz);
    *x += rx;
    *y += ry;
    *z += rz;
}

extern f32 D_800A2F60;

/* Carries an actor standing on object objIndex: moves it with the object and turns it by the object's yaw spin. */
void func_1511473C(Actor *actor, s32 objIndex) {
    if (actor == NULL || objIndex >= gWorldObjectCount) {
        return;
    }
    func_15114348(objIndex, &actor->xpos, &actor->ypos, &actor->zpos);
    actor->angle += db_Object_array[objIndex].spinY * D_800A2F60;
    if (actor->unk137 != 0 && objIndex + 1 == D_800CBDA0[actor - movement_obj]) {
        actor->langle = actor->angle;
    }
}

/* Builds an object's rotation matrix: yaw first, then pitch/roll (func_150A8050 makes Euler rotations). */
void func_151148A8(MtxF mtx, WorldObject *obj) {
    MtxF pitchRoll;

    func_150A8050(mtx, 0.0f, obj->u, 0.0f);
    func_150A8050(pitchRoll, obj->t, 0.0f, obj->v);
    func_150A7A48(pitchRoll, mtx, mtx);
}

/* Builds an object's model matrix: scale, then rotation, then translation to (x, y + yOffset, z). */
void func_1511490C(MtxF mtx, WorldObject *obj) {
    MtxF scale;

    func_151148A8(mtx, obj);
    mtx[3][0] = obj->x;
    mtx[3][1] = obj->y + obj->yOffset;
    mtx[3][2] = obj->z;
    func_150A7CB0(scale, obj->xScale, obj->yScale, obj->zScale);
    func_150A7A48(scale, mtx, mtx);
}

/* The object whose id is `id`, or NULL (id 0 never matches). */
WorldObject *func_151149AC(u8 id) {
    s32 i;

    if (id == 0) {
        return NULL;
    }
    for (i = 0; i < gWorldObjectCount; i++) {
        if (db_Object_array[i].unique_id == id) {
            return i + db_Object_array;
        }
    }
    return NULL;
}

/* Releases the handle of each object that is no longer drawn (after its delay, if it is unattached). */
void func_15114A1C(void) {
    s32 i;
    void *handle;

    for (i = 0; i < gWorldObjectCount; i++) {
        if ((db_Object_array[i].state & 1) != 1) {
            // matching: the dead copies (handle, j) and empty ifs steer uopt's register choices (see notes)
            handle = D_800DBEF8[i];
            if (&D_800DBEF8) {}
            if (D_800DBEF8[i] != 0) {
                if (db_Object_array[i].attach != 3 && !(db_Object_array[i].viewFlags & 0x80)) {
                    s32 j;

                    j = i;
                    if (j) {}
                    func_10004074(D_800DBEF8[j]);
                    if (handle) {}
                    D_800DBEF8[j] = 0;
                } else if (D_800DBEFC[i] != 0) {
                    D_800DBEFC[i]--;
                } else {
                    s32 j;

                    j = i;
                    if (j) {}
                    func_10004074(D_800DBEF8[j]);
                    D_800DBEF8[j] = 0;
                }
            }
        } else if (!(db_Object_array[i].viewFlags & 0x80)) {
            D_800DBEF8[i] = 0;
        }
    }
}

/* Removes object `index` (frees its shadow, marks it removed), then drops removed objects off the array's end. */
void func_15114B94(u32 index) {
    u32 ungrouped;

    if (index < gWorldObjectCount) {
        if (db_Object_array[index].shadow != NULL) {
            func_1516972C(db_Object_array[index].shadow);
            db_Object_array[index].shadow = NULL;
        }
        db_Object_array[index].state |= 8;
        db_Object_array[index].inactive = 1;
    }
    index = gWorldObjectCount - 1;
    ungrouped = gWorldObjectCount - D_800DBF98;
    while (gWorldObjectCount != 0 && (db_Object_array[index].state & 8)) {
        if (index >= ungrouped && D_800DBF98 != 0) {
            D_800DBF98--;
        } else {
            ungrouped--;
        }
        gWorldObjectCount--;
        index--;
    }
}

/* Sound callback: moves the sound to its object (once, or every call when *follow is set). Returns 1 if not. */
s32 func_15114CC4(ObjSound *sound, s32 arg1, s32 *follow, s32 arg3) {
    WorldObject *obj = sound->obj;

    if (obj != NULL && (*follow != 0 || sound->positioned == 0)) {
        sound->positioned = 1;
        sound->x = obj->x;
        sound->y = obj->y;
        sound->z = obj->z;
        return 0;
    }
    return 1;
}

/*
 * Plays sound `sfx` at obj, keeping the handle in obj->sound. sfx -1 stops it. flags: 1 keep it if already
 * playing, 2 restart (positioned once), 4 one-shot, 8 don't stop or forget the current sound;
 * otherwise the sound follows the object through func_15114CC4.
 */
void func_15114D24(WorldObject *obj, s32 sfx, s32 arg2, s16 arg3, s16 arg4, s32 flags) {
    u16 handle;

    if (flags & 1) {
        if (obj->sound_id != 0) {
            if (func_1001147C(obj->sound_id) == sfx) {
                return;
            }
        }
    }

    if (sfx == -1) {
        if (obj->sound_id != 0) {
            func_100111C8(obj->sound_id);
            obj->sound_id = 0;
        }
        return;
    }

    if (flags & 8) {
        handle = 0;
    } else {
        handle = obj->sound_id;
    }

    if (flags & 4) {
        func_10010E78(handle, sfx, arg2, 0, 0, -1, obj->x, obj->y, obj->z, arg3, arg4);
        if ((flags & 8) == 0) {
            obj->sound_id = 0;
        }
    } else if (flags & 2) {
        if (handle != 0) {
            func_1001123C(handle);
            handle = 0;
        }
        obj->sound_id = func_10010E78(handle, sfx, arg2, 0, 0, -1, obj->x, obj->y, obj->z, arg3, arg4);
    } else {
        if (handle != 0) {
            func_1001123C(handle);
        }
        obj->sound_id = func_1000FA64(sfx, obj->x, obj->y, obj->z, arg2, arg4, arg3, (SoundRecordCallback)func_15114CC4, obj, 0, 0, 0);
    }
}

/* Starts a sound through func_1001001C with func_15114CC4 as its callback. */
void func_15114F04(s32 arg0, s32 arg1, s32 arg2) {
    func_1001001C((SoundRecordCallback)func_15114CC4, (void *)arg0, 0, arg1, arg2);
}

extern s32 D_800DBF9C;
extern s32 D_800DBFA0;
extern s32 D_800DBFA4;

/*
 * On level 0x12, 0x13 and 0x36 only: checks both D_800DBDD8 lists (stops with error 0xC00005A on a bad
 * entry), then stores the three arguments in D_800DBF9C..D_800DBFA4.
 */
void func_15114F44(s32 arg0, s32 arg1, s32 arg2) {
    s32 j;
    s32 i;
    u32 (*list)[3];

    if ((level == 0x12) || (level == 0x36) || (level == 0x13)) {
        for (j = 0; j < 2; j++) {
            list = (u32 (*)[3])D_800DBDD8[j];
            for (i = 0; i < D_800DBE18[j]; i++) {
                /* each entry holds three KSEG0 addresses, 4-byte aligned */
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
