#include <ultra64.h>

#define func_1515D4D4 func_1515D4D4_header
#include "functions.h"
#undef func_1515D4D4
void func_1515D4D4();
#define D_800D3098 D_800D3098_header
#define D_800DCD30 D_800DCD30_header
#include "variables.h"
#undef D_800D3098
#undef D_800DCD30

extern u8 D_800DCD30[4][3];

extern f32 D_80095C64[];
extern u8 D_800DDBD0[];
s32 func_1500AF08(s32, s32, s32, s16, s16, s16, s32, u8, s32);
void func_15009334(s32 arg0);

/** Resets the lighting state: default ambient colour, fog colours and the light lists. */
void func_150091D0(void) {
    s32 i;

    D_800DCD78 = 0;
    D_800DCD7C = 1;
    D_800DCD80 = 0;
    D_800DCD84 = 0;

    func_1515D4D4(255, 255, 255, 0);

    D_800DCD24[0] = 0;
    D_800DCD24[1] = 0;
    D_800DCD24[2] = 0;

    D_800DCD28[0] = 255;
    D_800DCD28[1] = 255;
    D_800DCD28[2] = 255;

    for (i = 0; i < 4; i++) {
        D_800DCD30[i][0] = 127;
        D_800DCD30[i][1] = 0;
        D_800DCD30[i][2] = 0;
    }
    D_800DCD3C = 0;
    D_800DCD3D = 0;
    bzero(&D_800DCD40, 56);
}

void func_150092DC(void) {
    u32 i;
    for (i = 0; i < D_800D3094; i++) {
        func_15009334(i);
    }
}

typedef struct Game36680Point {
    s16 position[3];
    u8 pad6[0xF];
    u8 type;
    u8 flags;
    u8 pad17;
    u32 color;
    u32 settings;
    u32 action;
    u8 pad24[0x10];
} Game36680Point; // size 0x34

typedef struct Game36680PointLink {
    struct Game36680PointLink *next;
    Game36680Point *point;
} Game36680PointLink;

