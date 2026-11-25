#include <ultra64.h>
/* functions.h declares these three with stale signatures; hide them and declare the real ones. */
#define func_150ADA20 func_150ADA20_u8_decl_in_functions_h
#define func_151D9B8C func_151D9B8C_decl_in_functions_h
#define func_1513D524 func_1513D524_decl_in_functions_h
#include "functions.h"
#undef func_150ADA20
#undef func_151D9B8C
#undef func_1513D524
void *func_1513D524(void *, u8, u8, u8, u8, u8, s32, u8, s32);
void func_151D9B8C(u8, f32, u8, void *, f32 *, s16, u8, u8, u8, u8, s32);
u32 func_150ADA20(void);
#include "variables.h"

typedef struct { f32 x, y, z; } Vec3f;

/* Ground probe result filled by func_1504715C / func_15046C80 / func_1504697C. */
typedef struct Game1DD500Ground {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 surface[0x14];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x20 */ s32 unk20;
} Game1DD500Ground; // size 0x24

/* One row of the per-actor-type effect profile table. */
typedef struct Game1DD500Profile {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u8 unkE; /* 1: flagged effect, 2: no effect */
    /* 0x0F */ u8 padF;
} Game1DD500Profile; // size 0x10

/* Parameters for the impact effect spawned by func_15153F18. */
typedef struct Game1DD500Impact {
    /* 0x00 */ s16 unk00;
    /* 0x02 */ s16 unk02;
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ f32 position[3];
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
} Game1DD500Impact;

void func_15143794(s16 yaw, s16 pitch, f32 speed, f32 *out);
void func_151DA6F8(f32 *, f32 *, f32, s16, s32, f32, s32, s32, f32, f32,
                   s32, s32, s32, s32, s32, s32, s32, s32);
void func_1504715C(Game1DD500Ground *, struct127 *);
s32 func_15134070(struct127 *);
void func_15143134(Vec3f *, f32 *, Mtx *);
void func_15153F18(Game1DD500Impact *, f32 *, Game1DD500Ground *, s32, s32);
extern Game1DD500Profile D_800A3FD8[];
extern Vec3f D_800AA120;

/* Effect profile index for an actor; actor types 0x53 and 0xA5 (and no actor) use profile 4. */
#define EFFECT_PROFILE(actor) \
    ((actor) != NULL ? (((actor)->id == 0x53 || (actor)->id == 0xA5) ? 4 : func_15134070(actor)) : 4)

/* Spawns one randomized particle at (x, y, z) launched in direction (yaw, pitch). */
s32 func_151B0050(s32 arg0, s32 arg1, f32 x, f32 y, f32 z, s32 arg5, s32 arg6, s32 arg7,
                  s16 pitch, s16 yaw, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE) {
    f32 pos[3];
    f32 dir[3];
    f32 speed;

    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    speed = func_150ADA68() * 5.3f + 4.4f;
    func_15143794(yaw, pitch, speed, dir);
    // matching: a literal 0.0f scale is folded away; IDO keeps the multiply for (0.68f - 0.68f)
    func_151DA6F8(pos, dir, func_150ADA68() * (0.68f - 0.68f) + 0.68f, (s16)(func_150ADA20() % 20 + 31),
                  func_150ADA20() % 101 + 155, func_150ADA68() * 2.33f + 6.7f,
                  (func_150ADA20() & 1) + 3, 1, 1.0f, 1.0f, 0, 0, 0, 16, 15, 0, argE, 1);
    return 1;
}

