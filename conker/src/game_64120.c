#include <ultra64.h>
#define func_15048A40 func_15048A40_hdr
#include "functions.h"
#undef func_15048A40
#include "variables.h"

s32 func_150380C0(f32 *, s32, s32, s32, f32, s32, f32 *, s32, s32, s32, s32);
extern f32 D_800C3FD0[];


extern f32 D_80098250;
extern f32 D_8009862C;
extern s16 D_800C3FF4[];
extern u16 *D_80084380[];
extern u8 D_80097E7C[];
extern u8 D_80098050[];

void func_15039A78(f32 *, f32 *, f32 *, f32 *, f32, f32, s32);

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} GameStruct_15039A54;

extern GameStruct_15039A54 D_80098068[];
void func_1503B708(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4,
                   s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);


typedef struct {
    u8 pad0[0x324];
    f32 *unk324;
} GameStruct_15036C70;

void func_15036C70(GameStruct_15036C70 *arg0) {
    s32 i;
    f32 val;

    arg0->unk324 = allocate_memory(0x48, 1, 0, 0);
    bzero(arg0->unk324, 0x48);
    val = D_80098250;
    for (i = 0; i != 0xC; i += 4) {
        *(f32 *)((u8 *)arg0->unk324 + i) = val;
        *(f32 *)((u8 *)arg0->unk324 + i + 0xC) = val;
    }
}

