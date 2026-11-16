#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u16 D_800C4ED0[];
typedef struct {
    u8 unk0;
    u8 pad1[0xF];
} D_800C666FEntry;

typedef struct {
    u8 pad_0[0x4];
    u8 field_0x4;
    u8 pad_0x5[0x9B];
    u32 field_0xA0;
    u8 field_0xA4;
    u8 field_0xA5;
    u8 field_0xA6;
    u8 field_0xA7;
    u8 pad_0xA8[0x50];
    u32 field_0xF8;
    u8 pad_0xFC[0xD8];
    void *field_0x1D4;
    void *field_0x1D8;
    u8 pad_0x1DC[0x88];
    void *field_0x264;
} ActorInteractionFields;

extern D_800C666FEntry D_800C666F[];
extern void *allocate_memory(s32, s32, s32, s32);
extern void func_1502C608(s32);
extern void func_1502DF38(s32, s32);
extern void func_1502E4C4(s32);
extern void func_1502EAFC(struct127 *);
extern void func_1502EC34(void *, s32 *, s32 *, s32 *, s32 *);
extern void func_1502EEF4(s32);
extern void func_1502F264(s32);
extern void func_1502FBE8(struct127 *);
extern void func_150345E4(s32);
extern void func_1503A08C(struct127 *);
extern void func_1503A830(struct127 *);
extern void func_1503DF48(s32);
extern void func_150A4B04(struct127 *);
extern void func_1517AD00(s32, s32, s32);

typedef struct { u32 first, second; } Game58F80Command;
typedef struct {
    s32 active;
    u8 model, kind;
    u8 pad6[0x17E];
    u32 flags;
    u8 pad188[0x1A4];
} Game58F80DrawActor;
extern u8 D_80084160[];
extern u8 D_80084190[];
extern s32 func_1506196C(struct127 *, s32);
void *func_1502C408(void *, s32);
s32 func_1502C974(s32, s32, s32, s32, s32);
void *func_150368C4(s32, s32, s32);
s32 func_15030E08(s32, s32, s32);

Game58F80Command *func_1502BAD0(Game58F80Command *commands, s32 mode, s16 arg2) {
    Game58F80DrawActor *actor;
    s32 index;
    s32 active;

    { Game58F80Command *opening = commands++; opening->first = 0xDE000000; opening->second = (u32)D_80084160; }
    actor = (Game58F80DrawActor *)&D_800CC2D0;
    for (index = 0; index != 25; index++, actor++) {
        D_8003C8E0 = (index & 0xFFFFFF) | 0x01000000;
        active = actor->active;
        if (active == 0 || actor->kind == 3 || actor->kind == 5) continue;
        if (actor->kind == 2) {
            if (mode != 2) continue;
        } else if (actor->model == 0xFF) continue;
        if (mode == 6) {
            if (actor->kind != 7) continue;
        } else if (mode == 0) {
            if (!((actor->flags >> 9) & 1)) continue;
        } else if (mode == 1) {
            if (actor->kind == 7 || actor->kind == 1) continue;
            if (actor->kind == 0 && func_1506196C((struct127 *)actor, arg2) < 0xFF) continue;
        } else if (mode == 2 && actor->kind != 2) {
            if (actor->kind == 7 || (actor->kind != 0 && actor->kind != 1)) continue;
            if (actor->kind == 0 && func_1506196C((struct127 *)actor, arg2) == 0xFF) continue;
        }
        if (actor->kind == 2) {
            commands = func_1502C408(commands, index);
        } else {
            commands = (Game58F80Command *)func_1502C974((s32)commands, index, arg2, mode, 0);
            if (index == 0) commands = func_150368C4((s32)commands, index, arg2);
        }
    }
    D_8003C8E0 = 0x01FFFFFF;
    if (mode == 1) commands = (Game58F80Command *)func_15030E08((s32)commands, arg2, 0);
    else if (mode == 2) commands = (Game58F80Command *)func_15030E08((s32)commands, arg2, 1);
    else if (mode == 6) commands = (Game58F80Command *)func_15030E08((s32)commands, arg2, 2);
    { Game58F80Command *closing = commands++; closing->first = 0xDE000000; closing->second = (u32)D_80084190; }
    D_8003C8E0 = 0;
    return commands;
}

void func_1502BD84(struct127 *arg0, s32 arg1) {
    arg0->unk1D4 = NULL;
    if (arg0->unk5 == 5) {
        func_1502DF38(arg1, 1);
        return;
    }
    if (arg0->id == 0xFF) {
        return;
    }
    if (arg0->unk5 == 3) {
        return;
    }
    if (arg0->unk5 == 2) {
        func_1502C608(arg1);
        return;
    }
    if (arg0->unk1C9 != 0) {
        func_1502FBE8(arg0);
    }
    func_1502E4C4(arg1);
    func_1502DF38(arg1, 0);
    func_1503A08C(arg0);
    if (arg0->unk1D4 == NULL) {
        arg0->unk1FC = 2;
    } else {
        func_150345E4(arg1);
        func_1503A830(arg0);
    }
    if (D_800C666F[arg1].unk0 != 0) {
        func_1503DF48(arg1);
    }
    if (arg0->interaction_state != 0) {
        func_1502EEF4(arg1);
        func_1502F264(arg1);
        if (*(u8 *)((u8 *)arg0 + 0xA4) != 0) {
            func_1502EAFC(arg0);
        }
        if ((arg0->unkF8 & 0x4000) != 0) {
            func_150A4B04(arg0);
        }
        if (*(s32 *)((u8 *)arg0 + 0x260) != 0) {
            func_1517AD00(*(u8 *)((u8 *)arg0 + 0x134), *(u8 *)((u8 *)arg0 + 0x135), arg1);
        }
    }
}

extern void func_1503F964(void);
extern void func_1502F3C8(void);
extern void func_15030468(void);
extern void func_1507C22C(s32);
extern void func_1502F948(ActorInteractionFields *);
extern u8 D_800C3E90;
extern s32 D_800C3E74;
extern u8 D_800C3E70;
extern s32 D_800D121C;

