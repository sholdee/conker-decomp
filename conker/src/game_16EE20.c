#include <ultra64.h>

// matching: one call below passes no argument (the id is still in a0), so this file keeps an unprototyped view
#define func_15083E90 func_15083E90_functions_h
#include "functions.h"
#undef func_15083E90
Actor *func_15083E90();
#include "variables.h"

/* Cached RDP state (prim/env colour, geometry and other modes, last texture) for func_15142B7C..func_15142FBC. */
extern s16 D_800DD1C0;
extern s16 D_800DD1C2;
extern s16 D_800DD1C4;
extern s16 D_800DD1C6;
extern s16 D_800DD1C8;
extern s16 D_800DD1CA;
extern s16 D_800DD1CC;
extern s16 D_800DD1CE;
extern s32 D_800DD1FC;
extern s32 D_800DD200;
extern s32 D_800DD1B0;
extern s16 D_800DD204;
extern s16 D_800DD206;
extern s32 D_800DD208;
extern s32 D_800DD20C;
extern s32 D_800DD210;
extern s32 D_800DD214;
extern s32 D_800DD218;
extern s32 D_800DD21C;
extern u8 D_800C3E90;
extern u8 D_8008A160[]; /* palettes of four RGB triples */
/* Debug breadcrumbs of func_15143134. */
extern s32 D_800DCA00;
extern Mtx *D_800DCA04;
extern f32 D_800DCA08;
extern f32 D_800DCA0C;
extern f32 D_800DCA10;

void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
Gfx *func_15094FE8(Gfx *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_1514306C(s32 *, s32, s32, u8);

void func_15141970(struct37 *arg0) {
    func_1514EDF0(arg0, arg0->unk2C);
}

void func_15141990(void *arg0) {
    func_15141970(arg0);
}

void func_151419B0(void *arg0) {
    func_15141970(arg0);
}

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ union {
        s32 w;
        u8 b;
    } unk4;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
} Msg151419D0;

/* The block at 0x28 of the receiving object. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 unk8;
} Link151419D0;

void func_1516972C(struct102 *arg0);

/* Message handler: message 0 destroys the object if it refers to it; 0x2D re-links it. */
void func_151419D0(struct102 *obj, Msg151419D0 *msg, u8 msgType) {
    Link151419D0 *link = (Link151419D0 *)&obj->unk28;
    s32 first;

    if (msgType == 0) {
        first = msg->unk0;
        if ((first == link->unk4) || (msg->unk4.b == link->unk8)) {
            func_1516972C(obj);
        }
    } else if (msgType == 0x2D) {
        if (link->unk4 == msg->unk0) {
            link->unk4 = msg->unk4.w;
            link->unk8 = msg->unk9;
        } else if (link->unk4 == msg->unk4.w) {
            link->unk4 = msg->unk0;
            link->unk8 = msg->unk8;
        }
    }
}

/* Payload copied into an attached effect (see func_15141E38). */
typedef struct {
    /* 0x00 */ s32 handler;    /* index into D_8008A0B4 */
    /* 0x04 */ Actor *owner;
    /* 0x08 */ u8 ownerId;     /* owner->unique_id */
} EffectInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 duration;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EffectInfo info;
} AttachedEffect;

/* Node of the list an actor keeps at 0x2F4 (see func_1514EC1C / func_1514ECE0). */
typedef struct Attachment {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ AttachedEffect *effect;
    /* 0x14 */ struct Attachment *next;
} Attachment;

/* List tag of the attached timed effects. */
#define ATTACH_TIMED_EFFECT 0x1A

typedef s32 (*EffectSelector)(s32, Actor *);
typedef void (*EffectHandler)(Actor *, s32, s32);

s32 func_15141C0C(Actor *actor);
s32 func_15141CC0(u32 arg0);
u32 func_1510F8CC(s32);
void func_15141E38(Actor *actor, s32 handler);
s32 func_1514ECE0(Attachment *, s16, Attachment **);

/* Runs the actor's model-specific effect selector, then every attached timed effect's handler. */
void func_15141A7C(Actor *actor, s32 arg1) {
    s32 selector;
    s32 handler;

    if (D_800BE616 == 0) {
        selector = func_15141C0C(actor);
        if (D_8008A084[selector] != 0) {
            handler = ((EffectSelector)D_8008A084[selector])(func_15141CC0(func_1510F8CC(actor->unk184)), actor);
            if (handler != -1) {
                if (D_8008A0B4[handler].unk0 != 0) {
                    if (D_8008A0B4[handler].unk4 > 0) {
                        func_15141E38(actor, handler);
                    } else {
                        ((EffectHandler)D_8008A0B4[handler].unk0)(actor, arg1, 0);
                    }
                }
            }
        }
        {
        Attachment *node;
        node = actor->attachments;
        if (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0) {
            do {
                AttachedEffect *effect;
                effect = node->effect;
                if (D_8008A0B4[effect->info.handler].unk0 != 0) {
                    // matching: the u32 view makes IDO reload the index instead of reusing it
                    ((EffectHandler)D_8008A0B4[*(u32 *)&effect->info.handler].unk0)(actor, arg1, effect->duration);
                }
                node = node->next;
            } while (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0);
        }
        }
    }
}

/* Maps an actor's model id to its effect selector index (11: none). */
s32 func_15141C0C(Actor *actor) {
    switch (actor->id) {
    case 121:
        return 10;
    case 33:
        return 9;
    case 123:
        return 8;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 150:
        return 0;
    case 16:
    case 145:
        return 1;
    case 43:
        return 2;
    case 84:
        return 5;
    case 54:
    case 83:
    case 165:
        return 6;
    case 88:
        return 7;
    case 69:
        return 3;
    case 75:
        return 4;
    default:
        return 11;
    }
}
/* Picks an effect variant from the current world (D_800BE9F0) and the given type. */
s32 func_15141CC0(u32 type) {
    if (D_800BE9F0 == 0x2F) {
        return 6;
    }
    if (D_800BE9F0 == 0x42) {
        return 7;
    }
    if (D_800BE9F0 == 0x27) {
        return 8;
    }
    if (D_800BE9F0 == 0x19) {
        return 5;
    }
    switch (type) {
    case 10:
        return 0;
    case 7:
        return 2;
    case 11:
        return 1;
    case 15:
        return 3;
    case 2:
    case 8:
    case 12:
        if (D_800BE9F0 == 2) {
            return 7;
        }
        return 4;
    case 5:
        if (D_800BE9F0 == 0x14) {
            return 5;
        }
        return 9;
    case 0:
        return 9;
    default:
        return 9;
    }
}

/* Attaches timed effect `handler` to the actor when both indices are valid and it has a duration. */
void func_15141DA4(void *actor, s32 selector, s32 handler) {
    if ((selector < 12) && (selector >= 0) &&
        (handler < 20) && (handler >= 0) &&
        (D_800BE616 == 0) &&
        (D_8008A084[selector] != 0) && (handler != -1)) {
        if ((D_8008A0B4[handler].unk0 != 0) && (D_8008A0B4[handler].unk4 > 0)) {
            func_15141E38(actor, handler);
        }
    }
}

s32 func_1514EC1C(s32, s32, s16);
void *memcpy(void *, const void *, u32);

/* Restarts the actor's timed effect `handler`, or creates and attaches it if it has none. */
void func_15141E38(Actor *actor, s32 handler) {
    Attachment *found;
    Attachment *node;
    s32 pad; // matching: unused stack slot (keeps `info` at sp+0x40)

    found = NULL;
    node = actor->attachments;
    if (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0) {
        do {
            AttachedEffect *effect;
            effect = node->effect;
            if (handler == effect->info.handler) {
                found = node;
                effect->duration = D_8008A0B4[handler].unk4;
            }
            node = node->next;
        } while (func_1514ECE0(node, ATTACH_TIMED_EFFECT, &node) != 0);
    }
    if (found == NULL) {
        AttachedEffect *effect;
        EffectInfo info;
        info.handler = handler;
        info.owner = actor;
        info.ownerId = actor->unique_id;
        effect = (AttachedEffect *)func_15149130(D_8008A0B4[handler].unk4, -1, -1, -1, 1, 50, 0xC, 0xFF, 1);
        if (effect != NULL) {
            memcpy(&effect->info, &info, sizeof(info));
            func_1514EC1C((s32)effect, (s32)actor, ATTACH_TIMED_EFFECT);
        }
    }
}

/* Particle spawn packet for func_1513C650 (struct157 layout with unsigned colour bytes). */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 color[6];
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 unk20;
    /* 0x21 */ u8 pad21;
    /* 0x22 */ s16 unk22;
    /* 0x24 */ s16 unk24;
} Packet15141F78;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 unk4[0x14];
    /* 0x18 */ s32 unk18;
} Source15141F78;

