#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_1510E7A4(void *, void *, f32 *, void *, void *, s32, f32, f32, f32, f32, u16, s32, f32, f32);
void func_15123070(struct108 *);
void func_1512D560(struct108 *, s32, struct168 *);

extern void *allocate_memory(s32, s32, s32, s32);
extern s32 func_1502B5C8(u32 *, s32, s32, s32, s32);
extern void func_1512ABF8(void);
extern void func_1512D238(void);
void func_150054C4(struct108 *view, s32 index, s16 resource, s32 mode);

/**
 * Loads the resource's data and allocates and initialises the D_80082FA0 + 1 view (struct108) states.
 */
void func_15005290(s32 resource, s32 mode) {
    struct108 *view;
    u32 size;
    s32 count;
    u32 i;

    D_800DC020 = func_1502B5C8(&size, 3, 12, resource, 8);
    D_80089550 = size / 24;
    count = D_80082FA0 + 1;
    D_800DBFF0 = allocate_memory(count * sizeof(struct108), 1, 1, 0);
    D_800DC2B0 = (s32)allocate_memory(count * 0xB0, 1, 0, 0);
    // matching: header declares D_800DC2A0 as a pointer, but it is an array of two pointers
    ((s32 *)&D_800DC2A0)[0] = (s32)allocate_memory(count * 64, 1, 1, 0);
    ((s32 *)&D_800DC2A0)[1] = (s32)allocate_memory(count * 64, 1, 1, 0);
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
    D_800DC010 = (s32)allocate_memory(D_80082FA0 * sizeof(struct108) + sizeof(struct108), 1, 2, 0);
}

void func_1512D66C(struct108 *arg0);
void func_1512D6F0(struct108 *arg0);
void func_15125924(struct108 *arg0);
void func_151298C0(struct108 *arg0, s32 arg1, struct127 *arg2);
void func_15085430(struct127 *, s32, s32);

/**
 * Resets view state `index` and attaches it, as camera, to actor D_800CC2D0[index].
 */
void func_150054C4(struct108 *view, s32 index, s16 resource, s32 mode) {
    u16 i;
    struct127 *actor;
    u8 actorIndex;

    bzero(view, sizeof(struct108));
    view->unk23C = 1;
    view->unk23D = index;
    for (i = 0; i < 21; i++) {
        func_151239CC(view, i);
        view->unk1B6[i] = 999;
    }
    actorIndex = view->unk23D;
    actor = &D_800CC2D0[actorIndex];
    // the header types unk5D4 and unk2AC as s32, but both hold f32s
    *(f32 *)&view->unk5D4 = ((f32)D_800BE624 * 60.0f) / 216.0f;
    view->unk5D8 = ((f32)D_800BE624 * 40.0f) / 216.0f;
    view->unk2C = 1;
    view->unkDC = 0;
    view->unk3D0 = actor;
    view->unk24C = 1.0f;
    view->unk250 = 1.0f;
    view->unk3CC = actorIndex;
    actor->camera = view;
    view->unk134 = 1;
    view->unk84 = 14;
    view->unk240 = 0;
    view->unk1E0 = 13;
    view->unk1B4 = 3;
    view->unk5CC = 1.0f;
    view->unk3A8 = 0.0f;
    func_15124B18(view);
    view->unk272 = -1;
    view->unk5FA = 1;
    view->unk3E8 = 1;
    view->unk67C = (u32)D_800B0DF0->unk2;
    view->unk2B0 = view->unk2A4;
    view->unk2B4 = view->unk2A8;
    view->unk2B8 = *(f32 *)&view->unk2AC;
    view->unk384 = 999.0f;
    view->unk5F0 |= 0x80000000;
    func_15124AB4(view);
    func_15125394(view);
    view->unk5FC = 10;
    view->unk26C = 1.0; // matching: 1.0f reorders the float constant loads; double (or int) does not
    func_1512D66C(view);
    actorIndex = view->unk23D;
    view->unk238 = -1;
    view->unk3CC = actorIndex;
    view->unk368 = actorIndex;
    actor = &D_800CC2D0[actorIndex];
    view->unk3D0 = actor;
    view->unk3D4 = (struct127 *)actor->unk31C; // header types unk31C as struct126 *
    func_151298C0(view, resource, actor);
    view->unk7CC = 2;
    view->unk3F4 = -1.0f;
    view->unk3F8 = -1.0f;
    view->unk7BC = 999.0f;
    view->unk3E0 = view->unk3A0;
    view->unk3D4->unk18C = 999.0f;
    func_1512D6F0(view);
    func_15125924(view);
    view->unk674 = 1.0f;
    func_15085430(view->unk3D0, mode, 0);
    func_15005A60(view, resource);
    view->unk364 = -10000.0f;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    *(vertex *)&view->unk2E0 = *(vertex *)&view->unk2BC;
    *(vertex *)&view->unk2EC = *(vertex *)&view->unk2F8;
    view->unk7E8 = 50.0f;
    view->unk86C = 999.0f;
    view->unk8BC = D_800BE2B0[view->unk23D];
    D_80089520 = 0;
    D_800C3671 = 0;
    D_80089524[view->unk23D] = 0;
    D_80089534[view->unk23D] = 0;
    view->unk5FE = 60;
    view->unk23C = 1;
}

