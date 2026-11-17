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
void func_151B6320(f32 *arg0, u8 arg1, s32 arg2);

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
    u8 pad31[0x54 - 0x31];
    Vec151B65D4 unk54;
    u8 pad60[0x94 - 0x60];
    Entry151B65D4 *entries;
    State151B65D4 *state;
} Obj151B65D4;
s32 func_151B6420(Obj151B65D4 *arg0);

f32 func_15143E64(void *);
s32 func_151B65D4(Obj151B65D4 *arg0);

typedef struct Trail151B6928 {
    struct17 pos;
    u8 pad0C[4];
    u8 alpha;
    u8 pad11[3];
    f32 rot;
    u8 pad18[4];
} Trail151B6928;

typedef struct Actor151B6928 {
    u8 pad0[0x10];
    struct17 pos;
    u8 pad1C[2];
    u16 unk1E;
    u8 pad20[5];
    u8 unk25;
    u8 pad26[6];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    u8 pad2F[0x55];
    u8 buffers[0x10];
    Trail151B6928 *entries;
} Actor151B6928;

Gfx *func_15142B7C(Gfx *gfx, s32 arg1, s32 arg2);
Gfx *func_15142E24(Gfx *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 arg7, s32 arg8, u8 *arg9, s32 argA);
Gfx *func_15142C10(Gfx *gfx, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5);
Gfx *func_15142FBC(Gfx *gfx, s32 arg1, s32 arg2, u8 *arg3);
Gfx *func_1513F4E4(Gfx *gdl, u8 arg1, u8 *arg2);
extern struct17 *func_15144B34(s32);
void func_151D5D60(void *, s16, s32, void **, u8 *);
extern u8 D_80090CD4[];
extern s32 D_800D2C9C;
extern struct {
    s32 unk0;
    s32 unk4;
} D_800A4AC8[];
Gfx *func_151B6928(Actor151B6928 *arg0, Gfx *gfx, s16 arg2);
void func_151B70B4(void *arg0, void *arg1, u8 arg2);

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
void func_151B7144(f32 *arg0, u8 arg1, s32 arg2);

typedef struct {
    struct17 unk0;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 pad16[2];
} Header151B7328;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 pad9[3];
} Payload151B7328;

typedef struct {
    s32 unk0;
    void *unk4;
} Data151B7328;

typedef struct {
    u8 pad0[0x98];
    Data151B7328 *unk98;
} Obj151B7328;

extern s32 (*D_8008FB90[])();
void *func_151B7328(void *arg0, u8 arg1, s32 arg2, u8 arg3, s32 arg4);
s32 func_151B7678(struct151B76CCArg0 *arg0, f32 *arg1);
s32 func_151B76CC(struct151B76CCArg0 *arg0, f32 *arg1);

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
s32 func_151B77F4(Game1E37D0RingObject *arg0);

extern u8 D_80091100[];
Gfx *func_151B7C38(Game1E37D0RingObject *arg0, Gfx *arg1, s16 arg2);

extern void (*D_8008FB98[])(void *, s32, u8);
void func_151B82CC(void *arg0, s32 arg1, u8 arg2);

struct S151B8318 {
    s32 unk0;
    u8 unk4;
};

struct T151B8318 {
    s32 unk0;
    struct S151B8318 *unk4;
};
void func_151B8318(struct102 *arg0, struct S151B8318 *arg1, u8 arg2);

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
