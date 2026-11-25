#include <ultra64.h>
#include "functions.h"
#include "variables.h"

#define OBJTYPE_TEXTBOX 0x51

/* TextBoxConfig.flags */
#define TEXTBOX_BUBBLES         0x2     /* body drawn with D_8009056C plus three fading D_8009058C circles */
#define TEXTBOX_INPUT           0x10    /* A/B buttons advance or close the box */
#define TEXTBOX_INPUT_CHECK     0x20    /* ... but only while func_151F2CDC() returns 0 */
#define TEXTBOX_NO_B_SKIP       0x40    /* B does not skip to the end */
#define TEXTBOX_B_RESTARTS      0x80    /* B on the last page restarts from the first */
#define TEXTBOX_CLOSE           0x100   /* request: close after the current page */
#define TEXTBOX_HOLD            0x200   /* keep the current page open */
#define TEXTBOX_SKIP_ANIM       0x400   /* skip every timed transition */
#define TEXTBOX_CB_CLOSED       0x1000  /* set while the callback runs: box closed */
#define TEXTBOX_CB_PAGE_ENDED   0x2000  /* set while the callback runs: a page was left */
#define TEXTBOX_CB_PAGE_SHOWN   0x4000  /* set while the callback runs: a page is fully shown */
#define TEXTBOX_SCREEN_POS      0x8000  /* positioned by screen percentages instead of a world point */
#define TEXTBOX_CLAMP           0x10000 /* keep a screen-positioned box inside the view bounds */

/* Message table entry (D_800DD254 / D_800DD258). */
typedef struct {
    u8 *data;
    s32 unk4;
} MessageEntry;

/* Text box setup: a message-table entry, or one supplied by the caller (see func_1516A7B0). */
typedef struct TextBoxConfig {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 text;            /* message table index, or the string itself when supplied */
    /* 0x08 */ u8 numPages;         /* strings in the text, separated by NULs */
    /* 0x09 */ u8 objIndex;         /* D_800CC2D0 object the box belongs to */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
    /* 0x0C */ f32 worldX;          /* projected to screen when not TEXTBOX_SCREEN_POS */
    /* 0x10 */ f32 worldY;
    /* 0x14 */ f32 worldZ;
    /* 0x18 */ f32 *screenXPercent; /* TEXTBOX_SCREEN_POS placement */
    /* 0x1C */ f32 *screenYPercent;
    /* 0x20 */ f32 *anglePercent;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ u16 duration;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 unk2B;            /* box is removed unless D_800C35EA == 1 */
    /* 0x2C */ void (*callback)(struct TextBoxConfig *config, s32 page, s32 state);
} TextBoxConfig;

/* Object type OBJTYPE_TEXTBOX, 0x74 bytes. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0xC];
    /* 0x0D */ u8 source;           /* 0: D_800DD254 text, 1: D_800DD258 text, 2: caller string */
    /* 0x0E */ u8 visible;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ TextBoxConfig *config;
    /* 0x14 */ u8 unk14;            /* set for caller-supplied configs; disables button input */
    /* 0x15 */ u8 state;            /* TEXTBOX_STATE_* */
    /* 0x16 */ s16 timer;
    /* 0x18 */ s16 fadeTimer;
    /* 0x1A */ u8 textAlpha;
    /* 0x1B */ u8 bubbleAlpha[3];
    /* 0x1E */ u8 bubbleIndex;
    /* 0x1F */ u8 placed;           /* screen position taken from the world point */
    /* 0x20 */ s8 page;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ u8 *text;            /* current page */
    /* 0x28 */ f32 scale;
    /* 0x2C */ f32 scaleSpeed;
    /* 0x30 */ f32 halfTextWidth;
    /* 0x34 */ f32 halfTextHeight;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 height;
    /* 0x40 */ f32 x;               /* anchor (tip of the tail) */
    /* 0x44 */ f32 y;
    /* 0x48 */ f32 bodyX;           /* scaled body position */
    /* 0x4C */ f32 bodyY;
    /* 0x50 */ f32 centerX;         /* unscaled body position: the text origin */
    /* 0x54 */ f32 centerY;
    /* 0x58 */ f32 angle;           /* direction from the anchor to the body, radians */
    /* 0x5C */ f32 anchorX;
    /* 0x60 */ f32 anchorY;
    /* 0x64 */ f32 baseX;
    /* 0x68 */ f32 baseY;
    /* 0x6C */ f32 yaw;             /* object heading relative to the camera, degrees */
    /* 0x70 */ f32 bubbleX;
} TextBox;