/* Spawns an impact effect at joint 6 of actor, styled by the profile of other. */
void func_151B01B8(struct127 *actor, struct127 *other) {
    Game1DD500Impact impact;
    u8 shade;
    Game1DD500Ground ground;
    s32 profile;

    if (actor != NULL && actor->unk1D4 != NULL && (actor->unk74 & 0xF) != 0xF) {
        func_1504715C(&ground, actor);
        profile = EFFECT_PROFILE(other);
        if (profile != 99) {
            if (D_800A3FD8[profile].unkE != 2) {
                shade = (actor->unk7A >> 8) + 0x40;
                func_15143134(&D_800AA120, impact.position, &((Mtx *)actor->unk1D4)[6]);
                impact.unk2C = 10;
                impact.unk2E = 20;
                impact.unk00 = shade - 0x68;
                impact.unk14 = 7.22f;
                impact.unk02 = 0xD0;
                impact.unk04 = -0x1B;
                impact.unk06 = 0x36;
                impact.unk30 = 3;
                impact.unk32 = 2;
                impact.unk34 = 30;
                impact.unk36 = 40;
                impact.unk38 = 155;
                impact.unk3A = 100;
                impact.unk18 = 3.04f;
                impact.unk1C = 0.810000062f;
                impact.unk20 = 0.405000031f;
                impact.unk24 = 6.0f;
                impact.unk28 = 13.90000057f;
                impact.unk40 = 0.5f;
                if (D_800A3FD8[profile].unkE == 1) {
                    impact.unk3C = 1;
                } else {
                    impact.unk3C = 0;
                }
                impact.unk44 = 16;
                impact.unk46 = 15;
                impact.unk48 = 0;
                func_15153F18(&impact, impact.position, &ground, 0xFF, 1);
            }
        }
    }
}

/* TEMP compat */
typedef struct {
    u8 pad_0x0[0x10];
    u8 field_0x10;
    u8 field_0x11;
    s8 field_0x12;
    s8 field_0x13;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    f32 field_0x20;
} CompactOscillatorMotionState;
typedef struct {
    u8 pad_0x0[0x2C];
    f32 field_0x2C;
    f32 field_0x30;
    u8 pad_0x34[0xDC];
    CompactOscillatorMotionState field_0x110;
} CompactOscillatorObject;
void func_151B1918(struct260 *arg0);
void func_15149368(struct260 *arg0);
typedef Game1DD500Ground Game1DD500Hit;
void func_15143874(s16 angle, f32 radius, f32 *outX, f32 *outZ);
s32 func_15046C80(f32 *, s32, f32, Game1DD500Ground *);
s32 func_1504697C(f32 *, s32, f32, Game1DD500Ground *);
void func_151DAB58(u8, f32, u8, f32 *, s32, u8, s32);
void func_15136698(f32, f32, u8, u8, s32, void *, f32 *, s32, s32, u8, s32);