extern Game36680Point *D_800D3098;
extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1515D5AC(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/** Applies one level point: sets ambient/fog colours, spawns a light, or links it into a point list. */
void func_15009334(s32 index) {
    s32 pad;
    s32 red;
    s32 green;
    s32 blue;
    s32 x;
    s32 y;
    s32 z;
    s32 settings;
    s32 strength;
    s32 enabled;
    s32 duration;
    s32 light;
    s32 callback;
    Game36680PointLink *link;
    s32 type;

    type = D_800D3098[index].type >> 2;
    switch (type) {
        case 9:
        case 10:
        case 16:
            red = D_800D3098[index].color & 0xFF;
            green = (D_800D3098[index].color >> 8) & 0xFF;
            blue = (D_800D3098[index].color >> 16) & 0xFF;
            D_800D3098[index].flags |= 4;
            switch (type) {
                case 10:
                    func_1515D4D4(red, green, blue, 0);
                    if (D_800D3098[index].action != 0) {
                        func_1500AB5C(D_800D3098[index].action);
                    }
                    break;
                case 9:
                    x = D_800D3098[index].position[0];
                    y = D_800D3098[index].position[1];
                    z = D_800D3098[index].position[2];
                    settings = (D_800D3098[index].settings >> 8) & 0xFF;
                    strength = (D_800D3098[index].color >> 24) & 0x7F;
                    enabled = (D_800D3098[index].color >> 31) & 1;
                    callback = D_800D3098[index].action & 0xFFFF;
                    duration = (D_800D3098[index].action >> 16) & 0xFFFF;
                    light = func_1515D5AC(x, y, z, settings, red, green, blue, strength, enabled, duration);
                    if (light != 0 && callback != 0) {
                        func_1500A94C(light, callback);
                    }
                    break;
                case 16:
                    D_800DCD24[0] = red;
                    D_800DCD24[1] = green;
                    D_800DCD24[2] = blue;
                    D_800DCD28[0] = D_800D3098[index].settings & 0xFF;
                    D_800DCD28[1] = (D_800D3098[index].settings >> 8) & 0xFF;
                    D_800DCD28[2] = (D_800D3098[index].settings >> 16) & 0xFF;
                    D_800DCD3C = (D_800D3098[index].settings >> 24) & 0xFF;
                    D_800DCD3D = (D_800D3098[index].color >> 24) & 0xFF;
                    break;
            }
            break;
        case 15:
            link = allocate_memory(sizeof(Game36680PointLink), 1, 0, 0);
            link->point = &D_800D3098[index];
            link->next = (Game36680PointLink *)D_800DCD80;
            D_800DCD80 = (s32)link;
        unused_label: // matching: an unused label keeps IDO from threading this break to the epilogue
            break;
        case 24:
            link = allocate_memory(sizeof(Game36680PointLink), 1, 0, 0);
            link->point = &D_800D3098[index];
            link->next = (Game36680PointLink *)D_800DCD84;
            D_800DCD84 = (s32)link;
            break;
    }
}

struct225 *func_1516037C();
struct225 *func_1516387C();

void func_150095D8(s32 light, s32 id) {
    func_1516127C(light, 255, 1);
}

void func_15009600(s32 light, s32 id) {
    func_15161334(light, 255, 1);
}

void func_15009628(s32 light, s32 id) {
    struct220 header;
    u8 kinds[4] = D_80082BE0;

    header.unk0 = 0;
    header.unk1 = -1;
    header.unk2 = 300;
    header.unk4 = kinds[id - 3];

    func_1516037C(&header, light, 0, 255, 1);
}

void func_1500969C(s32 light, s32 id) {
    func_15161408(light, 255, 1);
}

void func_150096C4(s32 light, s32 id) {
    struct220 header;
    u8 kinds[5] = D_80082BE4;

    header.unk0 = 0;
    header.unk1 = -1;
    header.unk2 = 300;
    header.unk4 = kinds[id - 8];

    func_1516037C(&header, light, 0, 255, 1);
}

void func_15009740(s32 light, s32 id) {
    func_15161494(light, 255, 1);
}

void func_15009768(s32 light, s32 id) {
    func_151615F8(light, 0, 0, 14, 255, 1);
}

void func_150097A4(s32 light, s32 id) {
    func_15161540(light, 255, 1);
}

void func_150097CC(s32 light, s32 id) {
    s32 effect = func_151615F8(light, 0, 2, 14, 255, 1);

    if (effect != 0) {
        func_150121C0(effect);
    }
}

void func_15009818(s32 light, s32 id) {
    func_151B8DB0(light, 0, 255, 1);
}

void func_15009844(s32 light, s32 id) {
    func_151B8DB0(light, 1, 255, 1);
}

void func_15009870(s32 light, s32 id) {
    func_15108850(light);
}

void func_15009894(s32 light, s32 id) {
    func_151615F8(light, 1, 0, 14, 255, 1);
}

void func_150098D0(s32 light, s32 id) {
    func_15162034(light, 255, 1);
}

void func_150098F8(s32 light, s32 id) {
    func_151623F4(light, 1, 22, 0, -1, 300, 255, 1);
}

void func_15009944(s32 light, s32 id) {
    func_15162740(light, 1, 6, 0, 300, 2, 255, 1);
}

void func_15009990(s32 light, s32 id) {
    struct132 *effect;
    u8 extra[1];

    extra[0] = D_80095B50[id];
    effect = func_1516295C(light, D_80095B48[id], D_80095B4C[id], 2, 300, 12, -1, 0, -1, 1, 1, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->unk48, extra, 1);
    }
}

void func_15009A38(s32 light, s32 id) {
    struct220 header;

    header.unk0 = 2;
    header.unk1 = -1;
    header.unk2 = 300;
    header.unk4 = D_80095B27[id];
    func_1516037C(&header, light, 0, 255, 1);
}

void func_15009AA0(s32 light, s32 id) {
    func_15162FAC(light, 1, 0, 2, 300, 0xD, 255, 1);
}

void func_15009AEC(s32 light, s32 id) {
    func_15162740(light, 2, 6, 0, 300, 2, 255, 1);
}

void func_15009B38(s32 light, s32 id) {
    func_15162740(light, 20, 6, 0, 300, 2, 255, 1);
}

void func_15009B84(s32 light, s32 id) {
    func_15162740(light, 3, 6, 0, 300, 2, 255, 1);
}

void func_15009BD0(s32 light, s32 id) {
    struct225 *effect;
    struct221 params;

    params.unk4 = 0.0f;
    params.unk8 = 90.0f;
    params.unk0 = func_15187EC0(0, 0.0f, 0, 0, 0, 220, 220, 255);
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->unk18, &params, sizeof(params));
    }
}