void func_1502BEE4(void) {
    struct127 *obj;
    struct127 *p;
    s32 i;
    s32 j;
    s32 state;
    u8 order[25];
    u8 depth[25];
    s32 n;
    s32 max;

    func_1503F964();
    max = 0;
    D_800C3E90 = 0;
    D_800C3E74 = 0;
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].unk274 != 0) {
            D_800C3E74 |= 1 << (D_800CC2D0[i].unk274 - 1);
        }
    }
    bzero(depth, 25);
    for (i = 0; i < 25; i++) {
        obj = &D_800CC2D0[i];
        state = obj->interaction_state;
        if (state != 0) {
            if (obj->unk65 != 0) {
                p = obj;
                depth[i] = 0;
                while (p->unk65 != 0) {
                    p = &D_800CC2D0[p->unk65 - 1];
                    depth[i]++;
                }
                if (max < depth[i]) {
                    max = depth[i];
                }
            } else {
                func_1502BD84(obj, i);
            }
        }
    }
    n = 0;
    if (max != 0) {
        for (i = 1; i <= max; i++) {
            for (j = 0; j < 25; j++) {
                if (i == depth[j]) {
                    order[n++] = j;
                }
            }
        }
        for (i = 0; i < n; i++) {
            func_1502BD84(&D_800CC2D0[order[i]], order[i]);
        }
    }
    func_1502F3C8();
    for (i = 0; i < 25; i++) {
        if (D_800CC2D0[i].interaction_state != 0) {
            func_1502F948((ActorInteractionFields *)&D_800CC2D0[i]);
        }
    }
    func_15030468();
    if (D_800BEAC0 == 0) {
        func_1507C22C(0);
    }
    D_800C3E70 = 0;
}

void func_1516972C(struct102 *arg0);
extern s32 D_80082FA0;
extern u8 D_800C3638;
extern struct127 D_800CC2D0[26];
extern s32 D_800D121C;
extern s32 func_150229E4(struct127 *);
extern s32 func_1506196C(struct127 *, s32);
extern struct102 *func_1510D970(s32, struct127 *, s32, s32, s32);
extern u8 D_800C3656;
void func_1502C1A4(void)
{
  struct127 *obj;
  s32 found;
  s32 i;
  s32 arg2;
  s32 arg4;
  s32 id;
  s32 state;
  struct102 *temp;
 obj = D_800CC2D0; do { state = obj->interaction_state; if (state != 0) { id = obj->id; found = 0; if ((((((((obj->unk5 != 3) && (id != 0xFF)) && (obj->unk123 == 0)) && ((obj->pad66 & 0x10) != 0x10)) && ((*((u16 *) (((u8 *) obj) + 0x160))) != 0)) && (obj->unk65 == 0)) && (((D_800C3638 == 0) || (D_800C3656 != 0)) || (func_150229E4(obj) != 0))) && ((obj->unk88 == 0) || (obj->unkCC >= (-0x96)))) { i = 0; if (D_80082FA0 >= 0) { do {
            if (func_1506196C(obj, i) != 0)
            {
              found = 1;
            }
            i++;
          }
          while (D_80082FA0 >= i);
        }
      }
      if (found != 0)
      {
        if ((*((struct102 **) (((u8 *) obj) + 0x2D4))) == 0)
        {
          arg2 = 0;
          if (obj->id == 0x4D)
          {
            arg2 = 2;
            arg4 = 1;
          }
          else
          {
            arg4 = 0;
          }
          *((struct102 **) (((u8 *) obj) + 0x2D4)) = func_1510D970(0, obj, arg2, 0, arg4);
          alloc_done:
          ;

          ;
          ;
        }
      }
      else
      {
        temp = *((struct102 **) (((u8 *) obj) + 0x2D4));
        if (temp != 0)
        {
          func_1516972C(temp);
          *((struct102 **) (((u8 *) obj) + 0x2D4)) = 0;
        }
      }
    }
    obj++;
  }
  while (obj != ((struct127 *) (&D_800D121C)));
}


extern s32 D_800C3E88;
extern s32 D_800C3E8C;
extern u16 D_800C3E7A;

void func_1502C380(void) {
    D_800C3E8C = D_800C3E88 = (&D_800C3E80)[D_800BE9C0];
    D_800C3E7A = 0;
}

extern u8 D_800CC406[];