extern u8 D_800C3FFA;
static f32 D_800C3FE8[3];
void func_150379DC(f32 *, s32, s32, s32, f32, s32, s32);
void func_15038620(s32, s32, f32 *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_15036CE8(void) {
    struct127 *obj;
    s32 sp70;
    f32 *data;
    f32 *pos;
    f32 val;
    s32 pad1;
    s32 pad2;
    s32 sp58;
    s32 pad3;
    u8 type;

    obj = &D_800CC2D0[D_800C3E78];
    data = *(f32 **)((u8 *)obj + 0x324);
    if (data == NULL) {
        return;
    }
    switch (obj->id) {
        case 59:
        case 117:
        case 128:
        case 136:
        case 144:
        case 150:
        case 152:
        case 156:
        case 157:
        case 159:
        case 160:
        case 176:
        case 177:
        case 178:
        case 180:
            if (D_800C3FFA == 0) {
                D_800C3FFA = 1;
                return;
            }
            break;
    }
    if (D_800C35EA == 1 && D_800C3FFA == 0) {
        D_800C3FFA = 1;
        return;
    }
    sp70 = 0;
    sp58 = 1;
    if (D_800C3FFA == 0) {
        type = 7;
        sp58 = 0;
        sp70 = 1;
        if (*(u16 *)((u8 *)D_800D154C + 0x2F8) & 0x100) {
            *(s16 *)((u8 *)data + 0x3A) = 0x2D;
            *(s16 *)((u8 *)data + 0x40) = 0x40;
        } else if (data[0] == -99999.0f) {
            D_800C3FFA = 1;
            return;
        }
    } else {
        type = 12;
        ;
    }
    if (D_800C3FFA == 0 && !(*(u16 *)((u8 *)D_800D154C + 0x2F8) & 0x100)) {
        D_800C3FE8[0] = -99999.0f;
        D_800C3FE8[1] = -99999.0f;
        D_800C3FE8[2] = -99999.0f;
    } else {
        func_150379DC(D_800C3FE8, type, 0, D_800C3E78, 90.0f, 0xFF, 0);
    }
    func_15038620(type, 0, D_800C3FE8, D_800C3E78, sp70, 0, D_800C3FFA, 3, 0, 0, 0, sp58, 0);
    D_800C3FFA++;
}

void func_15036F34(void) {
    s32 type;
    s32 sub;
    s32 d;
    s32 c;
    s32 e;
    s32 g2;
    f32 range;
    f32 yoff;
    s32 f;
    s32 sp78;
    s32 ra;
    s32 sp70;
    s32 reset;
    s32 force;
    s32 pad;
    s32 sp60;
    s32 i;

    sp78 = 0x100;
    sp60 = 1;
    range = 1600000000.0f;
    d = 1;
    c = 2;
    e = 3;
    force = 0;
    f = 0;
    g2 = 0;
    sp70 = 0;
    ra = 0;
    i = 0;
    yoff = 0.0f;
    do {} while (0);
    reset = 0;
    switch (D_800CC2D0[D_800C3E78].id) {
    case 130:
        type = 12;
        sub = 1;
        break;
    case 162:
        type = 4;
        sub = 3;
        break;
    case 164:
        type = 4;
        sub = 3;
        break;
    case 94:
        type = 4;
        sub = 3;
        break;
    case 151:
        type = 4;
        sub = 3;
        break;
    case 49:
        type = 5;
        sub = 4;
        break;
    case 139:
        type = 0x21;
        sub = 0x20;
        break;
    case 135:
    case 181:
        type = 4;
        sub = 3;
        break;
    case 134:
        sub = 3;
        type = 5;
        if (D_800C3FFA == 0) {
            type = 4;
            d = 10;
        } else {
            sub = 4;
        }
        break;
    case 153:
        sub = 3;
        type = 5;
        if (D_800C3FFA == 0) {
            type = 4;
            d = 10;
        } else {
            sub = 4;
        }
        break;
    case 17:
        sub = 3;
        type = 5;
        if (D_800C3FFA == 0) {
            type = 4;
            d = 10;
        } else {
            sub = 4;
        }
        break;
    case 20:
        sub = 3;
        type = 5;
        if (D_800C3FFA == 0) {
            type = 4;
            d = 10;
        } else {
            sub = 4;
        }
        break;
    case 121:
        type = 0x1B;
        sub = 0x1A;
        break;
    case 33:
        type = 0x19;
        sub = 0x18;
        break;
    case 123:
        type = 3;
        sub = 2;
        if (D_800C3FFA == 0) {
            ra = 1;
            break;
        }
        yoff = -165.0f;
        c = 10;
        d = 7;
        switch (D_800CC2D0[D_800C3E78].unk84.uh) {
        case 7:
        case 9:
            g2 = 1;
            break;
        case 20:
            sp78 = 0x7E;
            yoff = 0.0f;
            d = 9;
            c = 11;
            break;
        default:
            reset = 1;
        }
        if (D_800C3FFA == 1) {
            type = 0x16;
            sub = 0x16;
            i = 2;
            force = 1;
        } else {
            type = 0x1A;
            sub = 0x1A;
            i = 1;
        }
        goto setra;
    case 124:
        type = 1;
        sub = 0;
        g2 = 1;
        i = 3;
        break;
    case 108:
        type = 0x11;
        sub = 0x10;
        break;
    case 52:
        type = 4;
        sub = 3;
        break;
    case 118:
        type = 4;
        sub = 3;
        break;
    case 105:
        type = 4;
        sub = 3;
        break;
    case 138:
        if (D_800BE616 != 0) {
            return;
        }
        type = 4;
        sub = 0;
        if (D_800CC2D0[D_800C3E78].unk2E8 == 1) {
            return;
        }
        yoff = -100.0f;
        e = 2;
        c = 12;
        break;
    case 90:
    case 95:
    case 116:
    case 122:
    case 141:
        type = 3;
        sub = 2;
        break;
    case 88:
        type = 4;
        sub = 3;
        break;
    case 91:
        type = 4;
        sub = 3;
        break;
    case 84:
        type = 0x1D;
        sub = 0x1C;
        g2 = 1;
        break;
    case 41:
        type = 2;
        sub = 1;
        break;
    case 75:
        if (D_800C3FFA == 0) {
            type = 0x2B;
            sub = 4;
            d = 6;
            sp60 = 0;
            g2 = 1;
        } else {
            type = 0x2C;
            sub = 4;
            g2 = 1;
        }
    setra:
        ra = 1;
        break;
    case 80:
    case 154:
    case 155:
        type = 0xD;
        sub = 2;
        break;
    case 74:
        type = 1;
        sub = 0;
        e = 1;
        f = 1;
        break;
    case 71:
        type = 1;
        sub = 0;
        e = 1;
        f = 1;
        break;
    case 68:
        type = 3;
        sub = 3;
        break;
    case 40:
        if (D_800C3FFA == 0) {
            type = 2;
            sub = 0;
            e = 1;
            f = 1;
            d = 4;
            sp70 = 1;
        } else {
            type = 3;
            sub = 2;
            e = 2;
            f = 1;
            d = 4;
            sp70 = 1;
        }
        break;
    case 43:
        type = 1;
        sub = 0;
        c = 3;
        d = 3;
        e = 1;
        f = 1;
        g2 = 1;
        break;
    case 58:
    case 61:
        type = 5;
        sub = 4;
        c = 5;
        break;
    case 83:
    case 165:
        type = 5;
        sub = 0;
        c = 7;
        break;
    case 54:
        type = 4;
        sub = 3;
        c = 4;
        break;
    case 45:
        type = 5;
        sub = 4;
        g2 = 1;
        break;
    case 12:
    case 22:
    case 34:
    case 179:
        type = 4;
        sub = 3;
        break;
    case 125:
        type = 4;
        sub = 3;
        break;
    case 32:
    case 140:
    case 147:
        type = 2;
        sub = 1;
        break;
    case 5:
    case 173:
    case 174:
    case 175:
        sub = 0;
        type = 0x10;
        if (D_800C3FFA == 0) {
            type = 1;
            d = 2;
            sp60 = 0;
        } else {
            sub = 1;
        }
        break;
    case 18:
        e = 2;
        c = 8;
        ra = 1;
        if (D_800C3FFA == 0) {
            type = 1;
            sub = 0;
        } else if (D_800C3FFA == 1) {
            type = 3;
            sub = 0;
        } else {
            type = 5;
            sub = 0;
        }
        g2 = 1;
        break;
    default:
        return;
    }
    if (reset) {
        D_800C3FE8[0] = -99999.0f;
        D_800C3FE8[1] = -99999.0f;
        D_800C3FE8[2] = -99999.0f;
    } else if (D_800C3FFA == 0 || force) {
        func_1503B708(D_800C3FE8, type, sub, D_800C3E78,
                      D_80098068[c - 1].unk0 + D_80098068[c - 1].unk10, sp78, &range, f, g2, ra, i);
        D_800C3FE8[1] += yoff;
    }
    func_15038620(type, sub, D_800C3FE8, D_800C3E78, c, d, D_800C3FFA, e, f, sp70, 0, sp60, i);
    D_800C3FFA++;
}

void func_15038468(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5);

s32 func_15037698(s32 arg0, s32 arg1, s32 arg2, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 arg8, s32 arg9) {
    f32 (*src)[4];
    f32 (*dst)[4];
    f32 delta[3];
    f32 pos[3];
    f32 rx;
    f32 ry;
    f32 rz;
    f32 yaw;
    f32 pitch;

    if (arg8 != 0) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    src = ((f32 (*)[4][4])D_800CC2D0[arg0].unk1D4)[arg1];
    if (D_800CC2D0[arg0].unk1D4 == NULL) {
        return 0;
    }
    dst = ((f32 (*)[4][4])D_800CC2D0[arg0].unk1D4)[arg2];
    src[0][3] = src[1][3] = src[2][3] = dst[0][3] = dst[1][3] = dst[2][3] = 0.0f;
    src[3][3] = dst[3][3] = 1.0f;
    pos[0] = src[1][0];
    pos[1] = src[1][1];
    pos[2] = src[1][2];
    delta[0] = x - src[3][0];
    delta[1] = y - src[3][1];
    delta[2] = z - src[3][2];
    rx = delta[0] * dst[0][0] + delta[1] * dst[0][1] + delta[2] * dst[0][2];
    ry = delta[0] * dst[1][0] + delta[1] * dst[1][1] + delta[2] * dst[1][2];
    rz = delta[0] * dst[2][0] + delta[1] * dst[2][1] + delta[2] * dst[2][2];
    func_15038468(&yaw, &pitch, rx, ry, rz, arg9);
    if (yaw < range && -range < yaw) {
        out[0] = x;
        out[1] = y;
        out[2] = z;
        return 1;
    }
    return 0;
}

s32 func_15037880(s32 arg0, f32 *arg1) {
    struct127 *obj;
    s32 temp;
    f32 dx;
    f32 dz;

    obj = &D_800CC2D0[arg0];
    temp = obj->unk2F8 & 7;
    if (temp == 1) {
        arg1[0] = D_800DBFF0->unk2F8;
        arg1[1] = D_800DBFF0->unk2FC;
        arg1[2] = D_800DBFF0->unk300;
        return 1;
    }
    if (temp == 2) {
        if (obj->unk5 == 4) {
            dx = obj->x_position - (f32)D_800C3FF4[0];
            dz = obj->z_position - (f32)D_800C3FF4[2];
            if (D_8009862C < (dx * dx) + (dz * dz)) {
                return 1;
            }
        }
        arg1[0] = D_800C3FF4[0];
        arg1[1] = D_800C3FF4[1];
        arg1[2] = D_800C3FF4[2];
        return 1;
    }
    if (temp == 3) {
        return 1;
    }
    return 0;
}

typedef struct {
    f32 x, y, z;
} Vec3_150379DC;

typedef struct {
    s32 v[8];
} Ids_150379DC;

typedef struct Node_150379DC {
    u8 pad0[8];
    struct Node_150379DC *next;
    u8 padC[0x2C];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    u8 pad44[0x22];
    u16 unk66;
    u8 pad68[0x30];
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
} Node_150379DC;

typedef struct {
    u8 unk0;
    u8 pad1[0x47];
    u8 *unk48;
} S150319CC_150379DC;

extern Ids_150379DC D_800843E0;
extern u32 D_80084400;
extern u8 D_80084404[];
extern Node_150379DC *D_800DCE50[2][104];
S150319CC_150379DC *func_150319CC(s32, struct127 *);
void func_1507E9E8(struct127 *, s32);

void func_150379DC(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5, s32 arg6) {
    struct127 *obj;
    s32 result;
    s32 found;
    s16 x;
    u32 idx;
    struct160 *e;
    f32 maxDist;
    f32 ex;
    f32 ey;
    s32 p;
    s32 i;
    u32 bestIdx;
    f32 bestDist;
    f32 ez;
    f32 d;
    static u32 D_80084400;

    arg0[0] = -99999.0f;
    arg0[1] = -99999.0f;
    arg0[2] = -99999.0f;
    D_800C3FD0[0] = -99999.0f;
    D_800C3FD0[1] = -99999.0f;
    D_800C3FD0[2] = -99999.0f;
    if ((&D_800C35EA)[0] == 1 || (&D_800C35EA)[1] == 1) {
        return;
    }
    obj = &D_800CC2D0[arg3];
    if (obj->camera != NULL && obj->camera->unk2C == 0x100 && obj->camera->unk73C != 0) {
        Vec3_150379DC camPos;

        camPos = *(Vec3_150379DC *)((u8 *)obj->camera + 0x740);
        arg0[0] = camPos.x;
        arg0[1] = camPos.y;
        arg0[2] = camPos.z;
        return;
    }
    if (func_15037880(arg3, arg0) != 0) {
        return;
    }
    if (arg3 == 0 && obj->unk84.uh == 0x81) {
        S150319CC_150379DC *s;
        u8 *sub;
        struct113 *v;
        Vec3_150379DC tgt;
        struct127 *o;

        if ((s = func_150319CC(0x1F, obj)) != NULL && (sub = s->unk48) != NULL && sub[0x3F6] != 0 &&
            (v = ((struct113 **)(sub + 0x3E8))[D_800BE9C0]) != NULL &&
            (o = func_15083E90(s->unk0)) != NULL) {
            if (o->unk5 == 5) {
                tgt.x = o->x_position;
                tgt.y = o->y_position;
                tgt.z = o->z_position;
            } else {
                func_15043FF0((vertex *)&tgt, v);
            }
            if (func_15037698(arg3, arg1, arg2, tgt.x, tgt.y, tgt.z, arg0, arg4, 0, arg6) != 0) {
                return;
            }
        }
    }
    found = 0;
    x = (s32)D_800CC2D0[arg3].x_position;
    bestDist = 1600000000.0f;
    result = func_150380C0(arg0, arg1, arg2, arg3, arg4, arg5, &bestDist, 0, 0, 0, arg6);
    if (result != 0 && bestDist != 1600000000.0f) {
        found = 1;
    }
    {
        f32 pos[3];
        f32 range;
        Ids_150379DC ids;
        s32 ok;
        s32 type;
        Node_150379DC *node;
        f32 dx;
        f32 dy;
        f32 dz;

    if (D_80084400 >= D_800C6654) {
        D_80084400 = 0;
    }
    if (D_800C6654 != 0) {
        idx = D_80084400;
        bestIdx = -1;
        do {
            e = &(*(struct160 **)D_800C6650)[idx];
            if (x < e->unkE && e->unk10 < x && (u16)e->unk6 == 0) {
                maxDist = e->unk8;
                ex = e->unk0;
                ey = e->unk2;
                ez = e->unk4;
                dz = obj->z_position - ez;
                dy = obj->y_position - ey;
                dz *= dz;
                dx = x - ex;
                dy *= dy;
                d = dx * dx + dz + dy;
                if (d < maxDist && d < bestDist &&
                    func_15037698(arg3, arg1, arg2, ex, ey + 50.0f, ez, arg0, arg4, 0, arg6) != 0) {
                    bestIdx = idx;
                    bestDist = d;
                }
            }
            idx++;
            if (idx == D_800C6654) {
                idx = 0;
            }
        } while (idx != D_80084400);
        if (bestIdx != -1) {
            D_80084400 = bestIdx;
            found = 2;
        }
    }

    ids = D_800843E0;
    for (p = 0; p < 2; p++) {
        for (i = 0; i < 8; i++) {
            type = ids.v[i];
            node = D_800DCE50[p][type];
            while (node != NULL) {
                if (type == 25 || type == 72) {
                    extern s32 D_800C6654;
                    if (node->unk66 == 0) {
                        range = 250000.0f;
                        ok = 1;
                        pos[0] = node->unk38;
                        pos[1] = node->unk3C;
                        pos[2] = node->unk40;
                    } else {
                        ok = 0;
                    }
                } else {
                    range = 250000.0f;
                    ok = 1;
                    pos[0] = node->unk98;
                    pos[1] = node->unk9C;
                    pos[2] = node->unkA0;
                }
                if (ok) {
                    ez = pos[2];
                    ey = pos[1];
                    ex = pos[0];
                    dz = obj->z_position - ez;
                    dy = obj->y_position - ey;
                    dz *= dz;
                    dx = x - ex;
                    dy *= dy;
                    d = dx * dx + dz + dy;
                    if (d < range) {
                        dx = x - pos[0];
                        if (dx * dx + dz + dy < bestDist &&
                            func_15037698(arg3, arg1, arg2, ex, ey, ez, arg0, arg4, 0, arg6) != 0) {
                            found = 3;
                            bestDist = d;
                        }
                    }
                }
                node = node->next;
            }
        }
    }
    }
    if (found == 1) {
        func_1507E9E8(obj, result - 1);
        return;
    }
    if (found == 2) {
        if ((*(struct160 **)D_800C6650)[bestIdx].unk12 != 0) {
            func_1507E9E8(obj, (*(struct160 **)D_800C6650)[bestIdx].unk12 - 1);
        }
    } else if (found == 3) {
    }
}

s32 func_150380C0(f32 *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 argA) {
    s32 i;
    f32 dist;
    f32 dx;
    f32 dz;
    f32 dy;
    s32 found;
    f32 range;
    s32 selfId;
    s32 next;
    s32 flag;
    s32 excl;

    found = -1;
    flag = 1;
    excl = -1;
    if ((&D_800C35EA)[0] == 1 || (&D_800C35EA)[1] == 1) {
        arg0[0] = -99999.0f;
        arg0[1] = -99999.0f;
        arg0[2] = -99999.0f;
        return 0;
    }
    next = arg3 + 1;
    selfId = D_800CC2D0[arg3].id;
    if (selfId == 5 || selfId == 0xAD || selfId == 0xAE || selfId == 0xAF) {
        flag = 0;
    }
    if (selfId == 0x2D) {
        excl = 0x2C;
    }
    if (arg9 == 0) {
        range = D_800CC2D0[arg3].unk23D * 8;
        range = range * range;
    } else {
        range = 1600000000.0f;
    }
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state == 0) {
            continue;
        }
        if (D_800CC2D0[i].id == 0xFF) {
            continue;
        }
        if (i == arg3) {
            continue;
        }
        if (next == D_800CC2D0[i].unk65) {
            continue;
        }
        if (D_800CC2D0[arg3].unk65 == i + 1) {
            continue;
        }
        if (flag == 0 && selfId == D_800CC2D0[i].id) {
            continue;
        }
        if (excl == D_800CC2D0[i].id) {
            continue;
        }
        if ((arg8 != 0 || (D_800CC2D0[arg3].unk2F8 & 7) == 4) && D_800CC2D0[i].unk127 == 0xFF) {
            continue;
        }
        if ((D_800CC2D0[arg3].unk2F8 & 7) == 5 && D_800CC2D0[i].unk127 != 0xFF) {
            continue;
        }
        dx = D_800CC2D0[arg3].x_position - D_800CC2D0[i].x_position;
        dz = D_800CC2D0[arg3].z_position - D_800CC2D0[i].z_position;
        dy = D_800CC2D0[arg3].y_position - D_800CC2D0[i].y_position;
        dx = dx * dx;
        dz = dz * dz;
        dy = dy * dy;
        dist = dx + dz + dy;
        if (i == D_800CC2D0[arg3].unk222 && (D_800CC2D0[arg3].unk223 == 1 || D_800CC2D0[arg3].unk223 == 12)) {
            dist = 0.0f;
        }
        if ((dist < range && dist < *arg6) || arg5 == D_800CC2D0[i].id) {
            if (func_15037698(arg3, arg1, arg2, (s16)D_800CC2D0[i].pad1A4, *(s16 *)((u8 *)&D_800CC2D0[i] + 0x1AA),
                              *(s16 *)((u8 *)&D_800CC2D0[i] + 0x1A8), arg0, arg4, arg7, argA) != 0) {
                *arg6 = dist;
                found = i;
                if (arg5 == D_800CC2D0[i].id) {
                    break;
                }
            }
        }
    }
    if (found != -1) {
        return D_800CC2D0[found].pad139[2] + 1;
    }
    arg0[0] = -99999.0f;
    arg0[1] = -99999.0f;
    arg0[2] = -99999.0f;
    return 0;
}

