#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void func_151D5D60();
void func_1513173C(struct210 *);
void func_1513175C(struct210 *);
void func_151617C4(struct210 *arg0);
void func_151617E4(struct210 *arg0);

typedef struct Beam Beam;

/* Collision record filled in by func_150AC9C0 (0x12 bytes, halfword aligned). */
typedef struct {
    s16 unk0[9];
} HitInfo;

/* Aim state of the beam's owner (owner->aim). */
typedef struct {
    u8 pad0[0xB4];
    f32 targetDist;
    struct17 targetPos;
    u8 padC4[0x24];
    struct17 targetDir;
    HitInfo targetHit;
    u8 pad106[3];
    u8 targetType; // 0 = no target
    u8 pad10A[0x8D];
    u8 unk197;
} BeamOwnerAim;

/* The actor the beam is attached to. */
typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x38];
    u8 unk74;
    u8 pad75[0x15F];
    f32 (*boneMtx)[4][4];
    u8 pad1D8[0x144];
    BeamOwnerAim *aim;
} BeamOwner;

/* beam->data.startFx: placed at the beam's start, sized by the beam's width. */
typedef struct {
    u8 pad0[0x38];
    f32 sizeX;
    f32 sizeY;
    struct17 pos;
    u8 pad4C[0x5C];
    f32 baseSizeX;
    f32 baseSizeY;
    u8 padB0[0x20];
    Beam *beam;
    u8 flags; // 1 = visible
} BeamStartFx;

/* beam->data.hitFx: placed just short of the hit point on type 1 hits. */
typedef struct {
    u8 pad0[0x34];
    struct17 pos;
    u8 pad40[0xD0];
    u8 flags; // 1 = visible
    u8 pad111;
    HitInfo hit;
    Beam *beam;
} BeamHitFx;

typedef struct {
    u8 pad0[0x9];
    u8 disabled;
    u8 padA[4];
    s16 pos[3];
} BeamEndPointData;

/* beam->data.endPoint: tracks the beam's end in s16 coordinates. */
typedef struct {
    u8 pad0[0x14];
    BeamEndPointData *data;
    Beam *beam;
} BeamEndPoint;

/* beam->data.line: spans the beam from start to end. */
typedef struct {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0xC];
    s32 unk58;
    u8 pad5C[0x114];
    Beam *beam;
    u8 flags; // 1 = visible
} BeamLine;

/* beam->data.endFx: placed at the beam's end on type 2 and 3 hits. */
typedef struct {
    u8 pad0[0x40];
    struct17 pos;
    u8 pad4C[0x5C];
    Beam *beam;
    u8 pad[4];
    u8 flags; // 1 = visible
} BeamEndFx;

typedef struct {
    BeamOwner *owner;
    u8 unk4; // must match owner->unk3B
    u8 pad5[3];
    struct17 localStart;  // in bone space
    struct17 localTarget; // in bone space
    u8 bone;
    u8 pad21;
    u8 unk22;
    u8 flags; // 1 = drawn, 2 = aimed by the owner
    BeamStartFx *startFx;
    BeamHitFx *hitFx;
    BeamEndPoint *endPoint;
    BeamLine *line;
    BeamEndFx *endFx;
    f32 width;
    u8 pad3C[8];
    f32 length;
    f32 scrollBase;  // texture scroll speed = scrollBase + scrollRange * sin(scrollPhase)
    f32 scrollRange;
    f32 scrollPhase;
    u8 pad54[4];
    f32 texOffset;
    u8 pad5C[0x34];
    s16 unk90;
} BeamData;

struct Beam {
    u8 pad0[0x34];
    struct17 start;
    struct17 end;
    u8 pad4C[0x74];
    Vtx vtxTemplate[4];
    Vtx *vtxBuf[4];
    BeamData data;
};