s32 func_1502C3BC(s32 arg0) {
    s32 temp = D_800CC406[arg0 * 0x32C];

    if (temp >= 0x46) {
        temp = 0xB;
    }
    return temp;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_58F80/func_1502C408.s")

typedef struct {
    u8 pad0[0x7];
    u8 unk7;
    u8 pad8[0x58];
    s16 unk60;
    s16 unk62;
} Actor1502C608;

extern u8 *D_8008CA4C[];

#define ACTOR_1502C608 ((Actor1502C608 *)&D_800CC2D0[arg0])

void func_1502C608(s32 arg0) {
    s32 period;
    s16 pos;
    s16 speed;

    if (D_800BEAC0 == 0) {
        pos = ACTOR_1502C608->unk60;
        speed = ACTOR_1502C608->unk62;
        if (ACTOR_1502C608->unk7 != 0) {
            period = D_8008CA4C[func_1502C3BC(arg0)][4] << 8;
            pos += speed;
            if (pos >= period) {
                pos -= period;
            } else if (pos < 0) {
                pos += period;
            }
            ACTOR_1502C608->unk60 = pos;
        }
    }
}
#undef ACTOR_1502C608


extern f32 D_80096DE0;
extern f32 D_80096DE4;
extern s32 func_150849A0(struct127 *);
extern void func_150837D4(s32, s32, s32);

void func_1502C6E8(s32 arg0, s16 arg1, s32 arg2) {
    struct108 *cam;
    struct127 *obj;
    s32 mode;
    s32 lod;
    s32 maxLod;
    s32 model;
    f32 dists[4];
    f32 diff;
    f32 distSq;

    cam = &D_800DBFF0[arg1];
    obj = &D_800CC2D0[arg0];
    if (obj->unk1C9 != 0) {
        return;
    }
    if (obj->unk5 == 7) {
        return;
    }
    dists[0] = 500.0f;
    dists[1] = D_80096DE0;
    dists[2] = D_80096DE4;
    dists[3] = 2000.0f;
    maxLod = ((u8 *)obj)[0x2C8] - 1;
    if (maxLod == -1) {
        return;
    }
    mode = func_150849A0(obj);
    diff = cam->unk2F8 - obj->x_position;
    distSq = diff * diff;
    diff = cam->unk2FC - obj->y_position;
    diff = diff * diff;
    distSq += diff;
    diff = cam->unk300 - obj->z_position;
    diff = diff * diff;
    distSq += diff;
    if (distSq < (dists[0] * dists[0])) {
        lod = 0;
    } else if (distSq < (dists[1] * dists[1])) {
        lod = 1;
    } else if (distSq < (dists[2] * dists[2])) {
        lod = 2;
    } else if (distSq < (dists[3] * dists[3])) {
        lod = 3;
    } else {
        lod = 4;
    }
    if ((lod >= 2) && (obj->xz_velocity < 3.0f)) {
        lod--;
    }
    if (mode) {
        if (mode == 0x5A) {
            if (lod == 0) {
                lod = 1;
            }
        }
    } else if (D_800BE616 != 0) {
        lod = 1;
    }
    if (D_800C35EA == 1) {
        lod = 0;
    }
    if (maxLod < lod) {
        lod = maxLod;
        if (lod < 0) {
            lod = 0;
        }
    }
    if (lod == -1) {
        return;
    }
    if (lod == ((u8 *)obj)[0x1C8]) {
        return;
    }
    model = (*(u8 **)((u8 *)obj + 0x2C4))[lod];
    func_150837D4(arg0, model, 0);
    ((u8 *)obj)[0x1C8] = lod;
}

extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;
extern void *D_800DF7C0;
extern u8 D_800DF7C4;
extern s32 func_150849CC(struct127 *, s32 *);
extern void func_1502D630(struct127 *, s32 *, s32);
extern Gfx *func_1502CCFC(Gfx *, s32, s32, void *, s32, s32 *, s32, s32);
void func_1502D54C(s32, s32 *);

s32 func_1502C974(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 lod;
    s32 alpha;
    s32 colours[4];
    s32 cond;
    void *model;
    s32 orig;
    struct127 *obj;

    orig = arg0;
    if ((D_800C3638 != 0) && (D_800C3656 == 0) && (func_150229E4(&D_800CC2D0[arg1]) == 0)) {
        return arg0;
    }
    obj = &D_800CC2D0[arg1];
    if ((obj->unk74 & (1 << arg2)) == (1 << arg2)) {
        return arg0;
    }
    if (func_1506196C(obj, arg2) == 0) {
        return arg0;
    }
    if (obj->unk1D4 == NULL) {
        return arg0;
    }
    if ((arg3 != 4) && (arg3 != 5) && (arg3 != 3)) {
        func_1502C6E8(arg1, arg2, arg3);
        lod = ((u8 *)obj)[0x1C8];
    } else {
        func_150849CC(obj, &lod);
    }
    func_1502D54C(arg1, colours);
    if (*(s16 *)((u8 *)D_800B0DF0 + 0x3E) != 0) {
        func_1502D630(obj, colours, arg2);
    } else {
        colours[3] = 0xFF;
    }
    alpha = func_1506196C(obj, arg2);
    if (alpha < 0xFF) {
        if ((obj->camera != NULL) && (obj->camera->unk2C == 0x100) && (arg2 != obj->unk127)) {
            alpha = 0xFF;
        }
    }
    if (arg3 == 4) {
        alpha = (D_800DF7C4 * alpha) >> 8;
        model = D_800DF7C0;
    } else {
        model = obj->unk1D4;
    }
    arg0 = (s32)func_1502CCFC((Gfx *)arg0, arg1, arg2, model, alpha, colours, arg3, arg4);
    if (((arg0 - D_800BE9C8[D_800BE9C0]) >> 3) > D_800BEBA4) {
        cond = 1;
    } else {
        cond = 0;
    }
    if (cond) {
        return orig;
    }
    return arg0;
}

void func_1502CC34(void *arg0, s32 arg1, s32 arg2, void *arg3, s32 *arg4, s32 *arg5, s32 *arg6, s32 *arg7, s32 *arg8, s32 *arg9, s32 *argA, s32 *argB, s32 *argC, s32 *argD) {
    u8 *temp_v0;
    u8 *temp_v1;

    temp_v0 = D_800D9B68[arg1];
    *arg4 = temp_v0[0];
    temp_v1 = D_800D9B78[arg1];
    *arg5 = temp_v0[1];
    *arg6 = temp_v0[2];
    *arg7 = temp_v1[0];
    *arg8 = temp_v1[1];
    *arg9 = temp_v1[2];
    *argA = 0;
    *argB = 0;
    *argC = 0;
    *argD = *(s32 *)((u8 *)arg3 + 0xC);
    if (((u8 *)arg0)[0xA4] != 0) {
        func_1502EC34(arg0, argA, argB, argC, argD);
    }
}

extern s32 func_1503DA9C(struct127 *, s32, s32, s32);
Gfx *func_1502F01C(Gfx *, s32);
extern Gfx *func_1502F9FC(Gfx *, s32);
extern void func_1502FD70(struct127 *);
extern Gfx *func_15030F94(Gfx *, struct127 *, s32, s32);
extern Gfx *func_1515D914(Gfx *, s32, s32, s32, s32, s32, s32, s32, s32, u8 *, s32, u8 *, s32, void *);
extern void func_151EFE88(void *);
extern u16 D_800C4778[];
extern Gfx **D_800C48F0[];
extern Gfx **D_800C4488[];
extern u8 D_800DD2E4[];
extern u8 D_800D9900;
extern u8 D_800D9E28[];
extern u8 D_800C3E98[];
extern u8 D_C3E98[];
extern u8 D_80084160[];
extern u8 D_800832C0[];
extern u8 D_80082FC0[];
extern u8 D_80083140[];
extern u8 D_80084190[];

Gfx *func_1502CCFC(Gfx *gdl, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 *arg5, s32 arg6, s32 arg7) {
    struct127 *obj;
    s32 i;
    s32 sp144;
    s32 sp140;
    s32 sp13C;
    s32 idx;
    s32 t;
    s32 sp130;
    s32 flag;
    s32 sp128;
    s32 sp124;
    s32 sp120;
    s32 sp11C;
    s32 sp118;
    s32 sp114;
    s32 sp110;
    s32 sp10C;
    s32 sp108;
    s32 sp104;
    s32 n;

    obj = &D_800CC2D0[arg1];
    sp140 = obj->id;
    if ((arg6 == 3) || (arg6 == 4) || (arg6 == 5)) {
        sp140 = func_150849CC(obj, &sp144);
    } else {
        sp144 = ((u8 *)obj)[0x1C8];
    }
    if (func_1503DA9C(obj, sp140, sp144, (arg6 == 3) ? 0 : 1) != 0) {
        return gdl;
    }
    idx = D_800BE9C0;
    t = idx * 4;
    if (*(s32 *)((u8 *)obj + sp144 * 8 + t + 0x28C) == 0) {
        return gdl;
    }
    if (arg7 != 0) {
        gSPDisplayList(gdl++, (u32)D_80084160);
    } else {
        gDPPipeSync(gdl++);
    }
    gdl = func_1502F01C(gdl, arg1);
    gdl = func_1502F9FC(gdl, arg1);
    gSPSegment(gdl++, 3, (u32)arg3);
    gSPSegment(gdl++, 1, *(s32 *)((u8 *)obj + sp144 * 8 + t + 0x28C) - 0x38);
    func_1502CC34(obj, arg2, arg4, arg5, &sp128, &sp124, &sp120, &sp11C, &sp118, &sp114, &sp110, &sp10C, &sp108, &sp104);
    if (arg6 == 5) {
        gSPSegment(gdl++, 8, (u32)D_800832C0);
    } else if (arg6 == 3) {
        gSPSegment(gdl++, 8, (u32)D_800832C0);
    } else if (arg4 < 0xFF) {
        gSPSegment(gdl++, 8, (u32)D_80082FC0);
    } else {
        gSPSegment(gdl++, 8, (u32)D_80083140);
    }
    gSPClearGeometryMode(gdl++, 0x600);
    if (arg6 == 0) {
        gSPSetGeometryMode(gdl++, 0x200);
    } else {
        gSPSetGeometryMode(gdl++, 0x400);
    }
    flag = 1;
    sp130 = *(s16 *)((u8 *)D_800B0DF0 + 0x3E);
    if ((obj->pad66 & 0xC) == 0xC) {
        gSPSetGeometryMode(gdl++, 0x20000);
        gSPClearGeometryMode(gdl++, 0x400000);
    } else if ((obj->pad66 & 0xC) != 8) {
        gSPClearGeometryMode(gdl++, 0x420000);
        flag = 0;
    } else if (D_800DCD7C != 0) {
        gSPSetGeometryMode(gdl++, 0x420000);
    } else {
        gSPClearGeometryMode(gdl++, 0x420000);
        flag = 0;
    }
    if (arg6 == 3) {
        gSPClearGeometryMode(gdl++, 0x420000);
        flag = 0;
    }
    if (flag != 0) {
        t = (((u8 *)obj)[0x2FE] == 0) ? 0xA : 2;
        if (D_800D9900 != 0) {
            t |= 8;
        }
        gdl = func_1515D914(gdl, arg2, (s32)obj->x_position, (s32)obj->y_position, (s32)obj->z_position,
                            obj->unk184, *(s32 *)((u8 *)obj + arg2 * 4 + 0x304), ((u8 *)obj)[0x301],
                            *(s32 *)((u8 *)obj + 0x314) + D_800BE9C0 * 8, (u8 *)obj + 0x302, 2, (u8 *)obj + 0x1DD, t, D_800D9E28);
    }
    sp13C = -1;
    for (i = 0; i < D_800C4778[sp140]; i++) {
        if (!(*(s32 *)((u8 *)obj + 0x94) & (1 << i))) {
            if (!(*(s32 *)((u8 *)obj + 0x98) & (1 << i))) {
                if (sp13C != 0) {
                    gDPPipeSync(gdl++);
                    sp13C = 0;
                    if (sp130 == 0) {
                        gDPSetFogColor(gdl++, sp110, sp10C, sp108, sp104);
                    } else {
                        gDPSetFogColor(gdl++, D_800DD2E4[0], D_800DD2E4[1], D_800DD2E4[2], sp104);
                    }
                    gDPSetPrimColor(gdl++, 0xF2, 0, sp128, sp124, sp120, 0);
                    gDPSetEnvColor(gdl++, sp11C, sp118, sp114, arg4);
                }
            } else if (sp13C != 1) {
                gDPPipeSync(gdl++);
                gDPSetFogColor(gdl++, 0, 0, 0, 0xFF);
                gDPSetPrimColor(gdl++, 0xF2, 0, 0, 0, 0, 0);
                gDPSetEnvColor(gdl++, 0, 0, 0, 0xFF);
                sp13C = 1;
            }
            if (arg6 == 3) {
                if (D_800C48F0[sp140] != 0) {
                    gSPDisplayList(gdl++, (u32)D_800C48F0[sp140][i]);
                    continue;
                }
            } else {
                func_151EFE88(D_800C3E98);
                gSPMatrix(gdl++, (u32)D_C3E98, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(gdl++, (u32)D_800C4488[sp140][i]);
            }
        }
    }
    func_1502FD70(obj);
    ((u8 *)obj)[0x2FE] = 1;
    if (arg6 == 0) {
        gdl = func_15030F94(gdl, obj, arg2, 1);
    }
    if (arg7 != 0) {
        gSPDisplayList(gdl++, (u32)D_80084190);
    }
    if (arg6 == 0) {
        gSPGeometryMode(gdl++, 0x200, 0x400);
    }
    return gdl;
}

void func_1502D54C(s32 arg0, s32 *arg1) {
    u8 *temp_v0;

    temp_v0 = (u8 *)&D_800CC2D0[arg0];
    if ((temp_v0[0x66] & 0xC) == 4) {
        arg1[0] = (temp_v0[0x1E0] + temp_v0[0x1DD]) / 2;
        arg1[1] = (temp_v0[0x1E1] + temp_v0[0x1DE]) / 2;
        arg1[2] = (temp_v0[0x1E2] + temp_v0[0x1DF]) / 2;
        arg1[0] = 0xFF - arg1[0];
        arg1[1] = 0xFF - arg1[1];
        arg1[2] = 0xFF - arg1[2];
        return;
    }
    arg1[2] = 0xFF;
    arg1[1] = 0xFF;
    arg1[0] = 0xFF;
}

void func_150A7A00(void *, f32, f32, f32, f32 *, f32 *, f32 *, f32 *);
extern f32 D_80096DE8;
extern f32 D_80096DEC;
extern f32 D_80096DF0;
extern f32 D_80096DF4;
extern u8 D_800D2CA8[];
extern u16 D_800DD2E8;
extern u16 D_800DD2EC;

void func_1502D630(struct127 *arg0, s32 *arg1, s32 arg2) {
    f32 far;
    f32 near;
    f32 step;
    f32 first;
    f32 last;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 amount;

    if (D_800D9B1C != 0.0f) {
        first = 1.0f / D_800D9B1C;
    } else {
        first = D_80096DE8;
    }
    if (D_800D9B20 != 0.0f) {
        last = 1.0f / D_800D9B20;
    } else {
        last = D_80096DEC;
    }
    step = (first - last) * D_80096DF0;
    near = (f32)(u32)D_800DD2E8 * step + last;
    far = (f32)(u32)D_800DD2EC * step + last;
    func_150A7A00(D_800D2CA8 + (arg2 << 6), arg0->x_position,
                 arg0->y_position, arg0->z_position, &x, &y, &z, &w);
    w = (w != 0.0f) ? 1.0f / w : D_80096DF4;
    if (w < 0) {
        amount = 0;
    } else if (w >= far) {
        amount = 0;
    } else if (w <= near) {
        amount = 1.0f;
    } else {
        amount = (w - far) / (near - far);
    }
    arg1[3] = (1.0f - amount) * 255.0f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_58F80/func_1502D824.s")

extern u16 D_800C4ED0[];

s32 func_1502DB20(s32 arg0) {
    switch (arg0) {
    case 0x3B:
    case 0x75:
    case 0x82:
    case 0x88:
    case 0x90:
    case 0x96:
    case 0x98:
    case 0x9C:
    case 0x9D:
    case 0x9F:
    case 0xA0:
    case 0xB1:
    case 0xB2:
    case 0xB4:
        return D_800C4ED0[arg0] - 4;
    }
    return D_800C4ED0[arg0];
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_58F80/func_1502DB84.s")

extern void func_1502DB84(f32 mtx[4][4], s32 arg1);
typedef struct {
    u8 pad0[0xC8];
    f32 unkC8;
} Obj58F80C8;


void func_1502DF38(s32 arg0, s32 arg1) {
    f32 mtx[4][4];
    struct127 *obj;
    struct127 *other;
    s32 visible;
    s32 i;
    s32 j;
    s32 max;
    s32 v;
    f32 (*src)[4];
    f32 scale;
    void *ptr;
    s32 idx;

    obj = &D_800CC2D0[arg0];
    if (obj->xz_scale == 0.0f) {
        obj->xz_scale = 0.1000000015f;
        obj->unk154 = 0.1000000015f;
    }
    if (obj->y_scale == 0.0f) {
        obj->y_scale = 0.1000000015f;
        obj->unk158 = 0.1000000015f;
    }
    if ((obj->pad66 & 0x20) == 0x20) {
        visible = 0;
    } else {
        visible = 1;
    }
    if (obj->unk31C != NULL && obj->unk31C->pad196[1] != 0) {
        visible = 0;
    }
    if ((1 << arg0) & D_800C3E74) {
        visible = 0;
    }
    if (obj->id == 0x8A) {
        other = func_15072208(obj, 0);
        if (other != NULL && other->unk31C != NULL && other->unk31C->pad196[1] != 0) {
            visible = 0;
        }
    }
    if (visible) {
        if ((obj->unk74 & 0xF) == 0xF) {
            return;
        }
        max = 0;
        for (i = 0; i <= D_80082FA4; i++) {
            v = func_1506196C(obj, i);
            if (max < v) {
                max = v;
            }
        }
        if (max == 0) {
            return;
        }
    }
    D_800C3E78 = arg0;
    if (obj->unk65 && !(obj->unk101 & 0x10)) {
        idx = obj->unk65 - 1;
        src = (f32 (*)[4])D_800CC2D0[idx].unk1D4;
        if (src == NULL) {
            return;
        }
        src = (f32 (*)[4])((u8 *)src + (obj->unk5C << 6));
        if (!(obj->unk101 & 4)) {
            for (i = 0; i < 3; i++) {
                if (i == 1) {
                    scale = obj->y_scale;
                } else {
                    scale = obj->xz_scale;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = src[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = src[3][i];
            }
            mtx[3][3] = 1.0f;
        } else {
            f32 len;
            for (i = 0; i < 3; i++) {
                len = sqrtf(src[i][0] * src[i][0] + src[i][1] * src[i][1] + src[i][2] * src[i][2]);
                if (i == 1) {
                    scale = obj->y_scale / len;
                } else {
                    scale = obj->xz_scale / len;
                }
                for (j = 0; j < 3; j++) {
                    mtx[i][j] = src[i][j] * scale;
                }
                mtx[i][3] = 0.0f;
                mtx[3][i] = src[3][i];
            }
            mtx[3][3] = 1.0f;
        }
    } else {
        func_150A9B0C(mtx, obj->unkB8, obj->unk40, obj->unkC4, obj->xz_scale, obj->y_scale, obj->xz_scale);
        if (obj->unk101 & 0x10) {
            other = &D_800CC2D0[obj->unk65 - 1];
            src = (f32 (*)[4])D_800CC2D0[obj->unk65 - 1].unk1D4;
            if (src == NULL) {
                mtx[3][0] = 0;
                mtx[3][1] = 0;
                mtx[3][2] = 0;
            } else {
                src = (f32 (*)[4])((u8 *)src + (obj->unk5C << 6));
                mtx[3][0] = src[3][0];
                mtx[3][1] = src[3][1];
                mtx[3][2] = src[3][2];
            }
        } else {
            other = obj;
            mtx[3][0] = obj->x_position;
            mtx[3][1] = obj->y_position + obj->y_scale * ((Obj58F80C8 *)obj)->unkC8;
            mtx[3][2] = obj->z_position;
        }
        if (other->unk2D0 != NULL && *(s32 *)((u8 *)other->unk2D0 + 0x28) == 0) {
            mtx[3][1] += other->unkCC;
        }
    }
    if (arg1) {
        ptr = (void *)D_800C3E88;
        obj->unk1D4 = ptr;
        bcopy(mtx, ptr, 0x40);
        D_800C3E88 += 0x40;
        D_800C3E7A++;
        return;
    }
    func_1502DB84(mtx, arg0);
}

extern u8 D_800C3E90;
extern void func_150A9984(s32, s32);

void func_1502E474(void) {
    if (D_800C3E7A != 0) {
        func_150A9984((&D_800C3E80)[D_800BE9C0], D_800C3E7A);
    }
    D_800C3E90 = 1;
}

typedef struct {
    u8 pad0[0xE];
    u16 unkE;
    s16 unk10;
    u8 pad12[0x2C];
    u16 unk3E;
} Ent1502E4C4;

extern u8 D_800C3660;
extern u8 D_800D9C10[][0x40];
extern s32 func_150A6360(s32, void *, f32, f32, f32, f32, f32, f32);
extern s32 func_1503F904(struct127 *, s32, s32);
void func_1502E9FC(s32 arg0, s32 arg1);

void func_1502E4C4(s32 arg0) {
    struct127 *obj;
    u8 id;
    u8 *tbl;
    s32 base;
    f32 width;
    f32 height;
    f32 yoff;
    f32 thresh;
    s32 maxv;
    s32 i;
    s32 mask;
    s32 flag;
    s32 lim0;
    s32 lim;
    s32 old;
    s32 cmd;

    mask = 1;
    obj = &D_800CC2D0[arg0];
    id = obj->id;
    if (obj->unk5 == 7) {
        obj->unk74 &= 0xFFF0;
        return;
    }
    old = obj->unk74;
    obj->unk74 |= 0xF;
    if ((D_800C3638 != 0) && (D_800C3656 == 0) && (func_150229E4(obj) == 0)) {
        return;
    }
    if ((obj->unk127 != 0xFF) && !((u16)D_800D2340 & (1 << obj->unk127))) {
        return;
    }
    if ((obj->unk101 & 1) == 1) {
        return;
    }
    if (obj->unk7 == 0) {
        return;
    }
    if (obj->unk65 != 0) {
        obj->unk74 = D_800CC2D0[obj->unk65 - 1].unk74;
        return;
    }
    width = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unkE * obj->xz_scale;
    height = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unkE * obj->y_scale;
    yoff = (((Ent1502E4C4 *)D_800D1C90[id])->unk10 + obj->unkCC) * obj->y_scale;
    *(f32 *)((u8 *)obj + 0x270) = width;
    lim0 = *(u16 *)&obj->unk10;
    if ((D_800C35EA == 1) && (D_800C3660 == 0)) {
        lim0 = 0;
    }
    maxv = 0;
    if (obj->unk101 & 0x80) {
        thresh = 0.0f;
    } else {
        if (((Ent1502E4C4 *)D_800D1C90[id])->unk3E == 0) {
            thresh = obj->unkE4;
        } else if (((Ent1502E4C4 *)D_800D1C90[id])->unk3E == 1) {
            thresh = 0.0f;
        } else {
            thresh = (u32)((Ent1502E4C4 *)D_800D1C90[id])->unk3E * obj->xz_scale;
        }
    }
    for (i = 0; i <= D_80082FA0; i++) {
        s32 r;

        base = i * 0x180 + D_800BE628;
        tbl = D_800D9C10[i];
        flag = 0;
        if (D_800DBFF0[i].unk3D4->unk197 != 0) {
            lim = 100000;
        } else {
            lim = lim0;
        }
        r = func_150A6360(base, tbl, obj->x_position, obj->y_position + yoff,
                          obj->z_position, width, height, 20000.0f);
        if (r == 0) {
            flag = 1;
        } else {
            if (r < 0) {
                r = -r;
            }
            if (maxv < r) {
                maxv = r;
            }
            if (((obj->pad66 & 0x80) != 0x80) && (r <= thresh) && (D_800C35EA != 1)) {
                flag = 1;
            }
        }
        if (flag == 0) {
            if ((lim != 0) && (i != arg0)) {
                if (lim < r) {
                    cmd = 8;
                } else {
                    cmd = 9;
                }
                func_1506160C(obj, cmd & 0xFF, 0, 0, i);
            } else {
                obj->unkE.ub[1] |= (1 << i) << 4;
                func_1506160C(obj, 9, 0, 0, i);
                cmd = -1;
            }
            if ((obj->unkF8 & 0x800000) && (cmd != 8) && (D_800C35EA != 1) && (func_1503F904(obj, i, old) != 0)) {
                flag = 1;
            }
        } else {
            cmd = 10;
        }
        if (cmd != -1) {
            func_1506160C(obj, cmd & 0xFF, 1, 0, i);
        }
        if (flag == 0) {
            obj->unk74 &= ~mask;
        }
        if (func_1506196C(obj, i) == 0) {
            obj->unk74 |= mask;
        }
        mask <<= 1;
    }
    func_1502E9FC((s32)obj, maxv);
    if ((obj->camera != NULL) && ((obj->camera->unk2C == 0x10000) || (((struct127 *)obj->unk31C)->unk197 != 0))) {
        obj->unk74 |= 1 << arg0;
    }
    if (D_800C35EA != 1) {
        if (obj->pad2FC[0] != 0) {
            obj->unk74 |= obj->pad2FC[0];
        }
    }
}

void func_1502E9FC(s32 arg0, s32 arg1) {
}

void func_1502EA0C(ActorInteractionFields *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    arg0->field_0xA4 = 4;
    arg0->field_0xA5 = 0;
    arg0->field_0xA6 = arg5;
    arg0->field_0xA0 = (arg4 << 24) | (arg1 << 16) | (arg2 << 8) | arg3;
    arg0->field_0xA7 = 0xFF;
}

typedef struct {
    u8 pad0[0xA4];
    /* 0xA4 */ u8 unkA4;
} S1502EA50;

void func_1502EA50(S1502EA50 *arg0) {
    arg0->unkA4 = 0x5;
}

void func_1502EA60(u8 *arg0, s32 arg1) {
    arg0[0xA4] = 2;
    arg0[0xA5] = 0xFF;
    arg0[0xA6] = arg1;
}

void func_1502EA7C(u8 *arg0, s32 arg1) {
    arg0[0xA4] = 3;
    arg0[0xA5] = 0xFF;
    arg0[0xA6] = arg1;
}

void func_1502EA98(ActorInteractionFields *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if (arg0->field_0xA4 != 6 && arg0->field_0xA4 != 7) {
        if (arg5 != 0) {
            arg0->field_0xA5 = 0xFF;
        } else {
            arg0->field_0xA5 = 0;
        }
        arg0->field_0xA0 = (arg4 << 24) | (arg1 << 16) | (arg2 << 8) | arg3;
        arg0->field_0xA6 = arg6;
    }
    arg0->field_0xA4 = 6;
}

void func_1502EAFC(struct127 *arg0) {
    switch (*(u8 *)((u8 *)arg0 + 0xA4)) {
    case 2:
    case 3: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if (lo < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 - lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA5) = 0U;
        }
    }
        break;
    case 5: {
        s32 t1 = D_800BE9E4 * 0xA;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA7);

        if (t1 < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA7) = (u8) (v1 - t1);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA4) = 0U;
        }
    }
        /* fallthrough */
    case 4:
        *(u8 *)((u8 *)arg0 + 0xA5) += *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        break;
    case 6: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if ((s32) v1 < (0xFF - lo)) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 + lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA5) = 0xFFU;
        }
        *(u8 *)((u8 *)arg0 + 0xA4) = 7U;
    }
        break;
    case 7: {
        s32 lo = *(u8 *)((u8 *)arg0 + 0xA6) * D_800BE9E4;
        u8 v1 = *(u8 *)((u8 *)arg0 + 0xA5);

        if (lo < (s32) v1) {
            *(u8 *)((u8 *)arg0 + 0xA5) = (u8) (v1 - lo);
        } else {
            *(u8 *)((u8 *)arg0 + 0xA4) = 0U;
        }
    }
        break;
    }
}

