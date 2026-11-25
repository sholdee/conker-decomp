#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#define D_800CBE00 D_800CBE00_s32
#define D_800D3300 D_800D3300_hdr
#define D_800DBE3C D_800DBE3C_hdr
#define D_800DBEF4 D_800DBEF4_hdr
#include "variables.h"
#undef D_800CBE00
#undef D_800D3300
#undef D_800DBE3C
#undef D_800DBEF4


/* A timed node in the D_800CBE00 list, allocated by func_15044964. */
typedef struct TriggerNode {
    /* 0x00 */ struct TriggerNode *next;
    /* 0x04 */ s16 lifetime; // frames left; -1 = never expires
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8 check;     // index into D_80085E80
    /* 0x0D */ u8 action;    // index into D_80085E8C, run when check passes
    /* 0x0E */ u8 delay;     // frames before check starts running
    /* 0x0F */ u8 padF;
} TriggerNode;

/* A TriggerNode with a box volume (func_150448D0). */
typedef struct TriggerVolume {
    /* 0x00 */ TriggerNode node;
    /* 0x10 */ s16 depth;
    /* 0x12 */ s16 width;
    /* 0x14 */ s16 height;
    /* 0x16 */ u8 angle;
    /* 0x17 */ u8 pad17;
    /* 0x18 */ s16 *position;
    /* 0x1C */ s16 *size;
} TriggerVolume;

extern TriggerNode *D_800CBE00;
extern s32 (*D_80085E80[])(TriggerNode *);
extern void (*D_80085E8C[])(void);
extern f32 D_80098D60;
extern f32 D_80098D64;
extern f32 D_80098D68;
extern f32 sqrtf(f32);
extern s32 D_800CBD9C;
extern s32 func_15145C90(s32);
extern void *allocate_memory(s32, s32, s32, s32);
TriggerNode *func_15044964(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z);

#pragma function sqrtf

void func_15044370(void) {
    D_800CBD9C = 0;
}

extern u8 D_80089120[4];
extern u8 D_800CBDD3;
extern f32 D_800CBDF4;
extern f32 D_800CBDF8;
void func_1510F800(s32);
void func_15044660(struct127 *, f32, f32, f32);
s32 func_150AB1F0(f32, f32, f32, struct127 *, s32);
void func_150AC3E4(f32, f32, f32, struct127 *, s32);

s32 func_15044380(f32 x, f32 y, f32 z, struct127 *obj, s32 arg4, s32 arg5) {
    s32 i;
    s32 total;
    s32 saved_mode;

    D_800CBDF4 = -32768.0f;
    D_800CBDF8 = -32768.0f;
    total = 0;
    obj->pad275 = 0;
    func_15044660(obj, x, y, z);
    saved_mode = D_800CBDD3;
    for (i = 3; i >= 0; i--) {
        if (D_80089120[i] == 1 && (i != 3 || !(obj->unkF8 & 0x200))) {
            func_1510F800(i);
            if (D_800DBE62 == 0) continue;
            total += func_150AB1F0(x, y, z, obj, arg4);
        }
    }
    if (arg5 != 0) {
        for (i = 0; i < 3; i++) {
            if (D_80089120[i] == 1) {
                func_1510F800(i);
                if (D_800DBE62 != 0) {
                    func_150AC3E4(x, y, z, obj, 0);
                }
            }
        }
    }
    func_1510F800(0);
    D_800CBDD3 = saved_mode;
    return total;
}

/* Copies a triangle's three s16 vertices into out, relative to (ox, oy, oz) and rotated about Y. */
void func_1504452C(s16 **vertices, struct17 *out, f32 sinY, f32 cosY, f32 ox, f32 oy, f32 oz, s32 offset) {
    s16 *vtx;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    for (i = 0; i < 3; i++) {
        vtx = (s16 *)((u8 *)vertices[i] + offset);
        x = vtx[0] - ox;
        y = vtx[1] - oy;
        z = vtx[2] - oz;
        out[i].unk0 = (x * cosY) + (z * sinY);
        out[i].unk4 = y;
        out[i].unk8 = (z * cosY) - (x * sinY);
    }
}

void func_15044658(void) {
}

extern f32 D_800CBDD8;
extern f32 D_800CBDDC;
void func_1507C3E0(struct127 *obj, s16 *height, s16 *arg2, s16 *arg3);

/* Sets D_800CBDD8/D_800CBDDC (a height and a mid-height) for obj's collision probe. */
void func_15044660(struct127 *obj, f32 x, f32 y, f32 z) {
    s16 height;
    s16 unk2C;
    s16 unk2A;
    s32 half;
    s32 index;

    func_1507C3E0(obj, &height, &unk2C, &unk2A);
    if (obj->interaction_state == 0x2D || obj->interaction_state == 0x2E || obj->interaction_state == 0x2C) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
    } else {
        height += fabsf(y - obj->y_position);
        index = obj - D_800CC2D0;
    }
    if (obj->unk5 == 5) {
        D_800CBDDC = 0.0f;
        D_800CBDD8 = height;
        return;
    }
    if (obj->in_water != 0) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x53) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x28) {
        half = 20;
        D_800CBDD8 = height - half;
        D_800CBDDC = half + D_800CBDD8 * 0.5f;
        return;
    }
    if (index >= 0 && index < D_8008FD8C && obj->unk28 != 0.0f) {
        D_800CBDD8 = height;
        D_800CBDDC = height >> 1;
        return;
    }
    if (obj->id == 0x25) {
        half = height / 2;
        D_800CBDD8 = height - half;
        D_800CBDDC = half + D_800CBDD8 * 0.5f;
        return;
    }
    D_800CBDD8 = height;
    D_800CBDDC = height >> 1;
}

/* Adds a box trigger volume whose position and size are read from the given s16 pointers. */
TriggerVolume *func_150448D0(s32 lifetime, s32 delay, s32 action, s32 depth, s32 width, s32 height, s32 angle,
                             s16 *position, s16 *size) {
    TriggerVolume *volume;

    volume = (TriggerVolume *)func_15044964(sizeof(TriggerVolume), 1, lifetime, delay, action, 0, 0, 0);
    if (volume == NULL) {
        return NULL;
    }
    volume->depth = depth;
    volume->width = width;
    volume->height = height;
    volume->angle = angle;
    volume->position = position;
    volume->size = size;
    return volume;
}