/* Spawns a burst of particles of the given kind, sized by `scale`. */
void func_15141F78(u8 kind, Source15141F78 *src, f32 scale, u8 arg3, Vec3f *pos, u8 arg5) {
    Packet15141F78 packet;
    f32 size;
    s32 flag;

    packet.unk6 = kind;
    packet.unk7 = 0;
    packet.unk0 = 0x6F701;
    packet.unk4 = (func_150ADA20() % 61U) + 100;
    packet.unk8 = 0;
    packet.unkC = 0;
    packet.color[0] = (func_150ADA20() & 0x7F) + 128;
    packet.color[1] = 0xFF;
    packet.color[2] = 0xFF;
    packet.color[3] = 0xFF;
    packet.color[4] = 0xFF;
    packet.color[5] = 0xFF;
    packet.unk18 = 0x3B0002;
    packet.unk16 = 0;
    packet.unk17 = 7;
    packet.unk20 = 0xFF;
    packet.unk1C = src->unk18;
    packet.unk22 = 40;
    packet.unk24 = 6;

    size = ((func_150ADA68() * 5.0f) + 10.0f) * scale;
    if (arg5 == 2) {
        flag = 1;
    } else {
        flag = 0;
    }

    func_1513C650((s32)&packet, 0, 0, (s32)src->unk4, pos->x, src->unk0, pos->z, size, size, arg3, flag, 3, 1, 0, 0xFF, 1);
}

/* Template copied onto the stack for func_150A2AEC. */
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} Args151420F8;

extern Args151420F8 D_800A5200;
s32 func_150A2AEC(s32, s32, Args151420F8 *, s32);

s32 func_151420F8(s32 actor) {
    Args151420F8 args;

    args = D_800A5200;
    if (func_150A2AEC((actor - (s32)D_800CC2D0) / (s32)sizeof(Actor), 6, &args, actor) == -1) {
        return 0;
    }
    return 1;
}
/* Parameter block for func_15153F18. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ Vec3f pos;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ s8 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ s16 unk44;
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
} Params15153F18;


void func_15153F18(Params15153F18 *params, Vec3f *pos, s32, s32, s32);

/* Spawns the func_15153F18 effect of the given kind at `pos`. */
void func_15142180(u8 kind, Vec3f *pos, s32 arg2, f32 scaleA, f32 scaleB) {
    Params15153F18 params;

    params.pos = *pos;
    params.unk2C = 3;
    params.unk2E = 3;
    params.unk2 = 255;
    params.unk4 = -25;
    params.unk6 = 10;
    params.unk14 = 2.5f * scaleA;
    params.unk18 = (1.0f + 1.0f) * scaleA; // matching: folds to 2.0f but avoids IDO turning 2.0f * x into x + x
    params.unk1C = 0.1080000028f;
    params.unk20 = 0.2050000131f;
    params.unk24 = 3.0f * scaleB;
    params.unk28 = 3.5f * scaleB;
    params.unk0 = 0;
    params.unk30 = 3;
    params.unk32 = 1;
    params.unk34 = 9;
    params.unk36 = 15;
    params.unk38 = 180;
    params.unk3A = 75;
    params.unk40 = 0.0f;
    params.unk44 = 12;
    params.unk46 = 21;
    params.unk48 = arg2;
    params.unk3C = kind;
    func_15153F18(&params, &params.pos, 0, 0xFF, 1);
}

/* Debug tunable stub: returns the midpoint of min and max (the name and source location are unused). */
s32 func_151422C0(s32 id, const char *name, s32 min, s32 max, const char *file, s32 line) {
    return (max + min) >> 1;
}

/* Debug tunable stub: returns the default value unchanged. */
s32 func_151422DC(s32 id, const char *name, s32 min, s32 max, s32 value, const char *file, s32 line) {
    return value;
}

/* Returns arg4 unchanged. */
s32 func_151422F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    return arg4;
}

/* Fixed-point Mtx: integer halves of all elements, then the fraction halves. */
typedef struct {
    s16 whole[4][4];
    s16 frac[4][4];
} MtxFixed;

/* Matrix storage, read as fixed point or as floats depending on D_800C3E90. */
typedef union {
    MtxFixed fx;
    MtxF mf;
} MtxView;

/* Copies the translation of bone matrix bones[bone] to out (fixed-point when D_800C3E90 is set). */
void func_15142314(struct255 *bones, s32 bone, Vec3f *out) {
    MtxView *m;

    // matching: a byte offset (not &mtx[index]) gives IDO's base + offset operand order
    m = (MtxView *)((u8 *)bones + bone * sizeof(Mtx));
    if (D_800C3E90 != 0) {
        // matching: the original recomputes m here; IDO keeps both computations
        m = (MtxView *)((u8 *)bones + bone * sizeof(Mtx));
        {
            f32 scale = 1.0f / 65536.0f;
            out->x = ((f32)(m->fx.whole[3][0] << 16) + (f32)m->fx.frac[3][0]) * scale;
            out->y = ((f32)(m->fx.whole[3][1] << 16) + (f32)m->fx.frac[3][1]) * scale;
            out->z = ((f32)(m->fx.whole[3][2] << 16) + (f32)m->fx.frac[3][2]) * scale;
        }
        return;
    }
    out->x = m->mf[3][0];
    out->y = m->mf[3][1];
    out->z = m->mf[3][2];
}

/* Cosine of a binary angle (256 steps per turn), from the quarter-wave table D_8009A220. */
f32 func_151423D8(u8 angle) {
    s32 index;
    s32 a = angle;
    u16 quadrant;

    if (a & 0x40) {
        index = 0x40 - (a & 0x3F);
    } else {
        index = a & 0x3F;
    }
    quadrant = a & 0xC0;
    if ((quadrant == 0) || (quadrant == 0xC0)) {
        return D_8009A220[index];
    }
    return -D_8009A220[index];
}

/* Resolves an actor unique id (0xFF: `self`) to an actor with a model (unk1D4), or NULL. */
Actor *func_15142444(u8 uniqueId, Actor *self) {
    s32 id;
    Actor *actor;

    id = uniqueId;
    if (id == 0xFF) {
        if (self->unk1D4 != NULL) {
            return self;
        }
        return NULL;
    }

    if ((self != NULL) && (self->interaction_state != 0) && (id == self->unique_id)) {
        if (self->unk1D4 != NULL) {
            return self;
        }
        return NULL;
    }

    actor = func_15083E90();
    if ((actor != NULL) && (actor->unk1D4 != NULL)) {
        return actor;
    }
    return NULL;
}
void func_150A8050(f32 (*)[4], f32, f32, f32);

/*
 * Builds out = scale * rotation * translation: func_150A8050 makes the rotation, rows 0/2 are scaled
 * by scaleXZ and row 1 by scaleY, columns by (sx, sy, sz).
 */
