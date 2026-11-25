#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Per-update-routine parameters (sprite +0x18, descriptor +0x08). */
typedef union {
    struct {
        s32 frame; /* frame (8.8) to stop scrolling at */
        s32 y;     /* y to stop scrolling at */
        s32 stage;
    } scroll; /* func_150B6E3C */
    struct {
        s32 elapsed;
    } slide; /* func_150B709C */
    struct {
        s16 endX;
        s16 endY;
        s32 duration;
        s16 startX;
        s16 startY;
    } move; /* func_150B73F0: from start to end as the timer runs down */
} SpriteParams;

/* SpriteObj descriptor passed to func_15169968/func_15169900, which copy it to the new sprite at +0x10. */
typedef struct {
    /* 0x00 */ void *texture;
    /* 0x04 */ void *unk4;
    /* 0x08 */ SpriteParams params;
    /* 0x14 */ s16 timer;
    /* 0x16 */ s16 frame;      /* 8.8 animation frame */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 x;
    /* 0x20 */ f32 y;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 scaleX;     /* 0x1000 = 1.0 */
    /* 0x2A */ s16 scaleY;
    /* 0x2C */ s16 width;
    /* 0x2E */ s16 height;
    /* 0x30 */ u8 update;      /* index into the D_8008CA20 update routines */
    /* 0x31 */ u8 frameCount;
    /* 0x32 */ u8 red;
    /* 0x33 */ u8 green;
    /* 0x34 */ u8 blue;
    /* 0x35 */ u8 alpha;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
} SpriteDesc;

/* A sprite object: a 0x10-byte header followed by its SpriteDesc fields. */
typedef struct {
    /* 0x00 */ u8 header[0x10];
    /* 0x10 */ void *texture;
    /* 0x14 */ void *unk14;
    /* 0x18 */ SpriteParams params;
    /* 0x24 */ s16 timer;
    /* 0x26 */ s16 frame;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ f32 x;
    /* 0x30 */ f32 y;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ s16 unk36;
    /* 0x38 */ s16 scaleX;
    /* 0x3A */ s16 scaleY;
    /* 0x3C */ s16 width;
    /* 0x3E */ s16 height;
    /* 0x40 */ u8 update;
    /* 0x41 */ u8 frameCount;
    /* 0x42 */ u8 red;
    /* 0x43 */ u8 green;
    /* 0x44 */ u8 blue;
    /* 0x45 */ u8 alpha;
    /* 0x46 */ u8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ u8 unk48;
    /* 0x49 */ u8 unk49;
} SpriteObj;


void func_150B6C90(void);
void func_150B6D34(void);
void func_150B6D78(void);
void func_150B7484(void);
void func_150B7560(void);
void func_150B765C(void);
void func_150B77A8(void);
void func_150B768C(void);
void func_150B791C(void);

/* Runs the command queued in D_800D9890 (each handler resets it to 3). */
void func_150B6BC0(s32 unused) {
    switch (D_800D9890) {
    case 1:
        func_150B6C90();
        return;
    case 2:
        func_150B6D34();
        return;
    case 4:
        func_150B6D78();
        return;
    case 5:
        func_150B7484();
        return;
    case 6:
        func_150B7560();
        return;
    case 7:
        func_150B765C();
        return;
    case 8:
        func_150B77A8();
        return;
    case 9:
        func_150B768C();
        return;
    case 10:
        func_150B791C();
    default:
        return;
    }
}

extern struct102 *D_800D9894;
// matching: defined in-file so as1 shares one lui across the unrolled stores in func_150B6C90
static struct102 *D_800D9898[10];
struct Obj151A4FD0 *func_151A4FD0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, u8 a6, s32 a7);

/* Recreates the func_151A4FD0 effect and forgets all sprites. */
void func_150B6C90(void) {
    s32 i;

    if (D_800D9894 != NULL) {
        func_1516972C(D_800D9894);
    }
    D_800D9894 = (struct102 *)func_151A4FD0(0, 0, 0xFF, 0, 0, D_800BE9E8, 0, 0);
    D_800D9890 = 3;
    for (i = 0; i < 10; i++) {
        D_800D9898[i] = NULL;
    }
}