void func_1502EC34(void *arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    ActorInteractionFields *a = (ActorInteractionFields *)arg0;
    f32 f;

    switch (a->field_0xA4) {
    case 1:
        *arg1 = a->field_0xA5;
        *arg2 = a->field_0xA6;
        *arg3 = a->field_0xA7;
        if (a->field_0xA0 >= 0x100) {
            f = *(f32 *)a->field_0xA0;
            f = (f < 0.0f) ? 0.0f : ((f > 255.0f) ? 255.0f : f);
            *arg4 = f * 2.55f;
        } else {
            *arg4 = a->field_0xA0;
        }
        break;
    case 2:
    case 3:
        *arg1 = 0;
        *arg2 = 0;
        *arg3 = 0;
        *arg4 = a->field_0xA5;
        if (a->field_0xA4 == 3) {
            *arg4 = 0xFF - *arg4;
        }
        break;
    case 4:
    case 5: {
        s32 alpha;
        f32 c;
        alpha = ((s32)a->field_0xA0 >> 24) & 0xFF;
        *arg1 = ((s32)a->field_0xA0 >> 16) & 0xFF;
        *arg2 = ((s32)a->field_0xA0 >> 8) & 0xFF;
        *arg3 = a->field_0xA0 & 0xFF;
        c = (cosf(a->field_0xA5 * 0.02454369329f) + 1.0f) * 0.5f;
        *arg4 = alpha + 64.0f * c;
        *arg4 += ((0xFF - *arg4) * (0xFF - a->field_0xA7)) >> 8;
    }
        break;
    case 6:
    case 7: {
        s32 alpha;
        alpha = ((s32)a->field_0xA0 >> 24) & 0xFF;
        *arg1 = ((s32)a->field_0xA0 >> 16) & 0xFF;
        *arg2 = ((s32)a->field_0xA0 >> 8) & 0xFF;
        *arg3 = a->field_0xA0 & 0xFF;
        *arg4 = 0xFF - ((a->field_0xA5 * alpha) >> 8);
        break;
    }
    }
}