/* TextBox.state */
#define TEXTBOX_STATE_INIT      0x1
#define TEXTBOX_STATE_GROW      0x2
#define TEXTBOX_STATE_SETTLE    0x4
#define TEXTBOX_STATE_BUBBLES   0x8
#define TEXTBOX_STATE_TEXT_IN   0x10
#define TEXTBOX_STATE_SHOW      0x20
#define TEXTBOX_STATE_TEXT_OUT  0x40
#define TEXTBOX_STATE_SHRINK    0x80

extern MessageEntry *D_800DD254;
extern MessageEntry *D_800DD258;

extern void func_150428D4(u8 *, s32 *, s32 *, s32 *);
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source);

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8;
    u8 unk9;
    char padA[0x26 - 0xA];
    u16 unk26;
    char pad28[3];
    u8 unk2B;
    void (*unk2C)(void *, s32, s32);
} Struct1516AA70_Data;

typedef struct {
    char pad0[0xD];
    u8 unkD;
    u8 unkE;
    u8 unkF;
    Struct1516AA70_Data *unk10;
    u8 unk14;
    u8 unk15;
    s16 unk16;
    s16 unk18;
    u8 unk1A;
    u8 unk1B[3];
    u8 unk1E;
    u8 unk1F;
    s8 unk20;
    char pad21[3];
    u8 *unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    char pad38[8];
    f32 unk40;
    f32 unk44;
    char pad48[0x14];
    f32 unk5C;
    f32 unk60;
    char pad64[0xC];
    f32 unk70;
} Struct1516AA70;

extern u8 D_800CC4B3[][0x32C];
s32 func_151F2CDC(void);
void func_1516CC58();

