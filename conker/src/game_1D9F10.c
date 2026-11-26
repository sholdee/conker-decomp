#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/*
 * Swinging ropes. A rope is a chain of up to 20 nodes hanging from a fixed top node; an
 * object (unique_id 1) can grab it, climb it and swing on it. Ropes live in object lists
 * 0x2F and 0x4E (D_800A9270).
 */

/* The two object lists ropes are allocated in. */
static const s32 D_800A9270[] = { 0x2F, 0x4E };

/** Creates the RopeRider link for obj; returns NULL if obj can't ride ropes or allocation fails. */
RopeRider *func_151ACA60(Actor *obj, f32 radius, s32 extraSize) {
    RopeRider *rider;

    if (obj == NULL) {
        return NULL;
    }
    rider = func_15167A68(0x30, 0, extraSize + 0x30, 1, 0xFF, 1);
    if (rider == NULL) {
        return NULL;
    }
    if (func_151ACB38(obj, &rider->unk18) == 0) {
        func_1516979C(&rider->header);
        return NULL;
    }
    rider->owner = obj;
    rider->ownerId = obj->unique_id;
    rider->ownerIndex = obj - movement_obj;
    rider->radius = radius;
    rider->unk10 = 1;
    rider->unk14 = 0;
    return rider;
}

/** Returns TRUE (and sets *out to 1) if obj may ride ropes, i.e. its unique_id is 1. */
s32 func_151ACB38(Actor *obj, u8 *out) {
    s32 ret = FALSE;

    if (obj->unique_id == 1) {
        *out = 1;
        ret = TRUE;
    }
    return ret;
}

/** RopeRider update: moves the owner along its rope while it is riding one. */
s32 func_151ACB60(RopeRider *rider) {
    s32 result;
    Rope *rope = rider->owner->mv->ropePtr;

    if (rope != NULL) {
        result = func_151AE3A8(rider);
        if (rider) {} // matching: keeps rider live across the call (IDO spills a0 in the jal delay slot)
        return result;
    }
}

void func_151ACB94(RopeRider *rider, s32 arg1, u8 arg2) {
    func_15169850(arg1, arg2, &rider->owner, &rider->ownerId, rider);
}

