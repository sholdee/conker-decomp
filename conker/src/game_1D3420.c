#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* An object that can trigger a one-shot effect at its position. */
struct Game1D3420Emitter {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 height;
    /* 0x0A */ u8 padA[0xA];
    /* 0x14 */ u8 ready; /* 1 while the effect can still be triggered */
};

/* Message 0x35: the list of trigger ids that fired. */
typedef struct Game1D3420Message {
    /* 0x00 */ s32 count;
    /* 0x04 */ u8 *ids;
    /* 0x08 */ s32 value;
} Game1D3420Message;

typedef struct Game1D3420Target {
    /* 0x00 */ Game1D3420Emitter *emitter;
    /* 0x04 */ u8 id;
    /* 0x05 */ u8 pad05[3];
    /* 0x08 */ s32 argument;
    /* 0x0C */ u8 flags; /* bit 0: play sounds, bit 1: copied into the effect's unk17 */
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ s32 value;
} Game1D3420Target;

struct Game1D3420Listener {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[0x1B];
    /* 0x28 */ Game1D3420Target target;
};

/* Message handler: on message 0x35 whose id list contains this listener's target id, fires the
 * target emitter's effect once and records the message value. */
void func_151A5F70(Game1D3420Listener *listener, void *data, u8 type) {
    Game1D3420Message *msg = data;
    s32 i;
    s32 found;
    Game1D3420Target *target;

    if (type != 0x35) {
        return;
    }
    target = &listener->target;
    i = 0;
    found = FALSE;
    while (i < msg->count && !found) {
        if (target->id == msg->ids[i]) {
            found = TRUE;
        } else {
            i++;
        }
    }
    if (found && target->emitter->ready == 1) {
        func_151A6068(target->emitter, target->argument, target->flags & 1, target->flags & 2,
                      listener->unkC, listener->unk1);
        target->emitter->ready = 0;
        target->value = msg->value;
    }
}

/* Per-effect state, copied to offset 0x110 of the spawned object. */
typedef struct Game1D3420EffectState {
    /* 0x00 */ Game1D3420Emitter *emitter;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ LightEffect *light; /* returned by func_1516284C */
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ f32 unk18;
} Game1D3420EffectState; /* size 0x1C */

typedef struct Game1D3420Effect {
    /* 0x000 */ DefParticleHead head;
    /* 0x110 */ Game1D3420EffectState state;
} Game1D3420Effect;

/* Spawns an effect object at the emitter; with playSound also starts a func_1516284C handle at the
 * emitter's mid-height and plays sound 0x1AA there. */
void func_151A6068(Game1D3420Emitter *emitter, s32 arg1, u8 playSound, u8 arg3, u8 arg4, s32 arg5) {
    ParticleDef spawn;
    Game1D3420EffectState init;
    Game1D3420Effect *effect;
    Game1D3420EffectState *state;
    LightEffectHeader header;
    Vec3i pos;

    init.emitter = emitter;
    init.unk4 = 0.0f;
    init.unk8 = (func_150ADA68() * 80.0f) + -380.0f; // matching: add of a negative literal, not sub.s
    init.unkC = (func_150ADA68() * 1300.0f) + 2700.0f;
    init.light = NULL;
    init.unk14 = func_150ADA20();
    init.unk15 = ((u8)func_150ADA20() & 3) + 4; // matching: the u8 narrowing changes the andi codegen
    init.unk16 = 0xFF;
    init.unk18 = 0.0f;
    init.unk17 = arg3;
    spawn.texture = 0x6B;
    spawn.type = 10;
    spawn.renderModes = 0x4C03;
    spawn.lifetime = 240;
    spawn.frame = 0;
    spawn.frameSpeed = 0;
    spawn.rgba[0] = 0xFF;
    spawn.rgba[1] = 0xFF;
    spawn.rgba[2] = 0xFF;
    spawn.rgba[3] = 0xFF;
    spawn.width = emitter->unk6;
    spawn.height = emitter->height;
    spawn.pos.x = emitter->x;
    spawn.pos.y = emitter->y;
    spawn.pos.z = emitter->z;
    spawn.flags = 0x01EC0009;
    spawn.unk44 = 0xFF;
    spawn.unk45 = 0xFF;
    spawn.unk46 = 0;
    spawn.unk47 = 0;
    spawn.unk48 = arg1;
    spawn.viewportMask = 0xFF;
    spawn.rot.x = 0.0f;
    spawn.rot.y = 0.0f;
    spawn.rot.z = 0.0f;
    spawn.scale.x = 1.0f;
    spawn.scale.y = 1.0f;
    spawn.scale.z = 1.0f;
    effect = func_1513D2F0(&spawn, &D_800A4AA0, 0, 0x28, 0, 0x20, 0, 0, 0, sizeof(init), arg4, arg5);
    if (effect != NULL) {
        state = &effect->state;
        memcpy(state, &init, sizeof(init));
        if (playSound) {
            pos.x = emitter->x;
            pos.y = emitter->y + (emitter->height >> 1);
            pos.z = emitter->z;
            header.flags = 2;
            header.update = 2;
            header.lifetime = 300;
            header.lightType = 6;
            state->light = func_1516284C(&header, &pos, 0xFF, 0x79, 0, 0xFF, 0, 0, 14, arg4, arg5);
        }
    }
    if (playSound) {
        func_1000FA64(0x1AA, emitter->x, emitter->y, emitter->z, 0x4000, 1000, 750, func_1000EF40, emitter,
                      0, 8, func_150ADA20() & 0x300);
    }
}

