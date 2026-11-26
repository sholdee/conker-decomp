#ifndef CONKER_TEXT_H
#define CONKER_TEXT_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "text": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   game/Text/FontLoad/fontload.c
 *   game/Text/TextInit/textinit.c
 *   game/Text/FixedFont/fixedfont.c
 *   game/Text/FontDraw/fontdraw.c
 *   game/Text/TextQueue/textqueue.c
 *   game/Text/ContextText/contexttext.c
 *   game/Text/TextBoxCreate/textboxcreate.c
 *   game/Text/TextBox/textbox.c
 *   game/Text/AmountText/amounttext.c
 */

/* game/Text/FontLoad/fontload.c */
void func_15015910(void);
void text_loadFont(s16 font);
void text_unpackGlyph(u8 *record, s32 glyph, s32 font);

/* game/Text/TextInit/textinit.c */
void text_resetContextText(void);
void text_initAvailable(void);
extern u8 gAvailableTexts[9]; // bit set over the 0x43 text ids

/* game/Text/FixedFont/fixedfont.c */
Gfx *text_setupFixedFont(Gfx *gfx);
Gfx *text_drawFixedString(Gfx *gfx, s32 x, s32 y, u8 *str);
s32 text_findFixedGlyph(u8 ch);
Gfx *text_drawFixedGlyph(Gfx *gfx, s32 x, s32 y, s32 glyph);

/* game/Text/FontDraw/fontdraw.c */
void text_getIconSize(s32 glyph, s32 *width, s32 *height, f32 *yOffset, s32 *noCombine, f32 *scale);
Gfx *text_drawString(Gfx *gfx, f32 x, f32 y, u8 *str, u8 r, u8 g, u8 b, u8 a, f32 scaleX, f32 scaleY, s32 len);
void text_measureString(u8 *str, s32 *width, s32 *height, s32 *depth);
s32 text_findGlyph(u8 ch);
extern u8 D_800CBD4C;  // cleared by func_15015910
extern u16 gTextAnimClock; // animation clock (high bits pick icon frames)

/* game/Text/TextQueue/textqueue.c */
void text_clearQueue(void);
void text_setFlags(u8 flags);
void text_printAt(s32 x, s32 y, u8 flags, char *format, ...);
void text_print(char *format, ...);
void text_parseFormat(u8 *format, s32 *argumentWords);
void text_setScale(f32 scale);
void text_setAnchor(Actor *object, s32 yOffset);
void text_setPosition(s16 x, s16 y);
void text_setColor(u8 red, u8 green, u8 blue, u8 alpha);
Gfx *text_drawQueue(Gfx *gfx);
extern Gfx gText2DSetupDL[]; // render setup for 2D text and sprites
void text_ringInit(MsgRing *ring, u8 *buffer, s32 capacity);
s32 text_ringWrite(u8 *buffer, s32 capacity, s32 offset, u8 *src, s32 size);
s32 text_ringRead(u8 *buffer, s32 capacity, s32 offset, u8 *dst, s32 size);
s32 text_ringSkip(u8 *buffer, s32 capacity, s32 offset, s32 size);
s32 text_ringPush(MsgRing *ring, u8 *message, s32 size);
s32 text_ringPop(MsgRing *ring, u8 *dst, s32 dstSize);

/* game/Text/ContextText/contexttext.c */
/* (Actor *obj, TriggerZone *zone, s32 slot, s32 textId); declared without a prototype: game/Zone/ZoneState/zonestate.c passes 5 arguments */
void text_queueObjectText();
void text_lockObjectText(s32 objIndex, s32 slot, s32 textId, s32 arg3);
void text_clearObjectText(s32 objIndex);
void text_getBitPos(s32 n, s32 *byteIndex, s32 *bitMask);
s32 text_isAvailable(s32 textId);
void text_playNextSound(void);
void text_soundCallback(TextBoxConfig *config, s32 page, s32 event);
void text_openContextText(s32 arg0);
void text_closeContextText(void);
void text_updateContextText(void);
s32 text_isContextTextClosed(void);
extern u8 gContextTextId;
extern u8 gContextTextOpen;
extern TextResource *gContextText; // current text
extern TextResource *gContextHeaderText; // header text prepended to it, or NULL
extern u8 *gContextCombinedText; // header + page break + current text
extern TextBox *gContextTextBox; // open text box
extern TextBoxConfig gContextTextBoxConfig;
extern f32 gContextTextScreenX;
extern f32 gContextTextScreenY;
extern f32 gContextTextAngle;
extern u8 gContextSoundNext;
extern u8 gContextSoundCount;
extern u16 *gContextSounds; // sound effects voiced while the text types out

/* game/Text/TextBoxCreate/textboxcreate.c */
s32 text_splitPages(u8 *text);
TextBox *text_createBox(u32 msgIndex, s32 objIndex, s32 arg2,
                       void (*callback)(TextBoxConfig *config, s32 page, s32 state),
                       TextBoxConfig *suppliedConfig, s32 owner);

/* game/Text/TextBox/textbox.c */
void text_updateBox(TextBox *box);
Gfx *text_drawBox(Gfx *gfx, TextBox *box, s32 arg2);
u8 *text_getPage(TextBoxConfig *config, s32 index, s16 source);
Gfx *text_drawBoxText(Gfx *gfx, TextBox *box, u8 r, u8 g, u8 b, u8 a);
void text_placeBox(TextBox *box);
void text_clampBoxToView(TextBox *box, f32 *x, f32 *y, f32 *angle);
void func_1516D2D8(void);
void text_removeBox(TextBox *box);
void text_closeBoxSkipPages(TextBox *box);
TextBox *text_closeBoxAfterPage(TextBox *box);
TextBox *text_closeBoxNow(TextBox *box);

/* game/Text/AmountText/amounttext.c */
void func_151D6970(Actor *character, void *obj);
void func_151D69B4(WorldObject *pickup, Actor *player);
void text_spawnAmount(Actor *owner, u8 arg1, s32 arg2);
s32 func_151D6E60(ModelEffect *fx);
s32 text_updateAmount(TextObj *obj);
void text_handleAmountMessage(TextObj *obj, u8 *payload, u8 type);
Gfx *func_151D710C(Gfx *gfx, ModelEffect *fx, s32 view, u8 *needSync, u8 *proceed);

#endif
