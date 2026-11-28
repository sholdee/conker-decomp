#include <ultra64.h>
#include <math.h>
#include "conker/beam.h"
#include "conker/collision.h"
#include "conker/effect.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/math.h"
#include "conker/particle.h"
#include "conker/texture.h"
#include "conker/weapon.h"



/* Per-material impact effects, indexed by the material id from collision_getSurfaceType. */
typedef struct Struct800A1070 {
    /* 0x00 */ u16 objectType;   // object spawned through effect_spawnQuadFx (0 = none)
    /* 0x02 */ u16 particleType; // particle burst type (0 = none)
    /* 0x04 */ u8 chance;        // percent chance of the weapon_spawnImpactBurst effect
    /* 0x05 */ u8 debrisType;    // effect_sprayDebris debris kind (0 = none)
    /* 0x06 */ u8 minCount;      // particle count = minCount + 1 + rand % countRange
    /* 0x07 */ u8 countRange;
} Struct800A1070;

static const Struct800A1070 gMaterialImpactDefs[] = {
    { 0, 0, 50, 0, 17, 40 },
    { 58, 59, 2, 10, 30, 12 },
    { 62, 0, 0, 10, 60, 20 },
    { 0, 63, 50, 4, 30, 12 },
    { 0, 63, 50, 4, 30, 12 },
    { 0, 63, 50, 5, 17, 40 },
    { 0, 63, 50, 9, 30, 12 },
    { 55, 0, 2, 0, 30, 12 },
    { 62, 19, 0, 8, 60, 20 },
    { 0, 63, 100, 0, 20, 6 },
    { 58, 59, 2, 10, 30, 12 },
    { 58, 59, 2, 10, 30, 12 },
    { 58, 0, 0, 10, 30, 12 },
    { 0, 0, 0, 0, 0, 0 },
    { 55, 0, 0, 17, 30, 12 },
    { 55, 56, 1, 17, 30, 12 },
    { 55, 56, 1, 17, 30, 12 },
    { 55, 56, 1, 17, 30, 12 },
    { 55, 56, 0, 17, 30, 12 },
    { 0, 0, 0, 0, 0, 0 },
};

/* Particle scale pair per `size`. */
static const u16 gMaterialImpactScales[][2] = { { 128, 96 }, { 256, 256 }, { 384, 384 } };

/* Particle count multiplier per `size`. */
static const f32 gMaterialImpactCountScales[] = { 0.33f, 1.0f, 2.0f };

/*
 * Spawns the impact effects of `material` at (x, y, z): a particle burst, debris (effect_sprayDebris),
 * a random weapon_spawnImpactBurst effect and an object. `size` indexes gMaterialImpactCountScales/gMaterialImpactScales, `tri` is the
 * hit triangle (three vertex pointers) or NULL, and `slot` indexes gAimImpacts (0xFF = none).
 * Bit 0x8000 of `material` is forwarded to effect_sprayDebris.
 */