/* The two u8 slots at actor record +0x6A (struct127 still calls this word pad68). */
typedef struct Act1502EE8C {
    char pad0[0x6A];
    /* 0x6A */ u8 unk6A[2];
    char pad6C[0x32C - 0x6C];
} Act1502EE8C;

s32 func_1502EE8C(s32 arg0, s32 arg1) {
    s32 v;
    s32 r;

    v = ((Act1502EE8C *)D_800CC2D0)[arg0].unk6A[arg1];
    if (v >= 2) {
        if (v >= 4) {
            r = 2;
        } else {
            r = v - 2;
        }
    } else {
        r = v;
    }
    return r;
}


/* The two u8 counters at actor record +0x6C (struct127 still calls this pad6C). */
typedef struct Act1502EEF4 {
    char pad0[0x6C];
    /* 0x6C */ u8 unk6C[2];
    char pad6E[0x32C - 0x6E];
} Act1502EEF4;

extern void func_1507E3C0(struct127 *);

void func_1502EEF4(s32 arg0) {
    s32 i;
    u8 r;

    for (i = 0; i < 2; i++) {
        if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] >= 10) {
            continue;
        }
        r = func_1502EE8C(arg0, i);
        if (r == 0) {
            if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] > 0) {
                ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i]--;
            }
        } else if (r == 1) {
            if (((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] < 2) {
                ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i]++;
            }
        } else {
            ((Act1502EEF4 *)D_800CC2D0)[arg0].unk6C[i] = 1;
        }
    }
    func_1507E3C0(&D_800CC2D0[arg0]);
}