/* Allocates a trigger node and appends it to the D_800CBE00 list. */
TriggerNode *func_15044964(s32 size, s32 check, s32 lifetime, s32 delay, s32 action, s32 x, s32 y, s32 z) {
    TriggerNode *node;
    TriggerNode *tail;

    node = allocate_memory(size, 1, 0, 0);
    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->lifetime = lifetime;
    node->check = check;
    node->action = action;
    node->delay = delay;
    node->x = x;
    node->y = y;
    node->z = z;

    if (D_800CBE00 == NULL) {
        D_800CBE00 = node;
    } else {
        for (tail = D_800CBE00; tail->next != NULL; tail = tail->next) {
        }
        tail->next = node;
    }

    return node;
}

/* Per frame: runs each node's check (and its action when the check passes), then ages and frees expired nodes. */
void func_15044A28(void) {
    TriggerNode *node;
    TriggerNode *prev;
    TriggerNode *next;
    s32 check;
    s32 countdown;
    s32 lifetime;
    s32 delay;

    prev = NULL;
    for (node = D_800CBE00; node != NULL; node = next) {
        countdown = node->delay;
        check = node->check;
        next = node->next;
        if (countdown == 0) {
            if (D_80085E80[check](node) != 0) {
                D_80085E8C[node->action]();
            }
        } else {
            delay = countdown - D_800BE9E4;
            if (delay < 0) {
                delay = 0;
            }
            node->delay = delay;
        }
        lifetime = node->lifetime;
        if (lifetime != -1) {
            lifetime -= D_800BE9E4;
            if (lifetime <= 0) {
                if (prev == NULL) {
                    D_800CBE00 = node->next;
                } else {
                    prev->next = node->next;
                }
                func_100043B4((s32 *)node, 2);
            } else {
                node->lifetime = lifetime;
                prev = node;
            }
        } else {
            prev = node;
        }
    }
}

struct conker15044CE4 {
    char pad0[6];
    s16 unk6;
    s16 unk8;
    s16 unkA;
    char padC[4];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8 unk16;
    char pad17;
    s16 *unk18;
    s16 *unk1C;
};


extern f32 func_15048A40(u8);
extern f32 func_150489B0(u8);

s32 func_15044B78(struct conker15044CE4 *arg0) {
    f32 s;
    f32 c;
    f32 rx;
    f32 x;
    f32 y;
    f32 z;
    f32 pad2c;
    f32 pad28;
    f32 pad24;
    s32 half;
    s32 h116;
    s32 h118;

    half = (s16)D_800CC2D0[0].unk31C->unk114 >> 1;
    h116 = (s16)D_800CC2D0[0].unk31C->unk116;
    h118 = (s16)D_800CC2D0[0].unk31C->unk118;
    x = D_800CC2D0[0].x_position - arg0->unk6;
    y = (D_800CC2D0[0].y_position - arg0->unk8) + half;
    z = D_800CC2D0[0].z_position - arg0->unkA;
    c = func_15048A40(arg0->unk16);
    s = func_150489B0(arg0->unk16);
    rx = z * s + x * c;
    x = x * s - z * c;
    if (fabsf(y) < arg0->unk14 + half && fabsf(x) < arg0->unk12 + h116 && fabsf(rx) < arg0->unk10 + h118) {
        return 1;
    }
    return 0;
}


void func_15044CE4(struct conker15044CE4 *arg0) {
    s16 *sp18 = arg0->unk18;
    s32 temp;

    arg0->unk6 = sp18[0];
    arg0->unk8 = sp18[1];
    arg0->unkA = sp18[2];
    temp = arg0->unk1C[0] / 32;
    arg0->unk10 = temp;
    arg0->unk12 = temp;
    arg0->unk14 = temp;
    func_15044B78(arg0);
}

extern s32 func_1505D1C4(f32, f32, f32, s32, s32, u16, s32, s32);

s32 func_15044D40(struct conker15044CE4 *arg0) {
    func_1505D1C4(arg0->unk6, arg0->unk8, arg0->unkA, arg0->unk10, 0xFF, 0, 0, 0);
    return 0;
}

s32 func_1505D024(struct127 *arg0, s32 arg1, u16 arg2, s32 arg3);

void func_15044DA0(void) {
    if (D_800CC2D0[0].stunned != 0) {
        return;
    }
    if (D_800CC2D0[0].immune != 0) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 5, D_800CC2D0[0].unk7A, -1);
}

void func_15044DE8(void) {
    if (D_800CC2D0[0].stunned != 0) {
        return;
    }
    if (D_800CC2D0[0].immune != 0) {
        return;
    }
    if (D_800C35EA != 1) {
        func_1505D024(&D_800CC2D0[0], 4, D_800CC2D0[0].unk7A, -1);
    }
}

void func_15044E40(void) {
    if (D_800CC2D0[0].stunned != 0) {
        return;
    }
    if (D_800CC2D0[0].immune != 0) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 0x40, D_800CC2D0[0].unk7A, -1);
}

void func_15044E88(void) {
    if (D_800CC2D0[0].stunned != 0) {
        return;
    }
    if (D_800CC2D0[0].immune != 0) {
        return;
    }
    func_1505D024(&D_800CC2D0[0], 1, D_800CC2D0[0].unk7A, -1);
}

struct conker15045714 {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
};

struct conker150472C0_sub
{
  u8 field_0x00[0x10];
  u16 field_0x10;
};
struct conker150472C0_dst
{
  f32 field_0x00;
  struct conker150472C0_sub field_0x04;
  u8 pad16[2];
  s32 field_0x18;
  u8 field_0x1C;
  u8 field_0x1D;
  u8 pad1E[2];
  s32 field_0x20;
};

typedef struct Game71820Candidate {
    s32 height;
    s32 *sources;
    s32 index;
    s32 padC;
} Game71820Candidate;

void func_1510F800(s32);
s32 func_150A3A70(s32, s32);
extern f32 D_80098D40;
extern u8 *D_800DBE3C;
extern Game71820Candidate D_800D3300[];