/* Big landing burst at actor: an impact effect, 12-17 splats scattered on the ground around it, and a mark under it. */
void func_151B03B8(struct127 *actor, u8 arg1, s32 arg2) {
    Game1DD500Impact impact;
    Game1DD500Ground ground;
    s32 count;
    Game1DD500Ground scatterGround;
    f32 probe[3];
    f32 spot[3];
    f32 center[3];
    Game1DD500Ground centerGround;

    if (actor != NULL) {
        func_1504715C(&ground, actor);
        impact.position[0] = actor->x_position;
        impact.position[1] = actor->y_position;
        impact.position[2] = actor->z_position;
        impact.unk14 = 5.08f;
        impact.unk2C = 10;
        impact.unk02 = 0xFF;
        impact.unk04 = -0x3F;
        impact.unk2E = 0;
        impact.unk00 = 0;
        impact.unk06 = 0x2D;
        impact.unk30 = 3;
        impact.unk32 = 2;
        impact.unk34 = 40;
        impact.unk36 = 20;
        impact.unk38 = 155;
        impact.unk3A = 100;
        impact.unk3C = 0;
        impact.unk44 = 16;
        impact.unk46 = 15;
        impact.unk48 = 0;
        impact.unk18 = 3.02f;
        impact.unk1C = 0.807000041f;
        impact.unk20 = 0.701000035f;
        impact.unk24 = 18.0f;
        impact.unk28 = 11.8f;
        impact.unk40 = 0.5f;
        func_15153F18(&impact, impact.position, &ground, 0xFF, 1);

        count = func_150ADA20() % 6 + 12;
        scatterGround.unk18 = 0;
        scatterGround.unk1C = 0;
        scatterGround.unk1D = 0;
        scatterGround.unk20 = 0;
        scatterGround.height = -10000.0f;
        probe[1] = actor->y_position + 100.0f;
        if (count != 0) {
            // matching: a do/while (--count != 0) loop schedules differently from this break form
            for (;;) {
                func_15143874(func_150ADA20() & 0xFF, func_150ADA68() * 100.0f + 110.0f, &spot[0], &spot[2]);
                spot[0] += actor->x_position;
                spot[2] += actor->z_position;
                probe[0] = spot[0];
                probe[2] = spot[2];
                if (!func_15046C80(probe, 0, actor->y_position - 500.0f, &scatterGround) || scatterGround.unk1D == 3) {
                    return;
                }
                spot[1] = scatterGround.height;
                spot[1] += 10.0f;
                // matching: (f32)100 is not CSE'd with the 100.0f above, which would tie up an FP register
                if (func_150ADA20() & 1) {
                    func_151D9B8C(0, func_150ADA68() * 4.5f + 15.0f, (u32)(func_150ADA68() * (f32)100 + 155.0f),
                                  scatterGround.surface, spot, 100, 0, 1, 0, arg1, arg2);
                } else {
                    func_151DAB58(0, func_150ADA68() * 5.9f + 6.1f, (u32)(func_150ADA68() * (f32)100 + 155.0f),
                                  spot, 1, arg1, arg2);
                }
                if (--count == 0) {
                    break;
                }
            }
        }

        center[0] = actor->x_position;
        center[1] = actor->y_position;
        center[2] = actor->z_position;
        centerGround.unk20 = 0;
        centerGround.unk1D = 0;
        centerGround.unk1C = 0;
        centerGround.unk18 = 0;
        centerGround.height = -10000.0f;
        {
            f32 above[3];

            above[0] = center[0];
            above[1] = center[1] + 100.0f;
            above[2] = center[2];
            if (!func_1504697C(above, 0, center[1] - 500.0f, &centerGround) || centerGround.unk1D == 3) {
                return;
            }
        }
        center[1] = centerGround.height;
        center[1] += 10.0f;
        func_15136698((func_150ADA68() * 40.0f + 120.0f) * 0.001f, (func_150ADA68() * 123.0f + 123.0f) * 0.00001f,
                      func_150ADA20() % 101 + 155, func_150ADA20() % 101 + 100, func_150ADA20() % 56 + 70,
                      centerGround.surface, center, 0, 1, arg1, arg2);
    }
}

typedef struct {
    void *obj;
    u8 kind;
} Msg151B09BC;

typedef struct {
    s32 a;
    s32 b;
    s32 c;
} Triple151B09BC;

typedef struct {
    void *obj;
    u8 kind;
    s32 unk08;
    Triple151B09BC slots[11];
    f32 unk90;
    u8 flag;
} Data151B09BC;

extern u8 D_800A3FE6[];
void *memcpy(void *, const void *, u32);

struct260 *func_151B09BC(void *arg0, void *arg1, s16 arg2, u8 arg3, s32 arg4) {
    struct260 *obj;
    Data151B09BC data;
    s32 index;
    s16 lifetime;
    u8 hasLifetime;
    Msg151B09BC msg;

    if (arg0 == NULL) {
        return NULL;
    }
    index = arg1 != 0 ?
        ((*(u8 *)((u8 *)arg1 + 4) == 0x53 || *(u8 *)((u8 *)arg1 + 4) == 0xA5) ?
            4 : func_15134070(arg1)) : 4;
    if (index == 0x63) {
        return NULL;
    }
    if (D_800A3FE6[index * 0x10] == 2) {
        return NULL;
    }
    if (D_800A3FE6[index * 0x10] == 1) {
        data.flag = 1;
    } else {
        data.flag = 0;
    }
    msg.obj = arg0;
    msg.kind = *((u8 *)arg0 + 0x3B);
    func_151494E0((s32)&msg, 0x13);
    hasLifetime = 0;
    if (arg2 == -1) {
        lifetime = 0x12C;
    } else {
        lifetime = arg2;
        hasLifetime = 1;
    }
    data.obj = arg0;
    data.kind = *((u8 *)arg0 + 0x3B);
    data.unk08 = 0;
    data.unk90 = 0.0f;
    {
        s32 i;
    for (i = 0; i < 11; i++) {
        data.slots[i].a = 0;
        data.slots[i].b = 0;
        data.slots[i].c = 0;
    }
    }
    obj = func_151491F4(lifetime, -1, 0x13, hasLifetime, 0xF, 0x98, arg3, arg4);
    if (obj != NULL) {
        memcpy((void *)((s32)obj + 0x28), &data, 0x98);
    }
    return obj;
}

