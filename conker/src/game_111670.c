#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
extern f32 func_15048A40(u8 arg0);
#include "variables.h"

extern s32 func_151EF610(void);

extern f32 D_800A1140;
extern f32 D_800A1144;
extern f32 D_800A1148;
extern f32 D_800A114C;
extern f32 D_800A1150;
extern f32 D_800A1154;
extern f32 D_800A1158;
extern f32 D_800A115C;
extern s32 func_15132A4C(void *, s32, s32, s32, u8, s32);

typedef struct Struct150E5558 {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x59 */ u8 pad59[3];
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6B */ u8 pad6B;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x71 */ u8 pad71;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[6];
} Struct150E5558;


/* Integer world position chosen by func_150E41C0 as the centre of func_150E42F8's scatter. */
typedef struct Struct150E42F8D99F0 {
    s16 x;
    s16 y;
    s16 z;
} Struct150E42F8D99F0;
extern Struct150E42F8D99F0 D_800D99F0;

/* Stores the point 500 units ahead of D_800DBFF0 (along its pitch 0x398 and yaw 0x3A0) in D_800D99F0. */
void func_150E41C0(void) {
    s32 x;
    s32 y;
    s32 z;
    f32 angle;
    f32 dy;
    f32 dz;
    f32 s;
    f32 c;
    f32 dx;

    x = D_800DBFF0->unk2F8;
    y = D_800DBFF0->unk2FC;
    z = D_800DBFF0->unk300;
    angle = D_800DBFF0->unk398;
    s = sinf(angle);
    c = cosf(angle);
    dx = 0.0f;
    dy = 500.0f * s;
    dz = -500.0f * c;
    angle = D_800DBFF0->unk3A0;
    s = sinf(angle);
    c = cosf(angle);
    x += dx * c + dz * s;
    z += dz * c - dx * s;
    y += dy;
    D_800D99F0.x = x;
    D_800D99F0.y = y;
    D_800D99F0.z = z;
}

extern s32 func_1510F8CC(s32);
extern s32 func_1510F8D8(s32, s32, s32, s32 *);
extern void func_150E4550(f32, f32, f32, s32, s32, s16 **, s32);

/* Drops `count` random ground hits within 500 units of D_800D99F0 and spawns each surface's effect. */
void func_150E42F8(s32 count) {
    s32 x;
    s32 y;
    s32 z;
    s32 centerX;
    s32 material;
    s32 centerZ;
    s32 idx;
    s32 i;
    f32 noHit;
    s32 surface;

    surface = 0;
    noHit = -10000.0f;
    centerX = D_800D99F0.x;
    centerZ = D_800D99F0.z;
    for (i = 0; i < count; i++) {
        y = 10000;
        x = func_150ADA20() % 500 + centerX;
        z = func_150ADA20() % 500 + centerZ;
        y = func_1510F8D8(x, y, z, &surface);
        if (noHit != y && surface != 0) {
            idx = (surface - D_800DBE3C) / 12;
            if (idx >= 0 && idx < D_800DBE4C) {
                material = func_1510F8CC(((s32 *)D_800DBE5C)[idx]);
                if (material != 0) {
                    func_150E4550(x, y, z, 0, material, (s16 **)surface, 0xFF);
                }
            }
        }
    }
}

/* Scatters one ground effect per 30 units of `amount` in front of D_800DBFF0. */
void func_150E4514(s32 amount) {
    func_150E41C0();
    func_150E42F8(amount / 30);
}

/* Per-material impact effects, indexed by the material id from func_1510F8CC. */
typedef struct Struct800A1070 {
    /* 0x00 */ u16 objectType;   // object spawned through func_15168800 (0 = none)
    /* 0x02 */ u16 particleType; // particle burst type (0 = none)
    /* 0x04 */ u8 chance;        // percent chance of the func_151C329C effect
    /* 0x05 */ u8 debrisType;    // func_150E4E04 debris kind (0 = none)
    /* 0x06 */ u8 minCount;      // particle count = minCount + 1 + rand % countRange
    /* 0x07 */ u8 countRange;
} Struct800A1070;

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