void func_1516AA70(Struct1516AA70 *arg0) {
    Struct1516AA70_Data *data;
    struct108 *sp48;
    s32 sp44;
    s32 sp40;
    s32 sp3C;
    s32 flag;
    s32 done;

    sp48 = D_800DBFF0;
    flag = arg0->unk10->unk0 & 0x400;
    if (arg0->unk10->unk2B != 0 && D_800C35EA != 1) {
        arg0->unk10->unk8 = 0;
        func_1516972C((struct102 *)arg0);
        return;
    }
    arg0->unk40 = arg0->unk5C;
    arg0->unk44 = arg0->unk60;
    switch (arg0->unk15) {
    case 0x1:
        arg0->unk16 = 0;
        arg0->unk18 = 0;
        if (flag) {
            arg0->unk2C = 1.0f;
        } else {
            arg0->unk2C = 0.3333333433f;
        }
        if (arg0->unk10->unk0 & 2) {
            arg0->unk1B[2] = 0;
            arg0->unk1B[1] = 0;
            arg0->unk1B[0] = 0;
            arg0->unk1E = 2;
            arg0->unk16 = 4;
            arg0->unk18 = 10;
            arg0->unk15 = 8;
            arg0->unk28 = 1.0f;
            func_1516CC58(arg0);
            arg0->unkE = 1;
            arg0->unk70 = arg0->unk5C;
        } else {
            arg0->unk15 = 2;
        }
        arg0->unk30 = 1.0f;
        arg0->unk34 = 1.0f;
        arg0->unk1A = 0;
        break;
    case 0x2:
        func_150428D4(arg0->unk24, &sp44, &sp40, &sp3C);
        arg0->unk30 = sp44 >> 1;
        arg0->unk34 = sp40 >> 1;
        arg0->unk28 += arg0->unk2C * D_800BE9A4;
        if (flag) {
            arg0->unk18 = 0;
        }
        if (arg0->unk28 > 0.75f && arg0->unk18 == 0) {
            arg0->unk18 = 10;
        }
        if (arg0->unk18 != 0) {
            arg0->unk18 -= D_800BE9A0;
            arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
            arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        }
        if (arg0->unk28 > 1.0f) {
            arg0->unk28 = 1.0f;
            arg0->unk15 = 4;
            arg0->unk16 = 180;
        }
        arg0->unkE = 1;
        break;
    case 0x4:
        if (flag) {
            arg0->unk16 = 0;
        }
        arg0->unk16 -= D_800BE9A0 * 45;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        arg0->unk18 -= D_800BE9A0;
        arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
        arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        if (arg0->unk16 == 0) {
            arg0->unk15 = 0x10;
        }
        break;
    case 0x8:
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        arg0->unk1B[arg0->unk1E] = (4 - (f32)arg0->unk16) / 4 * 170.0f;
        if (arg0->unk16 == 0) {
            if (--arg0->unk1E > 100) {
                arg0->unk15 = 0x10;
                arg0->unk18 = 10;
            } else {
                arg0->unk16 = 4;
            }
        }
        break;
    case 0x40:
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        if (flag) {
            arg0->unk16 = 0;
        }
        arg0->unk1A = arg0->unk16 / 10.0f * 255.0f;
        if (arg0->unk16 == 0) {
            arg0->unk20++;
            if (arg0->unk20 < arg0->unk10->unk8) {
                arg0->unk24 = func_1516C878((TextBoxConfig *)arg0->unk10, arg0->unk20, arg0->unkD);
                arg0->unk18 = 10;
                arg0->unk15 = 0x10;
                arg0->unk10->unk0 |= 0x2000;
                if (arg0->unk10->unk2C != NULL) {
                    arg0->unk10->unk2C(arg0->unk10, arg0->unk20 - 1, arg0->unk15);
                }
                arg0->unk10->unk0 &= ~0x2000;
            } else {
                arg0->unk18 = 5;
                arg0->unk15 = 0x80;
                arg0->unk28 = 1.0f;
                arg0->unk2C = 0.25f;
            }
        }
        if (!flag) {
            break;
        }
    case 0x10:
        func_150428D4(arg0->unk24, &sp44, &sp40, &sp3C);
        arg0->unk30 = sp44 >> 1;
        arg0->unk34 = sp40 >> 1;
        arg0->unk18 -= D_800BE9A0;
        arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
        if (flag) {
            arg0->unk18 = 0;
        }
        arg0->unk1A = (10.0f - arg0->unk18) / 10.0f * 255.0f;
        if (arg0->unk18 == 0) {
            arg0->unk16 = arg0->unk10->unk26;
            arg0->unk15 = 0x20;
            arg0->unk10->unk0 |= 0x4000;
        }
        if (!flag) {
            break;
        }
    case 0x20: {
        s32 bits;
        done = 0;
        if ((u16)(arg0->unk10->unk0 & 0x4000)) {
            if (arg0->unk10->unk2C != NULL) {
                arg0->unk10->unk2C(arg0->unk10, arg0->unk20, arg0->unk15);
            }
            arg0->unk10->unk0 &= ~0x4000;
        }
        arg0->unk16 -= D_800BE9A0;
        arg0->unk16 = (arg0->unk16 < 0) ? 0 : arg0->unk16;
        if ((u16)(arg0->unk10->unk0 & 0x10) && arg0->unk14 == 0) {
            if (!(arg0->unk10->unk0 & 0x20) || func_151F2CDC() == 0) {
                if ((arg0->unk10->unk0 & 0x80) && arg0->unk20 + 1 == arg0->unk10->unk8) {
                    if (D_800BE710[0] & 0x4000) {
                        arg0->unk20 = -1;
                        done = 1;
                        D_800D1994 = 0;
                    } else if (D_800BE710[0] & 0x8000) {
                        done = 1;
                    }
                } else if (D_800BE710[0] & 0x8000) {
                    done = 1;
                } else if (!(arg0->unk10->unk0 & 0x40) && (D_800BE710[0] & 0x4000)) {
                    done = 1;
                    arg0->unk20 = arg0->unk10->unk8;
                }
            }
        }
        bits = arg0->unk10->unk0 & 0xF00;
        if (bits & 0x100) {
            arg0->unk10->unk0 &= ~0x100;
            done = 1;
            bits = arg0->unk10->unk0 & 0xF00;
        }
        if (!(bits & 0x200) && done) {
            arg0->unk16 = 10;
            arg0->unk15 = 0x40;
        }
    }
        break;
    case 0x80:
        if (arg0->unk10->unk0 & 2) {
            arg0->unk18 -= D_800BE9A0;
            arg0->unk18 = (arg0->unk18 < 0) ? 0 : arg0->unk18;
            arg0->unk1B[0] = arg0->unk18 / 5.0f * 85.0f;
            arg0->unk1B[1] = arg0->unk1B[0];
            arg0->unk1B[2] = arg0->unk1B[0];
        } else {
            arg0->unk28 -= arg0->unk2C * D_800BE9A4;
        }
        if (arg0->unk28 < 0.0f || arg0->unk18 == 0) {
            arg0->unk28 = 0.0f;
            arg0->unk15 = 0;
            arg0->unk20 = 0;
            arg0->unk10->unk0 |= 0x1000;
            if (arg0->unk10->unk2C != NULL) {
                arg0->unk10->unk2C(arg0->unk10, arg0->unk20, arg0->unk15);
            }
            arg0->unk10->unk0 &= ~0x1000;
            D_800CC4B3[arg0->unk10->unk9][0] = 0;
            *(s16 *)((u8 *)sp48 + 0x6CC) = 0;
            arg0->unk10->unk8 = 0;
            func_1516972C((struct102 *)arg0);
        }
        break;
    }
    func_1516CC58(arg0);
}