void func_15009C7C(s32 light, s32 id) {
    struct225 *effect;
    struct221 params;

    params.unk4 = 0.0f;
    params.unk8 = 50.0f;
    params.unk0 = func_15187EC0(0, 0.0f, 0, 0, 0, 220, 220, 255);
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->unk18, &params, sizeof(params));
    }
}

void func_15009D28(s32 light, s32 id) {
    func_15163604(light, 1, 5, 300, 0, 0, 255, 1);
}

void func_15009D6C(s32 light, s32 id) {
    func_15163604(light, 2, 5, 300, 0, 0, 255, 1);
}

void func_15009DB0(s32 light, s32 id) {
    func_15162740(light, 4, 6, 0, 300, 2, 255, 1);
}

void func_15009DFC(s32 light, s32 id) {
    func_15162740(light, 5, 6, 0, 300, 2, 255, 1);
}

void func_15009E48(s32 light, s32 id) {
    func_151615F8(light, 1, 1, 14, 255, 1);
}

void func_15009E84(s32 light, s32 id) {
    func_15163604(light, 3, 5, 300, 0, 0, 255, 1);
}

void func_15009EC8(s32 light, s32 id) {
    func_1516381C(light, 32, 255, 1);
}

void func_15009EF4(s32 light, s32 id) {
    func_151615F8(light, 1, 3, 35, 255, 1);
}

void func_15009F30(s32 light, s32 id) {
    func_15163604(light, 4, 36, 300, 0, 0, 255, 1);
}

void func_15009F74(s32 light, s32 id) {
    struct225 *effect;
    struct221 params;

    params.unk4 = 0.0f;
    params.unk8 = 60.0f;
    params.unk0 = func_15187EC0(1, 0.1f, 100, 100, 100, 255, 100, 100);
    effect = func_1516387C(light, 2, 16, 300, 0, 12, 255, 1);
    if (effect != NULL) {
        memcpy(&effect->unk18, &params, sizeof(params));
    }
}

void func_1500A028(s32 light, s32 id) {
    func_15163604(light, 5, 5, 300, 0, 0, 255, 1);
}

void func_1500A06C(s32 light, s32 id) {
    func_15163604(light, 6, 5, 300, 0, 0, 255, 1);
}

void func_1500A0B0(s32 light, s32 id) {
    func_15162740(light, 7, 6, 0, 300, 2, 255, 1);
}

void func_1500A0FC(s32 light, s32 id) {
    func_15162740(light, 9, 6, 0, 300, 2, 255, 1);
}

void func_1500A148(s32 light, s32 id) {
    func_15162740(light, 10, 6, 0, 300, 2, 255, 1);
}

void func_1500A194(s32 light, s32 id) {
    func_15162740(light, 11, 38, 0, 300, 2, 255, 1);
}

void func_1500A1E0(s32 light, s32 id) {
    func_151615F8(light, 1, 4, 14, 255, 1);
}

void func_1500A21C(s32 light, s32 id) {
    func_15163604(light, 7, 5, 300, 0, 0, 255, 1);
}

void func_1500A260(s32 light, s32 id) {
    func_15162FAC(light, 2, 0, 2, 300, 13, 255, 1);
}

void func_1500A2AC(s32 light, s32 id) {
    u8 kinds[4] = D_80095B70;

    func_151B8DB0(light, kinds[id - 51], 255, 1);
}

void func_1500A2F4(s32 light, s32 id) {
    u8 kinds[2] = D_80095B74;

    func_15164208(light, kinds[id - 56], 255, 1);
}

void func_1500A33C(s32 light, s32 id) {
    func_15163604(light, 8, 5, 300, 0, 0, 255, 1);
}

void func_1500A380(s32 light, s32 id) {
    func_15163604(light, 9, 40, 300, 0, 0, 255, 1);
}

void func_1500A3C4(s32 light, s32 id) {
    func_15162740(light, 15, 6, 0, 300, 2, 255, 1);
}

void func_1500A410(s32 light, s32 id) {
    func_15163604(light, 10, 5, 300, 0, 0, 255, 1);
}

void func_1500A454(s32 light, s32 id) {
    func_151615F8(light, 0, 5, 14, 255, 1);
}

void func_1500A490(s32 light, s32 id) {
    func_15163604(light, 11, 5, 300, 0, 0, 255, 1);
}