void func_151424F4(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 sx, f32 sy, f32 sz,
                   f32 x, f32 y, f32 z) {
    MtxF mf;

    func_150A8050(&mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= sx * scaleXZ;
    mf[0][1] *= sy * scaleXZ;
    mf[0][2] *= sz * scaleXZ;
    mf[1][0] *= sx * scaleY;
    mf[1][1] *= sy * scaleY;
    mf[1][2] *= sz * scaleY;
    mf[2][0] *= sx * scaleXZ;
    mf[2][1] *= sy * scaleXZ;
    mf[2][2] *= sz * scaleXZ;
    guMtxF2L(&mf, out);
}

/* Builds a matrix at (x, y, z) whose forward axis points at (atX, atY, atZ), scaled per axis. */
void func_15142600(Mtx *out, f32 scaleXZ, f32 scaleY, f32 sx, f32 sy, f32 sz, f32 x, f32 y, f32 z,
                   f32 atX, f32 atY, f32 atZ) {
    MtxF mf;
    f32 inv;
    f32 fx, fy, fz;
    f32 ux, uy, uz;
    f32 rx, rz;

    fx = atX - x;
    fy = atY - y;
    fz = atZ - z;
    inv = 1.0f / sqrtf(fx * fx + fy * fy + fz * fz);
    fx *= inv;
    fy *= inv;
    fz *= inv;
    rx = fz;
    rz = -fx;
    inv = 1.0f / sqrtf(rx * rx + rz * rz);
    rx *= inv;
    rz *= inv;
    ux = fy * rz;
    uy = fz * rx - fx * rz;
    uz = -fy * rx;
    inv = 1.0f / sqrtf(ux * ux + uy * uy + uz * uz);
    mf[0][0] = rx * sx * scaleXZ;
    mf[1][0] = ux * inv * sx * scaleY;
    mf[2][0] = fx * sx * scaleXZ;
    mf[3][0] = x;
    mf[0][1] = 0.0f;
    mf[1][1] = uy * inv * sy * scaleY;
    mf[2][1] = fy * sy * scaleXZ;
    mf[3][1] = y;
    mf[0][2] = rz * sz * scaleXZ;
    mf[1][2] = uz * inv * sz * scaleY;
    mf[2][2] = fz * sz * scaleXZ;
    mf[3][2] = z;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
    guMtxF2L(mf, out);
}

/* Builds a rotation matrix at (x, y, z), rows 0/2 scaled by scaleXZ and row 1 by scaleY. */
void func_15142838(Mtx *out, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
    MtxF mf;

    func_150A8050(mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= scaleXZ;
    mf[0][1] *= scaleXZ;
    mf[0][2] *= scaleXZ;
    mf[1][0] *= scaleY;
    mf[1][1] *= scaleY;
    mf[1][2] *= scaleY;
    mf[2][0] *= scaleXZ;
    mf[2][1] *= scaleXZ;
    mf[2][2] *= scaleXZ;
    guMtxF2L(mf, out);
}

/* Float version of func_15142838. */
void func_15142914(MtxF mf, f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
    func_150A8050(mf, rotX, rotY, rotZ);
    mf[3][0] = x;
    mf[3][1] = y;
    mf[3][2] = z;
    mf[0][0] *= scaleXZ;
    mf[0][1] *= scaleXZ;
    mf[0][2] *= scaleXZ;
    mf[1][0] *= scaleY;
    mf[1][1] *= scaleY;
    mf[1][2] *= scaleY;
    mf[2][0] *= scaleXZ;
    mf[2][1] *= scaleXZ;
    mf[2][2] *= scaleXZ;
}

/* Picks one of the four RGB colours of palette `palette` at random. */
void func_151429E0(u8 palette, u8 *r, u8 *g, u8 *b) {
    u8 *color;

    color = &D_8008A160[(func_150ADA20() & 3) * 3 + palette * 12];
    *r = color[0];
    *g = color[1];
    *b = color[2];
}

typedef struct {
    /* 0x00 */ u8 pad0[0x3C];
    /* 0x3C */ s16 unk3C;
} Unk2D0View;

s32 func_15142A5C(Actor *actor) {
    Unk2D0View *unk2D0 = (Unk2D0View *)actor->unk2D0;
    s32 ret = 0;

    if (unk2D0->unk3C > 0) {
        return 1;
    }
    return ret;
}

/* Cubic Lagrange interpolation weights for samples at -1, 0, 1, 2 (t in [0, 1]). */
f32 func_15142A80(f32 t) {
    return ((1.0f - t) * (t - 2.0f) * t) * (1.0f / 6.0f);
}
f32 func_15142AC0(f32 t) {
    return ((t + 1.0f) * (t - 1.0f) * (t - 2.0f)) * 0.5f;
}
f32 func_15142B04(f32 t) {
    return ((2.0f - t) * (t + 1.0f) * t) * 0.5f;
}
f32 func_15142B44(f32 t) {
    return ((t + 1.0f) * (t - 1.0f) * t) * (1.0f / 6.0f);
}
/* Emits geometry-mode changes, skipping bits already in the cached state. */
Gfx *func_15142B7C(Gfx *gfx, s32 setBits, s32 clearBits) {
    if (~D_800DD200 & clearBits) {
        gSPClearGeometryMode(gfx++, clearBits);
        D_800DD200 |= clearBits;
    }

    if (~D_800DD1FC & setBits) {
        gSPSetGeometryMode(gfx++, setBits);
        D_800DD1FC |= setBits;
    }

    return gfx;
}

/* Sets the env colour unless it is already current; emits a pipe sync first if *needSync is 1. */
Gfx *func_15142C10(Gfx *gfx, s32 r, s32 g, s32 b, s32 a, u8 *needSync) {
    if ((r != D_800DD1C8) || (g != D_800DD1CA) || (b != D_800DD1CC) || (a != D_800DD1CE)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetEnvColor(gfx++, r, g, b, a);

        D_800DD1C8 = r;
        D_800DD1CA = g;
        D_800DD1CC = b;
        D_800DD1CE = a;
    }

    return gfx;
}

/* Sets the prim colour unless it is already current; emits a pipe sync first if *needSync is 1. */
Gfx *func_15142CF0(Gfx *gfx, s32 minLevel, s32 lodFrac, s32 r, s32 g, s32 b, s32 a, u8 *needSync) {
    if ((minLevel != D_800DD204) || (lodFrac != D_800DD206) || (r != D_800DD1C0) ||
        (g != D_800DD1C2) || (b != D_800DD1C4) || (a != D_800DD1C6)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetPrimColor(gfx++, minLevel, lodFrac, r, g, b, a);

        D_800DD204 = minLevel;
        D_800DD206 = lodFrac;
        D_800DD1C0 = r;
        D_800DD1C2 = g;
        D_800DD1C4 = b;
        D_800DD1C6 = a;
    }

    return gfx;
}

/* Loads a texture through func_15094FE8 unless the same one is already loaded (cached state). */
Gfx *func_15142E24(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8,
                   u8 *needSync, s32 argA) {
    s32 texture;

    texture = func_1514306C(arg1, arg6, arg2 >> 16, arg7);
    if ((texture != D_800DD1B0) || (arg3 != D_800DD208) || (arg4 != D_800DD20C) || (arg5 != D_800DD210) ||
        (arg8 != D_800DD214)) {
        if (*needSync == 1) {
            *needSync = 0;
        }
        if ((D_800BE9F0 == 0x18) || (D_800BE9F0 == 0x13) || (D_800BE9F0 == 6) || (D_800BE9F0 == 0x3B) ||
            (D_800BE9F0 == 2) || (D_800BE616 != 0)) {
            argA = 3;
        }
        gfx = func_15094FE8(gfx, arg1, arg2 >> 8, arg8, 0, 0, 0, arg3, arg4, arg5, argA);
        D_800DD1B0 = texture;
        D_800DD208 = arg3;
        D_800DD20C = arg4;
        D_800DD210 = arg5;
        D_800DD214 = D_800DD214; /* sic: arg8 is never cached */
    }
    return gfx;
}

/* Sets the other-mode words unless already current; emits a pipe sync first if *needSync is 1. */
Gfx *func_15142FBC(Gfx *gfx, s32 modeH, s32 modeL, u8 *needSync) {
    if ((modeH != D_800DD218) || (modeL != D_800DD21C)) {
        if (*needSync == 1) {
            gDPPipeSync(gfx++);
            *needSync = 0;
        }

        gDPSetOtherMode(gfx++, modeH | 0xF, modeL);

        D_800DD218 = modeH;
        D_800DD21C = modeL;
    }

    return gfx;
}

s16 func_15143044(u8 arg0, s32 arg1) {
    return 0x7FFF - arg0;
}

typedef struct {
    s32 *unk0;
    s32 unk4;
    s32 unk8;
} Entry1514306C;
extern Entry1514306C D_80090B60[];
extern s32 D_80091564[];
extern s32 D_80091514;
extern s32 D_800915B0;

/* Resolves the texture word for func_15142E24 according to `mode`. */
s32 func_1514306C(s32 *arg0, s32 arg1, s32 arg2, u8 mode) {
    s32 ret;

    switch (mode) {
        case 4:
            ret = D_800915B0;
            break;
        case 3:
            ret = D_80091514;
            break;
        case 1:
            ret = 0;
            break;
        case 2:
            ret = D_80091564[arg1];
            break;
        case 5:
            ret = arg1;
            break;
        case 6:
            /* *arg0 is either a pointer (KSEG addresses) to a table, or the value itself */
            arg1 = *arg0;
            if ((u32)*arg0 >= 0x10000000) {
                ret = ((s32 *)*arg0)[arg2];
            } else {
                ret = arg1;
            }
            break;
        default:
            ret = D_80090B60[arg1].unk0[arg2];
            break;
    }
    return ret;
}
/*
 * Transforms `offset` by mtx into out (just mtx's translation when offset is NULL or zero).
 * D_800DCA00 records the progress through the function (a debug breadcrumb).
 */
void func_15143134(Vec3f *offset, f32 *out, Mtx *mtx) {
    MtxF mf;

    D_800DCA00 = 1;
    if ((offset != NULL) && ((offset->x != 0.0f) || (offset->y != 0.0f) || (offset->z != 0.0f))) {
        D_800DCA00 = 2;
        D_800DCA08 = offset->x;
        D_800DCA0C = offset->y;
        D_800DCA10 = offset->z;
        if (D_800C3E90) {
            D_800DCA00 = 3;
            D_800DCA04 = mtx;
            guMtxL2F(mf, mtx);
            func_150A7960(mf[0], offset->x, offset->y, offset->z, out, out + 1, out + 2);
            D_800DCA00 = 4;
        // matching: an (unused) label here changes how IDO lays out the branch to the final store
        state4_done:;
        } else {
            D_800DCA00 = 5;
            D_800DCA04 = mtx;
            func_150A7960((f32 *)mtx, offset->x, offset->y, offset->z, out, out + 1, out + 2);
            D_800DCA00 = 6;
        }
    } else {
        D_800DCA00 = 7;
        func_15142314(mtx, 0, out);
        D_800DCA00 = 8;
    }
    D_800DCA00 = 0;
}

/* A spawn region around (x, y, z): a disc, cylinder, sphere or (rotated) box, by flags & 3. */
typedef struct SpawnRegion {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation; /* degrees */
    /* 0x14 */ u8 pad14;
    /* 0x15 */ u8 flags; /* bits 0-1: shape */
    /* 0x16 */ u8 pad16[0xE];
    /* 0x24 */ f32 cosineX;
    /* 0x28 */ f32 sineX;
    /* 0x2C */ f32 sineZ;
    /* 0x30 */ f32 cosineZ;
} SpawnRegion;

/* Picks a random (x, z) in the region's footprint, with the region's top and bottom heights. */
void func_151432BC(SpawnRegion *region, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    switch (region->flags & 3) {
    default:
        *x = region->x;
        *z = region->z;
        *top = region->y + region->height;
        *bottom = region->y - region->height;
        return;
    case 2:
        /* rotated box */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 offsetX;
            f32 offsetZ;

            angle = region->rotation * (256.0f / 360.0f);
            sine = func_151423D8(angle - 0x40);
            cosine = func_151423D8(angle);
            offsetX = func_150ADA68() * (2.0f * region->radiusX) + -region->radiusX;
            offsetZ = func_150ADA68() * (2.0f * region->radiusZ) + -region->radiusZ;
            *x = region->x + (offsetX * cosine + offsetZ * sine);
            *z = region->z + (offsetZ * cosine - offsetX * sine);
            *top = region->y + region->height;
            *bottom = region->y;
        }
        return;
    case 0:
        /* disc, from the base up */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 radius;

            angle = func_150ADA20() & 0xFF;
            sine = func_151423D8(angle - 0x40);
            cosine = func_151423D8(angle);
            radius = func_150ADA68() * region->radiusX;
            *x = region->x + radius * cosine;
            *z = region->z - radius * sine;
            *top = region->y + region->height;
            *bottom = region->y;
        }
        return;
    case 1:
        /* disc, centred vertically */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 radius;

            angle = func_150ADA20() & 0xFF;
            sine = func_151423D8(angle - 0x40);
            cosine = func_151423D8(angle);
            radius = func_150ADA68() * region->radiusX;
            *x = region->x + radius * cosine;
            *z = region->z - radius * sine;
            *top = region->y + region->height;
            *bottom = region->y - region->height;
        }
        return;
    }
}