/**
 * Moves camera `view` to `eye`, aims it at `actor`'s position and resets its derived state.
 */
void func_15005818(struct108 *view, struct127 *actor, vertex *eye) {
    vertex *viewEye;

    view->unk23C = 1;
    view->unk2F8 = eye->x;
    view->unk2FC = eye->y;
    view->unk300 = eye->z;
    // matching: word-wise 12-byte struct copies; the header splits these vectors into scalars
    *(vertex *)((u8 *)view + 0x27C) = *(vertex *)&view->unk2F8; // struct108 has no field at 0x27C
    *(vertex *)&view->unk304 = *(vertex *)&view->unk2F8;

    viewEye = (vertex *)&view->unk2F8; // matching: kept live across the call in a stack slot
    func_1510E7A4((u8 *)view + 0x644, (u8 *)view + 0x648, &view->unk354, &view->pad360, (u8 *)view + 0x640, 0,
                  view->unk2F8, view->unk2FC, view->unk300, view->unk2FC, 0, 0, -10000.0f, view->unk2FC);

    view->unk358 = view->unk354;
    view->unk35C = view->unk354;

    view->unk2A4 = actor->x_position;
    view->unk2A8 = actor->y_position;
    *(f32 *)&view->unk2AC = actor->z_position;
    *(vertex *)&view->unk2BC = *(vertex *)&view->unk2A4;
    *(vertex *)&view->unk2B0 = *(vertex *)&view->unk2A4;

    // matching: z is read through the vector (unk2AC is typed s32); *(f32 *)&view->unk2AC reorders the loads
    view->unk374 = sqrtf((view->unk2A4 - view->unk2F8) * (view->unk2A4 - view->unk2F8) +
                         (((vertex *)&view->unk2A4)->z - view->unk300) * (((vertex *)&view->unk2A4)->z - view->unk300));
    view->unk34C = view->unk2FC - view->unk354;
    view->unk348 = view->unk2FC - view->unk354;

    *(vertex *)&view->unk2E0 = *(vertex *)&view->unk2BC;
    *(vertex *)&view->unk2EC = *viewEye;
    func_15124B18(view);
    func_15125330(view);
    func_1512523C(view);
    func_15123070(view);

    view->unk5F0 |= 4;
    if (D_800BE9F0 != 0x21 && D_800C35EA != 1) {
        func_1512D560(view, 5, NULL);
        // the header declares D_800C3600 as a struct168 pointer, but it is the struct itself
        *(f32 *)((u8 *)&D_800C3600 + 0xC) = view->unk37C;
        func_1512D560(view, 8, (struct168 *)&D_800C3600);
        func_1512D560(view, 6, NULL);
    }
}