void effect_spawnMaterialImpact(f32 x, f32 y, f32 z, s32 size, s32 material, s16 **tri, s32 slot) {
    const Struct800A1070 *entry;
    s32 lifetime;
    s32 flag;
    s32 count;
    QuadFxData obj;

    lifetime = 120;
    flag = 0;
    if (material & 0x8000) {
        material ^= 0x8000;
        flag = 0x8000;
    }
    entry = &gMaterialImpactDefs[material];
    if (entry->particleType != 0) {
        count = math_rand() % entry->countRange + entry->minCount + 1;
        if (size != 1) {
            lifetime = 30;
        }
        count = gMaterialImpactCountScales[size] * count;
        if (entry->particleType == 0x13) {
            particle_spawnFloorSprite(x, y, z, 30, 0, 0x13, 0.0f, 0, count, 0xF, 0x55, 0, 0xFF, 0);
        } else if (tri != NULL) {
            particle_spawnTriSprite(x, y, z, tri, lifetime, 0xB5AD, entry->particleType, count, 0, 0, 0, 0xFF, 0);
        } else {
            particle_spawnFloorSprite(x, y, z, lifetime, 0xB5AD, entry->particleType, 0.0f, 0, count, 0, 0, 0, 0xFF, 0);
        }
    }
    if (entry->debrisType != 0) {
        effect_sprayDebris(x, y, z, entry->debrisType | flag, size, tri, slot);
    }
    if (size != 0 && math_rand() % 100 < entry->chance) {
        {
            Vec3f pos;
            s32 pad; // matching: keeps pos at its original stack offset

            pos.x = x;
            pos.y = y + 20.0f;
            pos.z = z;
            weapon_spawnImpactBurst(&pos, 0xFF, 1);
        }
        if (slot != 0xFF) {
            gAimImpacts[slot].ricochetChance = 100;
        }
    } else if (slot != 0xFF) {
        gAimImpacts[slot].ricochetChance = 0;
    }
    if (entry->objectType != 0) {
        bzero(&obj, sizeof(obj));
        obj.x = x;
        obj.y = y;
        obj.z = z;
        obj.callback = 0xD;
        obj.animIndex = entry->objectType;
        obj.slot = gMaterialImpactScales[size][0];
        obj.unk86 = gMaterialImpactScales[size][1];
        obj.flags = 0x2A00;
        obj.halfWidth = 1;
        obj.halfHeight = 1;
        obj.alpha = 0xFF;
        effect_initQuadFxTemplate(&obj, 0xB4, 0xB4, 0xB4, 0xFF);
        effect_spawnQuadFx(&obj, 0xFF, 0);
    }
}

/*
 * Probes the ground below `obj` (up to 5 random points within its radius), then spawns the hit
 * surface's impact effects there, passing the hit triangle transformed by obj's matrix.
 */
void effect_spawnGroundImpactsUnder(WorldObject *obj) {
    u16 idx;
    s32 i;
    s32 found;
    s32 tries;
    s16 *verts[3];
    s32 *tri;
    s32 x;
    s32 best;
    s32 z;
    f32 height;
    s32 material;
    Vtx vtx[3];
    f32 outx;
    f32 outy;
    f32 outz;
    MtxF mtx;

    height = -10000.0f;
    i = obj - db_Object_array;
    tries = 5;
    if (i < gWorldObjectCount && i >= 0) {
        idx = i;
        found = 0;
        collision_selectMap(2);
        collision_transformPropTris(1, &idx, 0);
        while (tries != 0) {
            x = obj->x;
            z = obj->z;
            if (obj->cullRadiusXZ != 0) {
                x += (math_rand() % obj->cullRadiusXZ) * 2 - obj->cullRadiusXZ;
                z += (math_rand() % obj->cullRadiusXZ) * 2 - obj->cullRadiusXZ;
            }
            found = collision_gatherPropGroundTris(x, z, 1, &idx);
            if (found) {
                best = -0x800000;
                while (found--) {
                    if (best < gGroundCandidates[found].height) {
                        best = gGroundCandidates[found].height;
                        tri = gGroundCandidates[found].tri;
                        height = best / 256.0f;
                    }
                }
                tries = 0;
                found = 1;
            } else {
                tries--;
            }
        }
        if (found) {
            if (obj->materials != NULL) {
                i = ((s32)tri - (s32)gCurMapTris) / 12;
                material = obj->materials[i - obj->firstTri];
            } else {
                material = obj->floorHitAttrib;
            }
            gfx_getObjectModelMtx(mtx, obj);
            if (tri != NULL) {
                // matching: IDO keeps the sum in one register only when built up in i
                i = tri[0];
                i += (s32)obj->vertices[gDrawBufferIndex];
                verts[0] = (s16 *)i;
                i = tri[1];
                i += (s32)obj->vertices[gDrawBufferIndex];
                verts[1] = (s16 *)i;
                i = tri[2];
                i += (s32)obj->vertices[gDrawBufferIndex];
                verts[2] = (s16 *)i;
                for (i = 0; i < 3; i++) {
                    math_transformPoint(mtx, verts[i][0], verts[i][1], verts[i][2], &outx, &outy, &outz);
                    vtx[i].v.ob[0] = outx;
                    vtx[i].v.ob[1] = outy;
                    vtx[i].v.ob[2] = outz;
                    verts[i] = (s16 *)&vtx[i];
                }
                tri = (s32 *)verts;
            }
            effect_spawnMaterialImpact(x, height, z, 1, collision_getSurfaceType(material), (s16 **)tri, 0);
        }
    }
}