void func_1500A4D4(s32 light, s32 id) {
    func_15163604(light, 12, 41, 300, 0, 0, 255, 1);
}

void func_1500A518(s32 light, s32 id) {
    func_15163604(light, 13, 41, 300, 0, 0, 255, 1);
}

void func_1500A55C(s32 light, s32 id) {
    func_151623F4(light, 1, 22, 0, -1, 300, 255, 1);
}

void func_1500A5A8(s32 light, s32 id) {
    func_15162740(light, 17, 6, 0, 300, 2, 255, 1);
}

void func_1500A5F4(s32 light, s32 id) {
    func_15162740(light, 18, 6, 0, 300, 2, 255, 1);
}

void func_1500A640(s32 light, s32 id) {
    func_15162740(light, 19, 6, 0, 300, 2, 255, 1);
}

void func_1500A68C(s32 light, s32 id) {
    func_15162740(light, 21, 6, 0, 300, 2, 255, 1);
}

extern u8 D_80082BEC[2][3];
s32 func_150B060C(u8, void *, s32);

typedef struct Game36680AttachmentObject {
    u8 pad0[0x10];
    s16 x;
    s16 y;
    s16 z;
} Game36680AttachmentObject;

/** Payload of an effect that attaches a light to a level object. */
typedef struct Game36680LightAttachment {
    f32 offset;
    f32 scale;
    Game36680AttachmentObject *object;
    f32 x;
    f32 y;
    f32 z;
} Game36680LightAttachment;

void func_1500A6D8(s32 light, s32 id) {
    struct220 header;
    Game36680LightAttachment attachment;
    s32 variant;
    u8 objects[2][3] = D_80082BEC;
    struct225 *effect;

    variant = (D_800BE9F0 == 6) ? 1 : 0;
    if (func_150B060C(objects[variant][id - 73], &attachment, id) != 0) {
        header.unk0 = 0;
        header.unk1 = 23;
        header.unk2 = 300;
        header.unk4 = 0;
        effect = func_1516037C(&header, light, sizeof(attachment), 255, 1);
        if (effect != NULL) {
            memcpy(&effect->unk18, &attachment, sizeof(attachment));
        }
    }
}

void func_1500A79C(s32 light, s32 id) {
    func_15162740(light, 22, 6, 0, 300, 2, 255, 1);
}

extern u8 D_80082BF4[6];

extern Game36680AttachmentObject *func_151149AC(u8);

void func_1500A7E8(s32 light, s32 id) {
    struct220 header;
    Game36680LightAttachment attachment;
    struct225 *effect;
    u8 objects[6] = D_80082BF4;

    attachment.object = func_151149AC(objects[id - 80]);
    if (attachment.object != NULL) {
        attachment.offset = -400.0f;
        attachment.scale = 4.0f;
        attachment.x = attachment.object->x;
        attachment.y = attachment.object->y;
        attachment.z = attachment.object->z;
        header.unk0 = 0;
        header.unk1 = 23;
        header.unk2 = 300;
        header.unk4 = 0;
        effect = func_1516037C(&header, light, sizeof(attachment), 255, 1);
        if (effect != NULL) {
            memcpy(&effect->unk18, &attachment, sizeof(attachment));
        }
    }
}

void func_1500A8C8(s32 light, s32 id) {
    func_151615F8(light, 0, 6, 14, 255, 1);
}

void func_1500A904(s32 light, s32 id) {
    func_151615F8(light, 1, (u8)(id - 80), 14, 255, 1);
}

/** Runs light callback `id` (1..88) from the D_80082BFC table on a freshly placed light. */
void func_1500A94C(s32 light, s32 id) {
    if (id > 0 && id < 89) {
        if (D_80082BFC[id] != NULL) {
            D_80082BFC[id](light);
        }
    }
}

/** Payload (at +0x28) of the func_15149130 effect spawned by func_1500A990. */
typedef struct Game36680ParticleEffect {
    u8 pad0[0x28];
    f32 params[12];
} Game36680ParticleEffect;

