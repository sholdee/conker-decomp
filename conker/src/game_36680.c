#include <ultra64.h>

#include "functions.h"
#define D_800DCD80 D_800DCD80_header
#define D_800DCD84 D_800DCD84_header
#define D_800BE4A0 D_800BE4A0_header
#define D_800DDA90 D_800DDA90_header
#define D_800B0DF0 D_800B0DF0_header
#include "variables.h"
#undef D_800DCD80
#undef D_800DCD84
#undef D_800BE4A0
#undef D_800DDA90
#undef D_800B0DF0


typedef struct Game36680PointLink {
    struct Game36680PointLink *next;
    TriggerZone *point;
} Game36680PointLink;

extern Game36680PointLink *D_800DCD80;
extern Game36680PointLink *D_800DCD84;

extern u8 D_800DDBD0[15];

void func_15009334(s32 index);

/** Resets the light state: the global light colour, the colour tables and the point lists. */
void func_150091D0(void) {
    s32 i;

    D_800DCD78 = 0;
    D_800DCD7C = 1;
    D_800DCD80 = NULL;
    D_800DCD84 = NULL;

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

/** Applies every level point (see func_15009334). */
void func_150092DC(void) {
    u32 i;

    for (i = 0; i < D_800D3094; i++) {
        func_15009334(i);
    }
}

extern void *allocate_memory(s32, s32, s32, s32);
s32 func_1515D5AC(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/**
 * Applies level point `index` by type: 10 sets the global light colour, 9 spawns a light (and runs its
 * callback), 16 sets the colour tables, 15 and 24 link the point into the D_800DCD80 / D_800DCD84 lists.
 */
void func_15009334(s32 index) {
    s32 pad; // matching: unused stack slot keeps the original frame layout
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

    type = D_800D3098[index].kind >> 2;
    switch (type) {
        case 9:
        case 10:
        case 16:
            red = D_800D3098[index].unk18 & 0xFF;
            green = (D_800D3098[index].unk18 >> 8) & 0xFF;
            blue = (D_800D3098[index].unk18 >> 16) & 0xFF;
            D_800D3098[index].flags |= 4;
            switch (type) {
                case 10:
                    func_1515D4D4(red, green, blue, 0);
                    if (D_800D3098[index].unk20 != 0) {
                        func_1500AB5C(D_800D3098[index].unk20);
                    }
                    break;
                case 9:
                    x = D_800D3098[index].x;
                    y = D_800D3098[index].y;
                    z = D_800D3098[index].z;
                    settings = (D_800D3098[index].unk1C >> 8) & 0xFF;
                    strength = (D_800D3098[index].unk18 >> 24) & 0x7F;
                    enabled = (D_800D3098[index].unk18 >> 31) & 1;
                    callback = D_800D3098[index].unk20 & 0xFFFF;
                    duration = (D_800D3098[index].unk20 >> 16) & 0xFFFF;
                    light = func_1515D5AC(x, y, z, settings, red, green, blue, strength, enabled, duration);
                    if (light != 0 && callback != 0) {
                        func_1500A94C(light, callback);
                    }
                    break;
                case 16:
                    D_800DCD24[0] = red;
                    D_800DCD24[1] = green;
                    D_800DCD24[2] = blue;
                    D_800DCD28[0] = D_800D3098[index].unk1C & 0xFF;
                    D_800DCD28[1] = (D_800D3098[index].unk1C >> 8) & 0xFF;
                    D_800DCD28[2] = (D_800D3098[index].unk1C >> 16) & 0xFF;
                    D_800DCD3C = (D_800D3098[index].unk1C >> 24) & 0xFF;
                    D_800DCD3D = (D_800D3098[index].unk18 >> 24) & 0xFF;
                    break;
            }
            break;
        case 15:
            link = allocate_memory(sizeof(Game36680PointLink), 1, 0, 0);
            link->point = &D_800D3098[index];
            link->next = D_800DCD80;
            D_800DCD80 = link;
        unused_label: // matching: an unused label keeps IDO from threading this break to the epilogue
            break;
        case 24:
            link = allocate_memory(sizeof(Game36680PointLink), 1, 0, 0);
            link->point = &D_800D3098[index];
            link->next = D_800DCD84;
            D_800DCD84 = link;
            break;
    }
}

struct225 *func_1516037C();
struct225 *func_1516387C();

/*
 * Light callbacks, the D_80082BFC table run by func_1500A94C: each attaches an effect to `light`.
 * `id` is the callback's own index in the table; some use it to pick an entry of a small local table.
 */

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
    func_15162FAC(light, 1, 0, 2, 300, 13, 255, 1);
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

/** Payload of an effect that attaches a light to a level object. */
typedef struct Game36680LightAttachment {
    f32 offset;
    f32 scale;
    WorldObject *object;
    f32 x;
    f32 y;
    f32 z;
} Game36680LightAttachment;

/** Attaches the light to the level object picked by `id` (and by whether D_800BE9F0 is 6). */
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

extern WorldObject *func_151149AC(u8);

/** Attaches the light to the object func_151149AC returns for `id`, starting at its position. */
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
void func_1500A990(s32 unused) {
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

typedef struct Game36680ResourceInfo {
    u8 flags;
    u8 frames;
    u16 length;
    s16 x;
    s16 y;
    u8 width;
    u8 pad9;
} Game36680ResourceInfo; // size 0xA

/** Resource picked for one of the 15 particle slots. */
typedef struct Game36680ResourceChoice {
    u16 id; // 0xFF: none
    u16 flags;
} Game36680ResourceChoice;

/** Texture file of one particle slot (loaded later by game_1A89B0). */
typedef struct Game36680ResourceFile {
    s32 devAddr;
    s32 size;
    s32 buf;
    u8 loaded;
    u8 padD[3];
} Game36680ResourceFile; // size 0x10

extern Game36680ResourceChoice D_800BE4A0[15];
extern Game36680ResourceFile D_800DDA90[15];
extern Game36680ResourceInfo D_80095CE0[];
extern u16 D_800BE478[15];
extern s16 D_800DDB80[15][2];
extern s32 func_1502B020(s32 *, s32, s32, s32);

/** Clears the particle slot tables and picks this level's resources. */
void func_1500ABA0(s32 arg0) {
    bzero(D_800BE4A0, sizeof(D_800BE4A0));
    func_1502B8E0(D_800BE4A0, sizeof(D_800BE4A0), 3, 12, arg0, 10);
    bzero(D_800DDA90, sizeof(D_800DDA90));
    bzero(&D_800DD478, 1560);
}

/** Looks up the texture file of every used particle slot and computes its texture extents. */
void func_1500AC14(void) {
    s32 i;
    s32 id;
    s32 size;
    u32 length;

    for (i = 0; i < 15; i++) {
        D_800DDA90[i].devAddr = 0;
        D_800DDA90[i].buf = 0;
        D_800DDA90[i].size = 0;
        D_800DDA90[i].loaded = 0;
        id = D_800BE4A0[i].id;
        if (id != 0xFF && (D_800BE478[i] != 0 || D_800BE4A0[i].id == 0x3B)) {
            D_800DDA90[i].devAddr = func_1502B020(&size, 2, 0, id);
            D_800DDA90[i].size = (size + 15) & ~15;
            length = D_80095CE0[id].length;
            if (size == 0 || length == 0) {
                length = 0;
                D_800DDA90[i].devAddr = 0;
            }
            D_800DDB80[i][0] = (D_80095CE0[id].width << 5) - 1;
            D_800DDB80[i][1] = (((length / D_80095CE0[id].width) << 6) / 5) - 1;
        }
    }
}

extern u8 D_80095CC4[];
extern s16 D_800BE438[15][2];
extern u8 D_800DDBC0[];
extern u8 D_800DDBE0[];
extern u8 D_800DDBF0[];
extern u8 D_800DDC20[][2];
extern u8 D_800DDC80[];

/** Sets up particle slot `slot` for resource `id` (0xFF: unused). */
void func_1500AD84(s32 slot, s32 id, s32 flags) {
    Game36680ResourceInfo *info;
    u8 *frames;
    s32 infoFlags;

    if (id != 255) {
        frames = &D_800DDBD0[slot];
        info = &D_80095CE0[id];
        *frames = info->frames;
        infoFlags = info->flags;
        if (infoFlags & 0x10) {
            *frames -= 1;
            D_800DDBE0[slot] = *frames;
        } else {
            D_800DDBE0[slot] = 0;
        }
        if (infoFlags & 0x80) {
            D_800DDBF0[slot] = 2;
        } else {
            D_800DDBF0[slot] = 1;
        }
        D_800BE438[slot][0] = info->x;
        D_800BE438[slot][1] = info->y;
        D_800DDBC0[slot] = id;
        D_800DDC80[slot] = flags & 0xFF7F;
        if (flags & 0x80) {
            D_800DDC00 |= 1 << slot;
        }
        if (infoFlags & 1) {
            D_800DDC04 |= 1 << slot;
        }
        if (infoFlags & 8) {
            D_800DDC08 |= 1 << slot;
        }
    } else {
        D_800DDBD0[slot] = 0;
    }
    D_800BE478[slot] = 0;
    D_800DDC20[slot][0] = D_80095CC4[slot];
    D_800DDC20[slot][1] = D_80095CC4[slot];
}

/** One particle placed on a level triangle; D_800DDD18 holds D_800B0DF0->maxParticles of them. */
typedef struct Game36680Particle {
    s16 x;
    s16 y;
    s16 z;
    u8 unk6; // (subtype << 5) | slot
    u8 animIndex; // slot * 4 + phase into D_800DDC90, 60: not animated
    u8 resource;
    u8 unk9;
    u8 kind;
    u8 unkB;
    u8 scaleX;
    u8 scaleY;
} Game36680Particle; // size 0xE

typedef struct Game36680LevelHeader {
    u8 pad0[0x18];
    u16 maxParticles;
} Game36680LevelHeader;

extern Game36680LevelHeader *D_800B0DF0;
extern Game36680Particle *D_800DDD18;
extern s32 func_151EF610(void);

/** Fills particle `index` for particle slot `slot`; returns the next free index. */
s32 func_1500AF08(s32 index, s32 slot, s32 subtype, s16 x, s16 y, s16 z, s32 jitter, u8 kind, s32 parity) {
    s16 scale;

    if (index >= (u32)(D_800B0DF0->maxParticles - 1)) {
        return D_800B0DF0->maxParticles - 1;
    }

    D_800DDD18[index].x = x;
    D_800DDD18[index].y = y;
    D_800DDD18[index].z = z;

    if (D_800DDBD0[slot] >= 2) {
        if ((1 << slot) & D_800DDC00) {
            D_800DDD18[index].animIndex = (slot << 2) + (parity & 1);
        } else {
            D_800DDD18[index].animIndex = (index & 3) + (slot << 2);
        }
    } else {
        D_800DDD18[index].animIndex = 60;
    }

    D_800DDD18[index].unk6 = (subtype << 5) | slot;
    D_800DDD18[index].kind = kind;
    D_800DDD18[index].resource = D_800DDBC0[slot];

    if (jitter != 0) {
        // matching: the "+ jitter - jitter" pair is needed for the original instruction sequence
        scale = (func_151EF610() % jitter + jitter) - jitter + 0x100;
        D_800DDD18[index].scaleX = (D_800BE438[slot][0] * scale) >> 10;
        if (D_80095CE0[D_800DDBC0[slot]].flags & 4) {
            scale = func_151EF610() % jitter * 4 + scale;
        }
        D_800DDD18[index].scaleY = (D_800BE438[slot][1] * scale) >> 10;
    } else {
        D_800DDD18[index].scaleX = D_800BE438[slot][0] >> 2;
        D_800DDD18[index].scaleY = D_800BE438[slot][1] >> 2;
    }
    return index + 1;
}

extern f32 D_80095C64[]; // pairs of interpolation weights

/** Linear interpolation from a to b by t, with the difference computed in the integer type of a and b. */
#define LERP_INT(a, b, t) ((f32)(a) + (f32)((b) - (a)) * (t))
/** Linear interpolation from a to b by t. */
#define LERP(a, b, t) ((a) + ((b) - (a)) * (t))

/** Scatters `count` particles over the triangle `tri` at the barycentric weights of D_80095C64. */
s32 func_1500B1F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0x80, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    return index;
}

typedef struct {
    Vec3f points[3];
} Game36680Triangle;

extern void func_15049350(Game36680Triangle);
extern f32 *D_80082D80[];
extern u16 D_800BE430;

/**
 * Places the particles of pattern `pattern` (D_80082D80: a pair count, then x/z offsets) around the
 * centre of triangle `tri`, projecting each onto the triangle's plane (computed by func_15049350).
 */
s32 func_1500B3B0(s16 pattern, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    u32 i;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    f32 x;
    f32 y;
    f32 z;
    Game36680Triangle triangle;

    centerX = 0.0f;
    centerY = 0.0f;
    centerZ = 0.0f;

    for (i = 0; i < 3; i++) {
        triangle.points[i].x = tri[i][0];
        centerX += triangle.points[i].x / 3.0f;
        triangle.points[i].y = tri[i][1];
        centerY += triangle.points[i].y / 3.0f;
        triangle.points[i].z = tri[i][2];
        centerZ += triangle.points[i].z / 3.0f;
    }

    func_15049350(triangle);

    for (i = 1; i <= D_80082D80[pattern][0] * 2;) {
        x = D_80082D80[pattern][i++] + centerX;
        z = D_80082D80[pattern][i++] + centerZ;
        if (D_800CC214 != 0.0f) {
            y = ((D_800CC21C - D_800CC210 * x) - D_800CC218 * z) / D_800CC214;
        } else {
            y = centerY;
        }
        index = func_1500AF08(index, slot, subtype, (s16)(s32)x, (s32)y, (s32)z, 0x80, kind, D_800BE430 >> slot);
    }

    D_800BE430 ^= 1 << slot;
    return index;
}

/** Places one particle at the centre of triangle `tri` and starts positional sound 0x4D there. */
s32 func_1500B714(s16 amount, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    u16 clamped;
    s32 x;
    s32 y;
    s32 z;

    clamped = amount;

    x = (tri[0][0] + tri[1][0] + tri[2][0]) / 3;
    y = (tri[0][1] + tri[1][1] + tri[2][1]) / 3;
    z = (tri[0][2] + tri[1][2] + tri[2][2]) / 3;

    index = func_1500AF08(index, slot, subtype, (s16)x, (s16)y, (s16)z, 0x80, kind, index);

    if ((f32)(u16)amount > 62.5f) {
        clamped = 62;
    }

    func_1000FA64(0x4D, (s16)x, (s16)y, (s16)z, 0x8000, clamped, clamped >> 3, 0, 0, 0, 0, 0);
    return index;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 87. */
void func_1500B8F4(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 87;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 8. */
void func_1500BAB8(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 8;
}
/** Like func_1500B1F4 without scale jitter, then sets the slot's frame count to 33. */
void func_1500BC7C(s16 count, u8 kind, s32 slot, s32 subtype, s32 index, s16 **tri) {
    s32 i;
    s32 n;
    s32 next;
    f32 *weights;
    f32 w0;
    f32 w1;

    // matching: a guarded do-while; the equivalent for loop allocates registers differently
    i = 0;
    n = count;
    if (count != 0) {
        weights = D_80095C64;
        do {
            w0 = weights[0];
            w1 = weights[1];
            next = func_1500AF08(index, slot, subtype,
                                 (s16)(s32)LERP(LERP_INT(tri[0][0], tri[1][0], w0), (f32)tri[2][0], w1),
                                 (s32)LERP(LERP_INT(tri[0][1], tri[1][1], w0), (f32)tri[2][1], w1),
                                 (s32)LERP(LERP_INT(tri[0][2], tri[1][2], w0), (f32)tri[2][2], w1),
                                 0, kind, index);
            i++;
            weights += 2;
            index = next;
        } while (i != n);
    }
    D_800DDBD0[slot] = 33;
}

void func_1500BE40(s32 unused) {
    D_800DDD10[0] = 0;
    D_800DDD10[1] = 0;
    D_800DDD0C = 110;
}

/** Resets the particle counters and slot bitmasks. */
void func_1500BE68(void) {
    D_800DDD1C = 0;
    D_8008CEB0 = 0;
    D_800DDC04 = D_800DDC00 = D_800DDC08 = 0;
    D_800DDD20 = 0;
    func_1500BE40(0);
}

typedef s32 (*ParticleEmitFunc)(s16, u8, s32, s32, s32, s16 **);

typedef struct Game36680Emitter {
    ParticleEmitFunc emit;
    s32 unk4;
    s32 unk8;
} Game36680Emitter;

typedef struct Game36680EmitterChoice {
    s16 emitter; // index into D_8008CEB8
    s16 count;   // first argument of the emit function
} Game36680EmitterChoice;

extern Game36680EmitterChoice D_8008CF00[][8];
extern Game36680Emitter D_8008CEB8[];
extern s32 D_8008CEB4;
extern u8 D_800DDC10[];
extern s16 D_800DDC90[60];
extern void func_1510F800(s32);
extern void func_1517B89C(s32, s32);

/**
 * Builds the level's particles: sets up the 15 particle slots, then for every level triangle whose
 * packed word names a slot, runs that slot's emitter for the triangle's kind.
 */
void func_1500BEC0(void) {
    s32 i;
    s32 slot;
    u32 packed;
    s32 kind;

    func_1500BE68();
    if (D_800B0DF0->maxParticles == 0) {
        return;
    }
    if ((D_800BE9F0 == 6) && (D_80038080 == 0)) {
        return;
    }
    if (D_800BE9F0 == 0x1D) {
        return;
    }

    for (i = 0; i < 15; i++) {
        func_1500AD84(i, D_800BE4A0[i].id, D_800BE4A0[i].flags);
    }

    D_800DDD18 = allocate_memory(D_800B0DF0->maxParticles * sizeof(Game36680Particle), 1, 2, 0);
    func_1510F800(0);

    if (D_800DBE5C != NULL) {
        for (i = 0; i < D_800DBE4C; i++) {
            packed = D_800DBE5C[i];
            if (packed & 0xF0000) {
                slot = ((packed >> 16) & 0xF) - 1;
                kind = (packed >> 20) & 7;
                // matching: D_800DBE3C (s32) holds the address of the 12-byte triangles (3 vertex
                // pointers each); indexing it as a typed array swaps the operands of an addu
                D_800DDD20 = D_8008CEB8[D_8008CF00[D_800DDC80[slot]][kind].emitter].emit(
                    D_8008CF00[D_800DDC80[slot]][kind].count, kind, slot,
                    ((packed >> 5) & 3) ^ 3, D_800DDD20, (s16 **)(D_800DBE3C + i * 12));
                D_800BE478[slot]++;
            }
        }
    }

    for (i = 0; i < D_800DDD20; i++) {
        D_800DDD18[i].unkB = 0;
    }

    D_8008CEB4 = 0;
    func_1517B89C(0, D_800DDD20 - 1);

    for (i = 0; i < 60; i++) {
        slot = i >> 2;
        if (D_800DDBD0[slot] != 0) {
            D_800DDC90[i] = (func_151EF610() % D_800DDBD0[slot]) << 8;
        } else {
            D_800DDC90[i] = 0;
        }
        D_800DDC10[slot] = func_151EF610();
    }
}