struct Struct150E4CBC {
    /* 0x00 */ EffectHeader link;
    /* 0x10 */ Vtx vtx[8];
    /* 0x90 */ s32 timer;
    /* 0x94 */ s16 widthScale;  // halfWidth = widthScale * 14 / 256 (gMaterialImpactScales[size][0])
    /* 0x96 */ s16 heightScale; // halfHeight ramps up to 90/256 of it and back (gMaterialImpactScales[size][1])
    /* 0x98 */ s16 frame;       // QuadFxData.frame; set to -1 when the effect is finished
    /* 0x9A */ u8 pad9A[8];
    /* 0xA2 */ s16 halfWidth;   // QuadFxData.halfWidth
    /* 0xA4 */ s16 halfHeight;
    /* 0xA6 */ u8 padA6[0xE];
    /* 0xB4 */ u8 alpha;
};

/*
 * Per-frame update (gParticleCallbacks[13]): halfHeight ramps up to 90/256 of heightScale over the first 9 ticks
 * and back down afterwards while alpha fades by 17 per tick; finished at alpha 0 or after 608 ticks.
 */
void func_150E4CBC(Struct150E4CBC *fx) {
    if (fx->timer < 9) {
        fx->halfHeight = fx->timer * 90 / 9 * fx->heightScale / 256.0f;
    } else {
        fx->halfHeight = (90 - (fx->timer * 90 - 810) / 600) * fx->heightScale / 256.0f;
        if (fx->alpha > gFrameTicks * 17) {
            fx->alpha -= gFrameTicks * 17;
        } else {
            fx->frame = -1;
        }
    }
    fx->halfWidth = (s32)(fx->widthScale * 14.0f) >> 8;
    if (fx->timer > 608) {
        fx->frame = -1;
    }
    fx->timer += gFrameTicks;
}

/* Computes the plane of a triangle; the (unnormalised) normal lands in gTriPlaneNormalX..gTriPlaneNormalZ. */

/*
 * Sprays debris particles of `kind` from (x, y, z). With a hit triangle `tri` the spray follows the
 * surface normal (also stored in gAimImpacts[slot]); otherwise it goes straight up. Bit 0x8000 of
 * `kind` suppresses the particles. Kinds 4, 5 and 9 sometimes add a effect_launchDebrisChunk chunk.
 */