/* Starts the three scrolling sprites (slots 5-7) moving on to their next stage. */
void func_150B6D34(void) {
    s32 i;
    SpriteObj *sprite;

    for (i = 0; i < 3; i++) {
        sprite = (SpriteObj *)D_800D9898[i + 5];
        if (sprite != NULL) {
            sprite->params.scroll.stage = 1;
        }
    }
    D_800D9890 = 3;
}

/* Frees the func_151A4FD0 effect and every sprite. */
void func_150B6D78(void) {
    s32 i;

    if (D_800D9894 != NULL) {
        func_1516972C(D_800D9894);
        D_800D9894 = NULL;
    }
    for (i = 0; i < 10; i++) {
        if (D_800D9898[i] != NULL) {
            func_1516972C(D_800D9898[i]);
            D_800D9898[i] = NULL;
        }
    }
    D_800D9890 = 3;
}

/* Update routine 4: advances unk34 by 0x30 per frame, wrapping from above 0x800 back to -0xC00. */
void func_150B6DFC(SpriteObj *sprite) {
    sprite->unk34 += D_800BE9E4 * 0x30;
    if (sprite->unk34 > 0x800) {
        sprite->unk34 = -0xC00;
    }
}

extern u8 D_80091918[];

/* Update routine 5: scrolls the sprite up through its animation frames, fading it with its distance
 * from y = 170, until it reaches its target frame and y. Once stopped at y = 170 it blinks, and after
 * the third blink it turns into the D_80091918 sprite run by update routine 8 (func_150B709C). */
void func_150B6E3C(SpriteObj *sprite) {
    u32 three;
    f32 dist;
    s32 alpha;

    dist = 0.0f; // matching: a dead first def makes IDO give dist the first FP register
    three = 3;   // matching: one register for the multu step and the stage compare
    if (sprite->params.scroll.stage == 0 || sprite->frame != sprite->params.scroll.frame ||
        (dist = (sprite->y < (f32)sprite->params.scroll.y) ? -(sprite->y - (f32)sprite->params.scroll.y)
                                                          : (sprite->y - (f32)sprite->params.scroll.y),
         (f32)(s32)(D_800BE9E4 * three) < dist)) {
        sprite->y -= (f32)(s32)(D_800BE9E4 * three);
        if (sprite->y <= 140.0f) {
            sprite->frame += 0x100;
            if (sprite->frame >= (sprite->frameCount << 8)) {
                sprite->frame = 0;
            }
            sprite->y = 200.0f - (140.0f - sprite->y);
        }
        alpha = 0xFF - ((sprite->y < 170.0f) ? (s32)-(sprite->y - 170.0f) : (s32)(sprite->y - 170.0f)) * 8;
        if (alpha < 0) {
            alpha = 0;
        }
        sprite->alpha = alpha;
        return;
    }

    sprite->y = sprite->params.scroll.y;
    if (sprite->params.scroll.y == 170) {
        alpha = sprite->alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            sprite->params.scroll.stage++;
            if (sprite->params.scroll.stage == three) {
                sprite->texture = D_80091918;
                sprite->update = 8;
                sprite->alpha = 0xFF;
                sprite->width = 128;
                sprite->height = 16;
                sprite->frame = 0;
                sprite->params.slide.elapsed = 0;
                return;
            }
            alpha = 0;
        }
        sprite->alpha = alpha;
        return;
    }
    alpha = 0xFF - ((sprite->y < 170.0f) ? (s32)-(sprite->y - 170.0f) : (s32)(sprite->y - 170.0f)) * 9;
    if (alpha < 0) {
        alpha = 0;
    }
    sprite->alpha = alpha;
}

/* Update routine 8: slides the sprite from (230, 170) to (146, 120) over 30 steps, then blinks it. */
void func_150B709C(SpriteObj *sprite) {
    s32 elapsed;
    s32 alpha;

    elapsed = sprite->params.slide.elapsed;
    if (elapsed == 30) {
        alpha = sprite->alpha;
        alpha += D_800BE9E4 * 8;
        if (alpha >= 0x100) {
            alpha = 0;
        }
        sprite->alpha = alpha;
    }
    elapsed += D_800BE9E4 * 2;
    if (elapsed > 30) {
        elapsed = 30;
    }
    sprite->params.slide.elapsed = elapsed;
    sprite->x = elapsed * -84 / 30 + 230;
    sprite->y = elapsed * -50 / 30 + 170;
}