f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_15038468(f32 *arg0, f32 *arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5) {
    f32 var_fv0;
    f32 factor;

    switch (arg5) {
    case 0:
        *arg0 = func_150484A0(arg2, arg4);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 1:
        *arg0 = func_150484A0(-arg4, arg2);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 2:
        *arg0 = func_150484A0(arg4, -arg2);
        var_fv0 = func_150484A0(arg3, sqrtf((arg2 * arg2) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    case 3:
        *arg0 = func_150484A0(arg4, -arg3);
        var_fv0 = func_150484A0(-arg2, sqrtf((arg3 * arg3) + (arg4 * arg4)));
        *arg1 = var_fv0;
        break;
    }
    factor = 57.295776f;
    *arg0 *= factor;
    *arg1 *= factor;
    var_fv0 = *arg0;
    if (var_fv0 > 180.0f) {
        *arg0 = var_fv0 - 360.0f;
    }
    var_fv0 = *arg1;
    if (var_fv0 > 180.0f) {
        *arg1 = var_fv0 - 360.0f;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_64120/func_15038620.s")

typedef struct {
    struct108 *unk0;
    u8 pad4[0x328];
} GameStruct_800CC5E8;

extern GameStruct_800CC5E8 D_800CC5E8[];

GameStruct_15039A54 *func_15039A54(s32 arg0, s32 arg1) {
    return &D_80098068[arg1];
}

void func_15039A78(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 arg4, f32 arg5, s32 arg6) {
    GameStruct_15039A54 *temp_v0;
    struct108 *temp_v1;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f12_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f12;
    f32 var_f12_2;

    temp_v1 = D_800CC5E8[arg6].unk0;
    temp_v0 = func_15039A54(arg6, 0);
    if ((temp_v1 != NULL) && (temp_v1->unk2C != 0x100)) {
        temp_f12 = temp_v0->unk10;
        temp_f0 = *arg0;
        if (((temp_v0->unk0 + temp_f12) < temp_f0) || (temp_f2 = temp_v0->unk4, (temp_f0 < (temp_f2 - temp_f12)))) {
            *arg0 = arg4;
            *arg1 = arg5;
            if (arg2 != NULL) {
                *arg2 = *arg0;
                *arg3 = *arg1;
            }
            return;
        }

        if (arg2 != NULL) {
            *arg2 = temp_f0;
            *arg3 = *arg1;
            temp_f2 = temp_v0->unk4;
            temp_f0 = *arg0;
        }

        if (temp_f0 < temp_f2) {
            *arg0 = temp_f2;
        } else {
            temp_f2_2 = temp_v0->unk0;
            if (temp_f2_2 < temp_f0) {
                var_f12 = temp_f2_2;
            } else {
                var_f12 = temp_f0;
            }
            *arg0 = var_f12;
        }

        temp_f14 = temp_v0->unk8;
        temp_f12 = temp_v0->unk14;
        temp_f2_3 = *arg1;
        if (((temp_f14 + temp_f12) < temp_f2_3) || (temp_f0_2 = temp_v0->unkC, (temp_f2_3 < (temp_f0_2 - temp_f12)))) {
            *arg1 = arg5;
            return;
        }

        if (temp_f2_3 < temp_f0_2) {
            *arg1 = temp_f0_2;
        } else {
            if (temp_f14 < temp_f2_3) {
                temp_f12 = temp_f14;
            } else {
                temp_f12 = temp_f2_3;
            }
            *arg1 = temp_f12;
        }
        return;
    }

    temp_f0 = *arg0;
    temp_f2 = temp_v0->unk4;
    if (temp_f0 < temp_f2) {
        *arg0 = temp_f2;
    } else {
        temp_f2_2 = temp_v0->unk0;
        if (temp_f2_2 < temp_f0) {
            var_f12 = temp_f2_2;
        } else {
            var_f12 = temp_f0;
        }
        *arg0 = var_f12;
    }

    temp_f2_3 = *arg1;
    temp_f0_2 = temp_v0->unkC;
    if (temp_f2_3 < temp_f0_2) {
        *arg1 = temp_f0_2;
    } else {
        temp_f14 = temp_v0->unk8;
        if (temp_f14 < temp_f2_3) {
            var_f12 = temp_f14;
        } else {
            var_f12 = temp_f2_3;
        }
        *arg1 = var_f12;
    }
}

void func_15039CC8(struct127 *arg0) {
    s32 id;
    s32 i;
    s32 idx;
    s32 count;
    s32 value;
    u16 *table;
    u8 *ptr;
    u8 *write_ptr;

    ptr = *(u8 **)((u8 *)arg0 + 0x324);
    if (ptr != NULL) {
        id = arg0->id;
        if (((id == 0x53) && (arg0->unk84.uh == 0x15)) || (D_800BE9B4 != 0)) {
            for (value = 0x28, i = 0, write_ptr = ptr; i != 3;) {
                i++;
                write_ptr++;
                write_ptr[0x2F] = value;
                write_ptr[0x32] = 0;
                write_ptr[0x35] = 4;
            }
        }

        idx = D_80097E7C[id];
        if (idx != 0) {
            idx--;
            table = D_80084380[idx];
            count = D_80098050[idx];
            for (i = 0; i < count; i++) {
                if (arg0->unk84.uh == table[i]) {
                    arg0->unk2FB |= 4;
                    return;
                }
            }

            if (arg0->interaction_state == 1) {
                if ((arg0->health == 0) ||
                        ((arg0->in_water != 0) && (arg0->xz_velocity < 5.0f)) ||
                        (((u8 *)arg0->unk31C)[0x197] != 0) ||
                        (arg0->xz_velocity > 20.0f)) {
                    arg0->unk2FB |= 4;
                }
            } else if (arg0->health == 0) {
                arg0->unk2FB |= 4;
            }
        }
    }
}

void func_15039ED0(s32 arg0, f32 *arg1, f32 *arg2, f32 *arg3, f32 *arg4, f32 arg5, f32 arg6, s32 arg7) {
    GameStruct_15039A54 *temp_v0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 var_f2;
    f32 var_f2_2;

    if (arg0 == 0) {
        func_15039A78(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        return;
    }

    temp_v0 = func_15039A54(arg7, arg0);
    temp_f12 = temp_v0->unk10;
    temp_f0 = *arg1;
    if (((temp_v0->unk0 + temp_f12) < temp_f0) || (var_f2 = temp_v0->unk4, (temp_f0 < (var_f2 - temp_f12)))) {
        *arg1 = arg5;
        *arg2 = arg6;
        if (arg3 != NULL) {
            *arg3 = *arg1;
            *arg4 = *arg2;
        }
        return;
    }

    if (arg3 != NULL) {
        *arg3 = temp_f0;
        *arg4 = *arg2;
        var_f2 = temp_v0->unk4;
        temp_f0 = *arg1;
    }

    if (temp_f0 < var_f2) {
        *arg1 = var_f2;
    } else {
        temp_f12_2 = temp_v0->unk0;
        if (temp_f12_2 < temp_f0) {
            var_f2 = temp_f12_2;
        } else {
            var_f2 = temp_f0;
        }
        *arg1 = var_f2;
    }

    temp_f12_3 = temp_v0->unk8;
    temp_f14 = temp_v0->unk14;
    temp_f0_2 = *arg2;
    if (((temp_f12_3 + temp_f14) < temp_f0_2) || (var_f2_2 = temp_v0->unkC, (temp_f0_2 < (var_f2_2 - temp_f14)))) {
        *arg2 = arg6;
        return;
    }

    if (temp_f0_2 < var_f2_2) {
        *arg2 = var_f2_2;
    } else {
        if (temp_f12_3 < temp_f0_2) {
            var_f2_2 = temp_f12_3;
        } else {
            var_f2_2 = temp_f0_2;
        }
        *arg2 = var_f2_2;
    }
}

typedef struct {
    s16 unk0;
    u8 pad2[0x24];
    u8 unk26;
    u8 pad27[0x9];
    s8 unk30;
    u8 pad31[0x9];
    u8 unk3A;
} Def_1503A08C;

s32 func_1503D774(s32 arg0, s32 arg1);
void func_1507C3E0(struct127 *arg0, s16 *arg1, s16 *arg2, s16 *arg3);

#define OBJ_S16(o) (*(s16 *)((u8 *)arg0 + (o)))
#define OBJ_S8(o) (*(s8 *)((u8 *)arg0 + (o)))

void func_1503A08C(struct127 *arg0) {
    Def_1503A08C *def;
    f32 (*m)[4];
    f32 cx;
    f32 cy;
    f32 cz;
    f32 mag;
    f32 ax;
    f32 ay;
    f32 az;
    f32 alen;
    f32 bx;
    f32 by;
    f32 bz;
    f32 blen;
    s16 sp36;
    s16 sp34;
    s16 sp32;
    s32 h;
    f32 scale;

    if (arg0->id != 0xFF) {
        if ((arg0->unk1D4 != NULL) && (D_800D1C90[arg0->id] != NULL)) {
            if (func_1503D774(arg0->id, 0) == 0) {
                def = (Def_1503A08C *)D_800D1C90[arg0->id];
                m = (f32 (*)[4])arg0->unk1D4;
                m = (f32 (*)[4])((u8 *)m + (def->unk26 << 6));
                scale = arg0->y_scale;
                alen = def->unk3A * scale;
                if (alen != 0) {
                    cx = m[0][0];
                    cy = m[0][1];
                    cz = m[0][2];
                    blen = cx * cx + cy * cy + cz * cz;
                    mag = blen;
                    if (blen != 0) {
                        mag = sqrtf(blen);
                    }
                    if (mag != 0) {
                        mag = alen / mag;
                    }
                    ax = cx * mag;
                    ay = cy * mag;
                    az = cz * mag;
                } else {
                    az = 0.0f;
                    ay = 0.0f;
                    ax = 0.0f;
                }
                blen = def->unk0 * scale;
                if (blen != 0) {
                    cx = m[1][0];
                    cy = m[1][1];
                    cz = m[1][2];
                    mag = sqrtf(cx * cx + cy * cy + cz * cz);
                    if (mag != 0) {
                        mag = blen / mag;
                    }
                    bx = cx * mag;
                    by = cy * mag;
                    bz = cz * mag;
                } else {
                    bx = by = bz = 0.0f;
                }
                cx = m[2][0];
                cy = m[2][1];
                cz = m[2][2];
                blen = def->unk30 * scale;
                if (blen != 0) {
                    mag = sqrtf(cx * cx + cy * cy + cz * cz);
                    if (mag != 0) {
                        mag = blen / mag;
                    }
                    cx *= mag;
                    cy *= mag;
                    cz *= mag;
                    mag = 127.0f / blen;
                    OBJ_S8(0x1B8) = cx * mag;
                    OBJ_S8(0x1B9) = cy * mag;
                    OBJ_S8(0x1BA) = cz * mag;
                } else {
                    mag = sqrtf(cx * cx + cy * cy + cz * cz);
                    if (mag != 0) {
                        mag = 127.0f / mag;
                    }
                    OBJ_S8(0x1B8) = cx * mag;
                    OBJ_S8(0x1B9) = cy * mag;
                    OBJ_S8(0x1BA) = cz * mag;
                    cx = cy = cz = 0.0f;
                }
                OBJ_S16(0x1A4) = m[3][0] + cx;
                OBJ_S16(0x1A6) = m[3][1] + cy;
                OBJ_S16(0x1A8) = m[3][2] + cz;
                OBJ_S16(0x1AA) = m[3][1] + by;
                mag = OBJ_S16(0x1A4);
                OBJ_S16(0x1AC) = mag + ax + bx;
                OBJ_S16(0x1AE) = OBJ_S16(0x1A6) + ay + by;
                OBJ_S16(0x1B0) = OBJ_S16(0x1A8) + az + bz;
                OBJ_S16(0x1B2) = mag - ax + bx;
                OBJ_S16(0x1B4) = OBJ_S16(0x1A6) - ay + by;
                OBJ_S16(0x1B6) = OBJ_S16(0x1A8) - az + bz;
            }
        } else {
            def = (Def_1503A08C *)D_800D1C90[arg0->id];
            if (def != NULL) {
                h = def->unk0;
            } else {
                h = 0;
            }
            func_1507C3E0(arg0, &sp36, &sp34, &sp32);
            OBJ_S16(0x1A4) = arg0->x_position;
            OBJ_S16(0x1A6) = arg0->y_position + (sp36 * 7) / 8;
            OBJ_S16(0x1A8) = arg0->z_position;
            OBJ_S16(0x1AA) = OBJ_S16(0x1A6) + h * arg0->y_scale;
            OBJ_S16(0x1AC) = OBJ_S16(0x1A4);
            OBJ_S16(0x1B2) = OBJ_S16(0x1A4);
            OBJ_S16(0x1AE) = OBJ_S16(0x1AA);
            OBJ_S16(0x1B4) = OBJ_S16(0x1AA);
            OBJ_S16(0x1B0) = OBJ_S16(0x1A8);
            OBJ_S16(0x1B6) = OBJ_S16(0x1A8);
            arg0->unk2CA = 0;
        }
    }
}

typedef struct {
    u8  pad0[0x30];
    f32 unk30;
    f32 unk34;
    f32 unk38;
} GameStruct_1503A60C;

void func_1503A60C(void) {
    GameStruct_1503A60C *dst = (GameStruct_1503A60C *)((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + 0x40);

    dst->unk30 = *(f32 *)&D_800CC2D0[D_800C3E78].pad174;
    dst->unk34 = D_800CC2D0[D_800C3E78].y_position;
    dst->unk38 = *(f32 *)&D_800CC2D0[D_800C3E78].pad178;
}

extern u8 D_800C3FFA;
extern u8 D_80084404[];
void func_150A7A48(f32 a[4][4], f32 b[4][4], f32 c[4][4]);

void func_1503A678(void) {
    struct127 *obj;
    f32 (*m)[4];
    f32 sz;
    f32 sy;
    f32 sx;
    f32 f;
    f32 inv;
    f32 mtx[4][4];
    f32 tmp[4][4];
    f32 pos[3];

    obj = &D_800CC2D0[D_800C3E78];
    if (obj->id == 0x22) {
        f = *((u8 *)&D_800CC2D0[D_800C3E78] + 0x20A);
        f /= 64.0f;
        f += 1.0f;
        if (D_800C3FFA == 0) {
            sx = f;
            sy = 1.0f;
            sz = f;
        } else {
            sx = 1.0f / f;
            sy = 1.0f;
            sz = 1.0f / f;
        }
        m = (f32 (*)[4])((u8 *)D_800CC2D0[D_800C3E78].unk1D4 + (D_80084404[D_800C3FFA] << 6));
        m[3][3] = 1.0f;
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        pos[0] = m[3][0];
        pos[1] = m[3][1];
        pos[2] = m[3][2];
        guMtxIdentF(mtx);
        func_150A7CB0(mtx, *(s32 *)&sx, *(s32 *)&sy, *(s32 *)&sz);
        func_150A7A48(m, mtx, tmp);
        bcopy(tmp, m, 0x40);
        m[3][0] = pos[0];
        m[3][1] = pos[1];
        m[3][2] = pos[2];
        D_800C3FFA++;
    }
}

extern u8 D_800C3FFA;
extern void func_15036F34(void);
extern void func_1503A678(void);

void func_1503A7F0(void) {
    s32 temp = D_800C3FFA;
    D_800C3FFA = 0;
    func_15036F34();
    D_800C3FFA = temp;
    func_1503A678();
}
f32 func_15048A40(u8 arg0);

#define VEC0 D_800C3FE8[0]
#define VEC1 D_800C3FE8[1]
#define VEC2 D_800C3FE8[2]
extern u8 D_800C3FDC;
extern f32 D_800C3FE0;
extern f32 D_800C3FE4;
f32 func_150489B0(u8);
s32 func_15022024(f32 *, f32 *, s32, s32);

typedef struct {
    u8 pad0[0x2C];
    s8 unk2C;
    s8 unk2D;
    s8 unk2E;
    s8 unk2F;
} Def_1503A830;

#define A_U8(o) (*(u8 *)((u8 *)arg0 + (o)))
#define A_S8(o) (*(s8 *)((u8 *)arg0 + (o)))
#define A_U16(o) (*(u16 *)((u8 *)arg0 + (o)))
#define A_S16(o) (*(s16 *)((u8 *)arg0 + (o)))
#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define APPROACH(fld, tgt)                                       \
    if (ABS((tgt) - (fld)) < step) {                             \
        (fld) = (tgt);                                           \
    } else {                                                     \
        (fld) += step * (((tgt) - (fld)) < 0 ? -1 : 1);         \
    }

void func_1503A830(struct127 *arg0) {
    s32 id;
    s32 type;
    s32 color;
    s32 doCall;
    f32 (*m)[4];
    f32 d[3];
    f32 a[2];
    f32 b[2];
    f32 fA;
    f32 fB;
    f32 fC;
    f32 fD;
    f32 padB4;
    f32 padB0;
    f32 vx;
    f32 vy;
    f32 vz;
    s32 step;
    u8 *p;
    u8 *q;
    s32 copyAB;
    s32 clamp;
    s32 snap;
    s32 n;
    s32 doAim;
    s32 noFx;
    Def_1503A830 *def;

    if (arg0->unk1D4 == NULL) {
        return;
    }
    VEC0 = D_800C3FD0[0];
    VEC1 = D_800C3FD0[1];
    VEC2 = D_800C3FD0[2];
    id = arg0->id;
    step = 8;
    color = 0x100;
    doCall = 1;
    copyAB = 1;
    clamp = 1;
    snap = 0;
    doAim = 1;
    noFx = 1;
    switch (id) {
        case 0x9A:
            type = 0xD;
            break;
        case 0x92:
            type = 3;
            break;
        case 0x52:
            type = 0;
            break;
        case 21:
            type = 3;
            break;
        case 0x54:
            type = 0x1D;
            break;
        case 0x45:
            type = 3;
            break;
        case 24:
        case 30:
            type = 2;
            if (id == 30) {
                color = 0x22;
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 0x82:
        case 0x96:
            type = 0xC;
            doCall = 0;
            break;
        default:
            return;
    }
    if (doCall) {
        func_150379DC(D_800C3FE8, type, type, arg0 - D_800CC2D0, 90.0f, color, 0);
    }
    m = (f32 (*)[4])((u8 *)D_800CC2D0[arg0 - D_800CC2D0].unk1D4 + (type << 6));
    m[3][3] = 1.0f;
    m[2][3] = 0.0f;
    m[1][3] = 0.0f;
    m[0][3] = 0.0f;
    a[0] = b[0] = 0.0f;
    if (A_U16(0x282) == 0) {
        A_U16(0x278) = 0;
        A_S8(0x284) = 0;
        A_S8(0x285) = 0;
        A_S8(0x286) = 0;
        A_S8(0x287) = 0;
    }
    if (A_U16(0x282) != 0) {
        doAim = 0;
        if (A_U8(0x276) == 0) {
            s32 lo;
            s32 hi;
            f32 s;
            f32 c;
            s32 pad68;
            f32 scale;
            f32 k;
            A_S16(0x27A) -= A_S8(0x284);
            A_S16(0x27E) -= A_S8(0x285);
            A_S16(0x27C) -= A_S8(0x286);
            A_S16(0x280) -= A_S8(0x287);
            lo = A_U16(0x278) & 0xFF;
            hi = A_U16(0x278) >> 8;
            lo += D_800BE9E4 * 3;
            hi -= D_800BE9E4 * 4;
            A_U16(0x278) = (hi << 8) | lo;
            if (A_U16(0x282) < 30) {
                scale = (f32)(A_U16(0x282) + 0U) / 30.0f;
            } else {
                scale = 1.0f;
            }
            s = func_15048A40(lo);
            c = func_150489B0(lo);
            k = 11.0f * scale;
            A_S8(0x284) = s * k;
            A_S8(0x285) = c * k;
            s = func_15048A40(hi);
            c = func_150489B0(hi);
            A_S8(0x286) = s * k;
            A_S8(0x287) = c * k;
        } else if (A_U8(0x276) == 1) {
            if (A_U16(0x278) == 0) {
                A_U16(0x278) = 1;
                A_S8(0x287) += func_150ADA20() % 20U;
            }
            if (A_U16(0x282) < 0xAE) {
                n = A_U16(0x282) / 6 + 1;
            } else {
                n = 30;
            }
            A_S8(0x284) = 0;
            A_S8(0x285) += n;
            A_S8(0x287) += n;
            A_S8(0x286) = A_S8(0x284);
            snap = 1;
            a[0] = A_S8(0x284);
            b[0] = A_S8(0x285);
            a[1] = A_S8(0x286);
            b[1] = A_S8(0x287);
        } else if (A_U8(0x276) == 2) {
            u8 *r;

            doAim = 1;
            p = (u8 *)func_1503195C(arg0, 0x2B, 0);
            if (p != NULL) {
                q = *(u8 **)(p + 0x48);
                if (q != NULL) {
                    r = ((u8 **)(q + 0x3E8))[D_800BE9C0];
                    if (r != NULL) {
                        r += 0x80;
                        func_15043FF0((vertex *)D_800C3FE8, (struct113 *)r);
                        noFx = 0;
                    }
                }
            }
        } else if (A_U8(0x276) == 3) {
            f32 (*mtx)[4];

            if (A_U16(0x278) != -1) {
                mtx = (f32 (*)[4])arg0->unk1D4;
                if (mtx != NULL) {
                    mtx = ((f32 (*)[4][4])mtx)[A_U16(0x278)];
                    VEC0 = mtx[3][0];
                    VEC1 = mtx[3][1];
                    VEC2 = mtx[3][2];
                }
            }
        } else if (A_U8(0x276) == 4) {
            if (arg0->unk84.uh == 0x26) {
                if (A_U16(0x278) == 0) {
                    A_S8(0x284) -= D_800BE9E4;
                    if (A_S8(0x284) < -20) {
                        A_S8(0x284) = -20;
                        A_U16(0x278) = 1;
                    }
                } else {
                    A_S8(0x284) += D_800BE9E4 * 3;
                    if (A_S8(0x284) > 20) {
                        A_S8(0x284) = 20;
                        A_U16(0x278) = 0;
                    }
                }
                A_S8(0x287) = 0;
                A_S8(0x286) = A_S8(0x284);
                A_S8(0x285) = A_S8(0x287);
                snap = 1;
            } else {
                A_U16(0x282) = 0;
                doAim = 1;
            }
        } else if (A_U8(0x276) == 5) {
            f32 kx = 0.3543307185f;
            f32 ky = 0.236220479f;

            a[0] = A_S8(0x284) * kx;
            a[1] = A_S8(0x286) * kx;
            b[0] = A_S8(0x285) * ky;
            b[1] = A_S8(0x287) * ky;
            clamp = 0;
            copyAB = 0;
        } else if (A_U8(0x276) == 6) {
            if (arg0->unk31C->unk78 == 0x2A) {
                b[0] = 30.0f;
                a[0] = -6.0f;
                a[1] = 6.0f;
            } else {
                a[0] = 0;
                a[1] = 0;
                b[0] = 0;
                if (A_S16(0x27E) == 0) {
                    A_U8(0x276) = 0;
                }
            }
            copyAB = 0;
            b[1] = b[0];
        }
        if (A_U16(0x282) != 0xFFFF) {
            if (A_U16(0x282) >= D_800BE9E4) {
                A_U16(0x282) -= D_800BE9E4;
            } else {
                A_U16(0x282) = 0;
            }
        }
    }
    if (doAim) {
        if (!(A_U8(0x2FB) & 4) || !noFx) {
            if (VEC0 != -99999.0f || VEC1 != -99999.0f || VEC2 != -99999.0f) {
            if (D_800C3FDC != 0) {
                D_800C3FDC = 0;
                a[0] = D_800C3FE0;
                b[0] = D_800C3FE4;
            } else {
                d[0] = VEC0 - m[3][0];
                d[1] = VEC1 - m[3][1];
                d[2] = VEC2 - m[3][2];
                vx = d[0] * m[0][0] + d[1] * m[0][1] + d[2] * m[0][2];
                vy = d[0] * m[1][0] + d[1] * m[1][1] + d[2] * m[1][2];
                vz = d[0] * m[2][0] + d[1] * m[2][1] + d[2] * m[2][2];
                a[0] = func_150484A0(vx, vz);
                b[0] = func_150484A0(vy, sqrtf(vx * vx + vz * vz));
                a[0] *= 57.29577637f;
                b[0] *= 57.29577637f;
                if (a[0] > 180.0f) {
                    a[0] -= 360.0f;
                }
                if (b[0] > 180.0f) {
                    b[0] -= 360.0f;
                }
            }
            } else {
                step = 4;
            }
        } else {
            a[0] = b[0] = 0.0f;
        }
    }
    if (A_U16(0x282) == 0 && func_15022024(a, b, arg0 - D_800CC2D0, 0)) {
        clamp = 1;
        copyAB = 0;
        snap = 0;
        step = 8;
    }
    if (clamp) {
        a[0] = CLAMP(a[0], -45.0f, 45.0f);
        b[0] = CLAMP(b[0], -15.0f, 30.0f);
        a[1] = CLAMP(a[1], -45.0f, 45.0f);
        b[1] = CLAMP(b[1], -15.0f, 30.0f);
    }
    if (copyAB) {
        a[1] = a[0];
        b[1] = b[0];
    }
    if (D_800D1C90[id] == NULL) {
        return;
    }
    fA = (a[0] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2D * a[0] / 45.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2C * a[0] / -45.0f;
    fB = (a[1] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2C * a[1] / 45.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2D * a[1] / -45.0f;
    fB = -fB;
    fC = (b[0] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2E * b[0] / 30.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2F * b[0] / -15.0f;
    fD = (b[1] >= 0.0f) ? ((Def_1503A830 *)D_800D1C90[id])->unk2E * b[1] / 30.0f : ((Def_1503A830 *)D_800D1C90[id])->unk2F * b[1] / -15.0f;
    APPROACH(A_S16(0x27A), (s32)fB);
    APPROACH(A_S16(0x27C), (s32)fA);
    APPROACH(A_S16(0x27E), (s32)fD);
    APPROACH(A_S16(0x280), (s32)fC);
    if (snap) {
        A_S16(0x27A) = A_S8(0x284);
        A_S16(0x27E) = A_S8(0x285);
        A_S16(0x27C) = A_S8(0x286);
        A_S16(0x280) = A_S8(0x287);
    }
    if (A_U16(0x282) != 0 && A_U8(0x276) == 0) {
        A_S16(0x27A) += A_S8(0x284);
        A_S16(0x27E) += A_S8(0x285);
        A_S16(0x27C) += A_S8(0x286);
        A_S16(0x280) += A_S8(0x287);
    }
}

void func_1503B708(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4,
                   s32 arg5, f32 *arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    f32 *scratch;
    f32 value;

    scratch = D_800C3FD0;
    value = -99999.0f;
    *(f32 *)((u8 *)arg0 + 0) = value;
    *(f32 *)((u8 *)arg0 + 4) = value;
    *(f32 *)((u8 *)arg0 + 8) = value;
    scratch[0] = value;
    scratch[1] = value;
    scratch[2] = value;
    if (func_15037880(arg3, arg0) == 0) {
        func_150380C0(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                      arg8, arg9, arg10);
    }
}