u8 *func_1507E908(void *, s32);
typedef struct {
    u8 pad0[8];
    u8 e[1][3];
} Fn1502F01CTbl;
typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Fn1502F01CTex;
extern Fn1502F01CTex *D_800C5338[];

Gfx *func_1502F01C(Gfx *gdl, s32 arg1) {
    u8 *obj;
    u8 sp80[2];
    s32 id;
    u8 *res;
    s32 i;
    Fn1502F01CTex *tex;

    obj = (u8 *)&D_800CC2D0[arg1];
    id = obj[4];
    res = NULL;
    for (i = 0; i < 2; i++) {
        if (obj[0x6C + i] >= 10) {
            sp80[i] = obj[0x6C + i] - 10;
        } else {
            sp80[i] = ((Fn1502F01CTbl *)D_800D1C90[id])->e[i][obj[0x6C + i]];
            if (obj[0x6F] && !res) {
                res = func_1507E908(obj, obj[0x6F]);
            }
            if (res != NULL) {
                s32 sp68[2];
                sp68[0] = res[0];
                sp68[1] = res[1];
                if (res != NULL) {
                    if (sp68[i] != ((Fn1502F01CTbl *)D_800D1C90[id])->e[i][2]) {
                        if (obj[0x6C + i] == 0) {
                            sp80[i] = sp68[i];
                        }
                    } else {
                        sp80[i] = sp68[i];
                    }
                }
            }
        }
    }
    tex = D_800C5338[obj[4]];
    if (tex != NULL) {
        gSPSegment(gdl++, 6, tex[sp80[0]].unk0);
        gSPSegment(gdl++, 7, tex[sp80[1]].unk0);
        gSPSegment(gdl++, 10, tex[obj[0x68]].unk0);
        gSPSegment(gdl++, 11, tex[obj[0x69]].unk0);
    }
    return gdl;
}