extern s32 D_800BE9E4;
void func_150B71A8(void *arg0)
{
  u8 *new_var;
  s16 temp_v0;
  unsigned int temp_v1;
  temp_v1 = 0x1000;
  temp_v0 = *((s16 *) (((u8 *) arg0) + 0x38));
  if (temp_v1 == temp_v0)
  {
  }
  else
  {
    *((s16 *) (((u8 *) arg0) + 0x38)) = temp_v0 + (D_800BE9E4 * 0x100);
    if ((*((s16 *) (((u8 *) arg0) - -0x38))) >= 0x1001)
    {
      *((s16 *) (((u8 *) arg0) + 0x38)) = temp_v1;
    }
    return;
  }
  temp_v0 = *((s16 *) (((u8 *) arg0) + 0x3A));
  if (temp_v1 != temp_v0)
  {
    *((s16 *) (((u8 *) arg0) + 0x3A)) = temp_v0 + (D_800BE9E4 * 0x100);
    if ((*((s16 *) (((u8 *) arg0) + 0x3A))) >= 0x1001)
    {
      new_var = (u8 *) arg0;
      *((s16 *) (new_var + 0x3A)) = temp_v1;
    }
  }
}


typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 unk33;
    /* 0x34 */ u8 unk34;
    /* 0x35 */ u8 unk35;
    /* 0x36 */ u8 unk36;
    /* 0x37 */ u8 unk37;
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ u8 pad3A[2];
} Struct150B7220;

extern u8 D_80091924;
s32 func_15169900(void *arg0, s32 arg1);

void func_150B7220(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
  Struct150B7220 sp44;
  s32 r;
  s32 x;
  s32 y;
  x = arg0;
  y = arg1;
  sp44.unk0 = &D_80091924;
  sp44.unk8 = (x << 16) | (y & 0xFFFF);
  sp44.unk28 = 0x1000;
  sp44.unk2A = 0x1000;
  sp44.unkC = arg2;
  sp44.unk14 = arg2;
  sp44.unk18 = 0;
  sp44.unk24 = 0;
  sp44.unk26 = 0;
  sp44.unk2C = 0x10;
  sp44.unk2E = 0x10;
  sp44.unk30 = 7;
  sp44.unk31 = 2;
  sp44.unk32 = 0xFF;
  sp44.unk33 = 0;
  sp44.unk34 = 0;
  if (y)
  {
  }
  sp44.unk35 = 0xFF;
  sp44.unk36 = 7;
  sp44.unk37 = 0x11;
  if (x < (D_800BE638 >> 1))
  {
 do { r = D_800BE638 - x; } while (0);
  }
  else
  {
    r = x;
  }
  sp44.unk10 = (x << 16) | ((y - r) & 0xFFFF);
  sp44.unk39 = 0;
  sp44.unk16 = 0;
  sp44.unk1C = x;
  sp44.unk20 = y - r;
  func_15169900(&sp44, arg3);
  sp44.unk10 = (x << 16) | ((y + r) & 0xFFFF);
  sp44.unk39 = 8;
  sp44.unk20 = y + r;
  func_15169900(&sp44, arg3);
  sp44.unk1C = x - r;
  sp44.unk10 = ((x - r) << 16) | (y & 0xFFFF);
  sp44.unk39 = 4;
  sp44.unk16 = 0x100;
  sp44.unk20 = y;
  func_15169900(&sp44, arg3);
  sp44.unk10 = ((x + r) << 16) | (y & 0xFFFF);
  sp44.unk39 = 0;
  sp44.unk1C = x + r;
  func_15169900(&sp44, arg3);
}

typedef struct {
    char pad18[0x18];
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    s16 unk20;
    s16 unk22;
    s16 unk24;
    char pad26[6];
    f32 unk2C;
    f32 unk30;
} Struct73F0;

