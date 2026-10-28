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

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B65D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B6928.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B70B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_1E37D0/func_151B7144.s")

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