/* 0x1C-byte impact slots; func_150E4550/func_150E4E04 take an index into this (0xFF = none). */
typedef struct {
    u8 unk0[3];
    u8 unk3;
    u8 pad4[0xC];
    f32 unk10;
    f32 unk14;
    f32 unk18;
} Struct800D9960;

extern Struct800D9960 D_800D9960[];
extern Struct800A1070 D_800A1070[];
extern f32 D_800A111C[];
extern u16 D_800A1110[][2];
void func_150E4E04(f32, f32, f32, s32, s32, s16 **, s32);
extern void func_15171D4C(f32, f32, f32, s32, s32, s32, f32, s32, s32, s32, s32, s32, s32, s32);
extern void func_15171F04(f32, f32, f32, s16 **, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_151C329C(f32 *, s32, s32);
extern void func_1516865C(void *, s32, s32, s32, s32);
extern void func_15168800(void *, s32, s32);

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
            D_800D9960[slot].unk3 = 100;
        }
    } else if (slot != 0xFF) {
        D_800D9960[slot].unk3 = 0;
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

/* Ground-probe hit records written to D_800D3300 by func_150A43E0. */
typedef struct unkfunc_1510F8D8 {
    s32 height; // 24.8 fixed point
    s32 *tri;   // collision triangle: three vertex-buffer offsets
    s32 unk8;
    s32 unkC;
} unkfunc_1510F8D8;

/* 0xA0-byte entries of the D_800DBEF4 array. */
typedef struct Struct150E4928 {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0xA];
    /* 0x20 */ s32 vertexBuffers[8]; // indexed by D_800BE9C0
    /* 0x40 */ s32 material;         // used when materials is NULL
    /* 0x44 */ s32 *materials;       // per-surface materials, from surface firstSurface on
    /* 0x48 */ u8 pad48[8];
    /* 0x50 */ u16 radius;           // random scatter around (x, z)
    /* 0x52 */ u8 pad52[6];
    /* 0x58 */ u16 firstSurface;
    /* 0x5A */ u8 pad5A[0x46];
} Struct150E4928;

extern void func_150A44F0(s32, s16 *, s32);
extern s32 func_150A43E0(s32, s32, s32, s16 *);
extern void func_150A7960(f32 mtx[4][4], f32 x, f32 y, f32 z, f32 *outX, f32 *outY, f32 *outZ);
extern void func_1511490C(f32 arg0[4][4], void *arg1);
extern void func_1510F800(s32);

/*
 * Probes the ground below `obj` (up to 5 random points within its radius), then spawns the hit
 * surface's impact effects there, passing the hit triangle transformed by obj's matrix.
 */
