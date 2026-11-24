#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_1503453C(s32);
extern void func_15036310(s32, s32, s32, s32);
extern void func_15034EB4(struct127 *, s32, s32);
extern void func_150F1A00(struct127 *);
extern void func_150DFDA4(struct127 *);


typedef struct GameCommand {
    s16 opcode;
    s16 value;
} GameCommand;

GameCommand *func_150341BC(GameCommand *, s32);
s16 *func_15034340(s16 *, s32);
s16 *func_150343B0(s16 *, s32);
GameCommand *func_15034420(GameCommand *, s32);
s16 *func_150344A0(s16 *, s32);
void *func_150BE438(void *, s32);
GameCommand *func_150B58F0(GameCommand *, s32);
GameCommand *func_150F1B48(GameCommand *, s32);
GameCommand *func_150D8590(GameCommand *, s32);
s32 func_1503DA3C(s32, s32);
s16 *func_15034860(GameCommand *, struct127 *, s32, s32);

void func_15033FE0(GameCommand *arg0, s32 arg1) {
    struct127 *actor;
    s32 first;
    GameCommand *cursor;

    actor = &D_800CC2D0[arg1];
    cursor = arg0;
    if (actor->unk1D4 == 0) {
        arg0->opcode = 0x1000;
        return;
    }
    if (actor->interaction_state == 1) {
        cursor = func_150341BC(arg0, arg1);
    } else {
        switch (actor->id) {
        case 0x2B:
            cursor = func_150D8590(arg0, arg1);
            break;
        case 0x28:
        case 0x77:
            cursor = (GameCommand *)func_150344A0(&arg0->opcode, arg1);
            break;
        case 0x4B:
            cursor = func_150B58F0(arg0, arg1);
            break;
        case 0x7B:
            cursor = func_150F1B48(arg0, arg1);
            break;
        case 0x8A:
            cursor = func_15034420(arg0, arg1);
            break;
        case 0x21:
            cursor = func_150BE438(arg0, arg1);
            break;
        case 0x98:
            cursor = (GameCommand *)func_15034340((s16 *)arg0, arg1);
            break;
        case 0x5A:
        case 0x74:
        case 0x7A:
        case 0x8D:
            cursor = (GameCommand *)func_150343B0((s16 *)arg0, arg1);
            break;
        }
    }
    first = func_1503DA3C(actor->id, 0);
    *func_15034860(cursor, actor, first, func_1503DA3C(actor->id, 1)) = 0x1000;
}

GameCommand *func_150341BC(GameCommand *arg0, s32 arg1) {
    struct127 *obj;
    struct126 *sub;
    s16 angle;
    s32 id;
    s32 op;

    obj = &D_800CC2D0[arg1];
    sub = obj->unk31C;
    if ((sub != NULL) && !(obj->unk2F8 & 0x80)) {
        angle = *(f32 *)&obj->pad54 * 182.0444489f;
        id = obj->unk84.uh;
        if ((id != 0x42) && (id != 0x14) && (id != 0x76)) {
            op = (sub->unk78 == 0xA) ? 0 : 4;
            arg0->opcode = op + 6;
            arg0->value = angle;
            arg0++;
        } else {
            arg0->opcode = 0xA;
            arg0->value = angle;
            arg0++;
            arg0->opcode = 4;
            arg0->value = -angle;
        lbl:
            arg0++;
        }
    }
    if (obj->unk1D1 != 0) {
        arg0->opcode = 6;
        arg0++;
        arg0[-1].value = obj->unk1D1 * 200;
    }
    if ((D_800CC2D0[D_800C3E78].unk31C != NULL) && (D_800CC2D0[D_800C3E78].unk31C->unk12 != 0)) {
        arg0->opcode = 8;
        arg0++;
        arg0[-1].value = -D_800CC2D0[D_800C3E78].unk31C->unk12;
    }
    return arg0;
}

s16 *func_15034340(s16 *arg0, s32 arg1) {
    struct127 *obj = &D_800CC2D0[arg1];

    if (obj->unk1D1 != 0) {
        *arg0 = 6;
        arg0 += 2;
        arg0[-1] = obj->unk1D1 * 200;
    }
    return arg0;
}

s16 *func_150343B0(s16 *arg0, s32 arg1) {
    struct127 *obj = &D_800CC2D0[arg1];

    if (obj->unk1D1 != 0) {
        *arg0 = 6;
        arg0 += 2;
        arg0[-1] = obj->unk1D1 * 200;
    }
    return arg0;
}

extern f32 D_80097D18;

GameCommand *func_15034420(GameCommand *arg0, s32 arg1) {
    s16 part;
    s16 value;

    value = (f32)D_800CC2D0[arg1].unk2E4 * D_80097D18;
    part = value >> 4;
    value -= part;
    arg0[0].value = part;
    arg0[1].value = value;
    arg0[0].opcode = 0;
    arg0[1].opcode = 0x18;
    return arg0 + 2;
}
s16 *func_150344A0(s16 *arg0, s32 arg1);

void func_1503B9BC(void);
void func_1503453C(s32 arg0);
void func_150345E4(s32 arg0);

extern s32 func_1502DB20(u8);
void func_15034728(struct127 *arg0);


extern void func_15034728(struct127 *);
void func_150347E8(void);