void effect_sprayDebris(f32 x, f32 y, f32 z, s32 kind, s32 size, s16 **tri, s32 slot) {
    f32 minScale;
    f32 rangeScale;
    s32 ok;
    s32 count;
    s32 noFx;
    s32 noSurface;
    s32 big;
    s32 i;
    Tri3f plane;
    f32 dirX;
    f32 dirY;
    f32 dirZ;
    f32 len;
    f32 baseSpeed;
    MtxF mtx;
    BillboardTemplate spawn;
    f32 speed;
    u8 angle;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 spread;

    ok = 1;
    count = 1;
    noSurface = 0;
    big = 0;
    noFx = 0;
    baseSpeed = 815.0f;
    bzero(&spawn, sizeof(spawn));
    if (kind & 0x8000) {
        noFx = 1;
        kind ^= 0x8000;
    }
    if (tri != NULL) {
        for (i = 0; i < 3; i++) {
            plane.pts[i].x = tri[i][0];
            plane.pts[i].y = tri[i][1];
            plane.pts[i].z = tri[i][2];
        }
        math_computeTriPlane(plane);
        dirX = -gTriPlaneNormalX;
        dirY = -gTriPlaneNormalY;
        dirZ = -gTriPlaneNormalZ;
        len = sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
        if (len < 9.999999747e-05f) {
            return;
        }
        dirX /= len;
        dirY /= len;
        dirZ /= len;
        if (slot != 0xFF && fabsf(dirY) < 0.8999999762f) {
            math_mtxLookAt(mtx, 0.0f, 0.0f, 0.0f, dirX, dirY, dirZ, 0.0f, 1.0f, 0.0f);
            math_transformPoint(mtx, 0.0f, 0.0f, 1.0f, &gAimImpacts[slot].normal[0], &gAimImpacts[slot].normal[1], &gAimImpacts[slot].normal[2]);
        } else {
            noSurface = 1;
        }
    } else {
        noSurface = 1;
    }
    if (slot != 0xFF && noSurface) {
        gAimImpacts[slot].ricochetChance = 0;
    }
    if (!noFx) {
        spawn.x = x;
        spawn.y = y;
        spawn.z = z;
        spawn.primR = 180;
        spawn.primG = 180;
        spawn.primB = 180;
        spawn.primA = 255;
        spawn.callback = -1;
        spawn.flags = 0;
        spawn.frame = 0;
        spawn.frameStep = 0;
        minScale = gMaterialImpactScales[size][1] * 95.0f / 256.0f;
        rangeScale = gMaterialImpactScales[size][1] * 130.0f / 256.0f;
        if (kind == 0x11) {
            spawn.anim = &D_8009187C;
        } else if (kind == 0xA) {
            spawn.anim = &D_800918A0;
        } else if (kind == 4 || kind == 9) {
            spawn.anim = &D_800918A0;
        } else if (kind == 8) {
            spawn.anim = &D_80090520;
            minScale = 180.0f;
            rangeScale = 200.0f;
        } else {
            spawn.anim = &D_800918AC;
        }
        if (size == 2) {
            big = 1;
            spawn.accelY = -373;
            count = 12;
            baseSpeed *= 0.5f * (math_rand() % 3 + 1);
        } else if (size == 0) {
            spawn.accelY = -218;
            count = 12;
            baseSpeed *= 0.5f;
        } else {
            spawn.accelY = -218;
        }
        while (count-- && ok) {
            speed = math_randomFloat() * 957.0f + baseSpeed;
            angle = math_rand() % 256;
            if (big) {
                speed += speed;
            }
            if (noSurface) {
                dirX = math_sinBinAngle(angle);
                dirY = 1.0f;
                dirZ = math_cosBinAngle(angle);
            } else {
                spread = math_sinBinAngle(angle);
                math_transformPoint(mtx, spread * 0.5f, math_cosBinAngle(angle) * 0.5f, 1.0f, &dirX, &dirY, &dirZ);
            }
            vx = 0.3149999976f * dirX;
            vy = dirY;
            vz = 0.3149999976f * dirZ;
            spawn.life = math_rand() % 2 + 386;
            spawn.velX = speed * vx;
            spawn.velY = speed * vy;
            spawn.velZ = speed * vz;
            math_rand();
            spawn.fadeStep = 10;
            spawn.width = math_randomFloat() * rangeScale + minScale;
            spawn.height = math_randomFloat() * rangeScale + minScale;
            if (!effect_spawnBillboard(&spawn, 0, 0, -1, 0xFF, 0)) {
                ok = 0;
            }
        }
        if ((kind == 4 || kind == 9 || kind == 5) && math_rand() % 5 == 0) {
            effect_launchDebrisChunk(x, y, z, dirX, dirY, dirZ, kind, size);
        }
    }
}