/** Ambient action 2: spawns a particle effect with three random angles. */
void func_1500A990(s32 arg0) {
    f32 params[12];
    f32 fraction;
    Game36680ParticleEffect *effect;

    params[0] = 35.0f;
    params[1] = 35.0f;
    params[2] = 60.0f;
    params[3] = 34.0f;
    params[4] = 30.0f;
    params[5] = 50.0f;

    fraction = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
    fraction *= 1.5259022e-05f; // 1/65535
    params[6] = (fraction + fraction) * 3.1415927f;

    fraction = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
    fraction *= 1.5259022e-05f;
    params[7] = (fraction + fraction) * 3.1415927f;

    fraction = (u32)(osGetCount() * func_150ADA20()) & 0xFFFF;
    fraction *= 1.5259022e-05f;

    params[8] = (fraction + fraction) * 3.1415927f;
    params[9] = -0.1370000094f;
    params[10] = 0.048f;
    params[11] = 0.007f;

    effect = (Game36680ParticleEffect *)func_15149130(300, -1, 45, -1, 0, 0, sizeof(params), 255, 1);
    if (effect != NULL) {
        memcpy(effect->params, params, sizeof(params));
    }
}

/** Runs ambient action `action` (1..5) from the D_80082D60 table. */
void func_1500AB5C(s32 action) {
    if (action > 0 && action < 6) {
        if (D_80082D60[action] != NULL) {
            D_80082D60[action]();
        }
    }
}

void func_1500ABA0(s32 arg0) {
    bzero(&D_800BE4A0, 60);
    func_1502B8E0(&D_800BE4A0, 60, 3, 12, arg0, 10);
    bzero(&D_800DDA90, 240);
    bzero(&D_800DD478, 1560);
}

// GOTO hell
typedef struct Game36680ResourceInfo {
    u8 flags;
    u8 frames;
    u16 length;
    s16 x;
    s16 y;
    u8 width;
    u8 pad9;
} Game36680ResourceInfo;

struct s1500AC14_a {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 unkC;
};
extern s32 func_1502B020(s32 *, s32, s32, s32);
extern s16 D_800DDB80[][2];
extern u8 D_80095CE0[];
extern u16 D_800BE478[];

#define P ((struct s1500AC14_a *)&D_800DDA90)
#define INFO(id) (&((Game36680ResourceInfo *)D_80095CE0)[id])
void func_1500AC14(void) {
    s32 i;
    s32 id;
    s32 size;
    u32 len;

    for (i = 0; i != 15; i++) {
        P[i].unk0 = 0;
        P[i].unk8 = 0;
        P[i].unk4 = 0;
        P[i].unkC = 0;
        id = ((u16 (*)[2])&D_800BE4A0)[i][0];
        if (id != 0xFF && (D_800BE478[i] != 0 || ((u16 (*)[2])&D_800BE4A0)[i][0] == 0x3B)) {
            P[i].unk0 = func_1502B020(&size, 2, 0, id);
            P[i].unk4 = (size + 0xF) & ~0xF;
            len = INFO(id)->length;
            if (size == 0 || len == 0) {
                len = 0;
                P[i].unk0 = 0;
            }
            D_800DDB80[i][0] = (INFO(id)->width << 5) - 1;
            D_800DDB80[i][1] = (((len / INFO(id)->width) << 6) / 5) - 1;
        }
    }
}

extern u8 D_80095CE0[];
extern u16 D_800BE478[];
extern u8 D_80095CC4[];
extern s16 D_800BE438[][2];
extern u8 D_800DDBC0[];
extern u8 D_800DDBE0[];
extern u8 D_800DDBF0[];
extern u8 D_800DDC20[][2];
extern u8 D_800DDC80[];

void func_1500AD84(s32 arg0, s32 arg1, s32 arg2) {
    Game36680ResourceInfo *info;
    u8 *cursor;
    s32 flags;

    if (arg1 != 255) {
        cursor = &D_800DDBD0[arg0];
        info = &((Game36680ResourceInfo *)D_80095CE0)[arg1];
        *cursor = info->frames;
        flags = info->flags;
        if (flags & 0x10) {
            *cursor -= 1;
            D_800DDBE0[arg0] = *cursor;
        } else {
            D_800DDBE0[arg0] = 0;
        }
        if (flags & 0x80) {
            D_800DDBF0[arg0] = 2;
        } else {
            D_800DDBF0[arg0] = 1;
        }
        cursor = (u8 *)D_800BE438[arg0];
        ((s16 *)cursor)[0] = info->x;
        ((s16 *)cursor)[1] = info->y;
        D_800DDBC0[arg0] = arg1;
        D_800DDC80[arg0] = arg2 & 0xFF7F;
        if (arg2 & 0x80) {
            D_800DDC00 |= 1 << arg0;
        }
        if (flags & 1) {
            D_800DDC04 |= 1 << arg0;
        }
        if (flags & 8) {
            D_800DDC08 |= 1 << arg0;
        }
    } else {
        D_800DDBD0[arg0] = 0;
    }
    arg1 = D_80095CC4[arg0];
    D_800BE478[arg0] = 0;
    cursor = D_800DDC20[arg0];
    cursor[0] = arg1;
    cursor[1] = arg1;
}
struct Struct800DDD18
{
  s16 unk0;
  s16 unk2;
  s16 unk4;
  u8  unk6;
  u8  unk7;
  u8  unk8;
  u8  unk9;
  u8  unkA;
  u8  unkB;
  u8  unkC;
  u8  unkD;
};
extern struct Struct800DDD18 *D_800DDD18;
extern u8 D_800DDBC0[];
extern u8 D_80095CE0[];
extern s16 D_800BE438[][2];
extern s32 func_151EF610(void);