typedef struct { f32 x, y, z; } Vec3f_151B0B88;

typedef struct Game1DD500Owner {
    s32 active;
    u8 kind;
    u8 pad5[0x36];
    u8 id;
    u8 pad3C[0x38];
    u8 flags74;
    u8 pad75[0x15F];
    s32 transform;
} Game1DD500Owner;

typedef struct Game1DD500Particle {
    u8 pad0[0x34];
    Vec3f_151B0B88 position;
    Vec3f_151B0B88 previous;
    u8 pad4C[0xC];
    s32 flags;
    u8 pad5C[0xB4];
    Vec3f_151B0B88 velocity;
} Game1DD500Particle;

typedef struct Game1DD500Node {
    Game1DD500Particle *object;
    s32 state;
    struct Game1DD500Node *next;
} Game1DD500Node;

typedef struct Game1DD500Packet {
    Game1DD500Owner *owner;
    u8 id;
    s32 count;
    Game1DD500Node nodes[11];
    f32 time;
    u8 kind;
} Game1DD500Packet;

typedef struct Game1DD500Mount {
    u8 joint;
    u8 pad1[3];
    Vec3f_151B0B88 offset;
} Game1DD500Mount;

typedef struct Game1DD500SparkPacket {
    u8 kind;
    u8 pad1;
    s16 flags;
    s16 life;
    u8 pad6[2];
    s32 field8;
    s32 fieldC;
    u8 color[4];
    f32 range[2];
    Vec3f_151B0B88 position;
    Vec3f_151B0B88 direction;
    Vec3f_151B0B88 scale;
    s32 mode;
    u8 field44;
    u8 field45;
    u8 pad46[0x12];
} Game1DD500SparkPacket;

typedef struct Game1DD500SparkMotion {
    u8 pad0[0xC];
    f32 fieldC;
    u8 field10;
    u8 field11;
    u8 field12;
    u8 field13;
    f32 field14;
    f32 field18;
    f32 field1C;
    f32 field20;
    u8 field24;
    u8 field25;
    u8 pad26[2];
} Game1DD500SparkMotion;

void func_1513F680(void *, u8, u8, u8, u8);
extern Game1DD500Mount D_800AA12C[];
extern Game1DD500Mount D_800AA1DC[];
extern f32 D_800AA2E0;
extern f32 D_800AA2E4;
extern f32 D_800AA2E8;
extern f32 D_800AA2EC;
extern u8 D_800AB310[];

