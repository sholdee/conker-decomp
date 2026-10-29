#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);
extern s32 func_15147A80(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
} Vec151B6320;

typedef struct {
    Vec151B6320 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Header151B6320;

typedef struct {
    f32 *unk0;
    u8 unk4;
    u8 pad5[3];
    Vec151B6320 unk8;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
} Payload151B6320;

typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 0x4];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8 pad2C[0x38 - 0x2C];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
} struct151B76CCObj;

typedef struct {
    u8 pad0[4];
    struct151B76CCObj **unk4;
} struct151B76CCList;

typedef struct {
    u8 pad0[0x98];
    struct151B76CCList *unk98;
} struct151B76CCArg0;

void func_151B6320(f32 *arg0, u8 arg1, s32 arg2) {
    s32 top_dummy;
    Header151B6320 header;
    s32 pad_dummy;
    Payload151B6320 payload;
    s32 *temp_v0;

    header.unk15 = 0xA;
    header.unk0.unk0 = arg0[5];
    header.unk0.unk4 = arg0[6];
    header.unk0.unk8 = arg0[7];
    header.unkC = 0x12C;
    header.unkE = 6;
    payload.unk0 = arg0;
    payload.unk4 = *(u8 *)((u8 *)arg0 + 0x3B);
    payload.unk8 = header.unk0;
    payload.unk14 = 0.0f;
    payload.unk18 = 0.0f;
    payload.unk20 = -16384.0f;
    payload.unk1C = -16384.0f;
    payload.unk24 = 0.0f;
    header.unk10 = 0xD;

    temp_v0 = (s32 *)func_15147A80(&header, 0x30, 0x1C, 0xB, 0xB, 0xB, 0, 0, 0, arg1, arg2);
    if (temp_v0 != NULL) {
        memcpy((void *)temp_v0[0x98 / 4], &payload, 0x2C);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B6420.s")

typedef struct { f32 x, y, z; } Vec151B65D4;

typedef struct {
    s32 unk0;
    u8 pad4[0x14 - 0x4];
    Vec151B65D4 pos;
    u8 pad20[0x3B - 0x20];
    u8 unk3B;
    f32 unk3C;
} Target151B65D4;

typedef struct {
    Target151B65D4 *target;
    u8 unk4;
    u8 pad5[3];
    Vec151B65D4 pos;
    f32 count;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} State151B65D4;

typedef struct {
    Vec151B65D4 pos;
    f32 unkC;
    u8 unk10;
    u8 pad11[3];
    f32 unk14;
    f32 unk18;
} Entry151B65D4;

typedef struct {
    u8 pad0[0x10];
    Vec151B65D4 pos;
    u8 pad1C[2];
    u16 flags;
    u8 pad20[5];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F;
    u8 unk30;
    u8 pad31[0x94 - 0x31];
    Entry151B65D4 *entries;
    State151B65D4 *state;
} Obj151B65D4;

f32 func_15143E64(void *);

s32 func_151B65D4(Obj151B65D4 *arg0) {
    State151B65D4 *state;
    Entry151B65D4 *entries;
    Target151B65D4 *target;
    Vec151B65D4 delta;
    s32 p0;
    s32 p1;
    f32 dist;

    state = arg0->state;
    entries = arg0->entries;
    target = state->target;
    if (target->unk0 == 0 || target->unk3B != state->unk4 || target->unk3C < 15.0f) {
        arg0->unk30 = 0;
        arg0->flags |= 8;
        return 1;
    }
    arg0->pos.x = target->pos.x;
    arg0->pos.y = target->pos.y;
    arg0->pos.z = target->pos.z;
    delta.x = arg0->pos.x - state->pos.x;
    delta.y = arg0->pos.y - state->pos.y;
    delta.z = arg0->pos.z - state->pos.z;
    dist = func_15143E64(&delta);
    state->count += dist * 0.003721999936f * D_800BE9A4;
    state->unk1C += dist * 4.409547806f;
    if (state->count > 1.0f) {
        Entry151B65D4 *e;
        Vec151B65D4 cur;
        f32 inv;
        f32 y;
        f32 dy;
        f32 a;
        f32 b;
        f32 dx;
        f32 dz;
        f32 dw;
        f32 diff;
        f32 da;
        inv = 1.0f / state->count;
        y = state->unk18 + D_800BE9A4;
        cur = state->pos;
        a = state->unk20;
        b = state->unk24;
        dy = -(y * inv);
        dx = delta.x * inv;
        dz = delta.y * inv;
        dw = delta.z * inv;
        diff = state->unk1C - state->unk20;
        da = diff * inv;
        do {
            e = &entries[arg0->head];
            e->pos = cur;
            e->unk10 = 0x9B;
            e->unk14 = a;
            e->unkC = 13.0f - y;
            y += dy;
            while (e->unk14 > 16384.0f) {
                e->unk14 -= 32768.0f;
            }
            e->unk18 = b;
            arg0->head++;
            if (arg0->head == arg0->capacity) {
                arg0->head = 0;
            }
            arg0->count++;
            if (arg0->head == arg0->tail) {
                arg0->tail++;
                if (arg0->tail == arg0->capacity) {
                    arg0->tail = 0;
                }
                arg0->count--;
            }
            cur.x += dx;
            cur.y += dz;
            cur.z += dw;
            a += da;
            b += dist * inv;
            state->count -= 1.0f;
        } while (state->count > 1.0f);
        state->pos = cur;
        state->unk20 = a;
        state->unk24 = b;
        state->unk18 = y;
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B6928.s")

void func_151B70B4(void *arg0, void *arg1, u8 arg2) {
    void *target;
    s32 current;
    s32 value;

    target = *(void **)((u8 *)arg0 + 0x98);
    current = *(s32 *)target;
    if (arg2 == 0) {
        if (current == *(s32 *)arg1) {
            u16 *flags = (u16 *)((u8 *)arg0 + 0x1E);

            *(s8 *)((u8 *)arg0 + 0x30) = 0;
            *flags |= 8;
        }
    } else if (arg2 == 0x2D) {
        value = *(s32 *)arg1;
        if (value == *(s32 *)target) {
            *(s32 *)target = *(s32 *)((u8 *)arg1 + 4);
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 9);
            return;
        }
        if (*(s32 *)((u8 *)arg1 + 4) == *(s32 *)target) {
            *(s32 *)target = value;
            *(u8 *)((u8 *)target + 4) = *(u8 *)((u8 *)arg1 + 8);
        }
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    struct17 unk30;
    struct17 unk3C;
    struct17 unk48;
    f32 unk54;
    s32 unk58;
    s32 unk5C;
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    s16 unk68;
    f32 unk6C;
} Spawn151B7144;

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_151B7144(f32 *arg0, u8 arg1, s32 arg2) {
    Spawn151B7144 spawn;
    s32 temp_v0;
    s32 temp_v1;

    spawn.unk1D = 0x29;
    spawn.unk8 = 0xE03;
    spawn.unk0 = 0x200005;
    spawn.unk4 = 0;
    spawn.unkC = 0;
    spawn.unk10 = 0;
    spawn.unk1E = 0x12;
    spawn.unk20 = 0xE;
    if (func_150ADA20() & 1) {
        temp_v1 = 0x40;
    } else {
        temp_v1 = 0;
    }
    if (func_150ADA20() & 1) {
        temp_v0 = 0x80;
    } else {
        temp_v0 = 0;
    }
    spawn.unk58 = temp_v0 | 1 | temp_v1 | 0x4000 | 0x8000 | 0x200 | 0x400 | 0x800 | 0x10000;
    spawn.unk60 = 3;
    spawn.unk61 = 3;
    spawn.unk62 = -1;
    spawn.unk63 = -1;
    spawn.unk64 = -1;
    spawn.unk65 = 0;
    spawn.unk22 = 0x28;
    spawn.unk14 = 0xDD;
    spawn.unk15 = 0xD3;
    spawn.unk16 = 0xCD;
    spawn.unk17 = 0xFF;
    spawn.unk18 = 0x57;
    spawn.unk19 = 0x55;
    spawn.unk1A = 0x5A;
    spawn.unk1C = 0xFF;
    spawn.unk54 = 0.0f;
    spawn.unk24 = 1.05674696f;
    spawn.unk30.unk0 = arg0[5];
    spawn.unk30.unk4 = arg0[6];
    spawn.unk30.unk8 = arg0[7];
    spawn.unk3C.unk0 = 0.0f;
    spawn.unk3C.unk4 = 0.0f;
    spawn.unk3C.unk8 = 0.0f;
    spawn.unk1B = func_150ADA20() % 56U + 0xC8;
    spawn.unkA = func_150ADA20() % 10U + 0x1E;
    spawn.unk28 = spawn.unk2C = func_150ADA68() * 50.0f + 89.0f;
    spawn.unk48.unk0 = 0.0f;
    spawn.unk48.unk4 = 0.0f;
    spawn.unk48.unk8 = 0.0f;
    func_15130280(&spawn, 1, 0, 0, arg1, arg2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B7328.s")

s32 func_151B7678(struct151B76CCArg0 *arg0, f32 *arg1) {
    struct151B76CCList *list;
    struct151B76CCObj **entry;
    struct151B76CCObj *obj;

    list = arg0->unk98;
    entry = list->unk4;
    obj = *entry;
    if ((obj->unk0 == 0) || (*((u8 *) entry + 4) != *((u8 *) obj + 0x3B))) {
        return 0;
    }

    arg1[0] = obj->unk14;
    arg1[1] = obj->unk18;
    arg1[2] = obj->unk1C;
    return 1;
}

s32 func_151B76CC(struct151B76CCArg0 *arg0, f32 *arg1) {
    struct151B76CCList *list;
    struct151B76CCObj **objPtr;
    struct151B76CCObj *obj;
    f32 mtx[4][4];

    list = arg0->unk98;
    objPtr = list->unk4;
    obj = *objPtr;
    func_150A8050(mtx, obj->unk20, obj->unk24, obj->unk28);
    mtx[3][0] = obj->unk38;
    mtx[3][1] = obj->unk3C;
    mtx[3][2] = obj->unk40;
    mtx[0][0] *= obj->unk18;
    mtx[0][1] *= obj->unk18;
    mtx[0][2] *= obj->unk18;
    mtx[1][0] *= obj->unk1C;
    mtx[1][1] *= obj->unk1C;
    mtx[1][2] *= obj->unk1C;
    mtx[2][0] *= obj->unk18;
    mtx[2][1] *= obj->unk18;
    mtx[2][2] *= obj->unk18;
    func_150A7960(mtx[0], 0.0f, 0.0f, -250.0f, arg1, arg1 + 1, arg1 + 2);
    return 1;
}

typedef struct { f32 x, y, z; } RingVec3f;

typedef struct Game1E37D0RingEntry {
    RingVec3f position;
    f32 distance;
    u8 state;
    u8 pad11[3];
} Game1E37D0RingEntry;

typedef struct Game1E37D0RingState {
    u8 *linked;
    u8 pad4[4];
    u8 selector;
} Game1E37D0RingState;

typedef struct Game1E37D0RingObject {
    u8 pad0[0x10];
    RingVec3f position;
    u8 pad1C[9];
    u8 capacity;
    u8 pad26[6];
    s8 count;
    s8 tail;
    s8 head;
    u8 pad2F[0x65];
    Game1E37D0RingEntry *entries;
    Game1E37D0RingState *state;
} Game1E37D0RingObject;

extern s32 (*D_8008FB90[])(Game1E37D0RingObject *, RingVec3f *);
f32 func_15143E64(void *);

s32 func_151B77F4(Game1E37D0RingObject *arg0) {
    Game1E37D0RingState *state;
    Game1E37D0RingEntry *entries;
    RingVec3f previous;
    RingVec3f delta;
    f32 distance;

    state = arg0->state;
    entries = arg0->entries;
    if (D_800BE9E4 > 0) {
        previous = arg0->position;
        if (D_8008FB90[state->selector](arg0, &arg0->position) == 0) {
            return 0;
        }
        if (state->linked != 0) {
            *(RingVec3f *)(state->linked + 0x40) = arg0->position;
        }
        delta.x = arg0->position.x - previous.x;
        delta.y = arg0->position.y - previous.y;
        delta.z = arg0->position.z - previous.z;
        distance = func_15143E64(&delta);
        {
            Game1E37D0RingEntry *entry = &entries[arg0->head];

            entry->position = arg0->position;
            entry->distance = distance;
            entry->state = 0xFF;
        }
        arg0->head++;
        if (arg0->head == arg0->capacity) {
            arg0->head = 0;
        }
        arg0->count++;
        if (arg0->head == arg0->tail) {
            arg0->tail++;
            if (arg0->tail == arg0->capacity) {
                arg0->tail = 0;
            }
            arg0->count--;
        }
    }
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B7998.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B7C38.s")

extern void (*D_8008FB98[])(void *, s32, u8);

void func_151B82CC(void *arg0, s32 arg1, u8 arg2) {
    void *v0 = *(void **)((u8 *)arg0 + 0x98);
    void (*f)(void *, s32, u8) = D_8008FB98[*(u8 *)((u8 *)v0 + 0x8)];
    if (f != 0) {
        f(arg0, arg1, arg2);
    }
}

struct S151B8318 {
    s32 unk0;
    u8 unk4;
};

struct T151B8318 {
    s32 unk0;
    struct S151B8318 *unk4;
};

void func_151B8318(struct102 *arg0, struct S151B8318 *arg1, u8 arg2) {
    int new_var;
    struct T151B8318 *v0 = *(struct T151B8318 **)((u8 *)arg0 + 0x98);
    struct S151B8318 *v1 = v0->unk4;

    if (arg2 == 0) {
        if ((v1->unk0 == (new_var = arg1->unk0)) || (arg1->unk4 == v1->unk4)) {
            func_1516972C(arg0);
        }
    }
}

void func_151B8370(void *arg0) {
    s32 v0 = *(s32 *)((u8 *)arg0 + 0x98);
    struct102 *v = *(struct102 **)v0;
    if (v != 0) {
        func_1516972C(v);
    }
}

extern void func_151B8370(void *);
extern void func_151478F4(void *);

void func_151B83A0(void *arg0) {
    func_151B8370(arg0);
    func_151478F4(arg0);
}

extern void func_15147928(void *);

void func_151B83CC(void *arg0) {
    func_151B8370(arg0);
    func_15147928(arg0);
}