/** Creates a rope from 0x24 bytes of spawn data: 20 nodes hanging from topY down to bottomY. */
Rope *func_151ACBD4(void *spawn, s32 extraSize) {
    Rope *rope;
    TextureDesc *tex;
    s32 i;
    s32 segments;
    s32 buf;
    s32 x;
    s32 bottomY;
    s32 topY;
    s32 z;
    s32 shift;
    s32 quarterW;
    u8 r;
    u8 g;
    u8 b;
    s32 vtxT;
    s32 rowH;
    s32 texT;
    s32 texS0;
    s32 texS1;
    s32 texS2;
    s32 texS3;

    rope = func_15167A68((level == 0x2C) ? 0x4E : 0x2F, 0, extraSize + 0x58, 1, 0xFF, 1);
    if (rope == NULL) {
        return NULL;
    }
    memcpy(&rope->id, spawn, 0x24);
    rope->rider = NULL;
    rope->regrabTimer = 0;
    rope->unk34 = 1.0f / rope->unk2C;
    segments = 19;
    rope->nodes = allocate_memory(20 * sizeof(RopeNode), 1, 0, 0);
    rope->nodeCount = segments + 1;
    x = rope->x;
    z = rope->z;
    topY = rope->topY;
    bottomY = rope->bottomY;
    for (i = 0; i < segments + 1; i++) {
        rope->nodes[i].pos[0] = x;
        rope->nodes[i].pos[1] = (i * (topY - bottomY)) / segments + bottomY;
        rope->nodes[i].pos[2] = z;
        rope->nodes[i].vel[0] = 0.0f;
        rope->nodes[i].vel[1] = 0.0f;
        rope->nodes[i].vel[2] = 0.0f;
    }
    rope->segmentLength = (f32)(topY - bottomY) / segments;
    rope->vertices = allocate_memory(2 * 20 * 4 * sizeof(Vtx), 1, 2, 0);
    if (level == 0xB) {
        rope->textureId = 0x37;
        shift = 4;
    } else {
        rope->textureId = 0x36;
        shift = 5;
    }
    r = 0xFF;
    g = 0xFF;
    if (level == 0x39) {
        r = 0x58;
        g = 0x4B;
        b = 0x2C;
    } else {
        b = 0xFF;
    }
    tex = &D_80090B60[rope->textureId];
    quarterW = tex->width >> 2;
    rowH = tex->height - 1;
    texS0 = 0x100 << 5;
    texS1 = (quarterW + 0x100) << 5;
    texS2 = ((quarterW * 2) + 0x100) << 5;
    texS3 = ((quarterW * 3) + 0x100) << 5;
    for (buf = 0; buf < 2; buf++) {
        if (segments) { // matching: stops IDO strength-reducing the buf index into the inner loop
            for (i = 0, texT = 0x100; i < 20; i++, texT += rowH) {
                vtxT = texT << shift;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.flag = 0;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.tc[0] = texS0;
                if (1) {} // matching: block boundary fixes IDO's register choice in this loop
                rope->vertices[((buf * 20 + i) * 4) + 0].v.tc[1] = vtxT;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.cn[0] = r;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.cn[1] = g;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.cn[2] = b;
                rope->vertices[((buf * 20 + i) * 4) + 0].v.cn[3] = 0xFF;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.flag = 0;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.tc[0] = texS1;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.tc[1] = vtxT;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.cn[0] = r;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.cn[1] = g;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.cn[2] = b;
                rope->vertices[((buf * 20 + i) * 4) + 1].v.cn[3] = 0xFF;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.flag = 0;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.tc[0] = texS2;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.tc[1] = vtxT;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.cn[0] = r;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.cn[1] = g;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.cn[2] = b;
                rope->vertices[((buf * 20 + i) * 4) + 2].v.cn[3] = 0xFF;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.flag = 0;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.tc[0] = texS3;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.tc[1] = vtxT;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.cn[0] = r;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.cn[1] = g;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.cn[2] = b;
                rope->vertices[((buf * 20 + i) * 4) + 3].v.cn[3] = 0xFF;
            }
        }
    }
    rope->releaseForce = 0; // matching: integer 0 (0.0f schedules differently)
    return rope;
}