s32 func_1500AF08(s32 arg0, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, u8 arg7, s32 arg8) {
    s16 scale;

    if (arg0 >= (u32)(*(u16 *)((u8 *)D_800B0DF0 + 0x18) - 1)) {
        return *(u16 *)((u8 *)D_800B0DF0 + 0x18) - 1;
    }

    D_800DDD18[arg0].unk0 = arg3;
    D_800DDD18[arg0].unk2 = arg4;
    D_800DDD18[arg0].unk4 = arg5;

    if (D_800DDBD0[arg1] >= 2) {
        if ((1 << arg1) & D_800DDC00) {
            D_800DDD18[arg0].unk7 = (arg1 << 2) + (arg8 & 1);
        } else {
            D_800DDD18[arg0].unk7 = (arg0 & 3) + (arg1 << 2);
        }
    } else {
        D_800DDD18[arg0].unk7 = 60;
    }

    D_800DDD18[arg0].unk6 = (arg2 << 5) | arg1;
    D_800DDD18[arg0].unkA = arg7;
    D_800DDD18[arg0].unk8 = D_800DDBC0[arg1];

    if (arg6 != 0) {
        scale = (func_151EF610() % arg6 + arg6) - arg6 + 0x100;
        D_800DDD18[arg0].unkC = (D_800BE438[arg1][0] * scale) >> 10;
        if (D_80095CE0[D_800DDBC0[arg1] * 10] & 4) {
            scale = func_151EF610() % arg6 * 4 + scale;
        }
        D_800DDD18[arg0].unkD = (D_800BE438[arg1][1] * scale) >> 10;
    } else {
        D_800DDD18[arg0].unkC = D_800BE438[arg1][0] >> 2;
        D_800DDD18[arg0].unkD = D_800BE438[arg1][1] >> 2;
    }
    return arg0 + 1;
}
s32 func_1500B1F4(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    s32 i;
    s32 count;
    s32 ret;
    f32 *weights;
    s16 *p0;
    s16 *p1;
    s16 *p2;
    f32 w0;
    f32 w1;
    f32 base;

    i = 0;
    count = arg0;
    if (arg0 != 0) {
        weights = D_80095C64;
        do {
            p2 = arg5[2];
            p1 = arg5[1];
            p0 = arg5[0];
            w0 = weights[0];
            w1 = weights[1];
            base = (f32)p0[0] + (f32)(p1[0] - p0[0]) * w0;
            ret = func_1500AF08(arg4, arg2, arg3,
                                 (s16)(s32)(((f32)p2[0] - base) * w1 + base),
                                 (s32)(((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0) + ((f32)p2[1] - ((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0)) * w1),
                                 (s32)(((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0) + ((f32)p2[2] - ((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0)) * w1),
                                 0x80, arg1, arg4);
            i++;
            weights += 2;
            arg4 = ret;
        } while (i != count);
    }
    return arg4;
}
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vtx1500B3B0;

typedef struct {
    Vtx1500B3B0 unk0[3];
} Tri1500B3B0;

extern void func_15049350(Tri1500B3B0);
extern f32 *D_80082D80[];
extern u16 D_800BE430;

s32 func_1500B3B0(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    u32 i;
    f32 sumX;
    f32 sumY;
    f32 sumZ;
    f32 x;
    f32 y;
    f32 z;
    Tri1500B3B0 tri;

    sumX = 0.0f;
    sumY = 0.0f;
    sumZ = 0.0f;

    for (i = 0; i < 3; i++) {
        tri.unk0[i].x = (f32)arg5[i][0];
        sumX += tri.unk0[i].x / 3.0f;
        tri.unk0[i].y = (f32)arg5[i][1];
        sumY += tri.unk0[i].y / 3.0f;
        tri.unk0[i].z = (f32)arg5[i][2];
        sumZ += tri.unk0[i].z / 3.0f;
    }

    func_15049350(tri);

    for (i = 1; (f32)i <= D_80082D80[arg0][0] * 2;) {
        x = D_80082D80[arg0][i] + sumX;
        i++;
        z = D_80082D80[arg0][i] + sumZ;
        i++;
        if (D_800CC214 != 0.0f) {
            y = ((D_800CC21C - D_800CC210 * x) - D_800CC218 * z) / D_800CC214;
        } else {
            y = sumY;
        }
        arg4 = func_1500AF08(arg4, arg2, arg3, (s16)(s32)x, (s32)y, (s32)z, 0x80, arg1, D_800BE430 >> arg2);
    }

    D_800BE430 ^= 1 << arg2;
    return arg4;
}

s32 func_1500B714(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 angle_f;

    angle_f = *(s16 *)&arg0;
    angle = angle_f;

    x = (arg5[0][0] + arg5[1][0] + arg5[2][0]) / 3;
    y = (arg5[0][1] + arg5[1][1] + arg5[2][1]) / 3;
    z = (arg5[0][2] + arg5[1][2] + arg5[2][2]) / 3;

    arg4 = func_1500AF08(arg4, arg2, arg3, (s16)x, (s16)y, (s16)z, 0x80, arg1, arg4);

    if ((f32)(u16)angle_f > 62.5f) {
        angle = 0x3E;
    }

    func_1000FA64(0x4D, (s16)x, (s16)y, (s16)z, 0x8000, angle, angle >> 3, 0, 0, 0, 0, 0);
    return arg4;
}
void func_1500B8F4(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    s32 i;
    s32 count;
    s32 ret;
    f32 *weights;
    s16 *p0;
    s16 *p1;
    s16 *p2;
    f32 w0;
    f32 w1;
    f32 base;

    i = 0;
    count = arg0;
    if (arg0 != 0) {
        weights = D_80095C64;
        do {
            p2 = arg5[2];
            p1 = arg5[1];
            p0 = arg5[0];
            w0 = weights[0];
            w1 = weights[1];
            base = (f32)p0[0] + (f32)(p1[0] - p0[0]) * w0;
            ret = func_1500AF08(arg4, arg2, arg3,
                                 (s16)(s32)(((f32)p2[0] - base) * w1 + base),
                                 (s32)(((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0) + ((f32)p2[1] - ((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0)) * w1),
                                 (s32)(((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0) + ((f32)p2[2] - ((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0)) * w1),
                                 0, arg1, arg4);
            i++;
            weights += 2;
            arg4 = ret;
        } while (i != count);
    }
    D_800DDBD0[arg2] = 0x57;
}
void func_1500BAB8(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    s32 i;
    s32 count;
    s32 ret;
    f32 *weights;
    s16 *p0;
    s16 *p1;
    s16 *p2;
    f32 w0;
    f32 w1;
    f32 base;

    i = 0;
    count = arg0;
    if (arg0 != 0) {
        weights = D_80095C64;
        do {
            p2 = arg5[2];
            p1 = arg5[1];
            p0 = arg5[0];
            w0 = weights[0];
            w1 = weights[1];
            base = (f32)p0[0] + (f32)(p1[0] - p0[0]) * w0;
            ret = func_1500AF08(arg4, arg2, arg3,
                                 (s16)(s32)(((f32)p2[0] - base) * w1 + base),
                                 (s32)(((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0) + ((f32)p2[1] - ((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0)) * w1),
                                 (s32)(((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0) + ((f32)p2[2] - ((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0)) * w1),
                                 0, arg1, arg4);
            i++;
            weights += 2;
            arg4 = ret;
        } while (i != count);
    }
    D_800DDBD0[arg2] = 0x8;
}
void func_1500BC7C(s16 arg0, u8 arg1, s32 arg2, s32 arg3, s32 arg4, s16 **arg5) {
    s32 i;
    s32 count;
    s32 ret;
    f32 *weights;
    s16 *p0;
    s16 *p1;
    s16 *p2;
    f32 w0;
    f32 w1;
    f32 base;

    i = 0;
    count = arg0;
    if (arg0 != 0) {
        weights = D_80095C64;
        do {
            p2 = arg5[2];
            p1 = arg5[1];
            p0 = arg5[0];
            w0 = weights[0];
            w1 = weights[1];
            base = (f32)p0[0] + (f32)(p1[0] - p0[0]) * w0;
            ret = func_1500AF08(arg4, arg2, arg3,
                                 (s16)(s32)(((f32)p2[0] - base) * w1 + base),
                                 (s32)(((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0) + ((f32)p2[1] - ((f32)p0[1] + (f32)(p1[1] - p0[1]) * w0)) * w1),
                                 (s32)(((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0) + ((f32)p2[2] - ((f32)p0[2] + (f32)(p1[2] - p0[2]) * w0)) * w1),
                                 0, arg1, arg4);
            i++;
            weights += 2;
            arg4 = ret;
        } while (i != count);
    }
    D_800DDBD0[arg2] = 0x21;
}

void func_1500BE40(s32 arg0) {
    D_800DDD10[0] = 0;
    D_800DDD10[1] = 0;
    D_800DDD0C = 110;
}

void func_1500BE68(void) {
    D_800DDD1C = 0;
    D_8008CEB0 = 0;
    D_800DDC04 = D_800DDC00 = D_800DDC08 = 0;
    D_800DDD20 = 0;
    func_1500BE40(0);
}


typedef struct {
    s16 unk0;
    s16 unk2;
} Struct8008CF00;

typedef s32 (*ParticleEmitFunc)(s16, u8, s32, s32, s32, s16 **);

typedef struct {
    ParticleEmitFunc unk0;
    s32 unk4;
    s32 unk8;
} Struct8008CEB8;

extern Struct8008CF00 D_8008CF00[][8];
extern Struct8008CEB8 D_8008CEB8[];
extern s32 D_8008CEB4;
extern u16 D_800BE478[];
extern u8 D_800DDC80[];
extern u8 D_800DDC10[];
extern s16 D_800DDC90[];
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1500AD84(s32, s32, s32);
extern void func_1510F800(s32);
extern void func_1517B89C(s32, s32);

void func_1500BEC0(void) {
    s32 i;
    s32 grp;
    u32 val;
    s32 sub;
    s32 kind;
    u16 *pair;

    func_1500BE68();
    if (*(u16 *)((u8 *)D_800B0DF0 + 0x18) == 0) {
        return;
    }
    if ((D_800BE9F0 == 6) && (D_80038080 == 0)) {
        return;
    }
    if (D_800BE9F0 == 0x1D) {
        return;
    }

    for (i = 0, pair = (u16 *)&D_800BE4A0; i < 15; i++, pair += 2) {
        func_1500AD84(i, pair[0], pair[1]);
    }

    D_800DDD18 = (struct Struct800DDD18 *)allocate_memory(*(u16 *)((u8 *)D_800B0DF0 + 0x18) * 14, 1, 2, 0);
    func_1510F800(0);

    if (*(u32 **)&D_800DBE5C != NULL) {
        for (i = 0; i < D_800DBE4C; i++) {
            val = (*(u32 **)&D_800DBE5C)[i];
            if (val & 0xF0000) {
                sub = ((val >> 16) & 0xF) - 1;
                kind = (val >> 20) & 7;
                D_800DDD20 = D_8008CEB8[D_8008CF00[D_800DDC80[sub]][kind].unk0].unk0(
                    D_8008CF00[D_800DDC80[sub]][kind].unk2, kind, sub,
                    ((val >> 5) & 3) ^ 3, D_800DDD20,
                    (s16 **)(D_800DBE3C + i * 12));
                D_800BE478[sub]++;
            }
        }
    }

    for (i = 0; i < D_800DDD20; i++) {
        D_800DDD18[i].unkB = 0;
    }

    D_8008CEB4 = 0;
    func_1517B89C(0, D_800DDD20 - 1);

    for (i = 0; i != 60; i++) {
        grp = i >> 2;
        if (D_800DDBD0[grp] != 0) {
            D_800DDC90[i] = (func_151EF610() % D_800DDBD0[grp]) << 8;
        } else {
            D_800DDC90[i] = 0;
        }
        D_800DDC10[grp] = func_151EF610();
    }
}