void func_150B73F0(Struct73F0 *a0) {
    s32 a = a0->unk18;
    s32 b = a0->unk1A;
    s32 c = a0->unk20;
    s32 e = a0->unk22;
    s32 d = a0->unk1C;
    s32 n = a0->unk24;
    s32 t1 = (n << 16) / d;
    a0->unk2C = (f32)((((c - a) * t1) >> 16) + a);
    a0->unk30 = (f32)((((e - b) * t1) >> 16) + b);
}

typedef struct {
    /* 0x00 */ void *unk0;
    /* 0x04 */ void *unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  pad1A[0x2];
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
    /* 0x2C */ s16 unk2C;
    /* 0x2E */ s16 unk2E;
    /* 0x30 */ u8  unk30;
    /* 0x31 */ u8  unk31;
    /* 0x32 */ u8  unk32;
    /* 0x33 */ u8  unk33;
    /* 0x34 */ u8  unk34;
    /* 0x35 */ u8  unk35;
    /* 0x36 */ u8  unk36;
    /* 0x37 */ u8  unk37;
    /* 0x38 */ u8  unk38;
    /* 0x39 */ u8  unk39;
} struct_7484;

extern u8 D_800918DC;
extern u8 D_800918E8;
extern u8 D_800918F4;
extern u8 D_80091900;
struct102 *func_15169968(struct_7484 *);

void func_150B7484(void) {
    struct_7484 sp1C;

    sp1C.unk31 = 1;
    sp1C.unk2C = 0x58;
    sp1C.unk1C = 60.0f;
    sp1C.unk20 = 60.0f;
    sp1C.unk14 = 0;
    sp1C.unk16 = 0;
    sp1C.unk18 = 0;
    sp1C.unk2E = 0x58;
    sp1C.unk30 = 6;
    sp1C.unk0 = &D_800918DC;
    sp1C.unk32 = 0xFF;
    sp1C.unk33 = 0;
    sp1C.unk34 = 0;
    sp1C.unk35 = 0xFF;
    sp1C.unk24 = 0;
    sp1C.unk26 = 0;
    sp1C.unk39 = 0;
    sp1C.unk36 = 7;
    sp1C.unk37 = 0x15;
    sp1C.unk38 = 1;
    sp1C.unk28 = 0x51;
    sp1C.unk2A = 0x51;
    if (D_800D9898[0] != 0) {
        func_1516972C((struct102 *)D_800D9898[0]);
    }
    D_800D9898[0] = (s32 *)func_15169968(&sp1C);
    D_800D9890 = 3;
}

void func_150B7560(void) {
    struct_7484 sp1C;

    if (D_800D9898[0] != 0) {
        func_1516972C((struct102 *)D_800D9898[0]);
        D_800D9898[0] = 0;
    }
    sp1C.unk18 = 0x4D;
    sp1C.unk31 = 0xF;
    sp1C.unk14 = 0;
    sp1C.unk16 = 0;
    sp1C.unk2C = 0x58;
    sp1C.unk2E = 0x58;
    sp1C.unk30 = 0;
    sp1C.unk0 = &D_800918DC;
    sp1C.unk24 = 0;
    sp1C.unk26 = 0;
    sp1C.unk32 = 0xFF;
    sp1C.unk33 = 0;
    sp1C.unk34 = 0;
    sp1C.unk35 = 0xFF;
    sp1C.unk36 = 7;
    sp1C.unk37 = 0x11;
    sp1C.unk38 = 1;
    sp1C.unk39 = 0;
    sp1C.unk28 = 0x1000;
    sp1C.unk2A = 0x1000;
    sp1C.unk1C = 60.0f;
    sp1C.unk20 = 60.0f;
    if (D_800D9898[3] != 0) {
        func_1516972C((struct102 *)D_800D9898[3]);
    }
    D_800D9898[3] = (s32 *)func_15169968(&sp1C);
    D_800D9890 = 3;
}

void func_150B76BC(s32, s32);

void func_150B765C(void) {
    func_150B76BC(0x3C, 1);
    D_800D9890 = 3;
}

void func_150B768C(void) {
    func_150B76BC(0xE6, 2);
    D_800D9890 = 3;
}

