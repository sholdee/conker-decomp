#include <ultra64.h>
#include "functions.h"
/* variables.h gets these types wrong, so they are shadowed here: D_800D3098 (`struct178 D_800D3098[73]`)
   holds a POINTER to the trigger-zone table, and the text-box globals below are pointers and a struct, not
   s32 / s32[12]. */
#define D_800D1944 D_800D1944_s32_decl_in_variables_h
#define D_800D1948 D_800D1948_s32_decl_in_variables_h
#define D_800D194C D_800D194C_s32_decl_in_variables_h
#define D_800D1950 D_800D1950_s32_decl_in_variables_h
#define D_800D1958 D_800D1958_array_decl_in_variables_h
#define D_800D1998 D_800D1998_s32_decl_in_variables_h
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D1944
#undef D_800D1948
#undef D_800D194C
#undef D_800D1950
#undef D_800D1958
#undef D_800D1998
#undef D_800D3098

/* Byte-wide character in a text resource that separates two pages (see func_1516A770). */
#define TEXT_PAGE_BREAK 0xBD

/* Tests bit n of a byte-array bit set. */
#define BIT_IS_SET(bits, n) ((bits)[(n) >> 3] & (1 << ((n) & 7)))

/* One trigger zone (same layout as TriggerZone in game_CDE80.c). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 depth;
    /* 0x0C */ f32 pitch;
    /* 0x10 */ f32 yaw;
    /* 0x14 */ u8  disabled;
    /* 0x15 */ u8  kind;
    /* 0x16 */ u8  flags;
    /* 0x17 */ u8  subtype;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ f32 sinPitch;
    /* 0x28 */ f32 cosPitch;
    /* 0x2C */ f32 sinYaw;
    /* 0x30 */ f32 cosYaw;
} TriggerZone; /* size 0x34 */

/* Partial view of the struct126 that struct127.unk31C points to: structs.h has 0x74 and 0x7A as padding.
   0x74 and 0x75 are two pending-text slots; bit 7 marks a slot set by a script, which zones don't override. */
typedef struct {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ u8 unk74;
    /* 0x75 */ u8 unk75;
    /* 0x76 */ u8 pad76[4];
    /* 0x7A */ u8 unk7A; /* index of the trigger zone that set unk75 */
} ObjTextSlots;

#define TEXT_SLOTS(obj) ((ObjTextSlots *)(obj)->unk31C)

/* Text resource loaded by func_1502B6BC: the text, then the sound effects voiced one per word. */
typedef struct {
    /* 0x0 */ char *text;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u16 *sounds;
    /* 0xC */ u32 soundsSize; /* in bytes */
} TextResource;

/* Text-box description passed to func_1516A7B0 (cleared as 48 bytes in game_439B0.c). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ char *text;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 pad9[0xF];
    /* 0x18 */ f32 *unk18;
    /* 0x1C */ f32 *unk1C;
    /* 0x20 */ f32 *unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ s32 unk2C;
} TextBoxDesc;

/* Text box returned by func_1516A7B0 (Struct1516A7B0 in game_197C20.c). */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u8 unk14;
    /* 0x15 */ u8 unk15; /* cleared when the box has closed */
} TextBox;

extern TextResource *D_800D1944;  /* current text */
extern TextResource *D_800D1948;  /* header text prepended to it, or NULL */
extern u8 *D_800D194C;            /* header + page break + current text */
extern TextBox *D_800D1950;       /* open text box */
extern TextBoxDesc D_800D1958;
extern u16 *D_800D1998;           /* sound effects voiced while the text types out */
extern TriggerZone *D_800D199C;   /* zone to disable once the text box closes */
extern TriggerZone *D_800D3098;   /* trigger-zone table */
extern u8 D_800BE580[];           /* bit set over text ids */
extern u8 D_800D2E40;

TextResource *func_1502B6BC(s32, s32, s32, s32, s32, s32, s32);
void *allocate_memory(s32, s32, s32, s32);
TextBox *func_1516A7B0(s32, s32, s32, void (*)(s32, s32, s32), TextBoxDesc *, s32);
void func_1516D2E0(TextBox *);
void func_10004074(void *);
void func_151F2D6C(s32, s32);
s32 strlen(const char *);
s32 func_15080738(s32 textId);

/* Queues text textId on obj's text slot (0 or 1), unless a script has locked that slot. Slot 1 also
   records the trigger zone, which func_15080C64 disables once text 0x2B / 0x2C has been shown. */