typedef struct {
    u8 pad0[6];
    s16 w;
    s16 h;
    u8 alpha;
    u8 r;
    u8 g;
    u8 flags;
    u8 padE[2];
    void *tex;
} Sprite_1516B6BC;

typedef struct {
    s32 unk0;
    char pad4[0x24];
    u8 unk28;
    u8 unk29;
    u8 unk2A;
} Data_1516B6BC;

typedef struct {
    char pad0[0xE];
    u8 unkE;
    char padF;
    Data_1516B6BC *unk10;
    char pad14;
    u8 unk15;
    char pad16[4];
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    char pad1E[0xA];
    f32 unk28;
    char pad2C[0xC];
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    char pad50[8];
    f32 unk58;
    f32 unk5C;
    char pad60[0x10];
    f32 unk70;
} Obj_1516B6BC;
/* Sprite texture header (D_8009055C body, D_8009056C bubble body, D_8009057C / D_8009059C tails,
 * D_8009058C bubble circle). */
typedef struct {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 width;
    /* 0x8 */ u16 height;
} TextBoxTexture;

extern TextBoxTexture D_8009055C;
extern TextBoxTexture D_8009057C;
#define WGFX1516B6BC(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (u32)(a); _g->words.w1 = (u32)(b); }
extern TextBoxTexture D_8009056C;
extern TextBoxTexture D_8009058C;
extern TextBoxTexture D_8009059C;
extern s32 D_800A4AC8[][2];
extern s32 D_800A4B90;
extern s32 D_800A4B94;
extern s32 D_800BE9C8[];
extern s32 D_800BEBA4;
extern u8 D_800D2DA8;
extern u8 D_800D2DA9;
extern u8 D_800D2DAA;
extern u8 D_800E0BDC;
Gfx *func_15094F70(Gfx *, TextBoxTexture *, s32, Sprite_1516B6BC *, s32, s32, s32, s32, s32);
Gfx *func_15095A48(Gfx *, Sprite_1516B6BC *, f32, f32);
Gfx *func_1516C934(Gfx *, Obj_1516B6BC *, s32, s32, s32, s32);