/* Direction (yaw, pitch in radians) scaled by length, into out[3]. */
void func_151436B4(f32 yaw, f32 pitch, f32 length, f32 *out) {
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    f32 horizontal;

    cosYaw = cosf(yaw);
    sinYaw = sinf(yaw);
    cosPitch = cosf(pitch);
    sinPitch = sinf(pitch);
    horizontal = length * cosPitch;
    out[0] = horizontal * sinYaw;
    out[1] = -length * sinPitch;
    out[2] = horizontal * cosYaw;
}

/* Planar direction (angle in radians) scaled by length. */
void func_1514373C(f32 angle, f32 length, f32 *x, f32 *z) {
    f32 c;
    f32 s;

    c = cosf(angle);
    s = sinf(angle);
    *x = length * s;
    *z = length * c;
}

/* Binary-angle (256 per turn) version of func_151436B4. */
void func_15143794(s32 yaw, s32 pitch, f32 length, f32 *out) {
    f32 cosYaw;
    f32 sinYaw;
    f32 cosPitch;
    f32 sinPitch;
    u8 quarterYaw;
    u8 quarterPitch;
    f32 horizontal;

    cosYaw = func_151423D8((u8)yaw);
    quarterYaw = (s16)yaw - 0x40;
    sinYaw = func_151423D8(quarterYaw);
    cosPitch = func_151423D8((u8)pitch);
    quarterPitch = (s16)pitch - 0x40;
    sinPitch = func_151423D8(quarterPitch);
    horizontal = length * cosPitch;
    out[0] = horizontal * sinYaw;
    out[1] = -length * sinPitch;
    out[2] = horizontal * cosYaw;
}

void func_15143834(s16 yaw, s16 pitch, f32 length, f32 *out) {
    func_15143794(yaw, pitch, length, out);
}

/* Binary-angle version of func_1514373C. */
void func_15143874(s32 angle, f32 length, f32 *x, f32 *z) {
    f32 c;
    f32 s;
    u8 quarter;

    c = func_151423D8((u8)angle);
    quarter = (s16)angle - 0x40;
    s = func_151423D8(quarter);
    *x = length * s;
    *z = length * c;
}

void func_15143D18(s32 *start, s32 *end, s32 lo, s32 hi);

/*
 * Searches D_800D3098[start..end) for a record equal to `key` in the fields selected by `fields`.
 * With bit 0x1000 every selected field must match, otherwise any one of them; the last hit wins.
 */
TriggerZone *func_151438D8(s32 start, s32 end, u16 fields, TriggerZone *key) {
    s32 i;
    TriggerZone *result;
    u16 checked;
    u16 matched;

    result = NULL;
    if (key == NULL) {
        return NULL;
    }
    func_15143D18(&start, &end, 0, D_800D3094);
    for (i = start; i < end; i++) {
        checked = 0;
        matched = 0;
        if (fields & 1) {
            if ((key->x == D_800D3098[i].x) && (key->y == D_800D3098[i].y) && (key->z == D_800D3098[i].z)) {
                checked |= 1;
                matched |= 1;
            }
        } else {
            checked |= 1;
        }
        if (fields & 0x2) {
            if ((key->radius == D_800D3098[i].radius) && (key->height == D_800D3098[i].height) && (key->depth == D_800D3098[i].depth)) {
                checked |= 0x2;
                matched |= 0x2;
            }
        } else {
            checked |= 0x2;
        }
        if (fields & 0x4) {
            if (key->pitch == D_800D3098[i].pitch) {
                checked |= 0x4;
                matched |= 0x4;
            }
        } else {
            checked |= 0x4;
        }
        if (fields & 0x8) {
            if (key->yaw == D_800D3098[i].yaw) {
                checked |= 0x8;
                matched |= 0x8;
            }
        } else {
            checked |= 0x8;
        }
        if (fields & 0x10) {
            if (key->disabled == D_800D3098[i].disabled) {
                checked |= 0x10;
                matched |= 0x10;
            }
        } else {
            checked |= 0x10;
        }
        if (fields & 0x20) {
            if (key->kind == (D_800D3098[i].kind >> 2)) {
                checked |= 0x20;
                matched |= 0x20;
            }
        } else {
            checked |= 0x20;
        }
        if (fields & 0x40) {
            if (key->flags == D_800D3098[i].flags) {
                checked |= 0x40;
                matched |= 0x40;
            }
        } else {
            checked |= 0x40;
        }
        if (fields & 0x80) {
            if (key->subtype == D_800D3098[i].subtype) {
                checked |= 0x80;
                matched |= 0x80;
            }
        } else {
            checked |= 0x80;
        }
        if (fields & 0x100) {
            if (key->unk18 == D_800D3098[i].unk18) {
                checked |= 0x100;
                matched |= 0x100;
            }
        } else {
            checked |= 0x100;
        }
        if (fields & 0x200) {
            if (key->unk1C == D_800D3098[i].unk1C) {
                checked |= 0x200;
                matched |= 0x200;
            }
        } else {
            checked |= 0x200;
        }
        if (fields & 0x400) {
            if (key->unk20 == D_800D3098[i].unk20) {
                checked |= 0x400;
                matched |= 0x400;
            }
        } else {
            checked |= 0x400;
        }
        if (fields & 0x1000) {
            if (checked == 0x7FF) {
                result = &D_800D3098[i];
            }
        } else {
            if (matched != 0) {
                result = &D_800D3098[i];
            }
        }
    }
    return result;
}
/* Orders the bounds lo/hi, then orders *start/*end and clamps them into [lo, hi]. XOR swaps. */
void func_15143D18(s32 *start, s32 *end, s32 lo, s32 hi) {
    s32 t;
    s32 t2;

    if (hi < lo) {
        t = lo ^ hi;
        t2 = hi ^ t;
        hi = t2;
        lo = t ^ t2;
    }
    if (*end < *start) {
        t = *end ^ *start;
        *start = t;
        t2 = *end ^ t;
        *end = t2;
        *start = *start ^ t2;
    }
    if (*start < lo) {
        *start = lo;
    }
    if (hi < *end) {
        *end = hi;
    }
}

/* Clamps *value into [lo, hi] (bounds in either order); returns 1 if raised, 2 if lowered, else 0. */
s32 func_15143DA8(s32 *value, s32 lo, s32 hi) {
    s32 t;
    s32 t2;

    if (hi < lo) {
        t = lo ^ hi;
        t2 = hi ^ t;
        hi = t2;
        lo = t ^ t2;
    }
    if (*value < lo) {
        *value = lo;
        return 1;
    }
    if (hi < *value) {
        *value = hi;
        return 2;
    }
    return 0;
}

/* The actor's facing angle (unk7A) as a binary angle, a quarter turn ahead. */
s32 func_15143E08(Actor *actor) {
    return (((s32)actor->unk7A >> 8) + 64) & 0xFF;
}