void func_151B0B88(u8 *arg0) {
    Game1DD500Packet *list = (Game1DD500Packet *)(arg0 + 0x28);
    u8 failed;

    failed = 0;
    if (list->owner->active == 0 || list->owner->id != list->id) {
        failed = 1;
    }
    if (!failed) {
        Game1DD500Node *head;
        Game1DD500Node *last;
        s32 i;
        u32 count;

        if (list->owner->transform == 0) {
            goto clear;
        }
        head = NULL;
        last = NULL;
        if ((list->owner->flags74 & 0xF) != 0xF) {
            count = 0;
            for (i = 0; i < 11; i++) {
                Game1DD500Node *node = &list->nodes[i];
                s32 keep;

                if (node->object != NULL) {
                    node->object->previous = node->object->position;
                    func_15143134(list->owner->kind == 0x36 ? &D_800AA1DC[i].offset.x : &D_800AA12C[i].offset.x,
                                  &node->object->position.x,
                                  list->owner->kind == 0x36 ? (D_800AA1DC[i].joint << 6) + list->owner->transform : (D_800AA12C[i].joint << 6) + list->owner->transform);
                    node->state -= D_800BE9E4;
                    if (node->state < 0) {
                        Vec3f_151B0B88 *velocity = &node->object->velocity;

                        velocity->x = (node->object->position.x - node->object->previous.x) * D_800BE9A8;
                        velocity->y = 0.0f;
                        velocity->z = (node->object->position.z - node->object->previous.z) * D_800BE9A8;
                        func_1513F680(node->object, 0x10, 0x1B, 0, 0x10);
                        node->object->flags |= 1;
                        node->object = NULL;
                        list->count--;
                        keep = 1;
                    } else {
                        keep = 0;
                    }
                } else {
                    keep = 1;
                }
                if (keep) {
                    if (last == NULL) {
                        last = node;
                    }
                    node->next = head;
                    head = node;
                    count++;
                }
            }
            if (count != 0) {
                list->time += func_150ADA68() * D_800AA2E0 * D_800AA2E4 * D_800BE9A4;
                if (list->time > 1.0f) {
                    Game1DD500SparkPacket packet;
                    Game1DD500SparkMotion motion;

                    last->next = head;
                    packet.kind = D_800AB310[list->kind];
                    packet.flags = 0x1001;
                    packet.life = func_150ADA20() % 51 + 0x34;
                    packet.field8 = 0;
                    packet.fieldC = 0;
                    packet.color[0] = 0;
                    packet.color[1] = 0;
                    packet.color[2] = 0;
                    packet.color[3] = 0xFF;
                    packet.mode = 0;
                    packet.scale.x = 1.0f;
                    packet.scale.y = 1.0f;
                    packet.scale.z = 1.0f;
                    motion.field10 = 0;
                    motion.field11 = 0;
                    packet.direction.x = 0.0f;
                    packet.direction.y = 0.0f;
                    packet.direction.z = 0.0f;
                    motion.field25 = list->kind;
                    motion.field24 = func_150ADA20() & 1;
                    packet.range[0] = 0.0f;
                    packet.range[1] = 0.0f;
                    motion.field14 = 5.0f;
                    motion.field18 = 25.0f;
                    motion.field1C = 2.0f;
                    motion.field20 = 8.0f;
                    do {
                        u32 skip;
                        s32 index;
                        void *result;

                        skip = func_150ADA20() % count;
                        while (skip--) {
                            last = head;
                            head = head->next;
                        }
                        index = head - list->nodes;
                        packet.field45 = func_150ADA20() % 101 + 100;
                        packet.field44 = func_150ADA20() % 101 + 0x9B;
                        motion.field12 = func_150ADA20() % 5 + 4;
                        motion.field13 = func_150ADA20() % 5 + 4;
                        motion.fieldC = (func_150ADA68() * 506.0f + D_800AA2E8) * D_800AA2EC;
                        func_15143134(&D_800AA12C[index].offset.x, &packet.position.x, (D_800AA12C[index].joint << 6) + list->owner->transform);
                        packet.direction = packet.position;
                        result = func_1513D524(&packet, 0x11, 0, 0, 0x10, ((func_150ADA20() % 1) ? 1 : 0) | 2, 0x28, arg0[0xC], arg0[1]);
                        head->object = result;
                        if (result != NULL) {
                            memcpy((u8 *)result + 0x110, &motion, sizeof(motion));
                            head->state = func_150ADA20() % 31 + 20;
                            head = head->next;
                            count--;
                            last->next = head;
                            list->count++;
                        }
                        list->time -= 1.0f;
                    } while (list->time > 1.0f && count != 0);
                }
            }
        } else {
        clear:
            if (list->count != 0) {
                func_151B1918((struct260 *)arg0);
            }
        }
    }
    if (failed) {
        *(s16 *)(arg0 + 0xE) = -1;
    }
}

typedef struct {
    f32 vx;
    f32 vy;
    f32 vz;
    f32 ay;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    u8 unk24;
    u8 unk25;
} Game1DD500Proj;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0x23];
    f32 unk30;
    f32 pos[3];
    f32 lag[3];
    u8 pad4C[0x10];
    u8 unk5C;
    u8 pad5D[0x1B];
    f32 unk78;
    f32 unk7C;
    u8 pad80[0x90];
    Game1DD500Proj proj;
} Game1DD500ProjObj;