void func_1502F264(s32 arg0) {
    struct127 *obj;
    struct127 *parent;
    f32 (*bone)[4][4];
    s32 i;

    obj = &D_800CC2D0[arg0];
    if (obj->unk65 == 0) {
        return;
    }
    parent = &D_800CC2D0[obj->unk65 - 1];
    if ((obj->unk101 & 2) == 2) {
        return;
    }
    bone = (f32 (*)[4][4])parent->unk1D4;
    if (bone == NULL) {
        obj->x_position = parent->x_position;
        obj->y_position = parent->y_position;
        obj->z_position = parent->z_position;
    } else {
        bone += obj->unk5C;
        obj->x_position = (*bone)[3][0];
        obj->y_position = (*bone)[3][1];
        obj->z_position = (*bone)[3][2];
    }
    obj->unk180 = parent->unk180;
    obj->y_velocity = -4.0f;
    if (!(obj->unk101 & 0x20)) {
        obj->unk76 = parent->unk76;
        obj->xz_velocity = parent->xz_velocity;
        obj->unk40 = parent->unk40;
        obj->unk76 = parent->unk76; /* copied twice in the original */
        obj->unk7A = parent->unk7A;
        obj->unk78 = parent->unk78;
    }
    if (!(obj->unk101 & 0x40)) {
        obj->unk7 = parent->unk7;
        obj->unk8 = parent->unk8;
        obj->unk9 = parent->unk9;
        obj->unkA = parent->unkA;
        obj->unkE.ub[1] = parent->unkE.ub[1];
        for (i = 0; i <= D_80082FA0; i++) {
            (&obj->unkB)[i] = (&parent->unkB)[i];
        }
    }

}

#pragma GLOBAL_ASM("asm/nonmatchings/game_58F80/func_1502F3C8.s")

typedef struct {
    u32 start;
    s32 count;
    s32 id;
} Seg1502F490;

typedef struct {
    u8 pad0[4];
    u8 unk4;
    u8 pad5[0x1D4 - 5];
    u8 *unk1D4;
    u8 *unk1D8;
} Obj1502F490;