/* The actor's facing angle in 256ths of a turn, relative to unk31C's angle when it has one. */
s32 func_15143E24(Actor *actor) {
    ActorCtl *ref = actor->unk31C;
    if (ref != NULL) {
        return (s16)((actor->unk7A - ref->unk12) >> 8);
    }
    return (s16)(actor->unk7A >> 8);
}
/* Length of a vector. */
f32 func_15143E64(Vec3f *v) {
    f32 x = v->x;
    f32 y = v->y;
    f32 z = v->z;

    return sqrtf(x * x + y * y + z * z);
}
typedef struct Func15143E94Spawn {
    s8 active;
    u8 pad1;
    s16 duration;
    s8 size;
    s8 enabled;
    s8 sentinel;
    u8 pad7;
} Func15143E94Spawn;
extern void func_151D8868(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/*
 * If any actor is alive and some i passes func_150A29C8(i, arg1) == 0, plays arg0 through func_1512D748
 * and spawns a short random func_151D8868 effect. Returns whether it did.
 */
u8 func_15143E94(s32 arg0, s32 arg1) {
    u8 ret;
    s8 i;
    s8 count;
    u8 found;
    u8 found2;
    Func15143E94Spawn spawn;

    ret = 0;
// matching: an unused label here splits the block, changing IDO's scheduling of the setup
start:
    count = D_80082FA0 + 1;
    i = 0;
    found = 0;
    while (!found && i < count) {
        if (D_800CC2D0[i].health != 0) {
            found = 1;
        } else {
            i++;
        }
    }
    i = 0;
    if (found) {
        found2 = 0;
        while (!found2 && i < count) {
            if (func_150A29C8(i, arg1) == 0) {
                found2 = 1;
            } else {
                i++;
            }
        }
        if (found2) {
            func_1512D748(&D_800DBFF0[D_800BE9E8], arg0, 1);
            spawn.active = 1;
            spawn.duration = (func_150ADA20() & 0xF) + 0x14;
            spawn.size = (func_150ADA20() & 3) + 4;
            spawn.sentinel = -1;
            spawn.enabled = 1;
            func_151D8868(&spawn, 0, 0xFF, 0);
            ret = 1;
        }
    }
    return ret;
}
typedef struct {
    u8 unk0;
    u8 pad1[0xB];
} Unk80090B64;
extern Unk80090B64 D_80090B64[];

/*
 * Advances a 16.16 position through sequence `id` (D_80090B64[id].unk0 steps long) by speed * D_800BE9E4.
 * Past either end it stops (1), clamps (2/0x10), bounces (4) or wraps; returns 1 when it stopped.
 */
s32 func_1514401C(u8 id, u32 *speed, s32 *pos, u8 flags) {
    s32 max;
    s32 ret;
    s32 n;

    ret = 0;
    n = D_80090B64[id].unk0;
    max = (n << 16) - 1;
    *pos += *speed * D_800BE9E4;
    if (max < *pos) {
        if (flags & 1) {
            ret = 1;
        } else if (flags & 2) {
            *speed = 0;
            *pos = max;
        } else if (flags & 4) {
            *pos = max - (*pos % max);
            *speed = -*speed;
        } else {
            do {
                *pos -= max;
            } while (max < *pos);
        }
    } else if ((*pos < 0) && !(flags & 8)) {
        if (flags & 0x10) {
            *speed = 0;
            *pos = 0;
        } else if (flags & 4) {
            *pos = -*pos % max;
            *speed = -*speed;
        } else {
            do {
                *pos += max;
            } while (*pos < 0);
        }
    }
    return ret;
}
/* Colour-combiner style blend: picks the output RGBA from colour 0, colour 1 and `level` per `mode`. */
void func_151441A4(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, s32 a0, u8 r1, u8 g1, u8 b1, u8 a1, u8 level, u8 mode) {
    switch (mode) {
    case 2:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a1;
        return;
    case 0:
        *outR = *outG = *outB = *outA = 0;
        return;
    case 1:
        *outR = *outG = *outB = 0;
        *outA = a1;
        return;
    case 3:
        *outR = (r0 * level) >> 8;
        *outG = (g0 * level) >> 8;
        *outB = (b0 * level) >> 8;
        *outA = 0;
        return;
    case 4:
        *outR = (r0 * level) >> 8;
        *outG = (g0 * level) >> 8;
        *outB = (b0 * level) >> 8;
        *outA = 0;
        return;
    default:
        *outR = (r0 * level) >> 8;
        *outG = (g0 * level) >> 8;
        *outB = (b0 * level) >> 8;
        *outA = 0;
        return;
    }
}
/* Second-cycle variant of func_151441A4. */
void func_151442FC(s16 *outR, s16 *outG, s16 *outB, s16 *outA, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1, u8 level, u8 mode) {
    switch (mode) {
    case 2:
    case 3:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = a0;
        return;
    case 13:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = 0;
        return;
    case 0:
        *outR = *outG = *outB = *outA = 0;
        return;
    case 1:
        *outB = level;
        *outG = level;
        *outR = level;
        *outA = 0;
        return;
    case 7:
    case 12:
        *outR = *outG = *outB = 0;
        *outA = a1;
        return;
    case 8:
        *outR = *outG = *outB = 0;
        *outA = a0;
        return;
    case 4:
        *outB = level;
        *outG = level;
        *outR = level;
        *outA = (a0 * a1) >> 8;
        return;
    case 5:
        *outB = level;
        *outG = level;
        *outR = level;
        *outA = (a0 * a1) >> 8;
        return;
    case 6:
        *outR = r0;
        *outG = g0;
        *outB = b0;
        *outA = a1;
        return;
    case 10:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a1;
        return;
    case 11:
        *outR = r1;
        *outG = g1;
        *outB = b1;
        *outA = a0;
        return;
    case 9:
        *outB = level;
        *outG = level;
        *outR = level;
        *outA = a1;
        return;
    default:
        *outB = level;
        *outG = level;
        *outR = level;
        *outA = (a0 * a1) >> 8;
        return;
    }
}
/* Wraps value into [lo, hi] (integer steps of hi - lo + 1). */
s32 func_151444DC(s32 value, s32 hi, s32 lo) {
    s32 range;
    if (hi < value) {
        range = hi - lo + 1;
        do {
            value -= range;
        } while (hi < value);
    }
    if (value < lo) {
        range = hi - lo + 1;
        do {
            value += range;
        } while (value < lo);
    }
    return value;
}

/* Wraps value into [lo, hi]. */
f32 func_15144528(f32 value, f32 hi, f32 lo) {
    f32 range;

    if (hi < value) {
        range = hi - lo;
        do {
            value -= range;
        } while (hi < value);
    }
    if (value < lo) {
        range = hi - lo;
        do {
            value += range;
        } while (value < lo);
    }
    return value;
}

/* Footprint area of a region: 2rx * 2rz for a box, pi r^2 for the round shapes. */
f32 func_15144598(TriggerZone *zone) {
    SpawnRegion *region = (SpawnRegion *)zone;
    f32 area;
    s16 radius;

    switch (region->flags & 3) {
    default:
        area = 1.0f;
        break;
    case 2:
        area = (f32)(region->radiusX * region->radiusZ) * 4.0f;
        break;
    case 0:
    case 1:
        radius = region->radiusX;
        area = (f32)(radius * radius) * 3.1415927f;
        break;
    }
    return area;
}

/* Volume of a region: box rx * h * rz, cylinder pi r^2 h, sphere (pi / 3) r^3. */
f32 func_1514462C(s32 arg0) {
    SpawnRegion *region = (SpawnRegion *)arg0;
    f32 volume;
    f32 radius;

    switch (region->flags & 3) {
    default:
        volume = 1.0f;
        break;
    case 2:
        volume = (f32)(region->radiusX * region->height * region->radiusZ);
        break;
    case 0:
        volume = (f32)(region->radiusX * region->radiusX) * 3.1415927f * (f32)region->height;
        break;
    case 1:
        radius = (f32)region->radiusX;
        volume = ((radius * 1.0471976f) * radius) * radius;
        break;
    }
    return volume;
}

/* Picks a random point inside the region into out[3], applying its tilt for boxes and cylinders. */
void func_1514470C(SpawnRegion *region, f32 *out) {
    f32 radiusZ;
    f32 height;
    f32 radiusX;
    f32 cosineX;
    f32 sineX;
    f32 sineZ;
    f32 cosineZ;
    s32 mode;

    radiusX = region->radiusX;
    height = region->height;
    radiusZ = region->radiusZ;
    cosineX = region->cosineX;
    sineX = region->sineX;
    sineZ = region->sineZ;
    cosineZ = region->cosineZ;
    mode = region->flags & 3;
    switch (mode) {
    default:
        out[0] = region->x;
        out[1] = region->y;
        out[2] = region->z;
        return;
    case 2:
        {
            f32 y;
            f32 x;
            f32 mixed;
            f32 z;
            f32 savedX;
            f32 rotatedZ;

            x = func_150ADA68() * (2.0f * radiusX) - radiusX;
            y = func_150ADA68() * height;
            z = func_150ADA68() * (2.0f * radiusZ) - radiusZ;
            savedX = x;
            mixed = y * cosineX + z * sineX;
            x = x * cosineZ + mixed * sineZ;
            y = y * sineX - z * cosineX;
            out[0] = region->x + x;
            out[1] = region->y + y;
            rotatedZ = mixed * cosineZ - savedX * sineZ;
            out[2] = region->z + rotatedZ;
        }
        return;
    case 0:
        {
            f32 y;
            f32 x;
            f32 mixed;
            f32 z;
            f32 savedX;
            f32 radius;
            u8 angle;

            radius = func_150ADA68() * radiusX;
            angle = func_150ADA20() & 0xFF;
            x = func_151423D8(angle - 0x40) * radius;
            y = func_150ADA68() * height;
            z = func_151423D8(angle) * radius;
            savedX = x;
            mixed = y * cosineX + z * sineX;
            x = x * cosineZ + mixed * sineZ;
            y = y * sineX - z * cosineX;
            out[0] = region->x + x;
            out[1] = region->y + y;
            out[2] = region->z + (mixed * cosineZ - savedX * sineZ);
        }
        return;
    case 1:
        {
            Vec3f offset;

            func_151436B4(func_150ADA68() * 6.2831855f, func_150ADA68() * 6.2831855f,
                          func_150ADA68() * radiusX, &offset.x);
            out[0] = region->x + offset.x;
            out[1] = region->y + offset.y;
            out[2] = region->z + offset.z;
        }
        return;
    }
}
/* Dot product. */
f32 func_15144A74(Vec3f *a, Vec3f *b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/* D_800DBFF0[index].unk380 wrapped into [0, 360] degrees. */
f32 func_15144AA8(s32 index) {
    f32 v = D_800DBFF0[index].unk380;
    if (360.0f < v) {
        do {
            v -= 360.0f;
        } while (360.0f < v);
    }
    if (v < 0.0f) {
        do {
            v += 360.0f;
        } while (v < 0.0f);
    }
    return v;
}

/* Position (unk2F8) of D_800DBFF0[index]. */
Vec3f *func_15144B34(s32 index) {
    return &D_800DBFF0[index].eye;
}

/* Wraps an angle into [0, 2 pi] radians. */
f32 func_15144B68(f32 angle) {
    f32 v = angle;
    if (6.2831855f < v) {
        do {
            v -= 6.2831855f;
        } while (6.2831855f < v);
    }
    if (v < 0.0f) {
        do {
            v += 6.2831855f;
        } while (v < 0.0f);
    }
    return v;
}

/* Wraps an angle into [0, 360] degrees. */
f32 func_15144BC8(f32 angle) {
    f32 v = angle;
    if (360.0f < v) {
        do {
            v -= 360.0f;
        } while (360.0f < v);
    }
    if (v < 0.0f) {
        do {
            v += 360.0f;
        } while (v < 0.0f);
    }
    return v;
}

/* Wraps a value into [0, 255] in steps of 255. */
s32 func_15144C2C(s16 value) {
    s16 v = value;

    while (v >= 256) {
        v -= 255;
    }
    while (v < 0) {
        v += 255;
    }

    return v;
}

/* Unsigned angular distance between two angles in radians, in [0, pi]. */
f32 func_15144C8C(f32 a, f32 b) {
    f32 diff;

    a = func_15144B68(a);
    diff = fabsf(a - func_15144B68(b));
    if (3.1415927f < diff) {
        diff = 6.2831855f - diff;
    }
    return diff;
}

void func_150A7A00(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6, f32 *arg7);

/*
 * Projects world point pos through viewport's matrix (D_800D9D10) to screen coordinates in screen[2].
 * Returns 0 when the depth is outside (D_800D9B20, 6000). The optional outputs get the projection's
 * remaining terms and 1 / depth.
 */
s32 func_15144CEC(f32 *pos, f32 *screen, f32 *outA, f32 *outDepth, f32 *outInvDepth, u8 viewport) {
    f32 a;
    f32 depth;
    f32 invDepth;
    f32 x;
    f32 y;

    if (outA == NULL) {
        outA = &a;
    }
    if (outDepth == NULL) {
        outDepth = &depth;
    }
    if (outInvDepth == NULL) {
        outInvDepth = &invDepth;
    }
    func_150A7A00((s32)((u8 *)D_800D9D10 + viewport * sizeof(Mtx)), pos[0], pos[1], pos[2], screen, screen + 1,
                  outA, outDepth);
    if ((6000.0f <= *outDepth) || (*outDepth <= D_800D9B20)) {
        return 0;
    }
    if (*outDepth != 0.0f) {
        *outInvDepth = 1.0f / *outDepth;
    } else {
        return 0;
    }
    x = D_800BE628[viewport].halfWidth;
    y = D_800BE628[viewport].halfHeight;
    screen[0] = D_800BE628[viewport].centerX + (screen[0] * (x + 5.0f)) * *outInvDepth;
    screen[1] = D_800BE628[viewport].centerY - (screen[1] * (y + 5.0f)) * *outInvDepth;
    return 1;
}

/* A triangle as three s16 vertices. */
typedef struct {
    s16 v0[3];
    s16 v1[3];
    s16 v2[3];
} Tri15144E80;

/*
 * Edge vectors of a triangle: normal = (v0 - v1) x (v2 - v1), binormal = edge x normal, edge = v0 - v1.
 * Returns 0 for a degenerate triangle (two equal vertices). `normal` may be NULL.
 */
s32 func_15144E80(Tri15144E80 *tri, Vec3f *binormal, Vec3f *edgeOut, Vec3f *normal) {
    f32 c[3];
    f32 b[3];
    f32 a[3];
    Vec3f normalBuf;
    Vec3f edge0;
    Vec3f edge1;

    if (normal == NULL) {
        normal = &normalBuf;
    }

    a[0] = tri->v0[0];
    a[1] = tri->v0[1];
    a[2] = tri->v0[2];
    b[0] = tri->v1[0];
    b[1] = tri->v1[1];
    b[2] = tri->v1[2];
    c[0] = tri->v2[0];
    c[1] = tri->v2[1];
    c[2] = tri->v2[2];

    if ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2])) {
        return 0;
    }
    if ((a[0] == c[0]) && (a[1] == c[1]) && (a[2] == c[2])) {
        return 0;
    }
    if ((b[0] == c[0]) && (b[1] == c[1]) && (b[2] == c[2])) {
        return 0;
    }

    edge0.x = a[0] - b[0];
    edge0.y = a[1] - b[1];
    edge0.z = a[2] - b[2];
    edge1.x = c[0] - b[0];
    edge1.y = c[1] - b[1];
    edge1.z = c[2] - b[2];

    func_151450B4(&edge0, &edge1, normal);
    func_151450B4(&edge0, normal, binormal);
    *edgeOut = edge0;
    return 1;
}