s32 func_151B118C(Game1DD500ProjObj *arg0) {
    f32 dx;
    f32 dz;
    f32 dy;
    Game1DD500Proj *p;
    s8 ret;
    f32 oldY;
    f32 oldVz;
    f32 oldVx;
    f32 probe[3];
    f32 pos[3];
    f32 size;

    p = &arg0->proj;
    oldY = arg0->pos[1];
    oldVx = p->vx;
    oldVz = p->vz;
    ret = 1;
    p->vx *= 0.25f * D_800BE9A4;
    p->vz *= 0.25f * D_800BE9A4;
    dx = (p->vx + oldVx) * 0.5f * D_800BE9A4;
    dy = p->vy * D_800BE9A4 + p->ay * D_800BE9A4 * D_800BE9A4 * 0.5f;
    dz = (p->vz + oldVz) * 0.5f * D_800BE9A4;
    p->vy += p->ay * D_800BE9A4;
    arg0->pos[0] += dx;
    arg0->pos[1] += dy;
    arg0->pos[2] += dz;
    arg0->lag[0] += dx;
    arg0->lag[1] += dy;
    arg0->lag[2] += dz;
    arg0->lag[0] = (arg0->lag[0] - arg0->pos[0]) * 0.706000030f + arg0->pos[0];
    arg0->lag[1] = (arg0->lag[1] - arg0->pos[1]) * 0.706000030f + arg0->pos[1];
    arg0->lag[2] = (arg0->lag[2] - arg0->pos[2]) * 0.706000030f + arg0->pos[2];
    if (p->unk24) {
        probe[0] = arg0->pos[0];
        probe[1] = oldY - arg0->unk30;
        probe[2] = arg0->pos[2];
        if (func_15046C80(probe, 0, arg0->pos[1] - arg0->unk30, &arg0->unk78)) {
            pos[0] = probe[0];
            pos[1] = arg0->unk78 + 5.0f;
            pos[2] = probe[2];
            size = ((p->unk14 + p->unk1C * 0.5f) + (p->unk18 + p->unk20 * 0.5f)) * 0.5f;
            if (func_150ADA20() & 1) {
                func_151D9B8C(p->unk25, size * 1.70000005f, arg0->unk5C, (void *)&arg0->unk7C, pos, 100, 0, 1, 0, arg0->unkC, arg0->unk1);
            } else {
                func_151DAB58(p->unk25, size * 0.680000007f, arg0->unk5C, pos, 1, arg0->unkC, arg0->unk1);
            }
            ret = 0;
        }
    }
    return ret;
}

s32 func_151B1478(struct260 *arg0) {
    s32 v1;
    if (*(s16 *)((u8 *)arg0 + 0x1C) < 0x20) {
        v1 = *(s16 *)((u8 *)arg0 + 0x1C) << 3;
        if (v1 < *(u8 *)((u8 *)arg0 + 0x5C)) {
            *(u8 *)((u8 *)arg0 + 0x5C) = v1;
        }
    }
    return 1;
}

struct Struct151B14AC {
    char pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    char pad44[0x4];
    f32 unk48;
    char pad4C[0x74];
    char unkC0[0x40];
    s16 *unk100[0xB];
};

extern void func_151D5D60();

s16 *func_151B14AC(struct Struct151B14AC *arg0, s16 arg1) {
    s16 *var_v0;
    s16 *ret;
    f32 sinval;
    f32 temp_f18;
    f32 temp_y_cos;
    f32 temp_y_sin;
    f32 temp_f2;
    f32 var_f14;
    f32 var_f2;
    f32 *temp_v1;
    f32 temp_f0;
    u8 sp3B;

    func_151D5D60(arg0->unk100, arg1, 0x40, &var_v0, &sp3B);
    ret = var_v0;
    if (var_v0) {
        if (sp3B) {
            memcpy(arg0->unk100[arg1], arg0->unkC0, 0x40);
            memcpy(arg0->unk100[arg1] + 0x20, arg0->unkC0, 0x40);
        }
    } else {
        return 0;
    }

    temp_v1 = &D_800DD1E8[arg1];
    var_f14 = -1.0f;
    var_f2 = ((*temp_v1 * (arg0->unk40 - arg0->unk34)) - (D_800DD1D8[arg1] * (arg0->unk48 - arg0->unk3C))) * D_800BE9A8;
    if (var_f2 < 0.0f) {
        var_f14 = 1.0f;
        var_f2 = -var_f2;
    }

    temp_f0 = sqrtf(var_f2) * 0.25f;
    if (1.570796371f < temp_f0) {
        temp_f0 = 1.570796371f;
    }

    temp_f0 = temp_f0 * var_f14;
    sinval = sinf(temp_f0);
    temp_f0 = cosf(temp_f0);
    temp_f2 = arg0->unk2C * temp_f0;
    temp_f18 = arg0->unk2C * sinval;
    temp_y_cos = arg0->unk30 * temp_f0;
    temp_y_sin = arg0->unk30 * sinval;

    var_v0[0] = (*temp_v1 * -temp_f2) + arg0->unk34;
    var_v0[1] = arg0->unk38 + temp_f18;
    var_v0[2] = arg0->unk3C - (D_800DD1D8[arg1] * -temp_f2);
    var_v0[3] = 0;

    var_v0[8] = (*temp_v1 * temp_f2) + arg0->unk34;
    var_v0[9] = arg0->unk38 - temp_f18;
    var_v0[10] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f2);
    var_v0[11] = 0;

    temp_f0 = temp_f2 - temp_y_sin;
    var_v0[16] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[17] = (arg0->unk38 - temp_f18) - temp_y_cos;
    var_v0[18] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f0);
    var_v0[19] = 0;

    temp_f0 = -(temp_f2 + temp_y_sin);
    var_v0[24] = (*temp_v1 * temp_f0) + arg0->unk34;
    var_v0[25] = (arg0->unk38 + temp_f18) - temp_y_cos;
    var_v0[26] = arg0->unk3C - (D_800DD1D8[arg1] * temp_f0);
    var_v0[27] = 0;

    return ret;
}


