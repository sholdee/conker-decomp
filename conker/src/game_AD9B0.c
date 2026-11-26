#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 gContextTextId;
u8 gContextTextOpen;
TextResource *gContextText; // current text
TextResource *gContextHeaderText; // header text prepended to it, or NULL
u8 *gContextCombinedText;           // header + page break + current text
TextBox *gContextTextBox;      // open text box
TextBoxConfig gContextTextBoxConfig;
f32 gContextTextScreenX;
f32 gContextTextScreenY;
f32 gContextTextAngle;
u8 gContextSoundNext;
u8 gContextSoundCount;
u16 *gContextSounds;          // sound effects voiced while the text types out
static TriggerZone *gContextTextZone; /* zone to disable once the text box closes */

#include "functions.h"
#include "variables.h"

/* Byte-wide character in a text resource that separates two pages (see text_splitPages). */
#define TEXT_PAGE_BREAK 0xBD

/* Tests bit n of a byte-array bit set. */
#define BIT_IS_SET(bits, n) ((bits)[(n) >> 3] & (1 << ((n) & 7)))

s32 strlen(const char *);

/* Queues text textId on obj's text slot (0 or 1), unless a script has locked that slot. Slot 1 also
   records the trigger zone, which text_updateContextText disables once text 0x2B / 0x2C has been shown. */
void text_queueObjectText(obj, zone, slot, textId)
    Actor *obj;
    TriggerZone *zone;
    s32 slot;
    s32 textId;
{
    if (obj != NULL && obj->type != 0 && obj->playernumber != 0xFF) {
        if (textId == 0x2B || textId == 0x2C) {
            gContextTextId = textId;
            gContextTextZone = zone;
            textId = 0x2A;
        } else if (text_isAvailable(textId)) {
            gContextTextId = textId;
            if (!BIT_IS_SET(D_800D2E60, textId)) {
                textId = 0x1A;
            }
        }

        if (slot == 0) {
            if (!(obj->mv->context_a & 0x80)) {
                obj->mv->context_a = textId;
            }
        } else if (!(obj->mv->context_b & 0x80)) {
            obj->mv->context_b = textId;
            obj->mv->cont_zone_ind = zone - zones;
        }
    }
}

/* Script command: sets text slot 0 or 1 of object objIndex, locked against TriggerZones (bit 7). */
void text_lockObjectText(s32 objIndex, s32 slot, s32 textId, s32 arg3) {
    if (textId != 0) {
        textId |= 0x80;
    }
    if (slot == 0) {
        movement_obj[objIndex].mv->context_a = textId;
    } else {
        movement_obj[objIndex].mv->context_b = textId;
    }
}

/* Clears both of an object's text slots unless a script locked them. */
void text_clearObjectText(s32 objIndex) {
    Actor *obj = &movement_obj[objIndex];

    if (obj->mv->context_a != 0 && !(obj->mv->context_a & 0x80)) {
        obj->mv->context_a = 0;
    }
    if (obj->mv->context_b != 0 && !(obj->mv->context_b & 0x80)) {
        obj->mv->context_b = 0;
    }
}

/* Splits bit number n of a byte-array bit set into its byte index and bit mask. */
void text_getBitPos(s32 n, s32 *byteIndex, s32 *bitMask) {
    *bitMask = 1 << (n & 7);
    *byteIndex = n >> 3;
}

/* Returns whether textId's bit is set in D_800BE580. */
s32 text_isAvailable(s32 textId) {
    s32 byteIndex;
    s32 bitMask;

    text_getBitPos(textId, &byteIndex, &bitMask);
    if (D_800BE580[byteIndex] & bitMask) {
        return TRUE;
    }
    return FALSE;
}

/* Plays the next of the current text's sound effects. */
void text_playNextSound(void) {
    s32 sound;

    if (gContextSounds != NULL && gContextSoundNext != gContextSoundCount) {
        sound = gContextSounds[gContextSoundNext];
        if (sound != 0) {
            func_1001263C(sound, 0x7FFF, 0x40);
        }
        gContextSoundNext++;
    }
}

/* Text-box callback (TextBoxConfig.callback): voices the next sound effect on event 0x20. */
void text_soundCallback(TextBoxConfig *config, s32 page, s32 event) {
    if (event == 0x20) {
        text_playNextSound();
    }
}

/* Opens a text box for text gContextTextId. With arg0 == 0, and bit 4 of D_800D2E60[8] clear, text 0x43 is
   shown first as its own page. */
