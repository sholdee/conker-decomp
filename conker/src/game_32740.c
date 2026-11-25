#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1510E7A4(void *, void *, f32 *, void *, void *, s32, f32, f32, f32, f32, u16, s32, f32, f32);
void func_15123070(Camera *);
void func_1512D560(Camera *, s32, struct168 *);

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1502B5C8(u32 *, s32, s32, s32, s32);
extern void func_1512ABF8(void);
extern void func_1512D238(void);
void func_150054C4(Camera *view, s32 index, s16 resource, s32 mode);

/**
 * Loads the resource's data and allocates and initialises the D_80082FA0 + 1 view (Camera) states.
 */
void func_15005290(s32 resource, s32 mode) {
    Camera *view;
    u32 size;
    s32 count;
    u32 i;

    D_800DC020 = func_1502B5C8(&size, 3, 12, resource, 8);
    D_80089550 = size / 24;
    count = D_80082FA0 + 1;
    D_800DBFF0 = allocate_memory(count * sizeof(Camera), 1, 1, 0);
    D_800DC2B0 = (s32)allocate_memory(count * 0xB0, 1, 0, 0);
    D_800DC2A0[0] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    D_800DC2A0[1] = allocate_memory(count * sizeof(Mtx), 1, 1, 0);
    for (i = 0; i <= D_80082FA0; i++) {
        view = &D_800DBFF0[i];
        D_800BE2B0[i] = (s32)allocate_memory(D_800BE620 * 2, 1, 2, 0);
        func_150054C4(view, i, resource, mode);
        {
            s32 pad; // matching: -g3 block marker keeps this store after the call

            D_800DBFF4[i] = 0;
        }
    }
    func_1512ABF8();
    func_1512D238();
    D_800894B0 = 0;
    D_800DC010 = allocate_memory(D_80082FA0 * sizeof(Camera) + sizeof(Camera), 1, 2, 0);
}

void func_1512D66C(Camera *arg0);
void func_1512D6F0(Camera *arg0);
void func_15125924(Camera *arg0);
void func_151298C0(Camera *arg0, s32 arg1, Actor *arg2);
void func_15085430(Actor *, s32, s32);

/**
 * Resets view state `index` and attaches it, as camera, to actor D_800CC2D0[index].
 */
void func_150054C4(Camera *view, s32 index, s16 resource, s32 mode) {
    u16 i;
    Actor *actor;
    u8 actorIndex;

    bzero(view, sizeof(Camera));
    view->snap = 1;
    view->playerIndex = index;
    for (i = 0; i < 21; i++) {
        func_151239CC(view, i);
        view->savedPreset[i] = 999;
    }
    actorIndex = view->playerIndex;
    actor = &D_800CC2D0[actorIndex];
    // the header types unk5D4 and unk2AC as s32, but both hold f32s
    *(f32 *)&view->unk5D4 = ((f32)D_800BE624 * 60.0f) / 216.0f;
    view->unk5D8 = ((f32)D_800BE624 * 40.0f) / 216.0f;
    view->mode = 1;
    view->unkDC = 0;
    view->player = actor;
    view->unk24C = 1.0f;
    view->unk250 = 1.0f;
    view->unk3CC = actorIndex;
    actor->camera = view;
    view->unk134 = 1;
    view->flags = 14;
    view->unk240 = 0;
    view->presetMask = 13;
    view->preset = 3;
    view->unk5CC = 1.0f;
    view->unk3A8 = 0.0f;
    func_15124B18(view);
    view->unk272 = -1;
    view->unk5FA = 1;
    view->unk3E8 = 1;
    view->unk67C = (u32)D_800B0DF0->unk2;
    view->lastPlayerPos.x = view->focus.x;
    view->lastPlayerPos.y = view->focus.y;
    view->lastPlayerPos.z = *(f32 *)&view->focus.z;
    view->unk384 = 999.0f;
    view->status |= 0x80000000;
    func_15124AB4(view);
    func_15125394(view);
    view->unk5FC = 10;
    view->unk26C = 1.0; // matching: 1.0f reorders the float constant loads; double (or int) does not
    func_1512D66C(view);
    actorIndex = view->playerIndex;
    view->unk238 = -1;
    view->unk3CC = actorIndex;
    view->controller = actorIndex;
    actor = &D_800CC2D0[actorIndex];
    view->player = actor;
    view->playerExt = actor->unk31C;
    func_151298C0(view, resource, actor);
    view->unk7CC = 2;
    view->unk3F4 = -1.0f;
    view->unk3F8 = -1.0f;
    view->unk7BC = 999.0f;
    view->unk3E0 = view->unk3A0;
    view->playerExt->unk18C = 999.0f;
    func_1512D6F0(view);
    func_15125924(view);
    view->followRateTarget = 1.0f;
    func_15085430(view->player, mode, 0);
    func_15005A60(view, resource);
    view->unk364 = -10000.0f;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->savedTarget = view->target;
    view->savedEye = view->eye;
    view->unk7E8 = 50.0f;
    view->pitchOverride = 999.0f;
    view->unk8BC = D_800BE2B0[view->playerIndex];
    D_80089520 = 0;
    D_800C3671 = 0;
    D_80089524[view->playerIndex] = 0;
    D_80089534[view->playerIndex] = 0;
    view->unk5FE = 60;
    view->snap = 1;
}

/**
 * Moves camera `view` to `eye`, aims it at `actor`'s position and resets its derived state.
 */
void func_15005818(Camera *view, Actor *actor, Vec3f *eye) {
    Vec3f *viewEye;

    view->snap = 1;
    view->eye.x = eye->x;
    view->eye.y = eye->y;
    view->eye.z = eye->z;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    view->unk27C = view->eye;
    view->prevEye = view->eye;

    viewEye = &view->eye; // matching: kept live across the call in a stack slot
    func_1510E7A4(&view->unk644, &view->unk648, &view->unk354, &view->unk360, &view->unk640, 0,
                  view->eye.x, view->eye.y, view->eye.z, view->eye.y, 0, 0, -10000.0f, view->eye.y);

    view->unk358 = view->unk354;
    view->unk35C = view->unk354;

    view->focus.x = actor->x_position;
    view->focus.y = actor->y_position;
    *(f32 *)&view->focus.z = actor->z_position;
    view->target = view->focus;
    view->lastPlayerPos = view->focus;

    // matching: z is read through the vector (unk2AC is typed s32); *(f32 *)&view->focus.z reorders the loads
    view->targetDistance = sqrtf((view->focus.x - view->eye.x) * (view->focus.x - view->eye.x) +
                         ((&view->focus)->z - view->eye.z) * ((&view->focus)->z - view->eye.z));
    view->unk34C = view->eye.y - view->unk354;
    view->height = view->eye.y - view->unk354;

    view->savedTarget = view->target;
    view->savedEye = *viewEye;
    func_15124B18(view);
    func_15125330(view);
    func_1512523C(view);
    func_15123070(view);

    view->status |= 4;
    if (D_800BE9F0 != 0x21 && D_800C35EA[0] != 1) {
        func_1512D560(view, 5, NULL);
        // the header declares D_800C3600 as a struct168 pointer, but it is the struct itself
        D_800C3600.unkC = view->yaw;
        func_1512D560(view, 8, &D_800C3600);
        func_1512D560(view, 6, NULL);
    }
}
