#include <ultra64.h>

#define func_15083E90 func_15083E90_s32_proto
#include "functions.h"
#undef func_15083E90
#define D_800D3098 D_800D3098_struct178_view
#include "variables.h"
#undef D_800D3098

extern f32 D_800A5698;
extern f32 D_800A5694;
extern f32 D_800A569C;
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
extern s32 D_800DCA00;
extern Mtx *D_800DCA04;
extern f32 D_800DCA08;
extern f32 D_800DCA0C;
extern f32 D_800DCA10;

struct127 *func_15083E90();
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
void func_15142314(Mtx *, s32, f32 *);
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
    /* 0x04 */ struct127 *owner;
    /* 0x08 */ u8 ownerId;     /* owner->unique_id */
} EffectInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 duration;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ EffectInfo info;
} AttachedEffect;

/* Node of the list an actor keeps at 0x2F4 (see func_1514EC1C / func_1514ECE0). */
typedef struct AttachNode {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ AttachedEffect *effect;
    /* 0x14 */ struct AttachNode *next;
} AttachNode;

/* struct127 (shared header) leaves the attachment list head at 0x2F4 inside pad2F0. */
#define ACTOR_ATTACHMENTS(actor) (*(AttachNode **)&(actor)->pad2F0[4])

/* List tag of the attached timed effects. */
#define ATTACH_TIMED_EFFECT 0x1A

typedef s32 (*EffectSelector)(s32, struct127 *);
typedef void (*EffectHandler)(struct127 *, s32, s32);

s32 func_15141C0C(struct127 *actor);
s32 func_15141CC0(u32 arg0);
u32 func_1510F8CC(s32);
void func_15141E38(struct127 *actor, s32 handler);
s32 func_1514ECE0(AttachNode *, s16, AttachNode **);