Gfx *func_1516B6BC(Gfx *gfx, Obj_1516B6BC *obj, s32 arg2) {
    Sprite_1516B6BC sprite;
    s32 pad80;
    f32 xoff;
    f32 yoff;
    s32 flag;
    s32 scale;
    s32 overflow;
    Gfx *start;
    f32 tw;
    f32 th;

    start = gfx;
    if (obj->unk10->unk0 & 4) {
        flag = 1;
    } else {
        flag = 0;
    }
    if (obj->unkE == 0) {
        return gfx;
    }
    if (D_800E0BDC == 0) {
        return gfx;
    }
    D_800D2DAA = 0;
    D_800D2DA9 = D_800D2DAA;
    D_800D2DA8 = D_800D2DA9;
    scale = D_800E0BDC + 1;
    sprite.flags = 0;
    sprite.alpha = (64 * scale) >> 8;
    tw = (f32)D_8009055C.width;
    th = (f32)D_8009055C.height;
    sprite.w = (2.0f * obj->unk38 + 1.0f) / (f32)D_8009055C.width * 2355.19995117f * obj->unk28;
    sprite.h = (obj->unk3C + 1.0f) / (f32)D_8009055C.height * 4259.83984375f * obj->unk28;
    xoff = obj->unk38 / (f32)D_8009055C.width * (f32)D_8009057C.width * 0.25f * obj->unk28;
    yoff = obj->unk3C / (f32)D_8009055C.height * (f32)D_8009057C.height * 0.5f * obj->unk28;
    obj->unk48 = cosf(obj->unk58 + 3.14159274101f) * (obj->unk38 * 0.5f) * 1.14999997616f * obj->unk28 + obj->unk40 - xoff;
    obj->unk4C = sinf(obj->unk58 + 3.14159274101f) * (obj->unk3C * 0.5f) * 1.03999996185f * obj->unk28 + obj->unk44 - yoff;
    WGFX1516B6BC(gfx++, 0xE7000000, 0);
    WGFX1516B6BC(gfx++, 0xFC30B261, 0xFF67FFFF);
    WGFX1516B6BC(gfx++, 0xEF002C0F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
    if (obj->unk10->unk0 & 2) {
        sprite.alpha = (f32)obj->unk1B * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    if (!flag) {
        gfx = func_15095A48(gfx, &sprite, obj->unk48 + 5.0f, obj->unk4C + -5.0f);
    }
    if (obj->unk58 > 0.0f && obj->unk58 < 3.14159274101f) {
        sprite.flags &= ~2;
    } else {
        sprite.flags |= 2;
    }
    if (obj->unk58 > 1.5707963705f && obj->unk58 < 4.71238899231f) {
        sprite.flags |= 1;
    } else {
        sprite.flags &= ~1;
    }
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    if (obj->unk10->unk0 & 2) {
        if (obj->unk15 == 1) {
            obj->unk70 = obj->unk5C;
        }
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = (f32)obj->unk1C * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        if (obj->unk58 > 1.5707963705f) {
            obj->unk70 = obj->unk70 + ((obj->unk40 + xoff) - obj->unk70) * 0.300000011921f;
            gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        } else {
            obj->unk70 = obj->unk70 + ((obj->unk40 - xoff) - obj->unk70) * 0.300000011921f;
            gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        }
        sprite.w /= 2;
        sprite.h /= 2;
        sprite.alpha = (f32)obj->unk1D * 64.0f / 255.0f;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - 2.5f, obj->unk44 + -2.5f);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + 2.5f, obj->unk44 + -2.5f);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (!flag) {
            if (obj->unk58 > 1.5707963705f) {
                gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff - 2.5f, obj->unk44 - yoff + -2.5f);
            } else {
                gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff + 2.5f, obj->unk44 - yoff + -2.5f);
            }
        }
    }
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    if (flag) {
        D_800D2DA8 = D_800D2DA9 = D_800D2DAA = 0;
    } else {
        D_800D2DA8 = obj->unk10->unk28;
        D_800D2DA9 = obj->unk10->unk29;
        D_800D2DAA = obj->unk10->unk2A;
    }
    sprite.flags = 0;
    if (flag) {
        sprite.alpha = 0x55;
    } else {
        sprite.alpha = 0xFF;
    }
    sprite.alpha = (sprite.alpha * scale) >> 8;
    WGFX1516B6BC(gfx++, 0xE7000000, 0);
    WGFX1516B6BC(gfx++, 0xFC30B261, 0xFF67FFFF);
    WGFX1516B6BC(gfx++, 0xEF002C0F, D_800A4AC8[25][0] | D_800A4AC8[25][1] | 4);
    if (obj->unk58 > 0.0f && obj->unk58 < 3.14159274101f) {
        sprite.flags &= ~2;
    } else {
        sprite.flags |= 2;
    }
    if (obj->unk58 > 1.5707963705f && obj->unk58 < 4.71238899231f) {
        sprite.flags |= 1;
    } else {
        sprite.flags &= ~1;
    }
    sprite.w = obj->unk38 / (f32)D_8009055C.width * 2355.19995117f * obj->unk28;
    sprite.h = obj->unk3C / (f32)D_8009055C.height * 4259.83984375f * obj->unk28;
    if (obj->unk10->unk0 & 2) {
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = obj->unk1C;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15095A48(gfx, &sprite, obj->unk70, obj->unk44 - yoff);
        sprite.w /= 2;
        sprite.h /= 2;
        gfx = func_15094F70(gfx, &D_8009058C, 0, &sprite, 0, 0, 0, 2, 3);
        sprite.alpha = obj->unk1D;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40, obj->unk44);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40, obj->unk44);
        }
    } else if (obj->unk10->unk0 & 8) {
        gfx = func_15094F70(gfx, &D_8009059C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff, obj->unk44 - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff, obj->unk44 - yoff);
        }
    } else {
        gfx = func_15094F70(gfx, &D_8009057C, 0, &sprite, 0, 0, 0, 2, 3);
        if (obj->unk58 > 1.5707963705f) {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 + xoff, obj->unk44 - yoff);
        } else {
            gfx = func_15095A48(gfx, &sprite, obj->unk40 - xoff, obj->unk44 - yoff);
        }
    }
    sprite.w = 2.0f * obj->unk38 / (f32)D_8009055C.width * 2355.19995117f * obj->unk28;
    sprite.h = obj->unk3C / (f32)D_8009055C.height * 4259.83984375f * obj->unk28;
    if (obj->unk10->unk0 & 2) {
        sprite.alpha = obj->unk1B;
        sprite.alpha = (sprite.alpha * scale) >> 8;
        gfx = func_15094F70(gfx, &D_8009056C, 0, &sprite, 0, 0, 0, 2, 3);
    } else {
        gfx = func_15094F70(gfx, &D_8009055C, 0, &sprite, 0, 0, 0, 2, 3);
    }
    gfx = func_15095A48(gfx, &sprite, (s32)obj->unk48, (s32)obj->unk4C);
    overflow = (D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3)) ? 1 : 0;
    if (overflow != 0) {
        return start;
    }
    return func_1516C934(gfx, obj, 0, 0, 0, (obj->unk1A * scale) >> 8);
}