s32 func_151B1828(CompactOscillatorObject *arg0) {
    CompactOscillatorObject *p = arg0;
    CompactOscillatorMotionState *f = &arg0->field_0x110;
    f32 r1;
    f32 r2;
    f->field_0x10 = f->field_0x10 + f->field_0x12 * D_800BE9E4;
    f->field_0x11 = f->field_0x11 + f->field_0x13 * D_800BE9E4;
    r1 = func_151423D8((u8)(f->field_0x10 - 0x40));
    r2 = func_151423D8((u8)(f->field_0x11 - 0x40));
    p->field_0x2C += (f->field_0x14 + f->field_0x1C * r1 - p->field_0x2C) * 0.5f;
    p->field_0x30 += (f->field_0x18 + f->field_0x20 * r2 - p->field_0x30) * 0.5f;
    return 1;
}

void func_151B1918(struct260 *arg0) {
    struct {
        s32 head[2];
        s32 count;
        s32 entries[11][3];
    } *list;
    s32 i;

    list = (void *)((u8 *)arg0 + 0x28);
    list->count = 0;
    *(f32 *)((u8 *)arg0 + 0xB8) = 0.0f;
    for (i = 0; i < 11; i++) {
        s32 *entry = list->entries[i];

        if (list->entries[i][0] != 0) {
            func_1516972C((struct102 *)entry[0]);
        }
        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 0;
    }
}

typedef struct {
    s32 unk0;
    u8  unk4;
} SubA151B19A4;

typedef struct {
    s32 unk0;
    union {
        s32 w4;
        u8  b4;
    } u4;
    u8  unk8;
    u8  unk9;
} ArgB151B19A4;

void func_151B19A4(struct260 *arg0, s32 arg1, u8 arg2) {
    SubA151B19A4 *temp_v0 = (SubA151B19A4 *)((u8 *)arg0 + 0x28);
    ArgB151B19A4 *b = (ArgB151B19A4 *)arg1;
    s32 v1 = temp_v0->unk0;

    if (arg2 == 0 || arg2 == 0x13) {
        if ((v1 == b->unk0) || (b->u4.b4 == temp_v0->unk4)) {
            func_1516972C((struct102 *)arg0);
        }
    } else if (arg2 == 0x2D) {
        if (temp_v0->unk0 == b->unk0) {
            temp_v0->unk0 = b->u4.w4;
            temp_v0->unk4 = b->unk9;
        } else if (temp_v0->unk0 == b->u4.w4) {
            temp_v0->unk0 = b->unk0;
            temp_v0->unk4 = b->unk8;
        }
    }
}

void func_151B1A58(struct260 *arg0) {
    func_151B1918(arg0);
    func_1514933C(arg0);
}

void func_151B1A84(struct260 *arg0) {
    func_151B1918(arg0);
    func_15149368(arg0);
}
