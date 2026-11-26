#include <ultra64.h>
#define func_15171F04 func_15171F04_header // matching: this file keeps its own declaration of func_15171F04
#include "functions.h"
#undef func_15171F04
#include "variables.h"



/* Per-material impact effects, indexed by the material id from func_1510F8CC. */
struct Struct800A1070 {
    /* 0x00 */ u16 objectType;   // object spawned through func_15168800 (0 = none)
    /* 0x02 */ u16 particleType; // particle burst type (0 = none)
    /* 0x04 */ u8 chance;        // percent chance of the func_151C329C effect
    /* 0x05 */ u8 debrisType;    // func_150E4E04 debris kind (0 = none)
    /* 0x06 */ u8 minCount;      // particle count = minCount + 1 + rand % countRange
    /* 0x07 */ u8 countRange;
};

typedef struct Struct150E4550 {
    /* 0x00 */ u8 pad0[0x84];
    /* 0x84 */ u16 unk84;
    /* 0x86 */ u16 unk86;
    /* 0x88 */ u8 pad88[4];
    /* 0x8C */ s16 x;
    /* 0x8E */ s16 y;
    /* 0x90 */ s16 z;
    /* 0x92 */ s16 unk92;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ u8 pad96[2];
    /* 0x98 */ s16 unk98;
    /* 0x9A */ u8 pad9A[6];
    /* 0xA0 */ u8 unkA0;
    /* 0xA1 */ u8 padA1;
    /* 0xA2 */ u8 unkA2;
    /* 0xA3 */ u8 padA3;
    /* 0xA4 */ u8 unkA4;
    /* 0xA5 */ u8 padA5[3];
} Struct150E4550;