/** Rope update: applies the rider's swing and release impulses, then steps the node chain. */
void func_151AD174(Rope *rope) {
    s32 angle;
    f32 force;
    s32 timer;
    s32 last;
    s32 node;
    f32 pos[3];
    f32 vel[3];
    f32 oldPos[20][3];
    f32 oldVel[20][3];
    s32 i;
    s32 j;
    f32 mid[3];
    f32 pull[3];
    f32 motion;
    f32 dist;
    f32 dy2;
    f32 t;
    f32 h;
    f32 len;
    f32 pad; // matching: unused stack slot
    f32 scale;
    f32 dx2;

    motion = 0.0f;
    if (rope->regrabTimer != 0) {
        rope->regrabTimer -= D_800BE9E4;
        if (rope->regrabTimer < 0) {
            rope->regrabTimer = 0;
        }
    }
    last = rope->nodeCount - 1;
    if (rope->rider != NULL) {
        node = rope->rider->mv->ropeNode;
        angle = (rope->rider->mv->ropeSwingAngle + (rope->rider->angle >> 8)) & 0xFF;
        force = rope->rider->mv->ropeSwingForce * 1.899999976f;
        rope->nodes[node].vel[0] += force * func_150489B0(angle);
        rope->nodes[node].vel[2] -= force * func_15048A40(angle);
        angle = rope->rider->angle >> 8;
        force = rope->rider->speed * 19.0f;
        rope->nodes[node].vel[0] += force * func_150489B0(angle);
        rope->nodes[node].vel[2] -= force * func_15048A40(angle);
        rope->nodes[node].vel[1] -= 1000.0f;
        timer = rope->rider->mv->ropeSwingTimer - 1;
        if (timer <= 0) {
            rope->rider->mv->ropeSwingForce = 0.0f;
            timer = 0;
        }
        rope->rider->mv->ropeSwingTimer = timer;
    }
    if (rope->releaseForce != 0.0f) {
        rope->nodes[rope->releaseNode].vel[0] -= rope->releaseForce * func_150489B0(rope->releaseAngle);
        rope->nodes[rope->releaseNode].vel[2] += rope->releaseForce * func_15048A40(rope->releaseAngle);
        rope->releaseForce *= 0.8999999762f;
    }
    for (i = 0; i < last + 1; i++) {
        for (j = 0; j < 3; j++) {
            oldPos[i][j] = rope->nodes[i].pos[j];
            oldVel[i][j] = rope->nodes[i].vel[j];
        }
    }

    /* Walk down from the node below the fixed anchor. */
    for (i = last - 1; i >= 0; i--) {
        for (j = 0; j < 3; j++) {
            pos[j] = oldPos[i][j];
            vel[j] = rope->nodes[i].vel[j];
            pull[j] = pos[j] - oldPos[i + 1][j]; // matching: dead store (pull is recomputed below)
        }

        /* Pull toward hanging straight down, len below the node above. */
        {
            f32 dx;
            f32 dy;
            f32 dz;

            dx = pos[0] - oldPos[i + 1][0];
            dy = pos[1] - oldPos[i + 1][1];
            dz = pos[2] - oldPos[i + 1][2];
            dist = sqrtf(dx * dx + dy * dy + dz * dz);
            if (dist == 0.0f) {
                dist = 1.0f;
            }
            len = 9.0f;
            t = sqrtf(dx * dx + dz * dz) / dist;
            t = len * t;
            h = t * t;
            t = len * len - h;
            if (h <= len * len) {
                t = sqrtf(t);
            } else {
                t = 0.0f;
            }
            dist = t / dist;
            pull[0] = dx * dist;
            pull[1] = dy * dist;
            pull[2] = dz * dist;
            pull[0] = 0.0f - pull[0];
            pull[1] = -len - pull[1];
            pull[2] = 0.0f - pull[2];
        }
        for (j = 0; j < 3; j++) {
            vel[j] += pull[j];
            vel[j] *= 0.9599999785f;
            if (i != 0) {
                /* Smooth toward the midpoint of the neighbouring nodes. */
                mid[j] = (oldPos[i - 1][j] + oldPos[i + 1][j]) * 0.5f - pos[j];
                vel[j] += mid[j] * 1.24;
            }
            pos[j] += vel[j];
            if (level == 0x39 && pos[0] > 1150.0f) {
                pos[0] = 1150.0f;
            }
        }

        /* Keep the node exactly segmentLength from the node above. */
        h = pos[2] - rope->nodes[i + 1].pos[2];
        dx2 = pos[0] - rope->nodes[i + 1].pos[0];
        dy2 = pos[1] - rope->nodes[i + 1].pos[1];
        motion += fabsf(vel[0]);
        motion += fabsf(vel[2]);
        t = sqrtf(h * h + (dx2 * dx2 + dy2 * dy2));
        if (t != 0.0f) {
            scale = rope->segmentLength / t;
        } else {
            scale = 1.0f;
        }
        for (j = 0; j < 3; j++) {
            rope->nodes[i].pos[j] = rope->nodes[i + 1].pos[j] + (pos[j] - rope->nodes[i + 1].pos[j]) * scale;
            rope->nodes[i].vel[j] = rope->nodes[i].pos[j] - oldPos[i][j];
        }
    }

    /* Creak when swinging hard, unless the last creak is still playing. */
    if (motion > 250.0f && (rope->soundHandle == 0 || func_1000F3D0(rope->soundHandle) == 0)) {
        rope->soundHandle = func_10010E78(0, (func_150ADA20() % 9) + 0x507, 30000, 0, 0, 0, pos[0], pos[1], pos[2], 250, -0x7E0C);
    }
}