/* Cross product out = a x b. */
void func_151450B4(Vec3f *a, Vec3f *b, Vec3f *out) {
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

/* Normalizes v into out; returns 0 for a zero vector. length and invLength are optional outputs. */
s32 func_15145128(Vec3f *v, Vec3f *out, f32 *length, f32 *invLength) {
    f32 sq;
    f32 invBuf;

    if (invLength == NULL) {
        invLength = &invBuf;
    }
    sq = v->x * v->x + v->y * v->y + v->z * v->z;
    if (sq == 0.0f) {
        return 0;
    }
    if (length != NULL) {
        *length = sqrtf(sq);
        *invLength = 1.0f / sqrtf(sq);
    } else {
        *invLength = 1.0f / sqrtf(sq);
    }
    out->x = *invLength * v->x;
    out->y = *invLength * v->y;
    out->z = *invLength * v->z;
    return 1;
}

s32 func_151452C4(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, Vec3f *hitNear,
                  Vec3f *hitFar, f32 *tNear, f32 *tFar);

/* Ray/sphere test (see func_151452C4) that also requires the hit to be closer than maxDist. */
s32 func_151451F0(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, f32 maxDist, Vec3f *hitNear,
                  Vec3f *hitFar, f32 *tNear, f32 *tFar) {
    f32 t;

    if (func_151452C4(origin, dir, center, radius, hitNear, hitFar, tNear, tFar) != 0) {
        t = *tNear;
        if (t < 0.0f) {
            if (*tFar < 0.0f) {
                return 0;
            }
        }
        if (0.0f <= t) {
            if (*tFar < 0.0f) {
                return 1;
            }
        }
        if (t < maxDist) {
            return 1;
        }
        return 0;
    }
    return 0;
}

/*
 * Intersects the ray origin + t * dir (dir normalized) with a sphere. Writes both intersection points
 * and their t; returns 0 on a miss or when the near point is behind the origin.
 */
s32 func_151452C4(Vec3f *origin, Vec3f *dir, Vec3f *center, f32 radius, Vec3f *hitNear,
                  Vec3f *hitFar, f32 *tNear, f32 *tFar) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f d;
    Vec3f o;
    f32 proj;
    f32 rr;
    f32 root;
    f32 t0;
    f32 t1;
    f32 dd;

    dx = center->x - origin->x;
    dy = center->y - origin->y;
    dz = center->z - origin->z;
    d = *dir;
    o = *origin;
    dd = (dx * dx) + (dy * dy) + (dz * dz);
    proj = (dx * d.x) + (dy * d.y) + (dz * d.z);
    rr = radius * radius;
    if (rr < dd - (proj * proj)) {
        return 0;
    }
    root = sqrtf(rr - (dd - (proj * proj)));
    if (proj < root) {
        root = -root;
    }
    t0 = proj - root;
    t1 = proj + root;
    hitNear->x = (t0 * d.x) + o.x;
    hitNear->y = (t0 * d.y) + o.y;
    hitNear->z = (t0 * d.z) + o.z;
    *tNear = t0;
    hitFar->x = (t1 * d.x) + o.x;
    hitFar->y = (t1 * d.y) + o.y;
    hitFar->z = (t1 * d.z) + o.z;
    *tFar = t1;
    {
        Vec3f diff;

        diff.x = hitNear->x - origin->x;
        diff.y = hitNear->y - origin->y;
        diff.z = hitNear->z - origin->z;
        if (func_15144A74(&diff, dir) < 0.0f) {
            return 0;
        }
        return 1;
    }
}

/* Whether point lies within radius of D_800DBFF0[index]'s position (unk2F8). */
s32 func_151454BC(u8 index, f32 radius, Vec3f *point) {
    f32 dx;
    f32 dy;
    f32 dz;
    Vec3f *pos;

    pos = func_15144B34(index);
    dx = point->x - pos->x;
    dy = point->y - pos->y;
    dz = point->z - pos->z;

    if ((radius * radius) < ((dx * dx) + (dy * dy) + (dz * dz))) {
        return 0;
    }
    return 1;
}

s32 func_1514563C(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t);

/* Closest point to `point` on the segment start .. start + dir, into out (t optional). */
void func_15145548(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t) {
    f32 tBuf;

    if (t == NULL) {
        t = &tBuf;
    }
    if (func_1514563C(start, dir, point, out, t) != 0) {
        if (*t < 0.0f) {
            *out = *start;
        } else if (1.0f < *t) {
            out->x = start->x + dir->x;
            out->y = start->y + dir->y;
            out->z = start->z + dir->z;
        }
    } else {
        *out = *start;
    }
}