/* Runs the actor's model-specific effect selector, then every attached timed effect's handler. */
void func_15141A7C(struct127 *actor, s32 arg1) {
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
        AttachNode *node;
        node = ACTOR_ATTACHMENTS(actor);
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
s32 func_15141C0C(struct127 *actor) {
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
void func_15141E38(struct127 *actor, s32 handler) {
    AttachNode *found;
    AttachNode *node;
    s32 pad; // matching: unused stack slot (keeps `info` at sp+0x40)

    found = NULL;
    node = ACTOR_ATTACHMENTS(actor);
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
        effect = (AttachedEffect *)func_15149130(D_8008A0B4[handler].unk4, -1, -1, -1, 1, 50, (struct37 *)0xC, 0xFF, 1);
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
void func_15141F78(u8 kind, Source15141F78 *src, f32 scale, u8 arg3, struct17 *pos, u8 arg5) {
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

    func_1513C650((s32)&packet, 0, 0, (s32)src->unk4, pos->unk0, src->unk0, pos->unk8, size, size, arg3, flag, 3, 1, 0, 0xFF, 1);
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
    if (func_150A2AEC((actor - (s32)D_800CC2D0) / (s32)sizeof(struct127), 6, &args, actor) == -1) {
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
    /* 0x08 */ struct17 pos;
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

extern f32 D_800A5470;
extern f32 D_800A5474;

void func_15153F18(Params15153F18 *params, struct17 *pos, s32, s32, s32);

/* Spawns the func_15153F18 effect of the given kind at `pos`. */
void func_15142180(u8 kind, struct17 *pos, s32 arg2, f32 scaleA, f32 scaleB) {
    Params15153F18 params;

    params.pos = *pos;
    params.unk2C = 3;
    params.unk2E = 3;
    params.unk2 = 255;
    params.unk4 = -25;
    params.unk6 = 10;
    params.unk14 = 2.5f * scaleA;
    params.unk18 = (1.0f + 1.0f) * scaleA;
    params.unk1C = D_800A5470;
    params.unk20 = D_800A5474;
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

s32 func_151422C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    return (arg3 + arg2) >> 1;
}

s32 func_151422DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    return arg4;
}

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
    f32 mf[4][4];
} MtxView;

/* Copies the translation of mtx[index] to out (fixed-point when D_800C3E90 is set). */
void func_15142314(Mtx *mtx, s32 index, f32 *out) {
    MtxView *m;

    m = (MtxView *)((u8 *)mtx + index * sizeof(Mtx));
    if (D_800C3E90 != 0) {
        // matching: the original recomputes m here; IDO keeps both computations
        m = (MtxView *)((u8 *)mtx + index * sizeof(Mtx));
        {
            f32 scale = 1.0f / 65536.0f;
            out[0] = ((f32)(m->fx.whole[3][0] << 16) + (f32)m->fx.frac[3][0]) * scale;
            out[1] = ((f32)(m->fx.whole[3][1] << 16) + (f32)m->fx.frac[3][1]) * scale;
            out[2] = ((f32)(m->fx.whole[3][2] << 16) + (f32)m->fx.frac[3][2]) * scale;
        }
        return;
    }
    out[0] = m->mf[3][0];
    out[1] = m->mf[3][1];
    out[2] = m->mf[3][2];
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
struct127 *func_15142444(u8 uniqueId, struct127 *self) {
    s32 id;
    struct127 *actor;

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
    f32 mf[4][4];

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
    f32 mf[4][4];
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
    f32 mf[4][4];

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
void func_15142914(f32 mf[4][4], f32 scaleXZ, f32 scaleY, f32 rotX, f32 rotY, f32 rotZ, f32 x, f32 y, f32 z) {
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

s32 func_15142A5C(struct127 *actor) {
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
void func_15143134(struct17 *offset, f32 *out, Mtx *mtx) {
    f32 mf[4][4];

    D_800DCA00 = 1;
    if ((offset != NULL) && ((offset->unk0 != 0.0f) || (offset->unk4 != 0.0f) || (offset->unk8 != 0.0f))) {
        D_800DCA00 = 2;
        D_800DCA08 = offset->unk0;
        D_800DCA0C = offset->unk4;
        D_800DCA10 = offset->unk8;
        if (D_800C3E90) {
            D_800DCA00 = 3;
            D_800DCA04 = mtx;
            guMtxL2F(mf, mtx);
            func_150A7960(mf[0], offset->unk0, offset->unk4, offset->unk8, out, out + 1, out + 2);
            D_800DCA00 = 4;
        // matching: an (unused) label here changes how IDO lays out the branch to the final store
        state4_done:;
        } else {
            D_800DCA00 = 5;
            D_800DCA04 = mtx;
            func_150A7960((f32 *)mtx, offset->unk0, offset->unk4, offset->unk8, out, out + 1, out + 2);
            D_800DCA00 = 6;
        }
    } else {
        D_800DCA00 = 7;
        func_15142314(mtx, 0, out);
        D_800DCA00 = 8;
    }
    D_800DCA00 = 0;
}

/* A spawn volume: a cylinder or box around (x, y, z). */
typedef struct Game16EE20Emitter {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radiusX;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 radiusZ;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 rotation;
    /* 0x14 */ u8 pad14;
    /* 0x15 */ u8 flags; /* bits 0-1: shape */
} Game16EE20Emitter;

/* Picks a random point in the emitter's volume: (x, z) and the top and bottom heights. */
void func_151432BC(Game16EE20Emitter *emitter, f32 *x, f32 *z, f32 *top, f32 *bottom) {
    switch (emitter->flags & 3) {
    default:
        *x = emitter->x;
        *z = emitter->z;
        *top = emitter->y + emitter->height;
        *bottom = emitter->y - emitter->height;
        return;
    case 2:
        /* rotated box */
        {
            u8 angle;
            f32 sine;
            f32 cosine;
            f32 offsetX;
            f32 offsetZ;

            angle = emitter->rotation * (256.0f / 360.0f);
            sine = func_151423D8(angle - 0x40);
            cosine = func_151423D8(angle);
            offsetX = func_150ADA68() * (2.0f * emitter->radiusX) + -emitter->radiusX;
            offsetZ = func_150ADA68() * (2.0f * emitter->radiusZ) + -emitter->radiusZ;
            *x = emitter->x + (offsetX * cosine + offsetZ * sine);
            *z = emitter->z + (offsetZ * cosine - offsetX * sine);
            *top = emitter->y + emitter->height;
            *bottom = emitter->y;
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
            radius = func_150ADA68() * emitter->radiusX;
            *x = emitter->x + radius * cosine;
            *z = emitter->z - radius * sine;
            *top = emitter->y + emitter->height;
            *bottom = emitter->y;
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
            radius = func_150ADA68() * emitter->radiusX;
            *x = emitter->x + radius * cosine;
            *z = emitter->z - radius * sine;
            *top = emitter->y + emitter->height;
            *bottom = emitter->y - emitter->height;
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

/* File-local shadow of struct178: the shared header types 0x06..0x33 as one u8 blob,
   which cannot express the s16/f32/u8/s32 members this search compares. */
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ u8  unk14;
    /* 0x15 */ u8  unk15;
    /* 0x16 */ u8  unk16;
    /* 0x17 */ u8  unk17;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ u8  unk24[0x10];
} ObjRec; /* size 0x34 */

/* variables.h declares D_800D3098 as struct178[73]; the asm loads it as a pointer (lw 0(D_800D3098)). */
extern ObjRec *D_800D3098;

void func_15143D18(s32 *start, s32 *end, s32 lo, s32 hi);

/*
 * Searches D_800D3098[start..end) for a record equal to `key` in the fields selected by `fields`.
 * With bit 0x1000 every selected field must match, otherwise any one of them; the last hit wins.
 */
ObjRec *func_151438D8(s32 start, s32 end, u16 fields, ObjRec *key) {
    s32 i;
    ObjRec *result;
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
            if ((key->unk0 == D_800D3098[i].unk0) && (key->unk2 == D_800D3098[i].unk2) && (key->unk4 == D_800D3098[i].unk4)) {
                checked |= 1;
                matched |= 1;
            }
        } else {
            checked |= 1;
        }
        if (fields & 0x2) {
            if ((key->unk6 == D_800D3098[i].unk6) && (key->unk8 == D_800D3098[i].unk8) && (key->unkA == D_800D3098[i].unkA)) {
                checked |= 0x2;
                matched |= 0x2;
            }
        } else {
            checked |= 0x2;
        }
        if (fields & 0x4) {
            if (key->unkC == D_800D3098[i].unkC) {
                checked |= 0x4;
                matched |= 0x4;
            }
        } else {
            checked |= 0x4;
        }
        if (fields & 0x8) {
            if (key->unk10 == D_800D3098[i].unk10) {
                checked |= 0x8;
                matched |= 0x8;
            }
        } else {
            checked |= 0x8;
        }
        if (fields & 0x10) {
            if (key->unk14 == D_800D3098[i].unk14) {
                checked |= 0x10;
                matched |= 0x10;
            }
        } else {
            checked |= 0x10;
        }
        if (fields & 0x20) {
            if (key->unk15 == (D_800D3098[i].unk15 >> 2)) {
                checked |= 0x20;
                matched |= 0x20;
            }
        } else {
            checked |= 0x20;
        }
        if (fields & 0x40) {
            if (key->unk16 == D_800D3098[i].unk16) {
                checked |= 0x40;
                matched |= 0x40;
            }
        } else {
            checked |= 0x40;
        }
        if (fields & 0x80) {
            if (key->unk17 == D_800D3098[i].unk17) {
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
s32 func_15143E08(struct127 *actor) {
    return (((s32)actor->unk7A >> 8) + 64) & 0xFF;
}

/* The actor's facing angle in 256ths of a turn, relative to unk31C's angle when it has one. */
s32 func_15143E24(struct127 *actor) {
    struct126 *ref = actor->unk31C;
    if (ref != NULL) {
        return (s16)((actor->unk7A - ref->unk12) >> 8);
    }
    return (s16)(actor->unk7A >> 8);
}
/* Length of a vector. */
f32 func_15143E64(struct17 *v) {
    f32 x = v->unk0;
    f32 y = v->unk4;
    f32 z = v->unk8;

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
void func_151441A4(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, s32 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13) {
    s16 temp_v0;
    s16 temp_v0_2;

    switch (arg13) {
    case 2:
        *arg0 = (s16) arg8;
        *arg1 = (s16) arg9;
        *arg2 = (s16) arg10;
        *arg3 = (s16) arg11;
        return;
    case 0:
        *arg3 = 0;
        temp_v0 = *arg3;
        *arg2 = temp_v0;
        *arg1 = temp_v0;
        *arg0 = temp_v0;
        return;
    case 1:
        *arg2 = 0;
        temp_v0_2 = *arg2;
        *arg1 = temp_v0_2;
        *arg0 = temp_v0_2;
        *arg3 = (s16) arg11;
        return;
    case 3:
        *arg0 = (s16) ((s32) (arg4 * arg12) >> 8);
        *arg1 = (s16) ((s32) (arg5 * arg12) >> 8);
        *arg2 = (s16) ((s32) (arg6 * arg12) >> 8);
        *arg3 = 0;
        return;
    case 4:
        *arg0 = (s16) ((s32) (arg4 * arg12) >> 8);
        *arg1 = (s16) ((s32) (arg5 * arg12) >> 8);
        *arg2 = (s16) ((s32) (arg6 * arg12) >> 8);
        *arg3 = 0;
        return;
    default:
        *arg0 = (s16) ((s32) (arg4 * arg12) >> 8);
        *arg1 = (s16) ((s32) (arg5 * arg12) >> 8);
        *arg2 = (s16) ((s32) (arg6 * arg12) >> 8);
        *arg3 = 0;
        return;
    }
}
void func_151442FC(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, u8 arg10, u8 arg11, u8 arg12, u8 arg13) {
    s16 temp_v0;
    s16 temp_v0_2;
    s16 temp_v0_3;

    switch (arg13) {
    case 2:
    case 3:
        *arg0 = (s16) arg4;
        *arg1 = (s16) arg5;
        *arg2 = (s16) arg6;
        *arg3 = (s16) arg7;
        return;
    case 13:
        *arg0 = (s16) arg4;
        *arg1 = (s16) arg5;
        *arg2 = (s16) arg6;
        *arg3 = 0;
        return;
    case 0:
        *arg3 = 0;
        temp_v0 = *arg3;
        *arg2 = temp_v0;
        *arg1 = temp_v0;
        *arg0 = temp_v0;
        return;
    case 1:
        *arg2 = (s16) arg12;
        *arg1 = (s16) arg12;
        *arg0 = (s16) arg12;
        *arg3 = 0;
        return;
    case 7:
    case 12:
        *arg2 = 0;
        temp_v0_2 = *arg2;
        *arg1 = temp_v0_2;
        *arg0 = temp_v0_2;
        *arg3 = (s16) arg11;
        return;
    case 8:
        *arg2 = 0;
        temp_v0_3 = *arg2;
        *arg1 = temp_v0_3;
        *arg0 = temp_v0_3;
        *arg3 = (s16) arg7;
        return;
    case 4:
        *arg2 = (s16) arg12;
        *arg1 = (s16) arg12;
        *arg0 = (s16) arg12;
        *arg3 = (s16) ((s32) (arg7 * arg11) >> 8);
        return;
    case 5:
        *arg2 = (s16) arg12;
        *arg1 = (s16) arg12;
        *arg0 = (s16) arg12;
        *arg3 = (s16) ((s32) (arg7 * arg11) >> 8);
        return;
    case 6:
        *arg0 = (s16) arg4;
        *arg1 = (s16) arg5;
        *arg2 = (s16) arg6;
        *arg3 = (s16) arg11;
        return;
    case 10:
        *arg0 = (s16) arg8;
        *arg1 = (s16) arg9;
        *arg2 = (s16) arg10;
        *arg3 = (s16) arg11;
        return;
    case 11:
        *arg0 = (s16) arg8;
        *arg1 = (s16) arg9;
        *arg2 = (s16) arg10;
        *arg3 = (s16) arg7;
        return;
    case 9:
        *arg2 = (s16) arg12;
        *arg1 = (s16) arg12;
        *arg0 = (s16) arg12;
        *arg3 = (s16) arg11;
        return;
    default:
        *arg2 = (s16) arg12;
        *arg1 = (s16) arg12;
        *arg0 = (s16) arg12;
        *arg3 = (s16) ((s32) (arg7 * arg11) >> 8);
        return;
    }
}
s32 func_151444DC(s32 arg0, s32 arg1, s32 arg2) {
    s32 range;
    if (arg1 < arg0) {
        range = arg1 - arg2 + 1;
        do {
            arg0 -= range;
        } while (arg1 < arg0);
    }
    if (arg0 < arg2) {
        range = arg1 - arg2 + 1;
        do {
            arg0 += range;
        } while (arg0 < arg2);
    }
    return arg0;
}
f32 func_15144528(f32 arg0, f32 arg1, f32 arg2) {
    f32 temp;

    if (arg1 < arg0) {
        temp = arg1 - arg2;
        do {
            arg0 -= temp;
        } while (arg1 < arg0);
    }
    if (arg0 < arg2) {
        temp = arg1 - arg2;
        do {
            arg0 += temp;
        } while (arg0 < arg2);
    }
    return arg0;
}
f32 func_15144598(struct134 *arg0) {
    f32 temp_f2;
    s16 temp_v0;
    s32 temp_t6;

    temp_t6 = *((u8 *)arg0 + 0x15) & 3;
    switch (temp_t6) {
    default:
        temp_f2 = 1.0f;
        break;
    case 2:
        temp_f2 = (f32)((s16)arg0->unk6 * (s16)arg0->unkA) * 4.0f;
        break;
    case 0:
    case 1:
        temp_v0 = (s16)arg0->unk6;
        temp_f2 = (f32)(temp_v0 * temp_v0) * D_800A5694;
        break;
    }
    return temp_f2;
}
f32 func_1514462C(s32 arg0) {
    struct169 *temp_a0;
    s32 temp_t6;
    f32 temp_f2;
    f32 temp_f0;

    temp_a0 = (struct169 *)arg0;
    temp_t6 = temp_a0->unk15 & 3;
    switch (temp_t6) {
    default:
        temp_f2 = 1.0f;
        break;
    case 2:
        temp_f2 = (f32)(temp_a0->unk6 * temp_a0->unk8 * temp_a0->unkA);
        break;
    case 0:
        temp_f2 = (f32)(temp_a0->unk6 * temp_a0->unk6) * D_800A5698 * (f32)temp_a0->unk8;
        break;
    case 1:
        temp_f0 = (f32)temp_a0->unk6;
        temp_f2 = ((temp_f0 * D_800A569C) * temp_f0) * temp_f0;
        break;
    }
    return temp_f2;
}
typedef struct Game16EE20SampleRegion {
    s16 x;
    s16 y;
    s16 z;
    s16 radiusX;
    s16 height;
    s16 radiusZ;
    u8 padC[9];
    u8 flags;
    u8 pad16[0xE];
    f32 cosineX;
    f32 sineX;
    f32 sineZ;
    f32 cosineZ;
} Game16EE20SampleRegion;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Func1514470C_Vec3f;

void func_1514470C(Game16EE20SampleRegion *arg0, f32 *arg1) {
    f32 radiusZ;
    f32 height;
    f32 radiusX;
    f32 cosineX;
    f32 sineX;
    f32 sineZ;
    f32 cosineZ;
    s32 mode;

    radiusX = arg0->radiusX;
    height = arg0->height;
    radiusZ = arg0->radiusZ;
    cosineX = arg0->cosineX;
    sineX = arg0->sineX;
    sineZ = arg0->sineZ;
    cosineZ = arg0->cosineZ;
    mode = arg0->flags & 3;
    switch (mode) {
    default:
        arg1[0] = arg0->x;
        arg1[1] = arg0->y;
        arg1[2] = arg0->z;
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
            arg1[0] = arg0->x + x;
            arg1[1] = arg0->y + y;
            rotatedZ = mixed * cosineZ - savedX * sineZ;
            arg1[2] = arg0->z + rotatedZ;
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
            arg1[0] = arg0->x + x;
            arg1[1] = arg0->y + y;
            arg1[2] = arg0->z + (mixed * cosineZ - savedX * sineZ);
        }
        return;
    case 1:
        {
            Func1514470C_Vec3f offset;

            func_151436B4(func_150ADA68() * 6.2831855f, func_150ADA68() * 6.2831855f,
                          func_150ADA68() * radiusX, &offset.x);
            arg1[0] = arg0->x + offset.x;
            arg1[1] = arg0->y + offset.y;
            arg1[2] = arg0->z + offset.z;
        }
        return;
    }
}
f32 func_15144A74(struct17 *arg0, struct17 *arg1) {
    return arg0->unk0 * arg1->unk0 + arg0->unk4 * arg1->unk4 + arg0->unk8 * arg1->unk8;
}
f32 func_15144AA8(s32 arg0) {
    f32 v = D_800DBFF0[arg0].unk380;
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
struct17 *func_15144B34(s32 arg0) {
    return (struct17 *)&D_800DBFF0[arg0].unk2F8;
}
f32 func_15144B68(f32 arg0);
extern f32 D_800A56A4;
f32 func_15144B68(f32 arg0)
{
  f32 new_var;
  new_var = D_800A56A4;
  if (1)
  {
    f32 v = arg0;
    if (new_var < v)
    {
      do
      {
        v -= new_var;
      }
      while (new_var < v);
    }
    if (v < 0.0f)
    {
      do
      {
        v += new_var;
      }
      while (v < 0.0f);
    }
    return v;
  }
}

f32 func_15144BC8(f32 arg0) {
    f32 v = arg0;
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

s32 func_15144C2C(s16 arg0) {
    s16 tmp1 = arg0;

    while (tmp1 >= 256)
    {
        tmp1 -= 255;
    }
    while (tmp1 < 0)
    {
        tmp1 += 255;
    }

    return tmp1;
}

f32 func_15144C8C(f32 arg0, f32 arg1) {
    f32 tmp;

    arg0 = func_15144B68(arg0);
    tmp = fabsf(arg0 - func_15144B68(arg1));
    if (D_800A56A8 < tmp) {
        tmp = D_800A56AC - tmp;
    }
    return tmp;
}

typedef struct {
    char pad0[0xC];
    f32 unkC;
    f32 unk10;
    char pad14[0x20];
    f32 unk34;
    f32 unk38;
    char pad3C[0x144];
} Struct15144CEC;
void func_150A7A00(s32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6, f32 *arg7);

s32 func_15144CEC(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, u8 arg5) {
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 x;
    f32 y;

    if (arg2 == NULL) {
        arg2 = &sp44;
    }
    if (arg3 == NULL) {
        arg3 = &sp40;
    }
    if (arg4 == NULL) {
        arg4 = &sp3C;
    }
    func_150A7A00((s32)((u8 *)&D_800D9D10 + (arg5 << 6)), arg0[0], arg0[1], arg0[2], arg1, arg1 + 1, arg2, arg3);
    if ((6000.0f <= *arg3) || (*arg3 <= D_800D9B20)) {
        return 0;
    }
    if (*arg3 != 0.0f) {
        *arg4 = 1.0f / *arg3;
    } else {
        return 0;
    }
    x = ((Struct15144CEC *)D_800BE628)[arg5].unkC;
    y = ((Struct15144CEC *)D_800BE628)[arg5].unk10;
    arg1[0] = ((Struct15144CEC *)D_800BE628)[arg5].unk34 + (arg1[0] * (x + 5.0f)) * *arg4;
    arg1[1] = ((Struct15144CEC *)D_800BE628)[arg5].unk38 - (arg1[1] * (y + 5.0f)) * *arg4;
    return 1;
}
struct struct_15144E80 {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
};

s32 func_15144E80(struct struct_15144E80 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3) {
    struct17 sp54;
    struct17 sp48;
    struct17 sp3C;
    struct17 sp30;
    struct17 sp24;
    struct17 sp18;

    if (arg3 != NULL) {
    } else {
        arg3 = &sp30;
    }

    ((struct17 *)&sp3C)->unk0 = (f32)arg0->unk0;
    ((struct17 *)&sp3C)->unk4 = (f32)arg0->unk2;
    ((struct17 *)&sp3C)->unk8 = (f32)arg0->unk4;
    ((struct17 *)&sp48)->unk0 = (f32)arg0->unk6;
    ((struct17 *)&sp48)->unk4 = (f32)arg0->unk8;
    ((struct17 *)&sp48)->unk8 = (f32)arg0->unkA;
    ((struct17 *)&sp54)->unk0 = (f32)arg0->unkC;
    ((struct17 *)&sp54)->unk4 = (f32)arg0->unkE;
    ((struct17 *)&sp54)->unk8 = (f32)arg0->unk10;

    if ((((struct17 *)&sp48)->unk0 == ((struct17 *)&sp3C)->unk0) && (((struct17 *)&sp48)->unk4 == ((struct17 *)&sp3C)->unk4) && (((struct17 *)&sp48)->unk8 == ((struct17 *)&sp3C)->unk8)) {
        return 0;
    }
    if ((((struct17 *)&sp54)->unk0 == ((struct17 *)&sp3C)->unk0) && (((struct17 *)&sp54)->unk4 == ((struct17 *)&sp3C)->unk4) && (((struct17 *)&sp54)->unk8 == ((struct17 *)&sp3C)->unk8)) {
        return 0;
    }
    if ((((struct17 *)&sp54)->unk0 == ((struct17 *)&sp48)->unk0) && (((struct17 *)&sp54)->unk4 == ((struct17 *)&sp48)->unk4) && (((struct17 *)&sp54)->unk8 == ((struct17 *)&sp48)->unk8)) {
        return 0;
    }

    sp24.unk0 = ((struct17 *)&sp3C)->unk0 - ((struct17 *)&sp48)->unk0;
    sp24.unk4 = ((struct17 *)&sp3C)->unk4 - ((struct17 *)&sp48)->unk4;
    sp24.unk8 = ((struct17 *)&sp3C)->unk8 - ((struct17 *)&sp48)->unk8;
    sp18.unk0 = ((struct17 *)&sp54)->unk0 - ((struct17 *)&sp48)->unk0;
    sp18.unk4 = ((struct17 *)&sp54)->unk4 - ((struct17 *)&sp48)->unk4;
    sp18.unk8 = ((struct17 *)&sp54)->unk8 - ((struct17 *)&sp48)->unk8;

    func_151450B4(&sp24, &sp18, arg3);
    func_151450B4(&sp24, arg3, arg1);
    *arg2 = sp24;
    return 1;
}

void func_151450B4(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    arg2->unk0 = arg0->unk4 * arg1->unk8 - arg0->unk8 * arg1->unk4;
    arg2->unk4 = arg0->unk8 * arg1->unk0 - arg0->unk0 * arg1->unk8;
    arg2->unk8 = arg0->unk0 * arg1->unk4 - arg0->unk4 * arg1->unk0;
}

s32 func_15145128(struct17 *arg0, struct17 *arg1, f32 *arg2, f32 *arg3) {
    f32 sq;
    f32 sp0;

    if (arg3 != 0) {
    } else {
        arg3 = &sp0;
    }
    sq = arg0->unk0 * arg0->unk0 + arg0->unk4 * arg0->unk4 + arg0->unk8 * arg0->unk8;
    if (sq == 0.0f) {
        return 0;
    }
    if (arg2 != 0) {
        *arg2 = sqrtf(sq);
        *arg3 = 1.0f / sqrtf(sq);
    } else {
        *arg3 = 1.0f / sqrtf(sq);
    }
    arg1->unk0 = *arg3 * arg0->unk0;
    arg1->unk4 = *arg3 * arg0->unk4;
    arg1->unk8 = *arg3 * arg0->unk8;
    return 1;
}
s32 func_151452C4(struct17 *arg0, struct17 *arg1, struct17 *arg2, f32 arg3, struct17 *arg4, struct17 *arg5, f32 *arg6, f32 *arg7);

s32 func_151451F0(struct17 *arg0, struct17 *arg1, struct17 *arg2, f32 arg3, f32 arg4, struct17 *arg5, struct17 *arg6, f32 *arg7, f32 *arg8) {
    f32 temp_f0;

    if (func_151452C4(arg0, arg1, arg2, arg3, arg5, arg6, arg7, arg8) != 0) {
        temp_f0 = *arg7;
        if (temp_f0 < 0.0f) {
            if (*arg8 < 0.0f) {
                return 0;
            }
        }
        if (0.0f <= temp_f0) {
            if (*arg8 < 0.0f) {
                return 1;
            }
        }
        if (temp_f0 < arg4) {
            return 1;
        }
        return 0;
    }
    return 0;
}
s32 func_151452C4(struct17 *arg0, struct17 *arg1, struct17 *arg2, f32 arg3, struct17 *arg4, struct17 *arg5, f32 *arg6, f32 *arg7) {
    f32 dx;
    f32 dy;
    f32 dz;
    struct17 dir;
    struct17 org;
    f32 proj;
    f32 rr;
    f32 root;
    f32 t0;
    f32 t1;
    f32 dd;

    dx = arg2->unk0 - arg0->unk0;
    dy = arg2->unk4 - arg0->unk4;
    dz = arg2->unk8 - arg0->unk8;
    dir = *arg1;
    org = *arg0;
    dd = (dx * dx) + (dy * dy) + (dz * dz);
    proj = (dx * dir.unk0) + (dy * dir.unk4) + (dz * dir.unk8);
    rr = arg3 * arg3;
    if (rr < dd - (proj * proj)) {
        return 0;
    }
    root = sqrtf(rr - (dd - (proj * proj)));
    if (proj < root) {
        root = -root;
    }
    t0 = proj - root;
    t1 = proj + root;
    arg4->unk0 = (t0 * dir.unk0) + org.unk0;
    arg4->unk4 = (t0 * dir.unk4) + org.unk4;
    arg4->unk8 = (t0 * dir.unk8) + org.unk8;
    *arg6 = t0;
    arg5->unk0 = (t1 * dir.unk0) + org.unk0;
    arg5->unk4 = (t1 * dir.unk4) + org.unk4;
    arg5->unk8 = (t1 * dir.unk8) + org.unk8;
    *arg7 = t1;
    {
        struct17 diff;

        diff.unk0 = arg4->unk0 - arg0->unk0;
        diff.unk4 = arg4->unk4 - arg0->unk4;
        diff.unk8 = arg4->unk8 - arg0->unk8;
        if (func_15144A74(&diff, arg1) < 0.0f) {
            return 0;
        }
        return 1;
    }
}

s32 func_151454BC(u8 arg0, f32 arg1, struct17 *arg2) {
    f32 tmp1;
    f32 tmp2;
    f32 tmp3;
    struct17 *temp_v0;

    temp_v0 = func_15144B34(arg0);
    tmp1 = arg2->unk0 - temp_v0->unk0;
    tmp2 = arg2->unk4 - temp_v0->unk4;
    tmp3 = arg2->unk8 - temp_v0->unk8;

    if ((arg1 * arg1) < ((tmp1 * tmp1) + (tmp2 * tmp2) + (tmp3 * tmp3))) {
        return 0;
    }
    return 1;
}

s32 func_1514563C(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 *arg4);

void func_15145548(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 *arg4) {
    f32 sp0;

    if (arg4 != 0) {
    } else {
        arg4 = &sp0;
    }
    if (func_1514563C(arg0, arg1, arg2, arg3, arg4) != 0) {
        if (*arg4 < 0.0f) {
            *arg3 = *arg0;
        } else if (1.0f < *arg4) {
            arg3->unk0 = arg0->unk0 + arg1->unk0;
            arg3->unk4 = arg0->unk4 + arg1->unk4;
            arg3->unk8 = arg0->unk8 + arg1->unk8;
        }
    } else {
        *arg3 = *arg0;
    }
}
s32 func_1514563C(struct17 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 *arg4) {
    f32 sp1C[1];
    f32 denom;
    f32 dot0;
    f32 dot1;

    if (arg4 == NULL) {
        arg4 = &sp1C[-3];
    }
    denom = (arg1->unk0 * arg1->unk0) + (arg1->unk4 * arg1->unk4) + (arg1->unk8 * arg1->unk8);
    if (denom == 0.0f) {
        return 0;
    }
    dot0 = (arg1->unk0 * arg0->unk0) + (arg1->unk4 * arg0->unk4) + (arg1->unk8 * arg0->unk8);
    dot1 = (arg1->unk0 * arg2->unk0) + (arg1->unk4 * arg2->unk4) + (arg1->unk8 * arg2->unk8);
    *arg4 = (dot1 - dot0) / denom;
    arg3->unk0 = arg0->unk0 + (*arg4 * arg1->unk0);
    arg3->unk4 = arg0->unk4 + (*arg4 * arg1->unk4);
    arg3->unk8 = arg0->unk8 + (*arg4 * arg1->unk8);
    return 1;
}

void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4) {
    s16 phi_t0;
    s16 phi_v1;
    volatile f32 angle;
    f32 spC;
    f32 sp8[1];
    f32 cpy;
    f32 temp;
    f32 sp4[1];
    f32 sp0[1];

    if ((arg0->id == 0x96) && (arg0->unk31C->matrix_physics != 0)) {
        phi_t0 = arg0->unk7A + arg0->unk31C->unk80;
    } else if (arg0->unk31C != NULL) {
        phi_t0 = arg0->unk7A - arg0->unk31C->unk12;
    } else {
        phi_t0 = arg0->unk7A;
    }
    if ((arg0->id == 0x96) && (arg0->unk31C->matrix_physics != 0)) {
        phi_v1 = arg0->unk31C->unk82 + 1024;
    } else {
        phi_v1 = arg0->unk1D1 * 200;
    }
    /* -g3 scope = uopt block boundary; the dead phi_v1 store keeps cpy from being copy-propagated */
    {
        s32 pad0;

        cpy = phi_v1 * 0.005493164f;
        phi_v1 = 0;
    }
    angle = cpy;
    temp = angle;
    cpy = temp;
    temp = temp * 0.01745329238f;
    sp0[0] = temp;
    func_1505A184(phi_t0, 2000.0f, cpy, &arg1->unk0, &arg1->unk8, &arg1->unk4);
    if (arg2 != NULL) {
        arg2->unk4 = cosf(sp0[0]) * 1000.0f;
        sp8[0] = sinf(sp0[0]) * 1000.0f;
        sp4[0] = phi_t0 * 9.587380191e-05f;
        arg2->unk0 = sp8[0] * cosf(sp4[0]);
        arg2->unk8 = -sp8[0] * sinf(sp4[0]);
        if (arg3 != NULL) {
            spC = sp0[0] + arg4;
            arg3->unk4 = cosf(spC) * 1000.0f;
            sp8[0] = sinf(spC) * 1000.0f;
            arg3->unk0 = sp8[0] * cosf(sp4[0]);
            arg3->unk8 = -sp8[0] * sinf(sp4[0]);
        }
    }
}
// NON-MATCHING: 90% there
// void func_15145740(struct127 *arg0, struct17 *arg1, struct17 *arg2, struct17 *arg3, f32 arg4) {
//     struct194 tmp;
//     f32 temp_f6;
//     s16 phi_v1;
//     s16 phi_t0;
//
//     if ((arg0->unk4 == 0x96) && ((arg0->unk31C->unk7D != 0))) {
//         phi_t0 = arg0->unk7A + arg0->unk31C->unk80;
//     } else {
//         if (arg0->unk31C != 0) {
//             phi_t0 = arg0->unk7A - arg0->unk31C->unk12;
//         } else {
//             phi_t0 = arg0->unk7A;
//         }
//     }
//     if ((arg0->unk4 == 0x96) && (arg0->unk31C->unk7D != 0)) {
//         phi_v1 = arg0->unk31C->unk82 + 1024;
//     } else {
//         phi_v1 = arg0->unk1D1 * 200;
//     }
//     tmp.unk14 = phi_t0;
//     tmp.unk10 = phi_v1 * 0.005493164f;
//     tmp.unk0 = tmp.unk10 * D_800A56B4;
//     func_1505A184(phi_t0, 2000.0f, tmp.unk10, &arg1->unk0, &arg1->unk8, &arg1->unk4);
//     if (arg2 != 0) {
//         arg2->unk4 = cosf(tmp.unk0) * 1000.0f;
//         temp_f6 = sinf(tmp.unk0) * 1000.0f;
//         tmp.unk8 = temp_f6;
//         tmp.unk4 = phi_t0 * D_800A56B8;
//         arg2->unk0 = cosf(tmp.unk4) * tmp.unk8;
//         arg2->unk8 = sinf(tmp.unk4) * -temp_f6;
//         if (arg3 != 0) {
//             tmp.unkC = tmp.unk0 + arg4;
//             arg3->unk4 = cosf(tmp.unkC) * 1000.0f;
//             tmp.unk8 = sinf(tmp.unkC) * 1000.0f;
//             arg3->unk0 = cosf(tmp.unk4) * tmp.unk8;
//             arg3->unk8 = sinf(tmp.unk4) * -tmp.unk8;
//         }
//     }
// }

void func_15145974(struct17 *arg0, f32 *arg1, f32 *arg2) {
    *arg1 = func_150484A0(arg0->unk0, arg0->unk8) * D_800A56BC;
    if (arg2 != NULL) {
        *arg2 = (func_150484A0(sqrtf(arg0->unk0 * arg0->unk0 + arg0->unk8 * arg0->unk8), arg0->unk4) * D_800A56C0) - 90.0f;
    }
}

f32 func_15145A0C(f32 arg0, f32 arg1, f32 arg2) {
    return D_800A548C[(s32)(arg0 * arg2 * 100.0f)] * arg1;
}


void func_15145A50(struct127 *arg0) {
    arg0->unk5 = 3;
    if (D_800BE9F0 != 51) {
        if ((D_800BE616 != 0) || (arg0->interaction_state == 5) || (arg0->interaction_state == 1) || (arg0->interaction_state == 21)) {
            arg0->interaction_state = 5;
            if (arg0->unk31C != NULL) {
                arg0->unk31C->unk78 = 0;
            }
        } else {
            func_15053694(arg0);
        }
    }
}

void func_1515C1A0(void *, struct17 *, f32 *, f32 *);

s32 func_15145AD8(struct17 *arg0, struct17 *arg1, struct127 *arg2, struct17 *arg3, struct17 *arg4, f32 *arg5, f32 *arg6, struct17 *arg7) {
    struct17 sp7C;
    f32 sp78;
    f32 sp74;
    struct17 sp68;
    struct17 sp5C;
    struct17 sp50;

    if (arg5 != NULL) {
        arg5 = &sp78;
    }
    if (arg6 != NULL) {
        arg6 = &sp74;
    }
    if (arg7 != NULL) {
        arg7 = &sp7C;
    }
    func_1515C1A0(arg2, arg7, arg5, arg6);
    if (*arg6 == 0.0f) {
        return 0;
    }
    if (*arg5 == 0.0f) {
        return 0;
    }
    {
    f32 sp4C;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    sp40 = arg2->unkDC;
    sp3C = arg2->unkE0;
    sp68.unk0 = arg0->unk0;
    sp68.unk4 = arg0->unk4 * sp40;
    sp68.unk8 = arg0->unk8;
    sp5C.unk0 = arg1->unk0;
    sp5C.unk4 = arg1->unk4 * sp40;
    sp5C.unk8 = arg1->unk8;
    {
    f32 sp38;
    if (func_15145128(&sp5C, &sp5C, &sp4C, &sp38) == 0) {
        return 0;
    }
    sp50.unk0 = arg7->unk0;
    sp50.unk4 = arg7->unk4 * sp40;
    sp50.unk8 = arg7->unk8;
    if (func_151451F0(&sp68, &sp5C, &sp50, *arg5, sp4C, arg3, arg4, &sp48, &sp44) == 0) {
        return 0;
    }
    arg3->unk4 *= sp3C;
    arg4->unk4 *= sp3C;
    }
    }
    return 1;
}

u8 func_15145C90(s32 arg0) {
    if (arg0 < 0) {
        return 1;
    } else {
        return (D_800DBEF4[arg0].unk6F & 0x80) == 0x80;
    }
}

void func_15145CD0(u8 *arg0, f32 **arg1, f32 **arg2, s32 arg3) {
    f32 matrix[4][4];
    f32 **src;
    f32 **dst;
    f32 *in;
    f32 *out;

    func_150A8050(matrix, *(f32 *)(arg0 + 0), *(f32 *)(arg0 + 4), *(f32 *)(arg0 + 8));
    matrix[3][0] = *(s16 *)(arg0 + 0x10);
    matrix[3][1] = *(s16 *)(arg0 + 0x12);
    matrix[3][2] = *(s16 *)(arg0 + 0x14);
    if (arg3 <= 0) {
        return;
    }
    src = arg1;
    dst = arg2;
    do {
        in = *src;
        out = *dst;
        func_150A7960(matrix[0], in[0], in[1], in[2], out, out + 1, out + 2);
        arg3--;
        src++;
        dst++;
    } while (arg3 > 0);
}
typedef struct {
    f32 x, y, z;
} Func15145DB4_Vec3f;

void func_15145DB4(u8 *arg0, Func15145DB4_Vec3f *arg1, Func15145DB4_Vec3f *arg2, s32 arg3) {
    f32 matrix[4][4];
    s32 i;

    func_150A8050(matrix, *(f32 *)(arg0 + 0), *(f32 *)(arg0 + 4), *(f32 *)(arg0 + 8));
    matrix[3][0] = *(s16 *)(arg0 + 0x10);
    matrix[3][1] = *(s16 *)(arg0 + 0x12);
    matrix[3][2] = *(s16 *)(arg0 + 0x14);
    for (i = 0; arg3 > 0; i++) {
        func_150A7960(matrix[0], arg1[i].x, arg1[i].y, arg1[i].z, &arg2[i].x, &arg2[i].y, &arg2[i].z);
        arg3--;
    }
}
void func_15145EA4(f32 **arg0, f32 **arg1, Mtx *arg2, s32 arg3) {
    f32 *input;
    f32 transform[4][4];
    f32 *output;
    f32 **outputs;

    if (D_800C3E90 != 0) {
        guMtxL2F(transform, arg2);
        if (arg3 > 0) {
            f32 **inputs = arg0;

            outputs = arg1;
            do {
                input = *inputs;
                if (input != NULL && (input[0] != 0.0f || input[1] != 0.0f || input[2] != 0.0f)) {
                    output = *outputs;
                    func_150A7960(transform[0], input[0], input[1], input[2], output, output + 1, output + 2);
                } else {
                    func_15142314(arg2, 0, *outputs);
                }
                arg3--;
                inputs++;
                outputs++;
            } while (arg3 > 0);
        }
    } else {
        if (arg3 > 0) {
            f32 **inputs = arg0;

            outputs = arg1;
            do {
                input = *inputs;
                if (input != NULL && (input[0] != 0.0f || input[1] != 0.0f || input[2] != 0.0f)) {
                    output = *outputs;
                    func_150A7960((f32 *)arg2, input[0], input[1], input[2], output, output + 1, output + 2);
                } else {
                    func_15142314(arg2, 0, *outputs);
                }
                arg3--;
                inputs++;
                outputs++;
            } while (arg3 > 0);
        }
    }
}
s32 func_15146078(struct17 *arg0, struct17 *arg1, struct17 *arg2) {
    u8 count;
    u8 axis;
    u8 zeroAxis;
    u32 i;
    u32 j;
    u32 k;
    f32 len;
    f32 invLen;

    if ((arg0->unk0 == 0.0f) && (arg0->unk4 == 0.0f) && (arg0->unk8 == 0.0f)) {
        return 0;
    }

    count = 0;
    if (arg0->unk0 == 0.0f) {
        count++;
        zeroAxis = 0;
    } else {
        axis = 0;
    }
    if (arg0->unk4 == 0.0f) {
        count++;
        zeroAxis = 1;
    } else {
        axis = 1;
    }
    if (arg0->unk8 == 0.0f) {
        count++;
        zeroAxis = 2;
    } else {
        axis = 2;
    }

    if (count == 2) {
        switch (axis) {
        case 0:
            arg1->unk0 = 0.0;
            arg1->unk4 = 1.0;
            arg1->unk8 = 0.0;
            arg2->unk0 = 0.0;
            arg2->unk4 = 0.0;
            arg2->unk8 = 1.0;
            break;
        case 1:
            arg1->unk0 = 1.0;
            arg1->unk4 = 0.0;
            arg1->unk8 = 0.0;
            arg2->unk0 = 0.0;
            arg2->unk4 = 0.0;
            arg2->unk8 = 1.0;
            break;
        case 2:
            arg1->unk0 = 1.0;
            arg1->unk4 = 0.0;
            arg1->unk8 = 0.0;
            arg2->unk0 = 0.0;
            arg2->unk4 = 1.0;
            arg2->unk8 = 0.0;
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
    ((f32 *)arg1)[i] = 1.0;
    ((f32 *)arg1)[j] = 1.0;
    ((f32 *)arg1)[k] = -((f32 *)arg0)[i] - (((f32 *)arg0)[j] / ((f32 *)arg0)[k]);
    func_151450B4(arg1, arg0, arg2);
    func_151450B4(arg2, arg0, arg1);
    func_15145128(arg1, arg1, &len, &invLen);
    func_15145128(arg2, arg2, &len, &invLen);
    return 1;
}
extern u8 D_800BE9C0;
extern u8 D_800D9BD0[][2][8];
extern s32 D_800D9E10[];
extern u8 D_800D9E20;
extern u8 D_800D9E21;
void *func_1515D914(void *, s16, s32, s32, s32, s32, s32, s32, s32, void *, s32, s32, s32, s32);
void *func_1515E544(void *, s32, u8, u8, void *);
void *func_151462C8(void *arg0, s32 *arg1, u8 arg2, void *arg3, u8 arg4, s16 arg5, struct17 *arg6, u8 arg7, s32 arg8)
{
  s32 temp_t1;
  s32 temp_t2;
  s32 temp_v1;
  s16 temp_a1;
  s32 var_t0;
  s32 var_v1;
  temp_v1 = arg5 * 4;
  temp_t1 = *((s32 *) ((((u8 *) arg1) + temp_v1) + 8));
  if (temp_t1 != 0)
  {
    temp_t2 = arg1[6];
    if (temp_t2 != 0)
    {
 do { if (arg2 == 1) { if (arg3 != 0) { if (((((*((s32 *) (((u8 *) arg3) + 0))) == 0) || (arg4 != (*(((u8 *) arg3) + 0x3B)))) || ((*(((u8 *) arg3) + 4)) == 0xFF)) || ((*(((u8 *) arg3) + 0x302)) == 0)) { arg2 = 2; } } else { arg2 = 0; } } switch (arg2) { case 1: arg0 = func_1515E544(arg0, *((s32 *) ((((u8 *) arg3) + temp_v1) + 0x304)), *(((u8 *) arg3) + 0x301), *(((u8 *) arg3) + 0x302), *((void **) (((u8 *) arg3) + 0x314))); break; case 2: arg0 = func_1515E544(arg0, D_800D9E10[arg5], D_800D9E20, D_800D9E21, D_800D9BD0[arg5][D_800BE9C0]); break; do { } while (0); case 0: default: temp_a1 = arg5; var_t0 = (arg7 & 1) ? (2) : (0); if (arg7 & 2) { var_v1 = 0x10; } else { var_v1 = 0; } arg0 = func_1515D914(arg0, temp_a1, (s32) arg6->unk0, (s32) arg6->unk4, (s32) arg6->unk8, arg8, temp_t1, arg1[0], temp_t2, arg1 + 1, arg1[7], 0, (var_v1 | 8) | var_t0, 0); break; } } while (0);
    }
  }
  return arg0;
}

extern s32 D_80082FA0;

u8 func_151464B8(s16 *arg0) {
    s32 i;
    s16 mask;
    s16 field;
    s32 ret;

    i = 0;
    mask = 0;
    for (; i <= D_80082FA0; i++) {
        mask |= 1 << i;
    }
    ret = (arg0[1] & mask) == 0;
    field = arg0[1];
    return ret;
}

void func_15146508(struct127 *arg0, struct127 *arg1) {
    struct193 tmp;

    tmp.unk0 = arg0;
    tmp.unk4 = arg1;
    tmp.unk8 = arg0->unique_id;
    tmp.unk9 = arg1->unique_id;
    func_15169040(&tmp, 45, arg0, arg1);
}

typedef struct {
    u8 pad0[0x3E8];
    Mtx *unk3E8[3];
    u8 pad3F4[2];
    u8 unk3F6;
} Func1514654C_Sub48;

typedef struct {
    u8 pad0[2];
    u8 unk2;
    u8 pad3[0x1B];
    u16 unk1E;
    u16 unk20;
    u8 pad22[0x12];
    Mtx *unk34;
    u8 pad38[0x10];
    Func1514654C_Sub48 *unk48;
} Func1514654C_Arg1;

s32 func_15031070(struct126 *, struct127 *, Mtx **, s32 *);
void func_15145EA4(f32 **, f32 **, Mtx *, s32);

s32 func_1514654C(struct127 *arg0, Func1514654C_Arg1 *arg1, s32 arg2, f32 **arg3, f32 **arg4, s32 arg5) {
    s32 i;
    Mtx *mtx;
    struct126 *obj;
    Mtx *sp90;
    s32 sp8C;
    f32 mf[4][4];

    if ((arg0 == NULL) || (arg1 == NULL) || (arg0->unk1D4 == NULL)) {
        return 0;
    }
    if (arg1->unk48 != NULL) {
        if (arg1->unk48->unk3F6 == 0) {
            return 0;
        }
        mtx = arg1->unk48->unk3E8[D_800BE9C0] + arg2;
    } else if (arg1->unk1E) {
        obj = func_1503195C(arg0, arg1->unk1E, 0);
        if (obj == NULL) {
            return 0;
        }
        if (func_15031070(obj, arg0, &sp90, &sp8C) == 0) {
            return 0;
        }
        if (*(s32 *)((u8 *)obj + 0x48) != 0) {
            mtx = &sp90[arg1->unk20];
        } else {
            mtx = sp90;
        }
    end:;
    } else if (arg1->unk34 != NULL) {
        mtx = arg1->unk34 + D_800BE9C0;
    } else {
        func_15145EA4(arg3, arg4, (Mtx *)arg0->unk1D4 + arg1->unk2, arg5);
        return 1;
    }
    if (mtx != NULL) {
        guMtxL2F(mf, mtx);
        for (i = 0; i < arg5; i++) {
            func_150A7960(mf[0], arg3[i][0], arg3[i][1], arg3[i][2], arg4[i], arg4[i] + 1, arg4[i] + 2);
        }
    } else {
        return 0;
    }
    return 1;
}

s32 func_1514672C(struct17 *arg0) {
    f32 temp_f2;
    f32 temp_f0;

    temp_f2 = D_800A56C4;
block_1514672C:
    if ((temp_f2 < fabsf(arg0->unk0)) || (temp_f2 < fabsf(arg0->unk8)) || (temp_f0 = arg0->unk4, (temp_f2 < temp_f0)) || (temp_f0 < D_800A56C8)) {
        return 0;
    }
    return 1;
}
// NON-MATCHING: JUSTREG: first 3 statements are out of order
// s32 func_1514672C(struct17 *arg0) {
//     if ((D_800A56C4 < fabsf(arg0->unk0)) || (D_800A56C4 < fabsf(arg0->unk8)) || (D_800A56C4 < arg0->unk4) || (arg0->unk4 < D_800A56C8)) {
//         return 0;
//     } else {
//         return 1;
//     }
// }

void func_151467A4(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 *arg7) {
    *arg0 = *arg0 - D_800BE9A4;
    if (*arg0 < 0.0f) {
        *arg0 = func_150ADA68() * arg1;
        if ((func_150ADA20() & 3) != 0) {
            *arg2 = (func_150ADA68() * (arg4 - arg3)) + arg3;
        } else {
            *arg2 = (func_150ADA68() * (arg5 - arg4)) + arg4;
        }
    }
    *arg7 = ((*arg2 - *arg7) * arg6) + *arg7;
}