void func_15080500(struct127 *obj, TriggerZone *zone, s32 slot, s32 textId) {
    ObjTextSlots *slots;
    s32 idx;
    s32 mask;

    if (obj != NULL && obj->interaction_state != 0 && obj->unk127 != 0xFF) {
        if (textId == 0x2B || textId == 0x2C) {
            D_800D1940 = textId;
            D_800D199C = zone;
            textId = 0x2A;
        } else if (func_15080738(textId)) {
            idx = textId >> 3;
            mask = 1 << (textId & 7);
            D_800D1940 = textId;
            if (!(D_800D2E60[idx] & mask)) {
                textId = 0x1A;
            }
        }

        if (slot == 0) {
            slots = TEXT_SLOTS(obj);
            if (!(slots->unk74 & 0x80)) {
                slots->unk74 = textId;
            }
        } else {
            slots = TEXT_SLOTS(obj);
            if (!(slots->unk75 & 0x80)) {
                slots->unk75 = textId;
                TEXT_SLOTS(obj)->unk7A = zone - D_800D3098;
            }
        }
    }
}

/* Script command: sets text slot 0 or 1 of object objIndex, locked against trigger zones (bit 7). */
void func_15080620(s32 objIndex, s32 slot, s32 textId, s32 arg3) {
    if (textId != 0) {
        textId |= 0x80;
    }
    if (slot == 0) {
        TEXT_SLOTS(&D_800CC2D0[objIndex])->unk74 = textId;
    } else {
        TEXT_SLOTS(&D_800CC2D0[objIndex])->unk75 = textId;
    }
}

/* Clears both of an object's text slots unless a script locked them. */
void func_150806A8(s32 objIndex) {
    struct127 *obj = &D_800CC2D0[objIndex];
    ObjTextSlots *slots = TEXT_SLOTS(obj);

    if (slots->unk74 != 0) {
        if (!(slots->unk74 & 0x80)) {
            slots->unk74 = 0;
            slots = TEXT_SLOTS(obj);
        }
    }
    if (slots->unk75 != 0) {
        if (!(slots->unk75 & 0x80)) {
            slots->unk75 = 0;
        }
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

    if (D_800D1998 != NULL) {
        if (D_800D1994 != D_800D1995) {
            sound = D_800D1998[D_800D1994];
            if (sound != 0) {
                func_1001263C(sound, 0x7FFF, 0x40);
            }
            D_800D1994++;
        }
    }
}

/* Text-box callback: voices the next sound effect at each space. */
void func_150807F4(s32 arg0, s32 arg1, s32 c) {
    if (c == ' ') {
        func_15080784();
    }
}

/* Opens a text box for text D_800D1940. With arg0 == 0, and bit 4 of D_800D2E68 clear, text 0x43 is
   shown first as its own page. */
void func_15080828(s32 arg0) {
    char *text;
    s32 withHeader;

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
            TextResource *unused;
            s32 soundCount;
            u32 headerSoundCount;

            D_800D1948 = func_1502B6BC(0, 0, 0, 3, 0x1A, D_800BEAAB, 0x43);
            if (D_800D1944 == NULL) {
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
        D_800D1958.text = text;
        D_800D1958.unk26 = 100;
        D_800D1958.unk8 = 1;
        D_800D1958.unk24 = 0;
        D_800D1958.unk2B = 0;
        D_800D1958.unk18 = &D_800D1988;
        D_800D1958.unk1C = &D_800D198C;
        D_800D1958.unk20 = &D_800D1990;
        D_800D1958.unk28 = 0xFF;
        D_800D1958.unk29 = 0xFF;
        D_800D1958.unk2A = 0xFF;
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
        header = D_800D1948;
        func_10004074(header);
        func_10004074(D_800D194C);
        func_10004074(D_800D1998);
        D_800D1948 = NULL;
    }
    func_151F2D6C(0, 0x5622);
}

/* Per frame: once the open text box has closed, frees it and disables the zone that showed it. */
void func_15080C64(void) {
    if (D_800D1941) {
        if (D_800D1950->unk15 == 0) {
            func_15080BE8();
            if (D_800BE9F0 != 0x29 && D_800BE9F0 != 0x2E) {
                D_800D2E60[8] |= 0x10;
            }
            if (D_800D199C != NULL) {
                D_800D199C->disabled = TRUE;
                D_800D199C = NULL;
            }
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