/* Launches a chunk of debris `kind` from (x, y, z), flying along (dirX, -dirY, dirZ) and oriented to match. */
void effect_launchDebrisChunk(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 kind, s32 size) {
    ParticleSpawn chunk;
    f32 pitch;
    f32 yaw;
    f32 speed;
    f32 rand;

    bzero(&chunk, sizeof(chunk));

    if (dirY != 0.0f) {
        pitch = math_atan2(dirX, dirY) * 57.29577637f; // radians to degrees
    } else {
        pitch = 90.0f;
    }

    if (dirZ != 0.0f) {
        yaw = math_atan2(dirX, dirZ) * 57.29577637f;
    } else {
        yaw = 90.0f;
    }

    chunk.rot.y = yaw;
    chunk.rot.z = pitch;
    chunk.scale.x = 1.0f;
    chunk.scale.y = 1.0f;
    chunk.scale.z = 1.0f;
    chunk.radius = 1.0f;
    chunk.sizeXZ = 0.1700000018f;
    chunk.sizeY = 0.1700000018f;
    chunk.pos.x = x;
    chunk.pos.y = y;
    chunk.pos.z = z;
    chunk.rot.x = 0.0f;

    if (kind == 4) {
        chunk.model = 6;
        chunk.elasticity = 0.6000000238f;
    } else if (kind == 9) {
        chunk.model = 8;
        chunk.elasticity = 0.3000000119f;
    } else {
        chunk.model = 7;
        chunk.elasticity = 0.3000000119f;
    }

    chunk.lifetime = math_rand() % 60 + 60;
    chunk.flags = 0x29E9;

    rand = math_randomFloat();
    chunk.gravity = -0.7799999714f;
    speed = rand * 4.599999905f + 1.5f;
    chunk.vel.x = speed * dirX;
    chunk.vel.y = -speed * dirY;
    chunk.vel.z = speed * dirZ;

    chunk.rotVel.x = math_randomFloat() * 6.0f + -3.0f;
    chunk.rotVel.z = math_randomFloat() * 6.0f + -3.0f;
    rand = math_randomFloat() * 6.0f + -3.0f;
    chunk.variant = 0;
    chunk.zone = 0;
    chunk.rotVel.y = rand;
    chunk.alpha = 0xFF;
    chunk.updateFn = 1;
    chunk.update2Fn = 0;
    chunk.floorHitFn = 3;
    chunk.floorType3Fn = 0;
    chunk.wallHitFn = 0;
    chunk.destroyFn = 0;
    chunk.squashFn = 0;
    chunk.matrixFn = 0;
    chunk.lightMode = 2;
    chunk.owner = 0;
    chunk.ownerId = 0;
    chunk.fadeStart = 0x20;
    chunk.fadeRate = 7;

    particle_newModelParticle(&chunk, 3, 0xFF, 0, 0xFF, 0);
}

/*
 * Floor-hit callback of the model particles (gModelParticleFloorFuncs[3]). A slow fall comes to rest radius above the
 * ground; otherwise the velocity is reflected off the hit triangle `verts` (or just flipped vertically without one) and
 * damped by elasticity.
 */
s32 effect_bounceOffTriangle(ModelParticle *obj, f32 x, f32 y, f32 z, f32 groundY, s16 (*verts)[3]) {
    Tri3f tri;
    s32 i;
    f32 len;
    f32 pad; // matching: unused stack slot
    f32 dot;
    Vec3f vel;
    Vec3f normal;
    Vec3f unit;

    obj->pos.y = obj->radius * 1.100000024f + groundY;
    if (obj->vel.y > -2.0f) {
        obj->vel.x = 0.0f;
        obj->flags &= ~0x6F;
        obj->vel.y = 0.0f;
        obj->vel.z = 0.0f;
    } else {
        if (verts != NULL) {
        lbl: // matching: a block boundary that stops IDO hoisting the loop setup above the NULL test
            for (i = 0; i < 3; i++) {
                tri.pts[i].x = verts[i][0];
                tri.pts[i].y = verts[i][1];
                tri.pts[i].z = verts[i][2];
            }
            math_computeTriPlane(tri);
            normal.x = -gTriPlaneNormalX;
            normal.y = -gTriPlaneNormalY;
            normal.z = -gTriPlaneNormalZ;
            len = math_length3(&normal.x);
            if (len > 9.999999747e-06f) {
                math_vec3Scale(&normal, 1.0f / len, &unit);
                vel.x = obj->vel.x;
                vel.y = obj->vel.y;
                vel.z = obj->vel.z;
                dot = math_dot3(&unit.x, &vel.x);
                math_vec3Scale(&unit, 2.0f * dot, &normal);
                math_vec3Sub(&vel, &normal, &normal);
                obj->vel.x = obj->elasticity * normal.x * 0.8000000119f;
                obj->vel.y = obj->elasticity * normal.y;
                obj->vel.z = obj->elasticity * normal.z * 0.8000000119f;
            }
        } else {
            obj->vel.x *= obj->elasticity;
            obj->vel.y = -obj->vel.y * obj->elasticity;
            obj->vel.z *= obj->elasticity;
        }
        obj->rotVel.x *= obj->elasticity;
        obj->rotVel.y *= obj->elasticity;
        obj->rotVel.z *= obj->elasticity;
    }
    return 1;
}