/* Closest point to `point` on the line start + t * dir, into out; returns 0 when dir is zero. */
s32 func_1514563C(Vec3f *start, Vec3f *dir, Vec3f *point, Vec3f *out, f32 *t) {
    f32 tBuf[1];
    f32 denom;
    f32 dot0;
    f32 dot1;

    if (t == NULL) {
        // matching: the fallback slot is 3 words below the buffer (sp+0x10); no declaration places it there
        t = &tBuf[-3];
    }
    denom = (dir->x * dir->x) + (dir->y * dir->y) + (dir->z * dir->z);
    if (denom == 0.0f) {
        return 0;
    }
    dot0 = (dir->x * start->x) + (dir->y * start->y) + (dir->z * start->z);
    dot1 = (dir->x * point->x) + (dir->y * point->y) + (dir->z * point->z);
    *t = (dot1 - dot0) / denom;
    out->x = start->x + (*t * dir->x);
    out->y = start->y + (*t * dir->y);
    out->z = start->z + (*t * dir->z);
    return 1;
}

/*
 * Aim of an actor (or of the vehicle it rides, model 150 with matrix physics): writes the point
 * 2000 units along its yaw/pitch to target and, optionally, unit-1000 direction vectors for the
 * pitch and for the pitch plus pitchOffset (radians).
 */
void func_15145740(Actor *actor, Vec3f *target, Vec3f *dir, Vec3f *offsetDir, f32 pitchOffset) {
    s16 yaw;
    s16 pitch;
    volatile f32 angle;
    f32 offsetPitchRad;
    f32 horizontal[1];
    f32 pitchDeg;
    f32 temp;
    f32 yawRad[1];
    f32 pitchRad[1];

    if ((actor->id == 150) && (actor->unk31C->matrix_physics != 0)) {
        yaw = actor->unk7A + actor->unk31C->unk80;
    } else if (actor->unk31C != NULL) {
        yaw = actor->unk7A - actor->unk31C->unk12;
    } else {
        yaw = actor->unk7A;
    }
    if ((actor->id == 150) && (actor->unk31C->matrix_physics != 0)) {
        pitch = actor->unk31C->unk82 + 1024;
    } else {
        pitch = actor->unk1D1 * 200;
    }
    // matching: this scope with its unused pad is a uopt block boundary, and the dead store to
    // pitch keeps pitchDeg from being copy-propagated
    {
        s32 pad0;

        pitchDeg = pitch * (360.0f / 65536.0f);
        pitch = 0;
    }
    // matching: the volatile round trip and the pitchDeg/temp copies steer IDO's FP register colouring
    angle = pitchDeg;
    temp = angle;
    pitchDeg = temp;
    temp = temp * 0.01745329238f; /* degrees to radians */
    pitchRad[0] = temp;
    func_1505A184(yaw, 2000.0f, pitchDeg, &target->x, &target->z, &target->y);
    if (dir != NULL) {
        dir->y = cosf(pitchRad[0]) * 1000.0f;
        horizontal[0] = sinf(pitchRad[0]) * 1000.0f;
        yawRad[0] = yaw * 9.587380191e-05f; /* 2 pi / 65536 */
        dir->x = horizontal[0] * cosf(yawRad[0]);
        dir->z = -horizontal[0] * sinf(yawRad[0]);
        if (offsetDir != NULL) {
            offsetPitchRad = pitchRad[0] + pitchOffset;
            offsetDir->y = cosf(offsetPitchRad) * 1000.0f;
            horizontal[0] = sinf(offsetPitchRad) * 1000.0f;
            offsetDir->x = horizontal[0] * cosf(yawRad[0]);
            offsetDir->z = -horizontal[0] * sinf(yawRad[0]);
        }
    }
}

/* Heading (and optionally elevation, from -90) in degrees of a vector; func_150484A0 is atan2. */
void func_15145974(Vec3f *v, f32 *heading, f32 *elevation) {
    *heading = func_150484A0(v->x, v->z) * 57.29577637f;
    if (elevation != NULL) {
        *elevation = (func_150484A0(sqrtf(v->x * v->x + v->z * v->z), v->y) * 57.29577637f) - 90.0f;
    }
}

f32 func_15145A0C(f32 arg0, f32 scale, f32 arg2) {
    return D_800A548C[(s32)(arg0 * arg2 * 100.0f)] * scale;
}

void func_15145A50(Actor *actor) {
    actor->unk5 = 3;
    if (D_800BE9F0 != 51) {
        if ((D_800BE616 != 0) || (actor->interaction_state == 5) || (actor->interaction_state == 1) ||
            (actor->interaction_state == 21)) {
            actor->interaction_state = 5;
            if (actor->unk31C != NULL) {
                actor->unk31C->unk78 = 0;
            }
        } else {
            func_15053694(actor);
        }
    }
}

void func_1515C1A0(void *, Vec3f *, f32 *, f32 *);

/*
 * Ray (start, dir) against the actor's collision ellipsoid (centre/radius from func_1515C1A0, y squashed
 * by unkDC); writes the near and far hits, unsquashed by unkE0. Returns 0 on a miss.
 */
s32 func_15145AD8(Vec3f *start, Vec3f *dir, Actor *actor, Vec3f *hitNear, Vec3f *hitFar,
                  f32 *radius, f32 *arg6, Vec3f *center) {
    Vec3f centerBuf;
    f32 radiusBuf;
    f32 arg6Buf;
    Vec3f s;
    Vec3f d;
    Vec3f c;

    /* sic: the fallbacks replace the caller's pointers when they are given */
    if (radius != NULL) {
        radius = &radiusBuf;
    }
    if (arg6 != NULL) {
        arg6 = &arg6Buf;
    }
    if (center != NULL) {
        center = &centerBuf;
    }
    func_1515C1A0(actor, center, radius, arg6);
    if (*arg6 == 0.0f) {
        return 0;
    }
    if (*radius == 0.0f) {
        return 0;
    }
    {
    f32 length;
    f32 tNear;
    f32 tFar;
    f32 squash;
    f32 unsquash;
    squash = actor->unkDC;
    unsquash = actor->unkE0;
    s.x = start->x;
    s.y = start->y * squash;
    s.z = start->z;
    d.x = dir->x;
    d.y = dir->y * squash;
    d.z = dir->z;
    {
    f32 invLength;
    if (func_15145128(&d, &d, &length, &invLength) == 0) {
        return 0;
    }
    c.x = center->x;
    c.y = center->y * squash;
    c.z = center->z;
    if (func_151451F0(&s, &d, &c, *radius, length, hitNear, hitFar, &tNear, &tFar) == 0) {
        return 0;
    }
    hitNear->y *= unsquash;
    hitFar->y *= unsquash;
    }
    }
    return 1;
}

/* Whether entry `index` of D_800DBEF4 has viewFlags bit 0x80 (hidden) (true for a negative index). */
u8 func_15145C90(s32 index) {
    if (index < 0) {
        return 1;
    } else {
        return (D_800DBEF4[index].viewFlags & 0x80) == 0x80;
    }
}

/* Transforms `count` points (given by pointer) by the object's rotation and position. */
void func_15145CD0(WorldObject *placement, f32 **in, f32 **out, s32 count) {
    MtxF mf;
    f32 **src;
    f32 **dst;
    f32 *p;
    f32 *q;

    func_150A8050(mf, placement->rotX, placement->rotY, placement->rotZ);
    mf[3][0] = placement->x;
    mf[3][1] = placement->y;
    mf[3][2] = placement->z;
    if (count <= 0) {
        return;
    }
    src = in;
    dst = out;
    do {
        p = *src;
        q = *dst;
        func_150A7960(mf[0], p[0], p[1], p[2], q, q + 1, q + 2);
        count--;
        src++;
        dst++;
    } while (count > 0);
}

/* Transforms the `count` points in[] by the object's rotation and position into out[]. */
void func_15145DB4(WorldObject *placement, Vec3f *in, Vec3f *out, s32 count) {
    MtxF mf;
    s32 i;

    func_150A8050(mf, placement->rotX, placement->rotY, placement->rotZ);
    mf[3][0] = placement->x;
    mf[3][1] = placement->y;
    mf[3][2] = placement->z;
    for (i = 0; count > 0; i++) {
        func_150A7960(mf[0], in[i].x, in[i].y, in[i].z, &out[i].x, &out[i].y, &out[i].z);
        count--;
    }
}

/* Array version of func_15143134: mfs each offsets[i] by mtx into results[i]. */
void func_15145EA4(f32 **offsets, f32 **results, Mtx *mtx, s32 count) {
    f32 *input;
    MtxF mf;
    f32 *output;
    f32 **outputs;

    if (D_800C3E90 != 0) {
        guMtxL2F(mf, mtx);
        if (count > 0) {
            f32 **inputs = offsets;

            outputs = results;
            do {
                input = *inputs;
                if (input != NULL && (input[0] != 0.0f || input[1] != 0.0f || input[2] != 0.0f)) {
                    output = *outputs;
                    func_150A7960(mf[0], input[0], input[1], input[2], output, output + 1, output + 2);
                } else {
                    func_15142314(mtx, 0, *outputs);
                }
                count--;
                inputs++;
                outputs++;
            } while (count > 0);
        }
    } else {
        if (count > 0) {
            f32 **inputs = offsets;

            outputs = results;
            do {
                input = *inputs;
                if (input != NULL && (input[0] != 0.0f || input[1] != 0.0f || input[2] != 0.0f)) {
                    output = *outputs;
                    func_150A7960((f32 *)mtx, input[0], input[1], input[2], output, output + 1, output + 2);
                } else {
                    func_15142314(mtx, 0, *outputs);
                }
                count--;
                inputs++;
                outputs++;
            } while (count > 0);
        }
    }
}
/*
 * Builds two unit vectors u, v perpendicular to n (axis-aligned when n lies along an axis).
 * Returns 0 for a zero n.
 */