void func_150E4928(Struct150E4928 *obj) {
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
    f32 mtx[4][4];

    height = -10000.0f;
    i = obj - (Struct150E4928 *)D_800DBEF4;
    tries = 5;
    if (i < D_800DBEF0 && i >= 0) {
        idx = i;
        found = 0;
        func_1510F800(2);
        func_150A44F0(1, &idx, 0);
        while (tries != 0) {
            x = obj->x;
            z = obj->z;
            if (obj->radius != 0) {
                x += (func_151EF610() % obj->radius) * 2 - obj->radius;
                z += (func_151EF610() % obj->radius) * 2 - obj->radius;
            }
            found = func_150A43E0(x, z, 1, &idx);
            if (found) {
                best = -0x800000;
                while (found--) {
                    if (best < ((unkfunc_1510F8D8 *)&D_800D3300)[found].height) {
                        best = ((unkfunc_1510F8D8 *)&D_800D3300)[found].height;
                        tri = ((unkfunc_1510F8D8 *)&D_800D3300)[found].tri;
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
                i = ((s32)tri - D_800DBE3C) / 12;
                material = obj->materials[i - obj->firstSurface];
            } else {
                material = obj->material;
            }
            func_1511490C(mtx, obj);
            if (tri != NULL) {
                // matching: IDO keeps the sum in one register only when built up in i
                i = tri[0];
                i += obj->vertexBuffers[D_800BE9C0];
                verts[0] = (s16 *)i;
                i = tri[1];
                i += obj->vertexBuffers[D_800BE9C0];
                verts[1] = (s16 *)i;
                i = tri[2];
                i += obj->vertexBuffers[D_800BE9C0];
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


typedef struct Struct150E4CBC {
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
} Struct150E4CBC;

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

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vtx150E5810;

typedef struct {
    Vtx150E5810 unk0[3];
} Tri150E5810;

extern void func_15049350(Tri150E5810);
extern f32 D_800CC214;
extern f32 D_800CC218;

typedef struct {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s16 unk0C;
    s16 unk0E;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    s16 unk18;
    s16 unk1A;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    s8 unk1F;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    u8 unk2A;
    u8 unk2B;
    u8 unk2C;
    u8 unk2D;
    u8 unk2E;
    u8 unk2F;
    s32 unk30;
    s16 unk34;
    s16 unk36;
} Spawn150E4E04;

extern s32 D_8009187C;
extern s32 D_800918A0;
extern s32 D_800918AC;
extern s32 D_80090520;
extern void func_150440A0(f32 arg0[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);
extern f32 func_150489B0(u8);
extern s32 func_15167D84(void *, s32, s32, s32, s32, s32);
void func_150E5558(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7);

void func_150E4E04(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4, s16 **arg5, s32 arg6) {
    f32 minScale;
    f32 rangeScale;
    s32 ok;
    s32 count;
    s32 noFx;
    s32 noSurface;
    s32 big;
    s32 i;
    Tri150E5810 tri;
    f32 x;
    f32 y;
    f32 z;
    f32 len;
    f32 scale;
    f32 mtx[4][4];
    Spawn150E4E04 spawn;
    f32 speed;
    u8 angle;
    f32 vx;
    f32 vy;
    f32 vz;
    f32 cs;

    ok = 1;
    count = 1;
    noSurface = 0;
    big = 0;
    noFx = 0;
    scale = 815.0f;
    bzero(&spawn, sizeof(spawn));
    if (arg3 & 0x8000) {
        noFx = 1;
        arg3 ^= 0x8000;
    }
    if (arg5 != NULL) {
        for (i = 0; i < 3; i++) {
            tri.unk0[i].x = arg5[i][0];
            tri.unk0[i].y = arg5[i][1];
            tri.unk0[i].z = arg5[i][2];
        }
        func_15049350(tri);
        x = -D_800CC210;
        y = -D_800CC214;
        z = -D_800CC218;
        len = sqrtf(x * x + y * y + z * z);
        if (len < 9.999999747e-05f) {
            return;
        }
        x /= len;
        y /= len;
        z /= len;
        if (arg6 != 0xFF && fabsf(y) < 0.8999999762f) {
            func_150440A0(mtx, 0.0f, 0.0f, 0.0f, x, y, z, 0.0f, 1.0f, 0.0f);
            func_150A7960(mtx, 0.0f, 0.0f, 1.0f, &D_800D9960[arg6].unk10, &D_800D9960[arg6].unk14, &D_800D9960[arg6].unk18);
        } else {
            noSurface = 1;
        }
    } else {
        noSurface = 1;
    }
    if (arg6 != 0xFF && noSurface) {
        D_800D9960[arg6].unk3 = 0;
    }
    if (noFx == 0) {
        spawn.unk10 = arg0;
        spawn.unk12 = arg1;
        spawn.unk14 = arg2;
        spawn.unk2C = 0xB4;
        spawn.unk2D = 0xB4;
        spawn.unk2E = 0xB4;
        spawn.unk2F = 0xFF;
        spawn.unk1F = -1;
        spawn.unk34 = 0;
        spawn.unk0C = 0;
        spawn.unk0E = 0;
        minScale = (u32)D_800A1110[arg4][1] * 95.0f * 0.00390625f;
        rangeScale = (u32)D_800A1110[arg4][1] * 130.0f * 0.00390625f;
        if (arg3 == 0x11) {
            spawn.unk00 = (s32)&D_8009187C;
        } else if (arg3 == 0xA) {
            spawn.unk00 = (s32)&D_800918A0;
        } else if (arg3 == 4 || arg3 == 9) {
            spawn.unk00 = (s32)&D_800918A0;
        } else if (arg3 == 8) {
            spawn.unk00 = (s32)&D_80090520;
            minScale = 180.0f;
            rangeScale = 200.0f;
        } else {
            spawn.unk00 = (s32)&D_800918AC;
        }
        if (arg4 == 2) {
            big = 1;
            spawn.unk22 = -0x175;
            count = 12;
            scale *= 0.5f * ((func_151EF610() % 3) + 1);
        } else if (arg4 == 0) {
            spawn.unk22 = -0xDA;
            count = 12;
            scale *= 0.5f;
        } else {
            spawn.unk22 = -0xDA;
        }
        while (count-- && ok) {
            speed = func_150ADA68() * 957.0f + scale;
            angle = func_151EF610() % 256;
            if (big) {
                speed += speed;
            }
            if (noSurface) {
                x = func_15048A40(angle);
                y = 1.0f;
                z = func_150489B0(angle);
            } else {
                cs = func_15048A40(angle);
                func_150A7960(mtx, cs * 0.5f, func_150489B0(angle) * 0.5f, 1.0f, &x, &y, &z);
            }
            vx = 0.3149999976f * x;
            vy = y;
            vz = 0.3149999976f * z;
            spawn.unk28 = (func_151EF610() % 2) + 0x182;
            spawn.unk16 = speed * vx;
            spawn.unk20 = speed * vy;
            spawn.unk18 = speed * vz;
            func_151EF610();
            spawn.unk2A = 10;
            spawn.unk24 = func_150ADA68() * rangeScale + minScale;
            spawn.unk26 = func_150ADA68() * rangeScale + minScale;
            if (!func_15167D84(&spawn, 0, 0, -1, 0xFF, 0)) {
                ok = 0;
            }
        }
        if ((arg3 == 4 || arg3 == 9 || arg3 == 5) && func_151EF610() % 5 == 0) {
            func_150E5558(arg0, arg1, arg2, x, y, z, arg3, arg4);
        }
    }
}

void func_150E5558(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7) {
    Struct150E5558 sp34;
    f32 var_f16;
    f32 var_f2;
    f32 temp_f2;
    f32 temp_f6;

    bzero(&sp34, sizeof(sp34));

    if (arg4 != 0.0f) {
        var_f16 = func_150484A0(arg3, arg4) * D_800A1140;
    } else {
        var_f16 = 90.0f;
    }

    if (arg5 != 0.0f) {
        var_f2 = func_150484A0(arg3, arg5) * D_800A1144;
    } else {
        var_f2 = 90.0f;
    }

    sp34.unk14 = var_f2;
    sp34.unk18 = var_f16;
    sp34.unk1C = 1.0f;
    sp34.unk20 = 1.0f;
    sp34.unk24 = 1.0f;
    sp34.unk00 = 1.0f;
    sp34.unk08 = sp34.unk0C = D_800A1148;
    sp34.unk28 = arg0;
    sp34.unk2C = arg1;
    sp34.unk30 = arg2;
    sp34.unk10 = 0.0f;

    if (arg6 == 4) {
        sp34.unk56 = 6;
        sp34.unk04 = D_800A114C;
    } else if (arg6 == 9) {
        sp34.unk56 = 8;
        sp34.unk04 = D_800A1150;
    } else {
        sp34.unk56 = 7;
        sp34.unk04 = D_800A1154;
    }

    sp34.unk54 = (func_151EF610() % 60) + 0x3C;
    sp34.unk50 = 0x29E9;

    temp_f6 = func_150ADA68();
    sp34.unk4C = D_800A1158;
    temp_f2 = (temp_f6 * D_800A115C) + 1.5f;
    sp34.unk34 = temp_f2 * arg3;
    sp34.unk38 = -temp_f2 * arg4;
    sp34.unk3C = temp_f2 * arg5;

    sp34.unk40 = (func_150ADA68() * 6.0f) + -3.0f;
    sp34.unk48 = (func_150ADA68() * 6.0f) + -3.0f;
    temp_f6 = (func_150ADA68() * 6.0f) + -3.0f;
    sp34.unk58 = 0;
    sp34.unk5C = 0;
    sp34.unk44 = temp_f6;
    sp34.unk60 = 0xFF;
    sp34.unk61 = 1;
    sp34.unk62 = 0;
    sp34.unk63 = 3;
    sp34.unk64 = 0;
    sp34.unk65 = 0;
    sp34.unk66 = 0;
    sp34.unk67 = 0;
    sp34.unk68 = 0;
    sp34.unk6A = 2;
    sp34.unk6C = 0;
    sp34.unk70 = 0;
    sp34.unk72 = 0x20;
    sp34.unk74 = 7;

    func_15132A4C(&sp34, 3, 0xFF, 0, 0xFF, 0);
}


typedef struct Struct150E5810 {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 pad18[0x24];
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ f32 unk50;
    /* 0x54 */ f32 unk54;
    /* 0x58 */ f32 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ s32 unk60;
} Struct150E5810;

s32 func_150E5810(Struct150E5810 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s16 (*arg5)[3]) {
    Tri150E5810 tri;
    s32 i;
    f32 len;
    f32 s;
    f32 dot;
    struct17 v;
    struct17 n;
    struct17 r;

    arg0->unk3C = arg0->unk10 * 1.100000024f + arg4;
    if (arg0->unk48 > -2.0f) {
        arg0->unk44 = 0.0f;
        arg0->unk60 &= ~0x6F;
        arg0->unk48 = 0.0f;
        arg0->unk4C = 0.0f;
    } else {
        if (arg5 != NULL) {
        lbl:
            for (i = 0; i < 3; i++) {
                tri.unk0[i].x = arg5[i][0];
                tri.unk0[i].y = arg5[i][1];
                tri.unk0[i].z = arg5[i][2];
            }
            func_15049350(tri);
            n.unk0 = -D_800CC210;
            n.unk4 = -D_800CC214;
            n.unk8 = -D_800CC218;
            len = func_150AD930(&n.unk0);
            if (len > 9.999999747e-06f) {
                func_15049148(&n, 1.0f / len, &r);
                v.unk0 = arg0->unk44;
                v.unk4 = arg0->unk48;
                v.unk8 = arg0->unk4C;
                dot = func_150AD900(&r.unk0, &v.unk0);
                func_15049148(&r, 2.0f * dot, &n);
                func_15048F58(&v, &n, &n);
                arg0->unk44 = arg0->unk14 * n.unk0 * 0.8000000119f;
                arg0->unk48 = arg0->unk14 * n.unk4;
                arg0->unk4C = arg0->unk14 * n.unk8 * 0.8000000119f;
            }
        } else {
            arg0->unk44 = arg0->unk44 * arg0->unk14;
            arg0->unk48 = -arg0->unk48 * arg0->unk14;
            arg0->unk4C = arg0->unk4C * arg0->unk14;
        }
        arg0->unk50 = arg0->unk50 * arg0->unk14;
        arg0->unk54 = arg0->unk54 * arg0->unk14;
        arg0->unk58 = arg0->unk58 * arg0->unk14;
    }
    return 1;
}