/** Rope draw: builds this frame's vertex ring around each node and emits the textured strip. */
Gfx *func_151AD92C(Gfx *dl, Rope *rope, s32 arg2) {
    s32 pad0; // matching: unused stack slot
    s32 i;
    s32 n;
    u16 w;
    s32 rowH;
    s32 shift;
    f32 offX, offY, offZ;
    f32 x, y, z;
    f32 dx, dy, dz;
    f32 len, inv;
    f32 pad; // matching: unused stack slot

    dy = 0.0f; // matching: dead store; changes IDO's float register assignment
    n = rope->nodeCount;
    w = D_80090B60[rope->textureId].width;
    rowH = D_80090B60[rope->textureId].height - 1;
    if (rope->textureId == 0x36) {
        shift = 5;
    } else {
        shift = 4;
    }
    for (i = 0; i < n; i++) {
        RopeNode *node = &rope->nodes[i];

        x = node->pos[0];
        y = node->pos[1];
        z = node->pos[2];
        if (i != n - 1) {
            /* direction to the next node up */
            dx = node[1].pos[0] - node->pos[0];
            dy = node[1].pos[1] - node->pos[1];
            dz = node[1].pos[2] - node->pos[2];
        } else {
            dy = 1.0f;
            dx = 0.0f;
            dz = 0.0f;
        }
        len = sqrtf(dz * dz + (-dy) * (-dy));
        if (len != 0.0f) {
            offX = 0.0f;
            inv = 1.0f / len;
            offY = dz * inv;
            offZ = (-dy) * inv;
        } else {
            offX = 100.0f;
            offY = 0.0f;
            offZ = 0.0f;
        }
        offX *= 4.0f;
        offY *= 4.0f;
        offZ *= 4.0f;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 0].v.ob[0] = x + offX;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 0].v.ob[1] = y + offY;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 0].v.ob[2] = z + offZ;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 2].v.ob[0] = x - offX;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 2].v.ob[1] = y - offY;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 2].v.ob[2] = z - offZ;
        len = sqrtf((-dy) * (-dy) + dx * dx);
        if (len != 0.0f) {
            offZ = 0.0f;
            inv = 1.0f / len;
            offX = (-dy) * inv;
            offY = dx * inv;
        } else {
            offX = 1.0f;
            offZ = 1.0f;
            offY = 0.0f;
        }
        offX *= 4.0f;
        offY *= 4.0f;
        offZ *= 4.0f;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 1].v.ob[0] = x + offX;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 1].v.ob[1] = y + offY;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 1].v.ob[2] = z + offZ;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 3].v.ob[0] = x - offX;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 3].v.ob[1] = y - offY;
        rope->vertices[(D_800BE9C0 * n + i) * 4 + 3].v.ob[2] = z - offZ;
    }
    gSPSetGeometryMode(dl++, G_CULL_BACK);
    gSPClearGeometryMode(dl++, G_CULL_FRONT);
    gSPTexture(dl++, 0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON);
    gDPPipeSync(dl++);
    dl = func_15094FE8(dl, &D_80090B60[rope->textureId], 0, 0, 0, 0, 0, 0, 0x100, 0x100, 0x3E);
    gDPSetCombineMode(dl++, G_CC_MODULATEIA, G_CC_MODULATEIA);
    gDPSetOtherMode(dl++, G_PM_NPRIMITIVE | G_CYC_1CYCLE | G_TP_PERSP | G_TD_CLAMP | G_TL_TILE | D_800D2C9C | G_TF_BILERP | G_TC_FILT | G_CK_NONE | 0xF, 0x552078);
    for (i = 0; i < n - 1; i++) {
        gSPVertex(dl++, &rope->vertices[(D_800BE9C0 * n + i) * 4], 8, 0);
        gSP1Triangle(dl++, 0, 1, 4, 0);
        gSP1Triangle(dl++, 1, 5, 4, 0);
        gSP1Triangle(dl++, 1, 2, 5, 0);
        gSP1Triangle(dl++, 2, 6, 5, 0);
        gSP1Triangle(dl++, 2, 3, 7, 0);
        gSP1Triangle(dl++, 2, 7, 6, 0);
        gSPModifyVertex(dl++, 0, G_MWO_POINT_ST, (((w + 0x100) & 0x7FF) << 21) | ((i * rowH + 0x100) << shift));
        gSPModifyVertex(dl++, 4, G_MWO_POINT_ST, (((w + 0x100) & 0x7FF) << 21) | (((i + 1) * rowH + 0x100) << shift));
        gSP1Triangle(dl++, 3, 0, 7, 0);
        gSP1Triangle(dl++, 0, 4, 7, 0);
        if (i == 0) {
            gSP1Triangle(dl++, 2, 1, 0, 0);
            gSP1Triangle(dl++, 3, 2, 0, 0);
        }
    }
    return dl;
}