void text_openContextText(s32 arg0) {
    char *text;
    s32 withHeader;

    // matching: nested ifs (two FALSE stores); `arg0 == 0 && ...` reshapes the branches
    if (arg0 == 0) {
        if (!(D_800D2E60[8] & 0x10)) {
            withHeader = TRUE;
        } else {
            withHeader = FALSE;
        }
    } else {
        withHeader = FALSE;
    }
    gContextText = func_1502B6BC(0, 0, 0, 3, 0x1A, D_800BEAAB, gContextTextId);
    if (gContextText != NULL) {
        gContextSoundNext = 0;
        gContextSounds = gContextText->sounds;
        gContextSoundCount = gContextText->soundsSize / 2;
        if (withHeader) {
            s32 headerLen;
            s32 textLen;
            s32 pad; // matching: unused stack slot (frame size)
            s32 soundCount;
            u32 headerSoundCount;

            gContextHeaderText = func_1502B6BC(0, 0, 0, 3, 0x1A, D_800BEAAB, 0x43);
            if (gContextText == NULL) { /* sic: tests the text, not the header just loaded */
                return;
            }
            headerLen = strlen(gContextHeaderText->text) + 1;
            textLen = strlen(gContextText->text) + 1;
            gContextCombinedText = allocate_memory(headerLen + textLen + 1, 1, 0, 0);
            bcopy(gContextHeaderText->text, gContextCombinedText, headerLen);
            gContextCombinedText[headerLen - 1] = TEXT_PAGE_BREAK;
            bcopy(gContextText->text, gContextCombinedText + headerLen, textLen);
            headerSoundCount = gContextHeaderText->soundsSize / 2;
            soundCount = gContextSoundCount + headerSoundCount;
            gContextSounds = allocate_memory(soundCount * 2, 1, 0, 0);
            bcopy(gContextHeaderText->sounds, gContextSounds, headerSoundCount * 2);
            bcopy(gContextText->sounds, gContextSounds + headerSoundCount, gContextSoundCount * 2);
            gContextSoundCount = soundCount;
        } else {
            gContextHeaderText = NULL;
        }
        gContextTextOpen = TRUE;
        if (withHeader) {
            text = (char *)gContextCombinedText;
        } else {
            text = gContextText->text;
        }
        gContextTextBoxConfig.flags = 0x38013;
        if ((withHeader || arg0 || !BIT_IS_SET(D_800D2E60, gContextTextId)) && D_800D2E40 == 0) {
            gContextTextBoxConfig.flags |= 0x60;
        }
        if (arg0 || gContextTextId == 0x42) {
            gContextTextBoxConfig.flags |= 0x80;
        }
        gContextTextBoxConfig.text = (s32)text;
        gContextTextBoxConfig.duration = 100;
        gContextTextBoxConfig.numPages = 1;
        gContextTextBoxConfig.unk24 = 0;
        gContextTextBoxConfig.unk2B = 0;
        gContextTextBoxConfig.screenXPercent = &gContextTextScreenX;
        gContextTextBoxConfig.screenYPercent = &gContextTextScreenY;
        gContextTextBoxConfig.anglePercent = &gContextTextAngle;
        gContextTextBoxConfig.r = 0xFF;
        gContextTextBoxConfig.g = 0xFF;
        gContextTextBoxConfig.b = 0xFF;
        gContextTextScreenX = 78.0f;
        gContextTextScreenY = 48.0f;
        gContextTextAngle = 9.0f;
        gContextTextBox = text_createBox(0, 0, 0, text_soundCallback, &gContextTextBoxConfig, 0);
        gContextTextBox->noInput = 0;
    }
}

/* Closes the text box and frees the current text (and the combined header copies). */
void text_closeContextText(void) {
    TextResource *header;

    gContextTextOpen = FALSE;
    text_removeBox(gContextTextBox);
    func_10004074(gContextText);
    if (gContextHeaderText != NULL) {
        header = gContextHeaderText; // matching: a fresh load into a named temp, not the tested value
        func_10004074(header);
        func_10004074(gContextCombinedText);
        func_10004074(gContextSounds);
        gContextHeaderText = NULL;
    }
    func_151F2D6C(0, 0x5622);
}

/* Per frame: once the open text box has closed, frees it and disables the zone that showed it. */
void text_updateContextText(void) {
    if (gContextTextOpen && gContextTextBox->state == 0) {
        text_closeContextText();
        if (level != 0x29 && level != 0x2E) {
            D_800D2E60[8] |= 0x10;
        }
        if (gContextTextZone != NULL) {
            gContextTextZone->inactive = TRUE;
            gContextTextZone = NULL;
        }
    }
}

/* Returns whether no text box is open. */
s32 text_isContextTextClosed(void) {
    if (!gContextTextOpen) {
        return TRUE;
    }
    return FALSE;
}