s32 func_15146078(Vec3f *n, Vec3f *u, Vec3f *v) {
    u8 count;
    u8 axis;
    u8 zeroAxis;
    u32 i;
    u32 j;
    u32 k;
    f32 len;
    f32 invLen;

    if ((n->x == 0.0f) && (n->y == 0.0f) && (n->z == 0.0f)) {
        return 0;
    }

    count = 0;
    if (n->x == 0.0f) {
        count++;
        zeroAxis = 0;
    } else {
        axis = 0;
    }
    if (n->y == 0.0f) {
        count++;
        zeroAxis = 1;
    } else {
        axis = 1;
    }
    if (n->z == 0.0f) {
        count++;
        zeroAxis = 2;
    } else {
        axis = 2;
    }

    // matching: double literals (0.0 / 1.0) here; f32 ones change IDO's FP register choice
    if (count == 2) {
        switch (axis) {
        case 0:
            u->x = 0.0;
            u->y = 1.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 0.0;
            v->z = 1.0;
            break;
        case 1:
            u->x = 1.0;
            u->y = 0.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 0.0;
            v->z = 1.0;
            break;
        case 2:
            u->x = 1.0;
            u->y = 0.0;
            u->z = 0.0;
            v->x = 0.0;
            v->y = 1.0;
            v->z = 0.0;
            break;
        }
        return 1;
    }

    i = 0;
    j = 1;
    k = 2;
    if ((count == 1) && (zeroAxis == 2)) {
        j = 2;
        k = 1;
    }
    ((f32 *)u)[i] = 1.0;
    ((f32 *)u)[j] = 1.0;
    ((f32 *)u)[k] = -((f32 *)n)[i] - (((f32 *)n)[j] / ((f32 *)n)[k]);
    func_151450B4(u, n, v);
    func_151450B4(v, n, u);
    func_15145128(u, u, &len, &invLen);
    func_15145128(v, v, &len, &invLen);
    return 1;
}
extern u8 D_800BE9C0;
extern u8 D_800D9BD0[][2][8];
extern s32 D_800D9E10[];
extern u8 D_800D9E20;
extern u8 D_800D9E21;
void *func_1515D914(void *, s16, s32, s32, s32, s32, s32, s32, s32, void *, s32, s32, s32, s32);
void *func_1515E544(void *, s32, u8, u8, void *);
/*
 * Draws `slot` of a model: with the owning actor's own textures (mode 1, falling back to the shared
 * ones in mode 2 when the actor is gone or has none), or plainly through func_1515D914.
 */
void *func_151462C8(void *gfx, s32 *model, u8 mode, Actor *actor, u8 uniqueId, s16 slot, Vec3f *pos,
                    u8 flags, s32 arg8) {
    s32 tex;
    s32 unk18;

    tex = model[slot + 2];
    if (tex != 0) {
        unk18 = model[6];
        if (unk18 != 0) {
            if (mode == 1) {
                if (actor != NULL) {
                    if ((actor->interaction_state == 0) || (actor->unique_id != uniqueId) ||
                        (actor->id == 0xFF) || (actor->unk302 == 0)) {
                        mode = 2;
                    }
                } else {
                    mode = 0;
                }
            }
            switch (mode) {
            case 1:
                gfx = func_1515E544(gfx, actor->unk304[slot],
                                    actor->unk301, actor->unk302,
                                    actor->unk314);
                break;
            case 2:
                gfx = func_1515E544(gfx, D_800D9E10[slot], D_800D9E20, D_800D9E21, D_800D9BD0[slot][D_800BE9C0]);
                break;
            case 0:
            default:
                gfx = func_1515D914(gfx, slot, (s32)pos->x, (s32)pos->y, (s32)pos->z, arg8, tex, model[0],
                                    unk18, model + 1, model[7], 0,
                                    ((flags & 1) ? 2 : 0) | (((flags & 2) ? 0x10 : 0) | 8), 0);
                break;
            }
        }
    }
    return gfx;
}

extern s32 D_80082FA0;

/* Whether none of the players' bits (0 .. D_80082FA0) are set in flags[1]. */
u8 func_151464B8(s16 *flags) {
    s32 i;
    s16 mask;
    s32 ret;

    i = 0;
    mask = 0;
    for (; i <= D_80082FA0; i++) {
        mask |= 1 << i;
    }
    ret = (flags[1] & mask) == 0;
    return ret;
}

/* Broadcasts message 45 (see func_151419D0) linking actors a and b. */
void func_15146508(Actor *a, Actor *b) {
    struct193 msg;

    msg.unk0 = a;
    msg.unk4 = b;
    msg.unk8 = a->unique_id;
    msg.unk9 = b->unique_id;
    func_15169040(&msg, 45, a, b);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x3E8];
    /* 0x3E8 */ Mtx *mtx[3]; /* per frame buffer (D_800BE9C0) */
    /* 0x3F4 */ u8 pad3F4[2];
    /* 0x3F6 */ u8 ready;
} AttachModel;

/* Where an attachment point's matrix comes from (see func_1514654C). */
typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u8 bone;      /* index into the actor's own matrices (unk1D4) */
    /* 0x03 */ u8 pad3[0x1B];
    /* 0x1E */ u16 partId;   /* part looked up with func_1503195C */
    /* 0x20 */ u16 partBone;
    /* 0x22 */ u8 pad22[0x12];
    /* 0x34 */ Mtx *mtx;     /* per frame buffer */
    /* 0x38 */ u8 pad38[0x10];
    /* 0x48 */ AttachModel *model;
} AttachPoint;

s32 func_15031070(ActorCtl *, Actor *, Mtx **, s32 *);
void func_15145EA4(f32 **, f32 **, Mtx *, s32);

/*
 * Transforms `count` offsets by the matrix of an attachment point on the actor: a model's matrix,
 * a part's bone, a fixed matrix, or else one of the actor's bones. Returns 0 if it is unavailable.
 */
s32 func_1514654C(Actor *actor, AttachPoint *point, s32 index, f32 **offsets, f32 **results, s32 count) {
    s32 i;
    Mtx *mtx;
    ActorCtl *part;
    Mtx *partMtx;
    s32 partCount;
    MtxF mf;

    if ((actor == NULL) || (point == NULL) || (actor->unk1D4 == NULL)) {
        return 0;
    }
    if (point->model != NULL) {
        if (point->model->ready == 0) {
            return 0;
        }
        mtx = point->model->mtx[D_800BE9C0] + index;
    } else if (point->partId) {
        part = func_1503195C(actor, point->partId, 0);
        if (part == NULL) {
            return 0;
        }
        if (func_15031070(part, actor, &partMtx, &partCount) == 0) {
            return 0;
        }
        /* func_1503195C returns a part object, not an ActorCtl (functions.h types it ActorCtl *) */
        if (*(s32 *)((u8 *)part + 0x48) != 0) {
            mtx = &partMtx[point->partBone];
        } else {
            mtx = partMtx;
        }
    // matching: an unused label ends this branch's block, changing IDO's branch layout
    end:;
    } else if (point->mtx != NULL) {
        mtx = point->mtx + D_800BE9C0;
    } else {
        func_15145EA4(offsets, results, (Mtx *)actor->unk1D4 + point->bone, count);
        return 1;
    }
    if (mtx != NULL) {
        guMtxL2F(mf, mtx);
        for (i = 0; i < count; i++) {
            func_150A7960(mf[0], offsets[i][0], offsets[i][1], offsets[i][2], results[i], results[i] + 1,
                          results[i] + 2);
        }
    } else {
        return 0;
    }
    return 1;
}

/* Whether a position lies inside the world bounds (|x|, |z| <= 19500 and -9500 <= y <= 19500). */
s32 func_1514672C(Vec3f *pos) {
    f32 limit;
    f32 y;

    limit = 19500.0f;
    if ((limit < fabsf(pos->x)) || (limit < fabsf(pos->z)) || (y = pos->y, (limit < y)) || (y < -9500.0f)) {
        return 0;
    }
    return 1;
}

/*
 * Random drift: when the timer runs out it restarts at a random time below maxTime and picks a new
 * target in [lo, mid) (3 in 4) or [mid, hi); value then eases towards the target by rate.
 */
void func_151467A4(f32 *timer, f32 maxTime, f32 *target, f32 lo, f32 mid, f32 hi, f32 rate, f32 *value) {
    *timer = *timer - D_800BE9A4;
    if (*timer < 0.0f) {
        *timer = func_150ADA68() * maxTime;
        if ((func_150ADA20() & 3) != 0) {
            *target = (func_150ADA68() * (mid - lo)) + lo;
        } else {
            *target = (func_150ADA68() * (hi - mid)) + mid;
        }
    }
    *value = ((*target - *value) * rate) + *value;
}