/** obj touched the rope zone `zone` (rope id: data1): grab it, or let go of the current rope and grab this one. */
void func_151AE06C(Actor *obj, TriggerZone *zone) {
    u8 canRide;
    u8 ropeId;

    if (func_151ACB38(obj, &canRide) != 0) {
        ropeId = zone->data1;
        if (obj->mv->rope == 0) {
            func_151AE0E4(obj, ropeId);
        } else if (obj->mv->rope != ropeId) {
            func_151AE264(obj);
            func_151AE0E4(obj, ropeId);
        }
    }
}

/** Grabs rope ropeId if it is free and obj is alive, not stunned and close to its grab node. */
void func_151AE0E4(Actor *obj, u8 ropeId) {
    Rope *rope;
    f32 pos[3];
    s32 i;
    s32 endNodes;
    f32 dist;

    rope = func_151AE590(ropeId);
    if (rope == NULL) {
        while (TRUE) {
        }
    }
    if (rope->regrabTimer != 0 || obj->energy == 0 || obj->stunned != 0) {
        return;
    }
    func_151AE2BC(&obj->mv->ropeProgress, rope, obj->xpos, obj->ypos, obj->zpos);
    endNodes = 140.0f / rope->segmentLength;
    i = (rope->nodeCount - endNodes - 1) * obj->mv->ropeProgress;
    pos[0] = rope->nodes[i].pos[0];
    pos[1] = rope->nodes[i].pos[1];
    pos[2] = rope->nodes[i].pos[2];
    dist = (pos[0] - obj->xpos) * (pos[0] - obj->xpos) +
           (pos[1] - obj->ypos) * (pos[1] - obj->ypos) +
           (pos[2] - obj->zpos) * (pos[2] - obj->zpos);
    if (dist > 10000.0f) {
        return;
    }
    obj->mv->rope = ropeId;
    obj->nojump = 18;
    obj->mv->ropePtr = rope;
    rope->rider = obj;
}

/** Lets go of the current rope, giving it a swing impulse from obj's horizontal speed. */
void func_151AE264(Actor *obj) {
    f32 force = obj->speed * 0.8899999857f;
    Rope *rope = obj->mv->ropePtr;

    rope->releaseForce = force;
    rope->releaseAngle = obj->angle >> 8;
    rope->releaseNode = obj->mv->ropeNode;
    rope->regrabTimer = 20;
    rope->rider = NULL;
    obj->mv->rope = 0;
    obj->mv->ropePtr = NULL;
}