/* --- func_151A6350 (ROM 0x1D3800): kept in this file, see the note in game_1D3800.c --- */
struct Game1D2B10QuadVertex {
    s16 x, y, z, flag;
    s16 s, t;
    u8 r, g, b, a;
};

struct Game1D2B10QuadOwner {
    u8 unknown0[0x2C];
    f32 width;
    f32 height;
    f32 x, y, z;
    u8 unknown40[0x80];
    Game1D2B10QuadVertex vertexTemplate[4];
    u8 *buffers[5]; /* each holds two copies of vertexTemplate */
    f32 unk114; /* texture s of the quad's first edge */
    f32 unk118;
    f32 unk11C; /* texture s span */
    u8 unknown120[6];
    u8 alpha;
};

/* Writes the 4 vertices of a vertical quad rotated by table angle `index` into the owner's vertex
 * buffer `index` (initialised from the template the first time) and returns them, or NULL. */
Game1D2B10QuadVertex *func_151A6350(Game1D2B10QuadOwner *owner, s16 index) {
    Game1D2B10QuadVertex *vertices;
    Game1D2B10QuadVertex *result;
    f32 dz;
    f32 dx;
    s16 sStart;
    s16 sEnd;
    s32 pad; // matching: unused stack slot in the original frame
    u8 isNew;

    func_151D5D60(owner->buffers, index, 0x40, &vertices, &isNew);
    result = vertices;
    if (vertices != NULL) {
        if (isNew) {
            memcpy(owner->buffers[index], owner->vertexTemplate, sizeof(owner->vertexTemplate));
            memcpy(owner->buffers[index] + sizeof(owner->vertexTemplate), owner->vertexTemplate,
                   sizeof(owner->vertexTemplate));
        }
    } else {
        return NULL;
    }
    {
        s16 tMax = 0x7C0;
        sStart = owner->unk114;
        sEnd = owner->unk114 + owner->unk11C;
        dz = D_800DD1D8[index] * owner->width;
        dx = D_800DD1E8[index] * owner->width;
        vertices->x = owner->x + dx;
        vertices->y = owner->y;
        vertices->z = owner->z - dz;
        vertices->s = sStart;
        vertices->t = tMax;
        vertices->a = owner->alpha;
        vertices->flag = 0;
        vertices++;
        vertices->x = owner->x - dx;
        vertices->y = owner->y;
        vertices->z = owner->z + dz;
        // matching: the comma joins below shift ugen's tree slots onto the heap state func_151A6068 leaves
        vertices->s = sStart, vertices->t = 0;
        vertices->a = owner->alpha;
        vertices->flag = 0;
        vertices++;
        vertices->x = owner->x - dx;
        vertices->y = owner->y + owner->height;
        vertices->z = owner->z + dz;
        vertices->s = sEnd;
        vertices->t = 0, vertices->a = 0, vertices->flag = 0;
        vertices++;
        vertices->x = owner->x + dx;
        vertices->y = owner->y + owner->height;
        vertices->z = owner->z - dz;
        vertices->s = sEnd;
        vertices->t = tMax;
        vertices->a = 0;
        vertices->flag = 0;
    }
    return result;
}