/* Releases the beam's child objects, then destroys it. */
void func_15101090(Beam *beam) {
    BeamData *data = &beam->data;

    // matching: tested as u16 but passed as s16, so IDO reuses the lhu without an andi
    if (*(u16 *)&beam->data.unk90) {
        func_100111C8(beam->data.unk90);
    }
    if (data->hitFx != NULL) {
        func_1516972C((struct102 *)data->hitFx);
    }
    if (data->endPoint != NULL) {
        func_1516972C((struct102 *)data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516972C((struct102 *)data->startFx);
    }
    if (data->line != NULL) {
        func_1516972C((struct102 *)data->line);
    }
    if (data->endFx != NULL) {
        func_1516972C((struct102 *)data->endFx);
    }
    func_1513CA6C((struct210 *)beam);
}

/* Counterpart of func_15101090 for the beam's child objects and func_1513CAA0. */
void func_15101148(Beam *beam) {
    BeamData *data = &beam->data;

    if (data->hitFx != NULL) {
        func_1516979C((struct102 *)data->hitFx);
    }
    if (data->endPoint != NULL) {
        func_1516979C((struct102 *)data->endPoint);
    }
    if (data->startFx != NULL) {
        func_1516979C((struct102 *)data->startFx);
    }
    if (data->line != NULL) {
        func_1516979C((struct102 *)data->line);
    }
    if (data->endFx != NULL) {
        func_1516979C((struct102 *)data->endFx);
    }
    func_1513CAA0((struct210 *)beam);
}

/* The functions below detach a child object from its beam before handing it on. */

void func_151011E8(BeamHitFx *fx) {
    BeamData *data = &fx->beam->data;

    data->hitFx = NULL;
    func_1513CA6C((struct210 *)fx);
}

void func_15101210(BeamHitFx *fx) {
    BeamData *data = &fx->beam->data;

    data->hitFx = NULL;
    func_1513CAA0((struct210 *)fx);
}

void func_15101238(BeamEndPoint *endPoint) {
    BeamData *data = &endPoint->beam->data;

    data->endPoint = NULL;
    func_151617C4((struct210 *)endPoint);
}

void func_15101260(BeamEndPoint *endPoint) {
    BeamData *data = &endPoint->beam->data;

    data->endPoint = NULL;
    func_151617E4((struct210 *)endPoint);
}

void func_15101288(BeamStartFx *fx) {
    BeamData *data = &fx->beam->data;

    data->startFx = NULL;
    func_1513173C((struct210 *)fx);
}

void func_151012B0(BeamStartFx *fx) {
    BeamData *data = &fx->beam->data;

    data->startFx = NULL;
    func_1513175C((struct210 *)fx);
}

void func_151012D8(BeamLine *line) {
    BeamData *data = &line->beam->data;

    data->line = NULL;
    func_151411A4((struct210 *)line);
}

void func_15101300(BeamLine *line) {
    BeamData *data = &line->beam->data;

    data->line = NULL;
    func_151411C4((struct210 *)line);
}

void func_15101328(BeamEndFx *fx) {
    BeamData *data = &fx->beam->data;

    data->endFx = NULL;
    func_1513173C((struct210 *)fx);
}

void func_15101350(BeamEndFx *fx) {
    BeamData *data = &fx->beam->data;

    data->endFx = NULL;
    func_1513175C((struct210 *)fx);
}

extern void func_151D5D60();

typedef struct {
    char pad_0[0x197];
    u8 field_0x197;
} ObjectState197;

typedef struct {
    char pad_0[0x31C];
    ObjectState197 *field_0x31C;
} ObjectWithState197;
typedef struct { f32 x, y, z; } F3_15101378;
#pragma pack(1)
typedef struct { s32 a, b, c, d; } Pq_15101378;
#pragma pack()
typedef struct { Pq_15101378 q; u16 h; } Pk_15101378;
#pragma pack()
typedef struct { u8 flag; u8 pad1; Pk_15101378 data; } XS_15101378;
typedef struct { u8 pad0[0x34]; F3_15101378 p34; u8 pad1[0x110 - 0x40]; XS_15101378 s; } X_15101378;
typedef struct { u8 pad0[0x9]; u8 f9; u8 pad1[4]; s16 sE; s16 s10; s16 s12; } P_15101378;
typedef struct { u8 pad0[0x14]; P_15101378 *p14; } Y_15101378;
typedef struct { u8 pad[4]; u8 flag; } VS_15101378;
typedef struct { u8 pad0[0x40]; F3_15101378 p40; u8 pad1[0xA8 - 0x4C]; VS_15101378 s; } V_15101378;
typedef struct { u8 pad0[0x38]; f32 f38; f32 f3C; F3_15101378 p40; u8 pad1[0xA8 - 0x4C]; f32 fA8; f32 fAC; u8 pad2[0xD4 - 0xB0]; u8 flagD4; } Z_15101378;
typedef struct { u8 pad0[0x34]; F3_15101378 p34; F3_15101378 p40; u8 pad1[0x58 - 0x4C]; s32 s58; u8 pad2[0x174 - 0x5C]; u8 flag174; } W_15101378;
typedef struct { u8 pad0[0xB4]; f32 fB4; F3_15101378 pB8; u8 padC4[0xE8 - 0xC4]; F3_15101378 pE8; Pk_15101378 pF4; u8 pad2[3]; u8 u109; } C_15101378;
typedef struct { s32 s0; u8 pad4[0x3B - 4]; u8 u3B; u8 pad3C[0x74 - 0x3C]; u8 u74; u8 pad75[0x1D4 - 0x75]; u8 *p1D4; u8 pad1D8[0x31C - 0x1D8]; C_15101378 *c; } O_15101378;
typedef struct {
    O_15101378 *owner;
    u8 u4;
    u8 pad5[3];
    f32 at8[3];
    f32 at14[3];
    u8 u20; u8 pad21[2]; u8 u23;
    Z_15101378 *z24;
    X_15101378 *x28;
    Y_15101378 *y2C;
    W_15101378 *w30;
    V_15101378 *v34;
    f32 f38;
    u8 pad3C[8];
    f32 f44;
} B_15101378;
typedef struct {
    u8 pad0[0x34];
    F3_15101378 p34;
    F3_15101378 p40;
    u8 pad4C[0x110 - 0x4C];
    B_15101378 b;
} A_15101378;
extern void func_15143134(void *, void *, void *);
extern f32 func_15143E64(void *);
extern s32 func_150AC9C0(f32, f32, f32, f32, f32, f32, void *, void *, void *, void *, void *, s32, s32, s32, f32);

s8 func_15101378(A_15101378 *arg0)
{
  s8 ret;
  C_15101378 *c;
  O_15101378 *owner;
  u8 hit;
  Pk_15101378 pk;
  F3_15101378 d;
  u8 ok;
  u8 *a2;
  F3_15101378 e;
  s32 tmp;
  s32 mode;
  s32 pad2;
  B_15101378 *b;
  u8 *ptr;
  ret = 1;
  owner = arg0->b.owner;
  b = &arg0->b;
  if ((owner->s0 == 0) || (owner->u3B != b->u4))
  {
    ret = 0;
  }
  else
  {
    ok = 1;
    ptr = owner->p1D4;
    if ((ptr != 0) && ((D_800C35EA != 1) || ((owner->u74 & 0xF) != 0xF)))
    {
      a2 = ((u8 (*)[0x40]) ptr)[b->u20];
      func_15143134(b->at8, &arg0->p34, a2);
      if (b->u23 & 2)
      {
        c = owner->c;
        mode = c->u109;
        if (mode != 0)
        {
          arg0->p40 = owner->c->pB8;
          b->f44 = owner->c->fB4;
          pk = owner->c->pF4;
          d.x = arg0->p40.x - arg0->p34.x;
          d.y = arg0->p40.y - arg0->p34.y;
          d.z = arg0->p40.z - arg0->p34.z;
        }
        else
        {
          ok = 0;
          mode = 2;
          b->f44 = 2000.0f;
          d.x = owner->c->pE8.x * 2000.0f;
          d.y = owner->c->pE8.y * 2000.0f;
          d.z = owner->c->pE8.z * 2000.0f;
          arg0->p40.x = arg0->p34.x + d.x;
          arg0->p40.y = arg0->p34.y + d.y;
          arg0->p40.z = arg0->p34.z + d.z;
        }
      }
      else
      {
        func_15143134(b->at14, &e, a2);
        e.x = e.x - arg0->p34.x;
        e.y = e.y - arg0->p34.y;
        e.z = e.z - arg0->p34.z;
        if (func_150AC9C0(arg0->p34.x, arg0->p34.y, arg0->p34.z, e.x, e.y, e.z, &tmp, &pk, &arg0->p40.x, &arg0->p40.y, &arg0->p40.z, 0, 0, 0, 0.0f) != (hit * 0))
        {
          d.x = arg0->p40.x - arg0->p34.x;
          d.y = arg0->p40.y - arg0->p34.y;
          d.z = arg0->p40.z - arg0->p34.z;
          hit = 1;
          b->f44 = func_15143E64(&d);
        }
        else
        {
          hit = 0;
        }
        mode = hit;
        lbl1:
        ;

        ;
      }
    }
    else
    {
      mode = 0;
    }
    if (mode != 0)
    {
      X_15101378 *x;
      XS_15101378 *xs;
      VS_15101378 *vs;
      V_15101378 *v;
      Y_15101378 *y;
      Z_15101378 *z;
      W_15101378 *w;
      b->u23 |= 1;
      if (b->x28 != 0)
      {
        x = b->x28;
        xs = &x->s;
        if ((mode == 1) && (ok != 0))
        {
          if (b->f44 > 40.0f)
          {
            f32 inv = 1.0f / b->f44;
            x->p34.x = arg0->p40.x - ((d.x * inv) * 20.0f);
            x->p34.y = arg0->p40.y - ((d.y * inv) * 20.0f);
            x->p34.z = arg0->p40.z - ((d.z * inv) * 20.0f);
          }
          else
          {
            x->p34 = arg0->p40;
          }
          xs->data = pk;
          xs->flag |= 1;
        }
        else
        {
          xs->flag &= ~1;
        }
      }
      if (b->v34 != 0)
      {
        v = b->v34;
        vs = &v->s;
        if (((mode == 2) || (mode == 3)) && (ok != 0))
        {
          v->p40 = arg0->p40;
          vs->flag |= 1;
        }
        else
        {
          vs->flag &= ~1;
        }
      }
      if (b->y2C != 0)
      {
        y = b->y2C;
        do {} while (0);
        y->p14->sE = (s32) arg0->p40.x;
        y->p14->s10 = (s32) arg0->p40.y;
        y->p14->s12 = (s32) arg0->p40.z;
        if (ok != 0)
        {
          y->p14->f9 = 0;
        }
        else
        {
          y->p14->f9 = 1;
        }
      }
      if (b->z24 != 0)
      {
        z = b->z24;
        z->p40 = arg0->p34;
        z->f38 = z->fA8 * b->f38;
        z->f3C = z->fAC * b->f38;
        z->flagD4 |= 1;
      }
      if (b->w30 != 0)
      {
        w = b->w30;
        w->p34 = arg0->p34;
        w->p40 = arg0->p40;
        w->s58 &= ~4;
        w->flag174 |= 1;
      }
    }
    else
    {
      X_15101378 *x;
      XS_15101378 *xs;
      VS_15101378 *vs;
      V_15101378 *v;
      Y_15101378 *y;
      Z_15101378 *z;
      W_15101378 *w;
      lbl2:
      ;

      ;
      b->u23 &= ~1;
      if (b->x28 != 0)
      {
        x = b->x28;
        x->s.flag &= ~1;
      }
      if (b->y2C != 0)
      {
        y = b->y2C;
        y->p14->f9 = 1;
      }
      if (b->z24 != 0)
      {
        z = b->z24;
        z->flagD4 &= ~1;
      }
      if (b->w30 != 0)
      {
        w = b->w30;
        w->flag174 &= ~1;
      }
      if (b->v34 != 0)
      {
        v = b->v34;
        v->s.flag &= ~1;
      }
    }
  }
  return ret;
}

typedef struct {
    ObjectWithState197 *unk0;
    char pad4[0x1E];
    u8 unk22;
    u8 unk23;
    char pad24[0x14];
    f32 unk38;
    char pad3C[0x8];
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    char pad54[0x4];
    f32 unk58;
} Sub151019C4;

typedef struct {
    char pad0[0x34];
    struct17 unk34;
    struct17 unk40;
    char pad4C[0x74];
    char unkC0[0x40];
    char unk100[0x10];
    Sub151019C4 unk110;
} Obj151019C4;

typedef struct {
    s16 x, y, z;
    u16 flag;
    s16 tc0, tc1;
    u8 color[4];
} Vtx151019C4;

extern f32 D_800A2288;
extern f32 *func_15144B34(s32);
extern void func_15145548(struct17 *, struct17 *, f32 *, struct17 *, s32);

void *func_151019C4(Obj151019C4 *arg0, s16 arg1) {
    Sub151019C4 *sub = &arg0->unk110;
    Vtx151019C4 *vtx;
    void *result;
    f32 *pos;
    struct17 dir;
    struct17 out;
    f32 dx, dy, dz;
    f32 pz, py, px;
    f32 lenSq;
    f32 scale;
    f32 sn;
    struct17 start;
    struct17 end;
    f32 len;
    u8 fresh;
    s32 t0, t1, diff;
    f32 maxLen;

    if (arg1 == sub->unk22 && (sub->unk0 == NULL || sub->unk0->field_0x31C->field_0x197 != 0)) {
        return 0;
    }
    if (sub->unk23 & 1) {
        func_151D5D60(arg0->unk100, arg1, 0x40, (void **)&vtx, &fresh);
        result = vtx;
        if (vtx != 0) {
            if (fresh != 0) {
                memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100), arg0->unkC0, 0x40);
                memcpy(*(u8 **)((u8 *)arg0 + arg1 * 4 + 0x100) + 0x40, arg0->unkC0, 0x40);
            }
        } else {
            return 0;
        }
        pos = func_15144B34(arg1);
        maxLen = D_800A2288;
        start = arg0->unk34;
        scale = sub->unk44;
        if (maxLen < scale) {
            f32 s = maxLen / scale;

            dir.unk0 = (arg0->unk40.unk0 - start.unk0) * s;
            dir.unk4 = (arg0->unk40.unk4 - start.unk4) * s;
            dir.unk8 = (arg0->unk40.unk8 - start.unk8) * s;
            end.unk0 = start.unk0 + dir.unk0;
            end.unk4 = start.unk4 + dir.unk4;
            end.unk8 = start.unk8 + dir.unk8;
            len = maxLen;
        } else {
            end = arg0->unk40;
            dir.unk0 = end.unk0 - start.unk0;
            dir.unk4 = end.unk4 - start.unk4;
            dir.unk8 = end.unk8 - start.unk8;
            len = sub->unk44;
        }
        func_15145548(&start, &dir, pos, &out, 0);
        dx = out.unk0 - pos[0];
        dy = out.unk4 - pos[1];
        dz = out.unk8 - pos[2];
        px = dir.unk4 * dz - dy * dir.unk8;
        py = dir.unk8 * dx - dz * dir.unk0;
        pz = dir.unk0 * dy - dx * dir.unk4;
        lenSq = px * px + py * py + pz * pz;
        if (lenSq == 0.0f) {
            pz = py = px = 0.0f;
        } else {
            scale = sub->unk38 / sqrtf(lenSq);
            px *= scale;
            py *= scale;
            pz *= scale;
        }
        sn = sinf(sub->unk50);
        sn = sub->unk48 + sub->unk4C * sn;
        t0 = sub->unk58;
        t1 = sub->unk58 + len * sn;
        diff = t1 - t0;
        {
        s32 t;
        if (t1 >= 0x8000 || t0 < -0x8000) {
            t = t0;
            while (t > 0x2000) {
                t -= 0x2000;
            }
            while (t < 0) {
                t += 0x2000;
            }
            t0 = t - 0x8000;
            t1 = t0 + diff;
        }
        }
        vtx->x = end.unk0 + px;
        vtx->y = end.unk4 + py;
        vtx->z = end.unk8 + pz;
        vtx->tc1 = 0;
        vtx->tc0 = t1;
        vtx->flag = 0;
        vtx++;
        vtx->x = end.unk0 - px;
        vtx->y = end.unk4 - py;
        vtx->z = end.unk8 - pz;
        vtx->tc1 = 0x7C0;
        vtx->tc0 = t1;
        vtx->flag = 0;
        vtx++;
        vtx->x = start.unk0 - px;
        vtx->y = start.unk4 - py;
        vtx->z = start.unk8 - pz;
        vtx->tc1 = 0x7C0;
        vtx->tc0 = t0;
        vtx->flag = 0;
        vtx++;
        vtx->x = start.unk0 + px;
        vtx->y = start.unk4 + py;
        vtx->z = start.unk8 + pz;
        vtx->tc1 = 0;
        vtx->tc0 = t0;
        vtx->flag = 0;
        return result;
    }
end:
    return 0;
}