void func_150B76BC(s32 arg0, s32 arg1) {
    struct_7484 sp24;

    sp24.unk1C = (f32)arg0;
    sp24.unk20 = 170.0f;
    sp24.unk31 = 1;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk30 = 6;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk0 = &D_800918E8;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0xFF;
    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk39 = 0;
    sp24.unk36 = 7;
    sp24.unk37 = 0x15;
    sp24.unk38 = 1;
    sp24.unk28 = 0x51;
    sp24.unk2A = 0x51;
    if (D_800D9898[arg1] != 0) {
        func_1516972C((struct102 *)D_800D9898[arg1]);
    }
    D_800D9898[arg1] = (s32 *)func_15169968(&sp24);
}

void func_150B77A8(void) {
    struct_7484 sp24;

    if (D_800D9898[1] != 0) {
        func_1516972C((struct102 *)D_800D9898[1]);
        D_800D9898[1] = 0;
    }

    sp24.unk1C = 60.0f;
    sp24.unk20 = 170.0f;
    sp24.unk2C = 0x40;
    sp24.unk2E = 0x40;
    sp24.unk30 = 4;
    sp24.unk0 = &D_800918F4;
    sp24.unk4 = &D_80091900;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk31 = 0;
    sp24.unk24 = -0x800;
    sp24.unk26 = 9;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0xFF;
    sp24.unk36 = 0xD;
    sp24.unk37 = 0x12;
    sp24.unk38 = 2;
    sp24.unk39 = 3;
    sp24.unk28 = 0x1000;
    sp24.unk2A = 0x1000;
    if (D_800D9898[4] != 0) {
        func_1516972C((struct102 *)D_800D9898[4]);
    }
    D_800D9898[4] = (s32 *)func_15169968(&sp24);

    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk30 = 0;
    sp24.unk36 = 7;
    sp24.unk37 = 0x11;
    sp24.unk0 = &D_800918E8;
    sp24.unk38 = 1;
    if (D_800D9898[8] != 0) {
        func_1516972C((struct102 *)D_800D9898[8]);
    }
    D_800D9898[8] = (s32 *)func_15169968(&sp24);
    D_800D9890 = 3;
}

extern u8 D_8009190C;

