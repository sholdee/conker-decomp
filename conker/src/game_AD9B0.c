#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 D_800D1940;
u8 D_800D1941;
TextResource *D_800D1944; // current text
TextResource *D_800D1948; // header text prepended to it, or NULL
u8 *D_800D194C;           // header + page break + current text
TextBox *D_800D1950;      // open text box
TextBoxConfig D_800D1958;
f32 D_800D1988;
f32 D_800D198C;
f32 D_800D1990;
u8 D_800D1994;
u8 D_800D1995;
u16 *D_800D1998;          // sound effects voiced while the text types out
static TriggerZone *D_800D199C; /* zone to disable once the text box closes */

#include "functions.h"
#include "variables.h"

/* Byte-wide character in a text resource that separates two pages (see func_1516A770). */
#define TEXT_PAGE_BREAK 0xBD

/* Tests bit n of a byte-array bit set. */
#define BIT_IS_SET(bits, n) ((bits)[(n) >> 3] & (1 << ((n) & 7)))

s32 strlen(const char *);

/* Queues text textId on obj's text slot (0 or 1), unless a script has locked that slot. Slot 1 also
   records the trigger zone, which func_15080C64 disables once text 0x2B / 0x2C has been shown. */
void func_15080500(obj, zone, slot, textId)
    Actor *obj;
    TriggerZone *zone;
    s32 slot;
    s32 textId;
{
    if (obj != NULL && obj->type != 0 && obj->playernumber != 0xFF) {
        if (textId == 0x2B || textId == 0x2C) {
            D_800D1940 = textId;
            D_800D199C = zone;
            textId = 0x2A;
        } else if (func_15080738(textId)) {
            D_800D1940 = textId;
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
void func_15080620(s32 objIndex, s32 slot, s32 textId, s32 arg3) {
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
void func_150806A8(s32 objIndex) {
    Actor *obj = &movement_obj[objIndex];

    if (obj->mv->context_a != 0 && !(obj->mv->context_a & 0x80)) {
        obj->mv->context_a = 0;
    }
    if (obj->mv->context_b != 0 && !(obj->mv->context_b & 0x80)) {
        obj->mv->context_b = 0;
    }
}

/* Splits bit number n of a byte-array bit set into its byte index and bit mask. */
void func_15080718(s32 n, s32 *byteIndex, s32 *bitMask) {
    *bitMask = 1 << (n & 7);
    *byteIndex = n >> 3;
}

/* Returns whether textId's bit is set in D_800BE580. */
s32 func_15080738(s32 textId) {
    s32 byteIndex;
    s32 bitMask;

    func_15080718(textId, &byteIndex, &bitMask);
    if (D_800BE580[byteIndex] & bitMask) {
        return TRUE;
    }
    return FALSE;
}

/* Plays the next of the current text's sound effects. */
void func_15080784(void) {
    s32 sound;

    if (D_800D1998 != NULL && D_800D1994 != D_800D1995) {
        sound = D_800D1998[D_800D1994];
        if (sound != 0) {
            func_1001263C(sound, 0x7FFF, 0x40);
        }
        D_800D1994++;
    }
}

/* Text-box callback (TextBoxConfig.callback): voices the next sound effect on event 0x20. */
void func_150807F4(TextBoxConfig *config, s32 page, s32 event) {
    if (event == 0x20) {
        func_15080784();
    }
}

/* Opens a text box for text D_800D1940. With arg0 == 0, and bit 4 of D_800D2E68 clear, text 0x43 is
   shown first as its own page. */
void func_15080828(s32 arg0) {
    char *text;
    s32 withHeader;

    // matching: nested ifs (two FALSE stores); `arg0 == 0 && ...` reshapes the branches
    if (arg0 == 0) {
        if (!(D_800D2E68 & 0x10)) {
            withHeader = TRUE;
        } else {
            withHeader = FALSE;
        }
    } else {
        withHeader = FALSE;
    }
    D_800D1944 = func_1502B6BC(0, 0, 0, 3, 0x1A, D_800BEAAB, D_800D1940);
    if (D_800D1944 != NULL) {
        D_800D1994 = 0;
        D_800D1998 = D_800D1944->sounds;
        D_800D1995 = D_800D1944->soundsSize / 2;
        if (withHeader) {
            s32 headerLen;
            s32 textLen;
            s32 pad; // matching: unused stack slot (frame size)
            s32 soundCount;
            u32 headerSoundCount;

            D_800D1948 = func_1502B6BC(0, 0, 0, 3, 0x1A, D_800BEAAB, 0x43);
            if (D_800D1944 == NULL) { /* sic: tests the text, not the header just loaded */
                return;
            }
            headerLen = strlen(D_800D1948->text) + 1;
            textLen = strlen(D_800D1944->text) + 1;
            D_800D194C = allocate_memory(headerLen + textLen + 1, 1, 0, 0);
            bcopy(D_800D1948->text, D_800D194C, headerLen);
            D_800D194C[headerLen - 1] = TEXT_PAGE_BREAK;
            bcopy(D_800D1944->text, D_800D194C + headerLen, textLen);
            headerSoundCount = D_800D1948->soundsSize / 2;
            soundCount = D_800D1995 + headerSoundCount;
            D_800D1998 = allocate_memory(soundCount * 2, 1, 0, 0);
            bcopy(D_800D1948->sounds, D_800D1998, headerSoundCount * 2);
            bcopy(D_800D1944->sounds, D_800D1998 + headerSoundCount, D_800D1995 * 2);
            D_800D1995 = soundCount;
        } else {
            D_800D1948 = NULL;
        }
        D_800D1941 = TRUE;
        if (withHeader) {
            text = (char *)D_800D194C;
        } else {
            text = D_800D1944->text;
        }
        D_800D1958.flags = 0x38013;
        if ((withHeader || arg0 || !BIT_IS_SET(D_800D2E60, D_800D1940)) && D_800D2E40 == 0) {
            D_800D1958.flags |= 0x60;
        }
        if (arg0 || D_800D1940 == 0x42) {
            D_800D1958.flags |= 0x80;
        }
        D_800D1958.text = (s32)text;
        D_800D1958.duration = 100;
        D_800D1958.numPages = 1;
        D_800D1958.unk24 = 0;
        D_800D1958.unk2B = 0;
        D_800D1958.screenXPercent = &D_800D1988;
        D_800D1958.screenYPercent = &D_800D198C;
        D_800D1958.anglePercent = &D_800D1990;
        D_800D1958.r = 0xFF;
        D_800D1958.g = 0xFF;
        D_800D1958.b = 0xFF;
        D_800D1988 = 78.0f;
        D_800D198C = 48.0f;
        D_800D1990 = 9.0f;
        D_800D1950 = func_1516A7B0(0, 0, 0, func_150807F4, &D_800D1958, 0);
        D_800D1950->unk14 = 0;
    }
}

/* Closes the text box and frees the current text (and the combined header copies). */
void func_15080BE8(void) {
    TextResource *header;

    D_800D1941 = FALSE;
    func_1516D2E0(D_800D1950);
    func_10004074(D_800D1944);
    if (D_800D1948 != NULL) {
        header = D_800D1948; // matching: a fresh load into a named temp, not the tested value
        func_10004074(header);
        func_10004074(D_800D194C);
        func_10004074(D_800D1998);
        D_800D1948 = NULL;
    }
    func_151F2D6C(0, 0x5622);
}

/* Per frame: once the open text box has closed, frees it and disables the zone that showed it. */
void func_15080C64(void) {
    if (D_800D1941 && D_800D1950->state == 0) {
        func_15080BE8();
        if (level != 0x29 && level != 0x2E) {
            D_800D2E60[8] |= 0x10;
        }
        if (D_800D199C != NULL) {
            D_800D199C->inactive = TRUE;
            D_800D199C = NULL;
        }
    }
}

/* Returns whether no text box is open. */
s32 func_15080CF4(void) {
    if (!D_800D1941) {
        return TRUE;
    }
    return FALSE;
}