const Struct800A1070 D_800A1070[] = {
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
const u16 D_800A1110[][2] = { { 128, 96 }, { 256, 256 }, { 384, 384 } };

/* Particle count multiplier per `size`. */
const f32 D_800A111C[] = { 0.33f, 1.0f, 2.0f };
/* Defined in game_19F150.c as void func_15171D4C(f32 x, f32 y, f32 z, s16 lifetime, u16 color, u8 animIndex,
   f32 angle, s32 dist, s32 size, s32 flags, s32 animSpeed, s32 arg11, u8 arg12, s32 arg13). This file was
   compiled with a view that takes s32 for the narrow parameters. */
void func_15171D4C(f32 x, f32 y, f32 z, s32 lifetime, s32 color, s32 animIndex, f32 angle, s32 dist, s32 size,
                   s32 flags, s32 animSpeed, s32 arg11, s32 arg12, s32 arg13);
// matching: a pointer where the definition has s32 (arg 4); s32 where the definition has s16 (arg 5)
extern void func_15171F04(f32, f32, f32, s16 **, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/*
 * Spawns the impact effects of `material` at (x, y, z): a particle burst, debris (func_150E4E04),
 * a random func_151C329C effect and an object. `size` indexes D_800A111C/D_800A1110, `tri` is the
 * hit triangle (three vertex pointers) or NULL, and `slot` indexes D_800D9960 (0xFF = none).
 * Bit 0x8000 of `material` is forwarded to func_150E4E04.
 */
void func_150E4550(f32 x, f32 y, f32 z, s32 size, s32 material, s16 **tri, s32 slot) {
    Struct800A1070 *entry;
    s32 lifetime;
    s32 flag;
    s32 count;
    Struct150E4550 obj;

    lifetime = 120;
    flag = 0;
    if (material & 0x8000) {
        material ^= 0x8000;
        flag = 0x8000;
    }
    entry = &D_800A1070[material];
    if (entry->particleType != 0) {
        count = func_151EF610() % entry->countRange + entry->minCount + 1;
        if (size != 1) {
            lifetime = 30;
        }
        count = D_800A111C[size] * count;
        if (entry->particleType == 0x13) {
            func_15171D4C(x, y, z, 30, 0, 0x13, 0.0f, 0, count, 0xF, 0x55, 0, 0xFF, 0);
        } else if (tri != NULL) {
            func_15171F04(x, y, z, tri, lifetime, 0xB5AD, entry->particleType, count, 0, 0, 0, 0xFF, 0);
        } else {
            func_15171D4C(x, y, z, (s16)lifetime, 0xB5AD, entry->particleType, 0.0f, 0, count, 0, 0, 0, 0xFF, 0);
        }
    }
    if (entry->debrisType != 0) {
        func_150E4E04(x, y, z, entry->debrisType | flag, size, tri, slot);
    }
    if (size != 0 && func_151EF610() % 100 < entry->chance) {
        {
            f32 pos[3];
            s32 pad; // matching: keeps pos at its original stack offset

            pos[0] = x;
            pos[1] = y + 20.0f;
            pos[2] = z;
            func_151C329C(pos, 0xFF, 1);
        }
        if (slot != 0xFF) {
            D_800D9960[slot].ricochetChance = 100;
        }
    } else if (slot != 0xFF) {
        D_800D9960[slot].ricochetChance = 0;
    }
    if (entry->objectType != 0) {
        bzero(&obj, sizeof(obj));
        obj.x = x;
        obj.y = y;
        obj.z = z;
        obj.unkA2 = 0xD;
        obj.unkA0 = entry->objectType;
        obj.unk84 = D_800A1110[size][0];
        obj.unk86 = D_800A1110[size][1];
        obj.unk98 = 0x2A00;
        obj.unk92 = 1;
        obj.unk94 = 1;
        obj.unkA4 = 0xFF;
        func_1516865C(&obj, 0xB4, 0xB4, 0xB4, 0xFF);
        func_15168800(&obj, 0xFF, 0);
    }
}

/*
 * Probes the ground below `obj` (up to 5 random points within its radius), then spawns the hit
 * surface's impact effects there, passing the hit triangle transformed by obj's matrix.
 */
void func_150E4928(WorldObject *obj) {
    s16 idx;
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
        func_1510F800(2);
        func_150A44F0(1, &idx, 0);
        while (tries != 0) {
            x = obj->x;
            z = obj->z;
            if (obj->cullRadiusXZ != 0) {
                x += (func_151EF610() % obj->cullRadiusXZ) * 2 - obj->cullRadiusXZ;
                z += (func_151EF610() % obj->cullRadiusXZ) * 2 - obj->cullRadiusXZ;
            }
            found = func_150A43E0(x, z, 1, &idx);
            if (found) {
                best = -0x800000;
                while (found--) {
                    if (best < D_800D3300[found].height) {
                        best = D_800D3300[found].height;
                        tri = D_800D3300[found].tri;
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
                i = ((s32)tri - (s32)D_800DBE3C) / 12;
                material = obj->materials[i - obj->firstTri];
            } else {
                material = obj->floorHitAttrib;
            }
            func_1511490C(mtx, obj);
            if (tri != NULL) {
                // matching: IDO keeps the sum in one register only when built up in i
                i = tri[0];
                i += (s32)obj->vertices[D_800BE9C0];
                verts[0] = (s16 *)i;
                i = tri[1];
                i += (s32)obj->vertices[D_800BE9C0];
                verts[1] = (s16 *)i;
                i = tri[2];
                i += (s32)obj->vertices[D_800BE9C0];
                verts[2] = (s16 *)i;
                for (i = 0; i < 3; i++) {
                    func_150A7960(mtx, verts[i][0], verts[i][1], verts[i][2], &outx, &outy, &outz);
                    vtx[i].v.ob[0] = outx;
                    vtx[i].v.ob[1] = outy;
                    vtx[i].v.ob[2] = outz;
                    verts[i] = (s16 *)&vtx[i];
                }
                tri = (s32 *)verts;
            }
            func_150E4550(x, height, z, 1, func_1510F8CC(material), (s16 **)tri, 0);
        }
    }
}

struct Struct150E4CBC {
    /* 0x00 */ u8 pad0[0x90];
    /* 0x90 */ s32 timer;
    /* 0x94 */ s16 unk94;
    /* 0x96 */ s16 unk96;
    /* 0x98 */ s16 unk98; // set to -1 when the effect is finished
    /* 0x9A */ u8 pad9A[8];
    /* 0xA2 */ s16 unkA2;
    /* 0xA4 */ s16 unkA4;
    /* 0xA6 */ u8 padA6[0xE];
    /* 0xB4 */ u8 alpha;
};

/*
 * Per-frame update (from a handler table): unkA4 ramps up to 90/256 of unk96 over the first 9 ticks
 * and back down afterwards while alpha fades by 17 per tick; finished at alpha 0 or after 608 ticks.
 */
void func_150E4CBC(Struct150E4CBC *fx) {
    if (fx->timer < 9) {
        fx->unkA4 = (f32)(fx->timer * 90 / 9 * fx->unk96) / 256.0f;
    } else {
        fx->unkA4 = (f32)((90 - (fx->timer * 90 - 810) / 600) * fx->unk96) / 256.0f;
        if (fx->alpha > D_800BE9E4 * 17) {
            fx->alpha -= D_800BE9E4 * 17;
        } else {
            fx->unk98 = -1;
        }
    }
    fx->unkA2 = (s32)(fx->unk94 * 14.0f) >> 8;
    if (fx->timer > 608) {
        fx->unk98 = -1;
    }
    fx->timer += D_800BE9E4;
}

/* Computes the plane of a triangle; the (unnormalised) normal lands in D_800CC210..D_800CC218. */

extern u8 D_800918AC[];
extern u8 D_80090520[];

/*
 * Sprays debris particles of `kind` from (x, y, z). With a hit triangle `tri` the spray follows the
 * surface normal (also stored in D_800D9960[slot]); otherwise it goes straight up. Bit 0x8000 of
 * `kind` suppresses the particles. Kinds 4, 5 and 9 sometimes add a func_150E5558 chunk.
 */
void func_150E4E04(f32 x, f32 y, f32 z, s32 kind, s32 size, s16 **tri, s32 slot) {
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
        func_15049350(plane);
        dirX = -D_800CC210;
        dirY = -D_800CC214;
        dirZ = -D_800CC218;
        len = sqrtf(dirX * dirX + dirY * dirY + dirZ * dirZ);
        if (len < 9.999999747e-05f) {
            return;
        }
        dirX /= len;
        dirY /= len;
        dirZ /= len;
        if (slot != 0xFF && fabsf(dirY) < 0.8999999762f) {
            func_150440A0(mtx, 0.0f, 0.0f, 0.0f, dirX, dirY, dirZ, 0.0f, 1.0f, 0.0f);
            func_150A7960(mtx, 0.0f, 0.0f, 1.0f, &D_800D9960[slot].normal[0], &D_800D9960[slot].normal[1], &D_800D9960[slot].normal[2]);
        } else {
            noSurface = 1;
        }
    } else {
        noSurface = 1;
    }
    if (slot != 0xFF && noSurface) {
        D_800D9960[slot].ricochetChance = 0;
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
        minScale = D_800A1110[size][1] * 95.0f / 256.0f;
        rangeScale = D_800A1110[size][1] * 130.0f / 256.0f;
        if (kind == 0x11) {
            spawn.anim = D_8009187C;
        } else if (kind == 0xA) {
            spawn.anim = D_800918A0;
        } else if (kind == 4 || kind == 9) {
            spawn.anim = D_800918A0;
        } else if (kind == 8) {
            spawn.anim = D_80090520;
            minScale = 180.0f;
            rangeScale = 200.0f;
        } else {
            spawn.anim = D_800918AC;
        }
        if (size == 2) {
            big = 1;
            spawn.accelY = -373;
            count = 12;
            baseSpeed *= 0.5f * (func_151EF610() % 3 + 1);
        } else if (size == 0) {
            spawn.accelY = -218;
            count = 12;
            baseSpeed *= 0.5f;
        } else {
            spawn.accelY = -218;
        }
        while (count-- && ok) {
            speed = func_150ADA68() * 957.0f + baseSpeed;
            angle = func_151EF610() % 256;
            if (big) {
                speed += speed;
            }
            if (noSurface) {
                dirX = func_15048A40(angle);
                dirY = 1.0f;
                dirZ = func_150489B0(angle);
            } else {
                spread = func_15048A40(angle);
                func_150A7960(mtx, spread * 0.5f, func_150489B0(angle) * 0.5f, 1.0f, &dirX, &dirY, &dirZ);
            }
            vx = 0.3149999976f * dirX;
            vy = dirY;
            vz = 0.3149999976f * dirZ;
            spawn.life = func_151EF610() % 2 + 386;
            spawn.velX = speed * vx;
            spawn.velY = speed * vy;
            spawn.velZ = speed * vz;
            func_151EF610();
            spawn.fadeStep = 10;
            spawn.width = func_150ADA68() * rangeScale + minScale;
            spawn.height = func_150ADA68() * rangeScale + minScale;
            if (!func_15167D84(&spawn, 0, 0, -1, 0xFF, 0)) {
                ok = 0;
            }
        }
        if ((kind == 4 || kind == 9 || kind == 5) && func_151EF610() % 5 == 0) {
            func_150E5558(x, y, z, dirX, dirY, dirZ, kind, size);
        }
    }
}

/* Launches a chunk of debris `kind` from (x, y, z), flying along (dirX, -dirY, dirZ) and oriented to match. */
void func_150E5558(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 kind, s32 size) {
    ParticleSpawn chunk;
    f32 pitch;
    f32 yaw;
    f32 speed;
    f32 rand;

    bzero(&chunk, sizeof(chunk));

    if (dirY != 0.0f) {
        pitch = func_150484A0(dirX, dirY) * 57.29577637f; // radians to degrees
    } else {
        pitch = 90.0f;
    }

    if (dirZ != 0.0f) {
        yaw = func_150484A0(dirX, dirZ) * 57.29577637f;
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

    chunk.lifetime = func_151EF610() % 60 + 60;
    chunk.flags = 0x29E9;

    rand = func_150ADA68();
    chunk.gravity = -0.7799999714f;
    speed = rand * 4.599999905f + 1.5f;
    chunk.vel.x = speed * dirX;
    chunk.vel.y = -speed * dirY;
    chunk.vel.z = speed * dirZ;

    chunk.rotVel.x = func_150ADA68() * 6.0f + -3.0f;
    chunk.rotVel.z = func_150ADA68() * 6.0f + -3.0f;
    rand = func_150ADA68() * 6.0f + -3.0f;
    chunk.variant = 0;
    chunk.unk5C = 0;
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
    chunk.unk6A = 2;
    chunk.owner = 0;
    chunk.ownerId = 0;
    chunk.fadeStart = 0x20;
    chunk.fadeRate = 7;

    func_15132A4C(&chunk, 3, 0xFF, 0, 0xFF, 0);
}

struct Struct150E5810 {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 bounce; // restitution applied on each ground hit
    /* 0x18 */ u8 pad18[0x24];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 velX;
    /* 0x48 */ f32 velY;
    /* 0x4C */ f32 velZ;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 flags;
};

/*
 * Ground-collision callback (from a handler table). A slow fall comes to rest; otherwise the velocity is
 * reflected off the hit triangle `verts` (or just flipped vertically without one) and damped by bounce.
 */
s32 func_150E5810(Struct150E5810 *obj, s32 arg1, s32 arg2, s32 arg3, f32 groundY, s16 (*verts)[3]) {
    Tri3f tri;
    s32 i;
    f32 len;
    f32 pad; // matching: unused stack slot
    f32 dot;
    Vec3f vel;
    Vec3f normal;
    Vec3f unit;

    obj->unk3C = obj->unk10 * 1.100000024f + groundY;
    if (obj->velY > -2.0f) {
        obj->velX = 0.0f;
        obj->flags &= ~0x6F;
        obj->velY = 0.0f;
        obj->velZ = 0.0f;
    } else {
        if (verts != NULL) {
        lbl: // matching: a block boundary that stops IDO hoisting the loop setup above the NULL test
            for (i = 0; i < 3; i++) {
                tri.pts[i].x = verts[i][0];
                tri.pts[i].y = verts[i][1];
                tri.pts[i].z = verts[i][2];
            }
            func_15049350(tri);
            normal.x = -D_800CC210;
            normal.y = -D_800CC214;
            normal.z = -D_800CC218;
            len = func_150AD930(&normal.x);
            if (len > 9.999999747e-06f) {
                func_15049148(&normal, 1.0f / len, &unit);
                vel.x = obj->velX;
                vel.y = obj->velY;
                vel.z = obj->velZ;
                dot = func_150AD900(&unit.x, &vel.x);
                func_15049148(&unit, 2.0f * dot, &normal);
                func_15048F58(&vel, &normal, &normal);
                obj->velX = obj->bounce * normal.x * 0.8000000119f;
                obj->velY = obj->bounce * normal.y;
                obj->velZ = obj->bounce * normal.z * 0.8000000119f;
            }
        } else {
            obj->velX *= obj->bounce;
            obj->velY = -obj->velY * obj->bounce;
            obj->velZ *= obj->bounce;
        }
        obj->unk50 *= obj->bounce;
        obj->unk54 *= obj->bounce;
        obj->unk58 *= obj->bounce;
    }
    return 1;
}