void func_150B791C(void) {
    struct_7484 sp24;

    if (D_800D9898[2] != 0) {
        func_1516972C((struct102 *)D_800D9898[2]);
        D_800D9898[2] = 0;
    }
    sp24.unk1C = 230.0f;
    sp24.unk31 = 3;
    sp24.unk2C = 0x40;
    sp24.unk2E = 0x10;
    sp24.unk30 = 5;
    sp24.unk24 = 0;
    sp24.unk26 = 0;
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk0 = &D_8009190C;
    sp24.unk4 = 0;
    sp24.unk8 = 0;
    sp24.unk20 = 150.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk10 = 0;
    sp24.unk32 = 0xFF;
    sp24.unk33 = 0;
    sp24.unk34 = 0;
    sp24.unk35 = 0x80;
    sp24.unk36 = 7;
    sp24.unk37 = 0x11;
    sp24.unk38 = 1;
    sp24.unk39 = 0;
    sp24.unk28 = 0x1000;
    sp24.unk2A = 0x1000;
    if (D_800D9898[5] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[5] = (s32 *)func_15169968(&sp24);
    sp24.unk16 = 0x100;
    sp24.unk8 = 0x100;
    sp24.unk20 = 170.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk35 = 0xFF;
    if (D_800D9898[6] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[6] = (s32 *)func_15169968(&sp24);
    sp24.unk16 = 0x200;
    sp24.unk8 = 0x200;
    sp24.unk20 = 190.0f;
    sp24.unkC = sp24.unk20;
    sp24.unk35 = 0x80;
    if (D_800D9898[7] != 0) {
        func_1516972C((struct102 *)D_800D9898[5]);
    }
    D_800D9898[7] = (s32 *)func_15169968(&sp24);
    sp24.unk14 = 0;
    sp24.unk16 = 0;
    sp24.unk18 = 0;
    sp24.unk31 = 0;
    sp24.unk2C = 0x58;
    sp24.unk2E = 0x40;
    sp24.unk35 = 0xFF;
    sp24.unk30 = 0;
    sp24.unk0 = &D_800918E8;
    sp24.unk1C = 230.0f;
    sp24.unk20 = 170.0f;
    if (D_800D9898[9] != 0) {
        func_1516972C((struct102 *)D_800D9898[9]);
    }
    D_800D9898[9] = (s32 *)func_15169968(&sp24);
    D_800D9890 = 3;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Vec3_150B85C0;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3_150B7B40;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C[3];
    /* 0x28 */ Vec3_150B7B40 pos;
    /* 0x34 */ f32 vel[3];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ f32 unk44;
    /* 0x48 */ f32 unk48;
    /* 0x4C */ f32 unk4C;
    /* 0x50 */ s32 unk50;
    /* 0x54 */ s16 unk54;
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 unk58;
    /* 0x5C */ s32 unk5C;
    /* 0x60 */ u8 unk60;
    /* 0x61 */ u8 unk61;
    /* 0x62 */ u8 unk62;
    /* 0x63 */ u8 unk63;
    /* 0x64 */ u8 unk64;
    /* 0x65 */ u8 unk65;
    /* 0x66 */ u8 unk66;
    /* 0x67 */ u8 unk67;
    /* 0x68 */ u8 unk68;
    /* 0x69 */ u8 pad69;
    /* 0x6A */ u8 unk6A;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ u8 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 unk74;
} Particle150B7B40;

typedef struct {
    Vec3_150B7B40 unk0;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 unk70;
    s32 unk74;
    s32 unk78;
    u8 unk7C;
    u8 unk7D;
    u8 pad7E[2];
    s16 unk80;
    s16 unk82;
    s32 unk84;
} Burst150B7B40;

typedef struct {
    /* 0x00 */ f32 unk00;
    /* 0x04 */ f32 unk04;
    /* 0x08 */ f32 unk08;
    /* 0x0C */ f32 unk0C;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  pad1D[0x7];
} Floor150B7B40;

extern f32 D_8009FD30[3];
extern f32 D_8009FD3C[3];
extern f32 D_8009FD48[3];

void func_15142314(s32, s32, f32 *);
void func_15143134(f32 *, f32 *, s32);
void func_1515C244(struct127 *, f32 *, f32 *, f32 *);
void func_150B82D0(Vec3_150B7B40 *, f32 *, f32 *, f32 *, u8, s32);
void func_150B85C0(Vec3_150B85C0 *, f32 *, f32 *, u8, s32);
void func_1504715C(Floor150B7B40 *, struct127 *);
s32 func_15046C80(Vec3_150B7B40 *, s32, f32, Floor150B7B40 *);
void func_1514C678(f32, f32, f32, f32, s32, s32, s32, s32, s32, f32, s32, u8);
s32 func_15132A4C(void *, s32, s32, s32, u8, s32);
void func_15151A38(Burst150B7B40 *, u8, s32);
void func_150B7B40(struct127 *arg0, u8 arg1, s32 arg2);

void func_15143794(s16, s16, f32, f32 *);
void *func_1518A3C0(f32 *, f32 *, f32, f32 *, f32 *, f32, f32, u8, s16, u8, s32, u8, s32);
void func_150B82D0(Vec3_150B7B40 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, u8 arg4, s32 arg5);


typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
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
    Vec3_150B85C0 unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[9];
} Fx150B85C0;

extern void *func_15130374(void *, u8, s32, u8, s32);
void func_150B85C0(Vec3_150B85C0 *arg0, f32 *arg1, f32 *arg2, u8 arg3, s32 arg4);

typedef struct {
    s32 unk00;
    s32 unk04;
    s16 unk08;
    s16 unk0A;
    s32 unk0C;
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
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    u8 unk65;
    u8 unk66;
    u8 pad67[0x9];
} Fx150B879C;

extern void func_15143874(s16, f32, f32 *, f32 *);
void *memcpy(void *, const void *, u32);
s32 func_150B879C(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, s32 arg6, s32 arg7, s16 arg8, s32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE);