/* Returns the start of page `index` of a config's text: pages are separated by runs of NULs. */
u8 *func_1516C878(TextBoxConfig *config, s32 index, s16 source) {
    u8 *str;
    u8 *uninit;
    s32 i;

    switch (source) {
    case 0:
        str = D_800DD254[config->text].data;
        break;
    case 1:
        str = D_800DD258[config->text].data;
        break;
    case 2:
        str = (u8 *)config->text;
        break;
    default:
        str = uninit; // matching: the original reads an uninitialised stack local here
        break;
    }
    for (i = 0; i < index; i++) {
        while (*str != 0) {
            str++;
        }
        while (*str == 0) {
            str++;
        }
    }
    return str;
}

typedef struct {
    u32 flags;
} Struct1516C934Font;

typedef struct {
    u8 pad0[0x10];
    Struct1516C934Font *unk10;
    u8 pad14[0x10];
    u8 *unk24;
    u8 pad28[0x8];
    f32 unk30;
    f32 unk34;
    u8 pad38[0x18];
    f32 unk50;
    f32 unk54;
} Struct1516C934;

Gfx *func_150417AC(Gfx *, f32, f32, u8 *, u8, u8, u8, u8, f32, f32, s32);
extern u8 D_800859A0[];

Gfx *func_1516C934(gfx, arg1, r, g, b, a)
    Gfx *gfx;
    Obj_1516B6BC *arg1;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
{
#define ARG1 ((Struct1516C934 *)arg1)
    Gfx *original;
    u8 line[84];
    Struct1516C934 *text;
    f32 offset;
    s32 i;
    s32 n;
    s32 first;

    i = 0;
    n = 0;
    text = ARG1;
    original = gfx;
lbl:
    {
        f32 x;
        f32 y;

        x = ARG1->unk50 - ARG1->unk30;
        y = ARG1->unk54 - ARG1->unk34;
        gSPDisplayList(gfx++, D_800859A0);
        first = 1;
        while (text->unk24[i] == '\n') {
            i++;
        }
        do {
            if ((D_800BEBA4 < (((s32)gfx - D_800BE9C8[D_800BE9C0]) >> 3) ? 1 : 0) != 0) {
                return original;
            }
            if (1) {
            }
            line[n] = text->unk24[i];
            if (line[n] == '\n') {
                line[n] = 0;
            }
            if (line[n] == 0) {
                s32 width;
                s32 height;
                s32 advance;

                func_150428D4(line, &width, &height, &advance);
                if (!first) {
                    y += (f32)height;
                } else {
                    first = 0;
                }
                if (text->unk10->flags & 0x20000) {
                    offset = ARG1->unk30 - (f32)(width >> 1) - 1.0f;
                } else {
                    offset = 0.0f;
                }
                if (text->unk10->flags & 0x40000) {
                    gfx = func_150417AC(gfx, x + offset + 1.0f, y, line, 0xFF, 0xFF, 0xFF, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else if (!(text->unk10->flags & 4)) {
                    gfx = func_150417AC(gfx, x + offset + 1.0f, y, line, 0, 0, 0, a, 4096.0f, 4096.0f, n);
                    n = 0;
                } else {
                    gfx = func_150417AC(gfx, x + offset, y, line, r, g, b, a, 4096.0f, 4096.0f, n);
                    n = 0;
                }
                y += (f32)advance;
            } else {
                n++;
            }
        } while (ARG1->unk24[i++] != 0);
    }
    return gfx;
#undef ARG1
}

/* What D_800BE628 points to. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 width;
    /* 0x08 */ f32 height;
    /* 0x0C */ u8 padC[0x18];
    /* 0x24 */ f32 minY;
    /* 0x28 */ f32 maxY;
    /* 0x2C */ f32 minX;
    /* 0x30 */ f32 maxX;
} TextBoxView;

#define VIEW ((TextBoxView *)D_800BE628)

extern f32 cosf(f32);
extern f32 sinf(f32);
void func_1501A764(s16 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4, f32 *arg5, f32 *arg6);
void func_1516D0CC(TextBox *box, f32 *x, f32 *y, f32 *angle);

/* Places a text box: its anchor comes from screen percentages or from its object's projected position,
 * and the body is set at an angle derived from the object's heading. */
void func_1516CC58(TextBox *box) {
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    TextBoxConfig *config;
    s32 xoff;
    s32 yoff;
    f32 scale;
    f32 angle;

    config = box->config;
    if (config->flags & TEXTBOX_SCREEN_POS) {
        scale = *config->screenXPercent * 0.01f;
        box->anchorX = VIEW->width * scale;
        scale = *config->screenYPercent * 0.01f;
        scale = 1.0f - scale;
        box->anchorY = VIEW->height * scale;
        angle = *config->anglePercent * 0.01f;
        angle = (angle < 0.0f) ? 0.0f : ((angle > 1.0f) ? 1.0f : angle);
        angle = 70.0f * angle + 55.0f;
        angle *= 0.017453292f;
        box->angle = angle;
        if (config->flags & TEXTBOX_CLAMP) {
            func_1516D0CC(box, &box->anchorX, &box->anchorY, &box->angle);
        }
    } else {
        func_1501A764(0, config->worldX, config->worldY, config->worldZ, &screenX, &screenY, &screenZ);
        if (box->placed == 0) {
            box->baseX = screenX;
            box->baseY = screenY;
            box->yaw = D_800CC2D0[box->config->objIndex].unk40 - D_800DBFF0->unk37C - 180.0f;
            box->placed = 1;
        }
        angle = box->yaw;
        box->anchorX = box->baseX;
        box->anchorY = box->baseY;
        if (angle < 0.0f) {
            angle += 360.0f;
        }
        if (angle > 180.0f) {
            angle -= 360.0f;
        }
        if (angle > 90.0f) {
            angle = 180.0f - angle;
        } else if (angle < -90.0) {
            angle = -180.0f - angle;
        }
        angle = angle / -180.0f + 0.5f;
        angle = 70.0f * angle + 55.0f;
        angle *= 0.017453292f;
        box->angle = angle;
        func_1516D0CC(box, &box->anchorX, &box->anchorY, &box->angle);
    }
    box->x = box->anchorX;
    box->y = box->anchorY;
    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f;
    yoff = box->height / D_8009055C.height * D_8009057C.height * 0.5f;
    box->centerX = (s32)(cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f + box->anchorX - xoff);
    box->centerY = sinf(box->angle + 3.1415927f) * (box->height * 0.5f) * 1.04f + box->anchorY - yoff;
}

/* Moves a box's anchor so the whole box stays inside the view bounds, then snaps it to whole pixels. */
void func_1516D0CC(TextBox *box, f32 *x, f32 *y, f32 *angle) {
    f32 top;
    f32 anchorY;
    f32 left;
    f32 right;
    f32 xoff;
    f32 halfW;
    f32 h;
    f32 bodyX;

    xoff = box->width / D_8009055C.width * D_8009057C.width * 0.25f * box->scale;
    bodyX = *x + cosf(box->angle + 3.1415927f) * (box->width * 0.5f) * 1.15f * box->scale - xoff;
    halfW = box->width * 0.5f;
    h = box->height;
    anchorY = *y;
    top = *y - h;
    left = bodyX - halfW;
    right = halfW + bodyX;
    if (box != NULL) {
        if (top < VIEW->minY) {
            *y = VIEW->minY + h;
        } else if (VIEW->maxY < anchorY) {
            *y = VIEW->maxY;
        }
        if (left < VIEW->minX) {
            *x -= left - VIEW->minX;
        } else if (VIEW->maxX < right) {
            *x += VIEW->maxX - right;
        }
        *x = (s32)*x;
        *y = (s32)*y;
    }
}

void func_1516D2D8(void) {
}

/* Removes a text box object immediately. */
void func_1516D2E0(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->numPages = 0;
        func_1516972C((struct102 *)box);
        box->state = 0;
    }
}

/* Asks a text box to close, skipping its remaining pages. */
void func_1516D328(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        box->page = box->config->numPages;
    }
}

/* Asks a text box to close after its current page; returns it, or NULL if it isn't one. */
TextBox *func_1516D378(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE;
        box->config->flags &= ~TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}

/* Asks a text box to close at once; returns it, or NULL if it isn't one. */
TextBox *func_1516D3C4(TextBox *box) {
    if (box != NULL && box->type == OBJTYPE_TEXTBOX) {
        box->config->flags |= TEXTBOX_CLOSE | TEXTBOX_SKIP_ANIM;
        return box;
    }
    return NULL;
}