s32 func_15044ED0(struct conker15045714 *arg0, f32 arg1, struct conker150472C0_dst *arg2) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    if (arg1 < arg0->field_0x04) {
        arg2->field_0x1C &= ~2;
        return 0;
    }
    arg2->field_0x00 = D_80098D40;
    best = -1;
    func_1510F800(0);
    count = func_150A3A70((s32)arg0->field_0x00, (s32)arg0->field_0x08);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;

        do {
            height = (f32)scan->height * 0.00390625f;
            if ((arg0->field_0x04 <= height) && (height < arg2->field_0x00)) {
                best = scan_index;
                arg2->field_0x00 = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        u32 copy_index;
        u8 *output;
        s32 offset;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg2;
        for (; copy_index != 3; copy_index++, output += 6) {
            s16 *point = (s16 *)(source[copy_index] + offset * 16);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        { s32 idx = ((u8 *)candidate->sources - D_800DBE3C) / 12; arg2->field_0x18 = ((s32 *)D_800DBE5C)[idx]; }
        arg2->field_0x1C |= 5;
        arg2->field_0x1D = 1;
        arg2->field_0x20 = 0;
        if (arg2->field_0x00 <= arg1) {
            arg2->field_0x1C |= 2;
            return 1;
        }
        return 0;
    }
    arg2->field_0x1C &= ~2;
    return 0;
}

extern f32 D_80098D44;
extern f32 D_800DBE68;
extern f32 D_800DBE6C;
extern f32 D_800DBE70;
extern f32 D_800DBE74;

s32 func_150450CC(struct conker15045714 *arg0, f32 arg1, struct conker150472C0_dst *arg2) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    if (arg0->field_0x04 < arg1) {
        arg2->field_0x1C &= ~2;
        return 0;
    }
    arg2->field_0x00 = D_80098D44;
    best = -1;
    func_1510F800(0);
    D_800DBE68 = arg0->field_0x00;
    D_800DBE6C = arg0->field_0x04;
    D_800DBE70 = arg0->field_0x08;
    D_800DBE74 = arg1;
    count = func_150A3A70((s32)arg0->field_0x00, (s32)arg0->field_0x08);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;

        do {
            height = (f32)scan->height * 0.00390625f;
            if ((height <= arg0->field_0x04) && (arg2->field_0x00 < height)) {
                best = scan_index;
                arg2->field_0x00 = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        u32 copy_index;
        u8 *output;
        s32 offset;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg2;
        for (; copy_index != 3; copy_index++, output += 6) {
            s16 *point = (s16 *)(source[copy_index] + offset * 16);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        if (D_800DBE5C != 0) {
            s32 idx = ((u8 *)candidate->sources - D_800DBE3C) / 12;
            arg2->field_0x18 = ((s32 *)D_800DBE5C)[idx];
        } else {
            arg2->field_0x18 = 0;
        }
        arg2->field_0x1C |= 7;
        arg2->field_0x1D = 1;
        arg2->field_0x20 = 0;
        if (arg1 <= arg2->field_0x00) {
            arg2->field_0x1C |= 2;
            return 1;
        }
        return 0;
    }
    arg2->field_0x1C &= ~2;
    return 0;
}

struct conker15045714;
struct conker150472C0_dst;

s32 func_15044ED0(struct conker15045714 *, f32, struct conker150472C0_dst *);
s32 func_150470B0(struct conker15045714 *, s32, struct conker150472C0_dst *);

s32 func_1504530C(s32 arg0, s32 arg1, s32 arg2) {
    switch (func_150470B0((struct conker15045714 *)arg0, arg1, (struct conker150472C0_dst *)arg2)) {
    case 0:
        return func_15044ED0((struct conker15045714 *)arg0, *(f32 *)&arg1, (struct conker150472C0_dst *)arg2);
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        break;
    }
}

void func_1510F800(s32);
typedef struct Vec3f_15045D48 {
    f32 x;
    f32 y;
    f32 z;
} Vec3f_15045D48;

typedef struct Game71820Hit {
    f32 height;
    s16 points[9];
    s32 object;
    u8 flags;
    u8 active;
    u8 pad1E[2];
    s32 field20;
} Game71820Hit;

s32 func_150A4FA0(s32, s32);
extern f32 D_80098D48;

s32 func_15045384(Vec3f_15045D48 *arg0, f32 arg1, Game71820Hit *arg2) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    if (arg1 < arg0->y) {
        arg2->flags &= ~2;
        return 0;
    }
    arg2->height = D_80098D48;
    best = -1;
    func_1510F800(3);
    count = func_150A4FA0((s32)arg0->x, (s32)arg0->z);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;

        do {
            height = (f32)scan->height * 0.00390625f;
            if ((arg0->y <= height) && (height < arg2->height)) {
                best = scan_index;
                arg2->height = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        u32 copy_index;
        u8 *output;
        s32 offset;
        s32 flags;
        s32 updated_flags;
        s32 active;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg2;
        for (; copy_index != 3; copy_index++, output += 6) {
            s16 *point = (s16 *)(source[copy_index] + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        arg2->object = 0;
        arg2->flags |= 6;
        arg2->active = 4;
        arg2->field20 = 0;
        if (arg2->height <= arg1) {
            arg2->flags |= 2;
            return 1;
        }
        return 0;
    }
    arg2->flags &= ~2;
    return 0;
}


extern void func_1510F800(s32);
extern s32 func_150A6500(s16, s16, s32, u16);

struct conker150472C0_src
{
  u8 pad0[0xC];
  f32 unkC;
  u8 pad10[0x34];
  struct conker150472C0_sub unk44;
  u8 pad56[3];
  u8 unk59;
  u8 pad5A[2];
  s32 unk5C;
  s32 unk60;
};

extern f32 D_80098D4C;

s32 func_1504554C(struct conker15045714 *arg0, f32 arg1, struct conker150472C0_dst *arg2) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    if (arg0->field_0x04 < arg1) {
        arg2->field_0x1C &= ~2;
        return 0;
    }
    arg2->field_0x00 = D_80098D4C;
    best = -1;
    func_1510F800(3);
    count = func_150A4FA0((s32)arg0->field_0x00, (s32)arg0->field_0x08);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;

        do {
            height = (f32)scan->height * 0.00390625f;
            if ((height <= arg0->field_0x04) && (arg2->field_0x00 < height)) {
                best = scan_index;
                arg2->field_0x00 = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        u32 copy_index;
        u8 *output;
        s32 offset;
        s32 flags;
        s32 updated_flags;
        s32 active;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg2;
        for (; copy_index != 3; copy_index++, output += 6) {
            s16 *point = (s16 *)(source[copy_index] + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        arg2->field_0x18 = 0;
        arg2->field_0x1C |= 6;
        arg2->field_0x1D = 4;
        arg2->field_0x20 = 0;
        if (arg1 <= arg2->field_0x00) {
            arg2->field_0x1C |= 2;
            return 1;
        }
        return 0;
    }
    arg2->field_0x1C &= ~2;
    return 0;
}

void func_15045714(struct conker15045714 *arg0, u16 arg1, s32 *arg2, s32 arg3) {
    func_1510F800(2);
    *arg2 = func_150A6500((s16)arg0->field_0x00, (s16)arg0->field_0x08, arg3, arg1);
}

struct Game71820Hit;
s32 func_15045F8C(struct conker15045714 *arg0, f32 arg1, s32 *arg2, struct Game71820Hit *arg3);

s32 func_15045780(struct conker15045714 *arg0, u16 arg1, f32 arg2, s32 arg3) {
    s32 sp1C;
    s32 sp18;

    if (arg0->field_0x04 < arg2) {
        *((u8 *)arg3 + 0x1C) &= ~2;
        return 0;
    }
    func_15045714(arg0, arg1, &sp1C, (s32)&sp18);
    return func_15045F8C(arg0, arg2, &sp18, (struct Game71820Hit *)arg3);
}

extern s32 func_15047004(struct conker15045714 *, s32, struct conker150472C0_dst *);
extern s32 func_15045780(struct conker15045714 *, u16, f32, s32);

s32 func_15045800(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (func_15047004((struct conker15045714 *)arg0, arg2, (struct conker150472C0_dst *)arg3)) {
    case 0:
        return func_15045780((struct conker15045714 *)arg0, (u16)arg1, *(f32 *)&arg2, arg3);
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        break;
    }
}

typedef struct Game71820Surface {
    u8 pad0[0x40];
    s32 default_material;
    s32 *materials;
    u8 pad48[0x10];
    u16 first_triangle;
    u8 pad5A[0x15];
    u8 flags;
    u8 pad70[0x30];
} Game71820Surface;
extern u8 *D_800DBE3C;
s32 func_150A43E0(s32, s32, s32, void *);
void func_150A44F0(s32, void *, s32);
extern f32 D_80098D50;
extern u8 D_800D37E0[];
extern s32 D_800DBEF4;

s32 func_15045880(Vec3f_15045D48 *arg0, f32 arg1, s32 *arg2, Game71820Hit *arg3) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    arg3->height = D_80098D50;
    best = -1;
    func_150A44F0(*arg2, D_800D37E0, 0);
    count = func_150A43E0((s32)arg0->x, (s32)arg0->z, *arg2, D_800D37E0);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;
        do {
            height = (f32)scan->height * 0.00390625f;
            if ((arg0->y <= height) && (height < arg3->height)) {
                best = scan_index;
                arg3->height = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        s32 copy_index;
        s32 *copy_source;
        u32 surface_index;
        u8 *output;
        s32 offset;
        s32 *materials;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg3;
        copy_source = source;
        for (; copy_index != 3; copy_index++, copy_source++, output += 6) {
            s16 *point = (s16 *)(*copy_source + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        surface_index = candidate->padC;
        arg3->field20 = surface_index * 0xA0 + D_800DBEF4;
        materials = ((Game71820Surface *)arg3->field20)->materials;
        if (materials != 0) {
            s32 triangle_index = ((u8 *)source - D_800DBE3C) / 12 - ((Game71820Surface *)arg3->field20)->first_triangle;
            *(s32 *)((u8 *)arg3 + 0x18) = materials[triangle_index];
        } else {
            *(s32 *)((u8 *)arg3 + 0x18) = ((Game71820Surface *)arg3->field20)->default_material;
        }
        arg3->flags |= 6;
        { s32 unused;
        if ((((Game71820Surface *)D_800DBEF4)[surface_index = candidate->padC].flags & 0x80) == 0x80) {
            Game71820Hit *h = arg3;
            h->flags |= 1;
        }
        }
        arg3->active = 2;
        if (arg3->height <= arg1) {
            arg3->flags |= 2;
            return 1;
        }
        return 0;
    }
    arg3->flags &= ~2;
    return 0;
}


extern f32 D_80098D54;

s32 func_15045AE4(Vec3f_15045D48 *arg0, f32 arg1, s32 *arg2, Game71820Hit *arg3) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    arg3->height = D_80098D54;
    best = -1;
    func_150A44F0(*arg2, D_800D37E0, 0);
    count = func_150A43E0((s32)arg0->x, (s32)arg0->z, *arg2, D_800D37E0);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;
        do {
            height = (f32)scan->height * 0.00390625f;
            if ((height <= arg0->y) && (arg3->height < height)) {
                best = scan_index;
                arg3->height = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        s32 copy_index;
        s32 *copy_source;
        u32 surface_index;
        u8 *output;
        s32 offset;
        s32 *materials;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg3;
        copy_source = source;
        for (; copy_index != 3; copy_index++, copy_source++, output += 6) {
            s16 *point = (s16 *)(*copy_source + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        surface_index = candidate->padC;
        arg3->field20 = surface_index * 0xA0 + D_800DBEF4;
        materials = ((Game71820Surface *)arg3->field20)->materials;
        if (materials != 0) {
            s32 triangle_index = ((u8 *)source - D_800DBE3C) / 12 - ((Game71820Surface *)arg3->field20)->first_triangle;
            *(s32 *)((u8 *)arg3 + 0x18) = materials[triangle_index];
        } else {
            *(s32 *)((u8 *)arg3 + 0x18) = ((Game71820Surface *)arg3->field20)->default_material;
        }
        arg3->flags |= 6;
        { s32 unused;
        if ((((Game71820Surface *)D_800DBEF4)[surface_index = candidate->padC].flags & 0x80) == 0x80) {
            Game71820Hit *h = arg3;
            h->flags |= 1;
        }
        }
        arg3->active = 2;
        if (arg1 <= arg3->height) {
            arg3->flags |= 2;
            return 1;
        }
        return 0;
    }
    arg3->flags &= ~2;
    return 0;
}


extern Game71820Candidate D_800D3300[];
extern u8 *D_800DBE3C;
s32 func_150A43E0(s32, s32, s32, void *);
void func_150A44F0(s32, void *, s32);
extern f32 D_80098D58;
extern u8 D_800D3830[];
extern s32 D_800DBEF4;

s32 func_15045D48(Vec3f_15045D48 *arg0, f32 arg1, s32 *arg2, Game71820Hit *arg3) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    arg3->height = D_80098D58;
    best = -1;
    func_150A44F0(*arg2, D_800D3830, 0);
    count = func_150A43E0((s32)arg0->x, (s32)arg0->z, *arg2, D_800D3830);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;
        do {
            height = (f32)scan->height * 0.00390625f;
            if ((arg0->y <= height) && (height < arg3->height)) {
                best = scan_index;
                arg3->height = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        s32 copy_index;
        s32 *copy_source;
        s32 surface_index;
        u8 *output;
        s32 offset;
        s32 *materials;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg3;
        copy_source = source;
        for (; copy_index != 3; copy_index++, copy_source++, output += 6) {
            s16 *point = (s16 *)(*copy_source + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        surface_index = candidate->padC;
        arg3->field20 = surface_index * 0xA0 + D_800DBEF4;
        materials = ((Game71820Surface *)arg3->field20)->materials;
        if (materials != 0) {
            s32 triangle_index = ((u8 *)source - D_800DBE3C) / 12 - ((Game71820Surface *)arg3->field20)->first_triangle;
            *(s32 *)((u8 *)arg3 + 0x18) = materials[triangle_index];
        } else {
            *(s32 *)((u8 *)arg3 + 0x18) = ((Game71820Surface *)arg3->field20)->default_material;
        }
        arg3->flags |= 6;
        if ((((Game71820Surface *)arg3->field20)->flags & 0x80) == 0x80) {
            arg3->flags |= 1;
        }
        arg3->active = 3;
        if (arg3->height <= arg1) {
            arg3->flags |= 2;
            return 1;
        }
        return 0;
    }
    arg3->flags &= ~2;
    return 0;
}

extern f32 D_80098D5C;

s32 func_15045F8C(struct conker15045714 *arg0, f32 arg1, s32 *arg2, Game71820Hit *arg3) {
    s32 count;
    s32 best;
    s32 scan_index;
    f32 height;

    arg3->height = D_80098D5C;
    best = -1;
    func_150A44F0(*(s32 *)arg2, D_800D3830, 0);
    count = func_150A43E0((s32)arg0->field_0x00, (s32)arg0->field_0x08, *(s32 *)arg2, D_800D3830);
    scan_index = 0;
    if (count > 0) {
        Game71820Candidate *scan = D_800D3300;
        do {
            height = (f32)scan->height * 0.00390625f;
            if ((height <= arg0->field_0x04) && (arg3->height < height)) {
                best = scan_index;
                arg3->height = height;
            }
            scan_index++;
            scan++;
        } while (scan_index < count);
    }
    if (best != -1) {
        Game71820Candidate *candidate;
        s32 *source;
        s32 copy_index;
        s32 *copy_source;
        s32 surface_index;
        u8 *output;
        s32 offset;
        s32 *materials;

        candidate = &D_800D3300[best];
        source = candidate->sources;
        offset = candidate->index;
        copy_index = 0;
        output = (u8 *)arg3;
        copy_source = source;
        for (; copy_index != 3; copy_index++, copy_source++, output += 6) {
            s16 *point = (s16 *)(*copy_source + offset);
            *(s16 *)(output + 4) = point[0];
            *(s16 *)(output + 6) = point[1];
            *(s16 *)(output + 8) = point[2];
        }
        surface_index = candidate->padC;
        arg3->field20 = surface_index * 0xA0 + D_800DBEF4;
        materials = ((Game71820Surface *)arg3->field20)->materials;
        if (materials != 0) {
            s32 triangle_index = ((u8 *)source - D_800DBE3C) / 12 - ((Game71820Surface *)arg3->field20)->first_triangle;
            *(s32 *)((u8 *)arg3 + 0x18) = materials[triangle_index];
        } else {
            *(s32 *)((u8 *)arg3 + 0x18) = ((Game71820Surface *)arg3->field20)->default_material;
        }
        arg3->flags |= 6;
        if ((((Game71820Surface *)arg3->field20)->flags & 0x80) == 0x80) {
            arg3->flags |= 1;
        }
        arg3->active = 3;
        if (arg1 <= arg3->height) {
            arg3->flags |= 2;
            return 1;
        }
        return 0;
    }
    arg3->flags &= ~2;
    return 0;
}

s32 func_150461D0(struct conker15045714 *arg0, u16 arg1, f32 arg2, struct conker150472C0_dst *arg3) {
    s32 sp74;
    s32 sp70;
    struct conker150472C0_dst sp4C;
    struct conker150472C0_dst sp28;
    u8 sp27;

    if (arg2 < arg0->field_0x04) {
        arg3->field_0x1C &= ~2;
        return 0;
    }
    sp4C = *arg3;
    sp28 = *arg3;
    func_15045714(arg0, arg1, &sp74, (s32)&sp70);
    sp27 = func_15045880((Vec3f_15045D48 *)arg0, arg2, &sp74, (Game71820Hit *)&sp4C);
    arg0 = (struct conker15045714 *)func_15045D48((Vec3f_15045D48 *)arg0, arg2, &sp70, (Game71820Hit *)&sp28);

    if (sp27 && ((s32)arg0 & 0xFF) != 0) {
        if (sp4C.field_0x00 < sp28.field_0x00) {
            *arg3 = sp4C;
        } else {
            *arg3 = sp28;
        }
        return 1;
    }
    if (sp27) {
        *arg3 = sp4C;
        return 1;
    }
    if (((s32)arg0 & 0xFF) != 0) {
        *arg3 = sp28;
        return 1;
    }
    if (sp4C.field_0x00 < sp28.field_0x00) {
        *arg3 = sp4C;
    } else {
        *arg3 = sp28;
    }
    arg3->field_0x1C &= ~2;
    return 0;
}

s32 func_15046460(struct conker15045714 *arg0, u16 arg1, f32 arg2, struct conker150472C0_dst *arg3) {
    s32 first_set;
    s32 second_set;
    struct conker150472C0_dst first_hit;
    struct conker150472C0_dst second_hit;
    u8 first_result;
    u8 second_result;

    if (arg0->field_0x04 < arg2) {
        arg3->field_0x1D = 0;
        arg3->field_0x20 = 0;
        arg3->field_0x1C &= ~2;
        return 0;
    }
    first_hit = *arg3;
    second_hit = *arg3;
    func_15045714(arg0, arg1, &first_set, (s32)&second_set);
    first_result = func_15045AE4((Vec3f_15045D48 *)arg0, arg2, &first_set, (Game71820Hit *)&first_hit);
    second_result = func_15045F8C(arg0, arg2, &second_set, (Game71820Hit *)&second_hit);
    if (first_result && second_result) {
        if (second_hit.field_0x00 < first_hit.field_0x00) {
            *arg3 = first_hit;
            return 1;
        }
        *arg3 = second_hit;
        return 1;
    }
    if (first_result != 0) {
        *arg3 = first_hit;
        return 1;
    }
    if (second_result != 0) {
        *arg3 = second_hit;
        return 1;
    }
    if (second_hit.field_0x00 < first_hit.field_0x00) {
        *arg3 = first_hit;
    } else {
        *arg3 = second_hit;
    }
    arg3->field_0x1C &= ~2;
    return 0;
}

s32 func_150461D0(struct conker15045714 *, u16, f32, struct conker150472C0_dst *);
s32 func_15046460(struct conker15045714 *, u16, f32, struct conker150472C0_dst *);
s32 func_150450CC(struct conker15045714 *, f32, struct conker150472C0_dst *);
s32 func_1504554C(struct conker15045714 *, f32, struct conker150472C0_dst *);

s32 func_150466F8(struct conker15045714 *arg0, u16 arg1, f32 arg2, struct conker150472C0_dst *arg3) {
    struct conker150472C0_dst sp4C;
    struct conker150472C0_dst sp28;
    u8 sp27;

    if (arg2 < arg0->field_0x04) {
        arg3->field_0x1C &= ~2;
        return 0;
    }

    sp4C = *arg3;
    sp28 = *arg3;
    sp27 = func_150461D0(arg0, arg1, arg2, &sp4C);
    arg0 = (struct conker15045714 *)func_15044ED0(arg0, arg2, &sp28);

    if (sp27 != 0 && ((s32)arg0 & 0xFF) != 0) {
        if (sp4C.field_0x00 < sp28.field_0x00) {
            *arg3 = sp4C;
        } else {
            *arg3 = sp28;
        }
        return 1;
    }

    if (sp27 != 0) {
        *arg3 = sp4C;
        return 1;
    }

    if (((s32)arg0 & 0xFF) != 0) {
        *arg3 = sp28;
        return 1;
    }

    if (sp4C.field_0x00 < sp28.field_0x00) {
        *arg3 = sp4C;
    } else {
        *arg3 = sp28;
    }
    arg3->field_0x1C &= ~2;
    return 0;
}

s32 func_1504697C(struct conker15045714 *arg0, u16 arg1, f32 arg2, struct conker150472C0_dst *arg3) {
    struct conker150472C0_dst sp4C;
    struct conker150472C0_dst sp28;
    u8 sp27;

    if (arg0->field_0x04 < arg2) {
        arg3->field_0x1C &= ~2;
        return 0;
    }

    sp4C = *arg3;
    sp28 = *arg3;
    sp27 = func_15046460(arg0, arg1, arg2, &sp4C);
    arg0 = (struct conker15045714 *)func_150450CC(arg0, arg2, &sp28);

    if (sp27 != 0 && ((s32)arg0 & 0xFF) != 0) {
        if (sp28.field_0x00 < sp4C.field_0x00) {
            *arg3 = sp4C;
        } else {
            *arg3 = sp28;
        }
        return 1;
    }

    if (sp27 != 0) {
        *arg3 = sp4C;
        return 1;
    }

    if (((s32)arg0 & 0xFF) != 0) {
        *arg3 = sp28;
        return 1;
    }

    if (sp28.field_0x00 < sp4C.field_0x00) {
        *arg3 = sp4C;
    } else {
        *arg3 = sp28;
    }
    arg3->field_0x1C &= ~2;
    return 0;
}

s32 func_150470B0(struct conker15045714 *, s32, struct conker150472C0_dst *);
s32 func_150466F8(struct conker15045714 *, u16, f32, struct conker150472C0_dst *);

s32 func_15046C00(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (func_150470B0((struct conker15045714 *)arg0, arg2, (struct conker150472C0_dst *)arg3)) {
    case 0:
        return func_150466F8((struct conker15045714 *)arg0, (u16)arg1, *(f32 *)&arg2, (struct conker150472C0_dst *)arg3);
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        break;
    }
}

s32 func_1504697C(struct conker15045714 *, u16, f32, struct conker150472C0_dst *);

s32 func_15046C80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (func_15047004((struct conker15045714 *)arg0, arg2, (struct conker150472C0_dst *)arg3)) {
    case 0:
        return func_1504697C((struct conker15045714 *)arg0, (u16)arg1, *(f32 *)&arg2, (struct conker150472C0_dst *)arg3);
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        break;
    }
}

s32 func_15046D00(struct conker15045714 *arg0, u16 arg1, f32 arg2, struct conker150472C0_dst *arg3) {
    struct conker150472C0_dst sp4C;
    struct conker150472C0_dst sp28;
    u8 sp27;

    if (arg0->field_0x04 < arg2) {
        arg3->field_0x1C &= ~2;
        return 0;
    }

    sp4C = *arg3;
    sp28 = *arg3;
    sp27 = func_1504697C(arg0, arg1, arg2, &sp4C);
    arg0 = (struct conker15045714 *)func_1504554C(arg0, arg2, &sp28);

    if (sp27 != 0 && ((s32)arg0 & 0xFF) != 0) {
        if (sp28.field_0x00 < sp4C.field_0x00) {
            *arg3 = sp4C;
        } else {
            *arg3 = sp28;
        }
        return 1;
    }

    if (sp27 != 0) {
        *arg3 = sp4C;
        return 1;
    }

    if (((s32)arg0 & 0xFF) != 0) {
        *arg3 = sp28;
        return 1;
    }

    if (sp28.field_0x00 < sp4C.field_0x00) {
        *arg3 = sp4C;
    } else {
        *arg3 = sp28;
    }
    arg3->field_0x1C &= ~2;
    return 0;
}

s32 func_15046D00(struct conker15045714 *, u16, f32, struct conker150472C0_dst *);

s32 func_15046F84(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    switch (func_15047004((struct conker15045714 *)arg0, arg2, (struct conker150472C0_dst *)arg3)) {
    case 0:
        return func_15046D00((struct conker15045714 *)arg0, (u16)arg1, *(f32 *)&arg2, (struct conker150472C0_dst *)arg3);
    case 1:
        return 0;
    case 2:
        return 1;
    default:
        break;
    }
}

extern s32 func_150A3FC4(f32, f32, s32, s32, s32);

s32 func_15047004(struct conker15045714 *a0, s32 a1, struct conker150472C0_dst *a2) {
    f32 sp24;

    if (a2->field_0x1C & 0x4) {
        if (func_150A3FC4(a0->field_0x00, a0->field_0x08, 0, (s32)&a2->field_0x04, (s32)&sp24)) {
            if (*(f32 *)&a1 <= sp24) {
                if (sp24 <= a0->field_0x04) {
                    a2->field_0x00 = sp24;
                    a2->field_0x1C |= 0x2;
                    return 2;
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 func_150470B0(struct conker15045714 *a0, s32 a1, struct conker150472C0_dst *a2) {
    f32 sp24;

    if (a2->field_0x1C & 0x4) {
        if (func_150A3FC4(a0->field_0x00, a0->field_0x08, 0, (s32)&a2->field_0x04, (s32)&sp24)) {
            if (sp24 <= *(f32 *)&a1) {
                if (a0->field_0x04 <= sp24) {
                    a2->field_0x00 = sp24;
                    a2->field_0x1C |= 0x2;
                    return 2;
                }
            }
            return 1;
        }
    }
    return 0;
}

typedef struct { s16 x, y, z; } Vec3s_1504715C;
typedef struct { Vec3s_1504715C v[3]; } Tri_1504715C;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ Tri_1504715C tri;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 unk1C;
    /* 0x1D */ u8 unk1D;
    /* 0x1E */ u8 pad1E[2];
    /* 0x20 */ s32 unk20;
} Out1504715C;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ f32 unk14;
    /* 0x018 */ f32 unk18;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 pad20[0xD8];
    /* 0x0F8 */ u32 unkF8;
    /* 0x0FC */ u8 padFC[0x84];
    /* 0x180 */ f32 unk180;
    /* 0x184 */ s32 unk184;
    /* 0x188 */ u8 pad188[4];
    /* 0x18C */ Tri_1504715C tri;
    /* 0x19E */ u8 pad19E[2];
    /* 0x1A0 */ u16 unk1A0;
} In1504715C;



void func_1504715C(Out1504715C *arg0, In1504715C *arg1) {
    s32 idx;
    s32 v;

    arg0->unk0 = arg1->unk180;
    if (arg1->unkF8 & 0x200000) {
        s16 x;
        s16 y;
        s16 z;

        x = arg1->unk14;
        y = arg1->unk180;
        z = arg1->unk1C;
        arg0->tri.v[0].x = x + 1000;
        arg0->tri.v[0].y = y;
        arg0->tri.v[0].z = z + 1000;
        arg0->tri.v[1].x = x - 1000;
        arg0->tri.v[1].y = y;
        arg0->tri.v[1].z = z;
        arg0->tri.v[2].x = x + 1000;
        arg0->tri.v[2].y = y;
        arg0->tri.v[2].z = z - 1000;
    } else {
        arg0->tri = arg1->tri;
    }
    arg0->unk18 = arg1->unk184;
    arg0->unk1C = 6;
    if (arg1->unk1A0 != 0) {
        idx = arg1->unk1A0 - 1;
        arg0->unk1D = 2;
        arg0->unk20 = idx * 0xA0 + D_800DBEF4;
        if (func_15145C90(idx) != 0) {
            v = 1;
        } else {
            v = 0;
        }
        arg0->unk1C |= v;
    lab: ;
    } else {
        arg0->unk1D = 1;
        arg0->unk20 = 0;
        arg0->unk1C |= 1;
    }
}

void func_150472C0(struct conker150472C0_dst *arg0, struct conker150472C0_src *arg1)
{
  s32 one;
  s32 temp_v0;
  s32 temp_v1;
  s32 temp_a2;
  u8 temp_t2;
  one = 1;
  arg0->field_0x00 = arg1->unkC;
  arg0->field_0x04 = (0, arg1->unk44);
  arg0->field_0x18 = arg1->unk60;
  temp_v0 = arg1->unk59;
  temp_v1 = (1 == (temp_v0 ^ 0)) ? (2) : (0);
  temp_t2 = 1;
  temp_a2 = (one == temp_v0) ? (one) : (0);
  temp_v0 = (one == temp_v0) ? (4) : (0);
  arg0->field_0x1C = (temp_v0 | temp_a2) | temp_v1;
  if (arg1->unk59 == one)
  {
    arg0->field_0x1D = temp_t2;
  }
  else
  {
    arg0->field_0x1D = 0;
  }
  arg0->field_0x20 = arg1->unk5C;
}


void func_15047390(f32 mf[4][4], f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 temp_f0;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f28;
    f32 temp_f30;
    f32 temp_f18;

    guMtxIdentF(mf);

    temp_f26 = arg4 - arg1;
    temp_f28 = arg5 - arg2;
    temp_f30 = arg6 - arg3;
    temp_f18 = sqrtf((temp_f26 * temp_f26) + (temp_f28 * temp_f28) + (temp_f30 * temp_f30));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D60;
    }
    temp_f0 = -1.0f / temp_f18;
    temp_f26 *= temp_f0;
    temp_f28 *= temp_f0;
    temp_f30 *= temp_f0;

    temp_f20 = (arg8 * temp_f30) - (arg9 * temp_f28);
    temp_f22 = (arg9 * temp_f26) - (arg7 * temp_f30);
    temp_f24 = (arg7 * temp_f28) - (arg8 * temp_f26);
    temp_f18 = sqrtf((temp_f20 * temp_f20) + (temp_f22 * temp_f22) + (temp_f24 * temp_f24));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D64;
    }
    temp_f0 = 1.0f / temp_f18;
    temp_f20 *= temp_f0;
    temp_f22 *= temp_f0;
    temp_f24 *= temp_f0;

    arg7 = (temp_f28 * temp_f24) - (temp_f30 * temp_f22);
    arg8 = (temp_f30 * temp_f20) - (temp_f26 * temp_f24);
    arg9 = (temp_f26 * temp_f22) - (temp_f28 * temp_f20);
    temp_f18 = sqrtf((arg7 * arg7) + (arg8 * arg8) + (arg9 * arg9));
    if (temp_f18 == 0.0f) {
        temp_f18 = D_80098D68;
    }
    temp_f0 = 1.0f / temp_f18;
    arg7 *= temp_f0;
    arg8 *= temp_f0;
    arg9 *= temp_f0;

    mf[0][0] = temp_f20;
    mf[1][0] = temp_f22;
    mf[2][0] = temp_f24;
    mf[3][0] = -((arg1 * temp_f20) + (arg2 * temp_f22) + (arg3 * temp_f24));
    mf[0][1] = arg7;
    mf[1][1] = arg8;
    mf[2][1] = arg9;
    mf[3][1] = -((arg1 * arg7) + (arg2 * arg8) + (arg3 * arg9));
    mf[0][2] = temp_f26;
    mf[1][2] = temp_f28;
    mf[2][2] = temp_f30;
    mf[3][2] = -((arg1 * temp_f26) + (arg2 * temp_f28) + (arg3 * temp_f30));
    mf[3][3] = 1.0f;
    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
}

void func_15047688(Mtx *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9) {
    f32 sp30[4][4];

    func_15047390(&sp30, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    guMtxF2L(&sp30, arg0);
}

#pragma intrinsic(sqrtf)

void func_15047700(f32 mf[4][4], s32 arg1, f32 xEye, f32 yEye, f32 zEye, f32 xAt, f32 yAt, f32 zAt, f32 xUp, f32 yUp, f32 zUp) {
    f32 len;
    f32 xLook;
    f32 yLook;
    f32 zLook;
    f32 xRight;
    f32 yRight;
    f32 zRight;

    guMtxIdentF(mf);

    xLook = xAt - xEye;
    yLook = yAt - yEye;
    zLook = zAt - zEye;
    if (xLook == 0.0f && yLook == 0.0f && zLook == 0.0f) {
        zLook = 1.0f;
        xLook = 0.0f;
        yLook = 0.0f;
    }

    len = -1.0f / sqrtf(xLook * xLook + yLook * yLook + zLook * zLook);
    xLook *= len;
    yLook *= len;
    zLook *= len;

    xRight = yUp * zLook - zUp * yLook;
    yRight = zUp * xLook - xUp * zLook;
    zRight = xUp * yLook - yUp * xLook;
    len = xRight * xRight + yRight * yRight + zRight * zRight;
    if (len != 0.0f) {
        len = 1.0f / sqrtf(len);
        xRight *= len;
        yRight *= len;
        zRight *= len;
    } else {
        xRight = 1.0f;
        yRight = 0.0f;
        zRight = 0.0f;
    }

    xUp = yLook * zRight - zLook * yRight;
    yUp = zLook * xRight - xLook * zRight;
    zUp = xLook * yRight - yLook * xRight;
    len = xUp * xUp + yUp * yUp + zUp * zUp;
    if (len != 0.0f) {
        len = 1.0f / sqrtf(len);
        xUp *= len;
        yUp *= len;
        zUp *= len;
    } else {
        xUp = 0.0f;
        yUp = 1.0f;
        zUp = 0.0f;
    }

    ((LookAt *)arg1)->l[0].l.dir[0] = (s32)((xRight * 128.0f) < 127.0f ? (xRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[0].l.dir[1] = (s32)((yRight * 128.0f) < 127.0f ? (yRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[0].l.dir[2] = (s32)((zRight * 128.0f) < 127.0f ? (zRight * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[0] = (s32)((xUp * 128.0f) < 127.0f ? (xUp * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[1] = (s32)((yUp * 128.0f) < 127.0f ? (yUp * 128.0f) : 127.0f) & 0xFF;
    ((LookAt *)arg1)->l[1].l.dir[2] = (s32)((zUp * 128.0f) < 127.0f ? (zUp * 128.0f) : 127.0f) & 0xFF;

    ((LookAt *)arg1)->l[0].l.col[0] = 0;
    ((LookAt *)arg1)->l[0].l.col[1] = 0;
    ((LookAt *)arg1)->l[0].l.col[2] = 0;
    ((LookAt *)arg1)->l[0].l.pad1 = 0;
    ((LookAt *)arg1)->l[0].l.colc[0] = 0;
    ((LookAt *)arg1)->l[0].l.colc[1] = 0;
    ((LookAt *)arg1)->l[0].l.colc[2] = 0;
    ((LookAt *)arg1)->l[0].l.pad2 = 0;
    ((LookAt *)arg1)->l[1].l.col[0] = 0;
    ((LookAt *)arg1)->l[1].l.col[1] = 0x80;
    ((LookAt *)arg1)->l[1].l.col[2] = 0;
    ((LookAt *)arg1)->l[1].l.pad1 = 0;
    ((LookAt *)arg1)->l[1].l.colc[0] = 0;
    ((LookAt *)arg1)->l[1].l.colc[1] = 0x80;
    ((LookAt *)arg1)->l[1].l.colc[2] = 0;
    ((LookAt *)arg1)->l[1].l.pad2 = 0;

    mf[0][0] = xRight;
    mf[1][0] = yRight;
    mf[2][0] = zRight;
    mf[3][0] = -(xEye * xRight + yEye * yRight + zEye * zRight);

    mf[0][1] = xUp;
    mf[1][1] = yUp;
    mf[2][1] = zUp;
    mf[3][1] = -(xEye * xUp + yEye * yUp + zEye * zUp);

    mf[0][2] = xLook;
    mf[1][2] = yLook;
    mf[2][2] = zLook;
    mf[3][2] = -(xEye * xLook + yEye * yLook + zEye * zLook);

    mf[0][3] = 0.0f;
    mf[1][3] = 0.0f;
    mf[2][3] = 0.0f;
    mf[3][3] = 1.0f;
}

void func_15047700(f32 mf[4][4], s32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

void func_15047B80(Mtx *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, f32 arg10) {
    f32 sp38[4][4];

    func_15047700(&sp38, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    guMtxF2L(&sp38, arg0);
}