extern u16 D_800C5EF8[];
void func_150A7960(f32 *, f32, f32, f32, f32 *, f32 *, f32 *);

void func_1502F490(Obj1502F490 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, s32 arg4) {
    f32 diff[3];
    f32 res[3];
    s32 idx[3];
    s32 i;
    s32 j;
    s32 mask;
    s32 seg;
    u8 id;
    u32 p[3];
    f32 v[2][3][3];
    f32 e1[2][3];
    f32 e2[2][3];
    u8 *mtx;
    f32 s;
    f32 t;

    id = arg0->unk4;
    if (D_800C6070[id] == 0) {
        return;
    }
    for (i = 0; i < 3; i++) {
        p[i] = *(s32 *)(D_800C6070[id] + arg4 * 12 + i * 4) + (&D_800D19A0)[id];
    }
    seg = 0;
    mask = 0;
    do {
        if (seg == D_800C5EF8[id]) {
            return;
        }
        for (i = 0; i < 3; i++) {
            if (!(mask & (1 << i))) {
                if (p[i] >= ((Seg1502F490 *)D_800C5C08[id])[seg].start && p[i] < ((Seg1502F490 *)D_800C5C08[id])[seg].start + ((Seg1502F490 *)D_800C5C08[id])[seg].count * 16) {
                    mask = (1 << i) | mask;
                    idx[i] = ((Seg1502F490 *)D_800C5C08[id])[seg].id;
                }
            }
        }
        seg++;
    } while (mask != 7);

    if (arg0->unk1D8 == NULL || arg0->unk1D4 == NULL) {
        return;
    }
    mtx = arg0->unk1D8;
    for (i = 0; i < 2; i++) {
        if (i == 1) {
            mtx = arg0->unk1D4;
        }
        for (j = 0; j < 3; j++) {
            func_150A7960((f32 *)(mtx + (idx[j] << 6)), ((s16 *)p[j])[0], ((s16 *)p[j])[1], ((s16 *)p[j])[2],
                          &v[i][j][0], &v[i][j][1], &v[i][j][2]);
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            e1[i][j] = v[i][1][j] - v[i][0][j];
            e2[i][j] = v[i][2][j] - v[i][0][j];
        }
    }
    diff[0] = *arg1 - v[0][0][0];
    diff[1] = *arg2 - v[0][0][1];
    diff[2] = *arg3 - v[0][0][2];
    s = e1[0][0] * e2[0][2] - e1[0][2] * e2[0][0];
    if (s != 0.0f) {
        s = (diff[0] * e2[0][2] - diff[2] * e2[0][0]) / s;
    } else {
        s = -100.0f;
    }
    if (s < 0.0f || s > 1.0f) {
        return;
    }
    if (e2[0][2] != 0.0f) {
        t = (diff[2] - s * e1[0][2]) / e2[0][2];
    } else {
        t = -100.0f;
    }
    if (t < 0.0f || t > 1.0f) {
        return;
    }
    for (i = 0; i < 3; i++) {
        res[i] = s * e1[1][i] + e2[1][i] * t;
    }
    diff[1] = s * e1[0][1] + e2[0][1] * t;
    *arg1 += res[0] + v[1][0][0] - diff[0] - v[0][0][0];
    *arg2 += res[1] + v[1][0][1] - diff[1] - v[0][0][1];
    *arg3 += res[2] + v[1][0][2] - diff[2] - v[0][0][2];
}

void func_1502F948(ActorInteractionFields *arg0) {
    s32 idx;
    void *temp;

    if ((arg0->field_0xF8 & 0x4000) && (arg0->field_0x264 != 0) && (arg0->field_0x1D4 != 0)) {
        idx = arg0->field_0x4;
        if (arg0->field_0x1D8 == 0) {
            temp = allocate_memory(D_800C4ED0[idx] << 6, 1, 1, 2);
            arg0->field_0x1D8 = temp;
            if (temp == 0) {
                return;
            }
        }
        bcopy(arg0->field_0x1D4, arg0->field_0x1D8, D_800C4ED0[idx] << 6);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_58F80/func_1502F9FC.s")

extern void func_1507EABC(void *);

#define actor ((u8 *)arg0)
void func_1502FBE8(struct127 *arg0) {
    s32 next;
    void *old;
    s32 selection;

    if (actor[0x1C9] == 0) {
        return;
    }
    old = ((void **)D_800D1588)[actor[4]];
    if (actor[0x1C9] == 0xFF) {
        actor[0x1C9] = 0;
        next = 0;
    } else {
        next = actor[0x1C9] - 1;
        if (next >= actor[0x2C9] || next == actor[0x1C8]) {
            return;
        }
    }
    selection = (*(u8 **)(actor + 0x2C4))[next];
    func_150837D4((actor - (u8 *)D_800CC2D0) / 0x32C, selection, 0);
    actor[0x1C8] = next;
    if (D_800C35EA == 1) {
        D_800C3638 = 0;
    }
    if (old != ((void **)D_800D1588)[selection]) {
        if (*(u16 *)(actor + 0x84) >= ((u16 *)D_800C5A90)[selection]) {
            *(u16 *)(actor + 0x84) = 0xFFFF;
            func_1505E650(arg0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        } else {
            s32 anim = *(u16 *)(actor + 0x84);

            *(u16 *)(actor + 0x84) = 0xFFFF;
            func_1505E650(arg0, anim, 1.0f, 0.0f, 0.0f, 0.0f, 0);
        }
    }
    if (D_800C35EA == 1) {
        D_800C3638 = 1;
    }
    func_1507EABC(actor);
}
#undef actor

typedef struct {
    u8 pad0[0x2E];
    u8 timer;
} Sub_1502FD70;

typedef struct {
    u8 pad0[4];
    u8 id;
    u8 pad5[0x144 - 5];
    Sub_1502FD70 *sub;
} Actor_1502FD70;

void func_1502FD70(struct127 *arg0) {
    Actor_1502FD70 *actor = (Actor_1502FD70 *)arg0;
    u8 id = actor->id;
    u8 *slot;

    if ((D_80038080 == 0) && (D_800BE9F0 == 0x1D)) {
        ((u8 *)D_800D2040)[id] = 2;
        return;
    }
    slot = &((u8 *)D_800D2040)[id];
    if (*slot != 0xFF) {
        if (actor->sub != NULL) {
            if (actor->sub->timer != 0xFF) {
                if (actor->sub->timer != 0) {
                    if (*slot < actor->sub->timer * 30) {
                        *slot = actor->sub->timer * 30;
                    }
                } else {
                    goto set3;
                }
            }
        } else {
set3:
            *slot = 3;
        }
    }
}