/** Sets *progress to how far up the rope height y is (0.0f bottom, 1.0f top). */
void func_151AE2BC(f32 *progress, Rope *rope, f32 x, f32 y, f32 z) {
    s32 n;
    s32 i;
    f32 frac;

    n = rope->nodeCount;
    for (i = 0; i < n - 1; i++) {
        if (y <= rope->nodes[i].pos[1]) {
            break;
        }
    }
    if (i >= n - 2) {
        *progress = 1.0f;
        return;
    }
    if (i == 0) {
        *progress = 0.0f;
        return;
    }
    {
        RopeNode *node = &rope->nodes[i];

        frac = node->pos[1] - node[-1].pos[1];
        frac = (frac != 0.0f) ? (y - node[-1].pos[1]) / frac : 0.0f;
    }
    *progress = (f32)(i - 1) / (n - 3) + frac / (n - 1);
}

/** Places the rider's owner on its rope at its progress, offset by radius in its facing direction. */
// matching: declared s32 with no return value; void changes the register allocation
s32 func_151AE3A8(RopeRider *rider) {
    f32 oz;
    f32 oy;
    f32 ox;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 radius;
    f32 progress;
    ActorCtl *ride;
    Actor *obj;
    Rope *rope;
    u8 angle;
    f32 dirX;
    f32 dirZ;
    s32 endNodes;
    s32 i;
    f32 t;
    s32 pad; // matching: unused stack slot
    f32 a[3];
    f32 b[3];

    obj = rider->owner;
    rope = obj->mv->ropePtr;
    angle = func_15143E08(obj) - 0x80;
    dirX = func_151423D8(angle - 0x40);
    dirZ = func_151423D8(angle);
    ride = obj->mv;
    progress = ride->ropeProgress;
    radius = rider->radius;
    ox = obj->xpos;
    oy = obj->ypos;
    oz = obj->zpos;
    endNodes = 140.0f / rope->segmentLength;
    {
        s32 pad3; // matching: block scope shifts IDO's lbu scheduling

        i = (rope->nodeCount - endNodes - 1) * progress;
    }
    ride->ropeNode = i;
    t = (rope->nodeCount - endNodes - 1) * progress - i;
    a[0] = rope->nodes[i].pos[0];
    a[1] = rope->nodes[i].pos[1];
    a[2] = rope->nodes[i].pos[2];
    b[0] = rope->nodes[i + 1].pos[0];
    b[1] = rope->nodes[i + 1].pos[1];
    b[2] = rope->nodes[i + 1].pos[2];
    {
        s32 pad2; // matching: block scope fixes IDO's float scheduling

        nx = a[0] + (b[0] - a[0]) * t;
        ny = a[1] + (b[1] - a[1]) * t;
        nz = a[2] + (b[2] - a[2]) * t;
        nx += radius * dirX;
        nz += radius * dirZ;
    }
    // matching: "(new - old) + old" rather than a plain store of the new position
    obj->xpos = (nx - ox) + ox;
    obj->ypos = (ny - oy) + oy;
    obj->zpos = (nz - oz) + oz;
}

/** Finds the rope with the given id in either rope object list; NULL if there is none. */
Rope *func_151AE590(u8 id) {
    u8 i;
    u8 j;
    Rope *rope;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            rope = (Rope *)D_800DCE50[j][D_800A9270[i]];
            while (rope != NULL) {
                Rope *next = (Rope *)rope->header.next;

                if (rope->id == id) {
                    return rope;
                }
                rope = next;
            }
        }
    }
    return NULL;
}

/** Rope message handler: forgets a removed rider (0), or follows a rider swap (0x2D: {old, new}). */
void func_151AE640(Rope *rope, Actor **objs, u8 msg) {
    Actor *first;

    if (msg == 0) {
        first = objs[0];
        if (first == rope->rider) {
            rope->rider = NULL;
        }
    keep_block: // matching: a label here keeps as1 from moving the store into the jr delay slot
        ;
    } else if (msg == 0x2D) {
        first = objs[0];
        if (first == rope->rider) {
            rope->rider = objs[1];
        // matching: indexing rope as an array (word 0x11 is rider) gives IDO's operand order here
        } else if (objs[1] == ((Actor **)rope)[0x11]) {
            rope->rider = first;
        }
    }
}
