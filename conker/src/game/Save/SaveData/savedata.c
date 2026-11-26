#include <ultra64.h>
#include "structs.h"

/* This file's .bss, defined before the headers declare the globals: IDO lays out .bss in the order
 * it first sees each name. */
static u8 gModeSettingsRecord[0x68]; /* the EEPROM record of blocks 0x44-0x51: a u16 checksum, then gModeSettings */
static u8 gEepromBuffer[0x80]; /* EEPROM transfer buffer; [0..1] is a u16 checksum */
static s8 gEepromFileSlots[4]; /* save slot loaded from each EEPROM save file, -1 if none */
s16 D_800BE3DC;
u8 D_800BE3DE;
s8 D_800BE3DF;
u8 D_800BE3E0;
u32 D_800BE3E4; // saved clock (seconds)
s32 gPlayTime; // play time (seconds)
s8 gEepromRequestArg; // argument of the pending EEPROM request gEepromRequest
u8 *gSaveEntryBits; /* bit width of each of the 69 gPlayedCutscenes entries in a save file */
SaveSlotInfo gSaveSlots[3]; // the three save slots

#include "functions.h"
#include "variables.h"

/*
 * EEPROM save data. The EEPROM is addressed in 8-byte blocks:
 *   blocks 0x00-0x03  settings (gSettings, range-checked by save_validateSettings), 0x20 bytes
 *   blocks 0x04-0x43  four save files of 0x80 bytes (16 blocks) each
 *   blocks 0x44-0x51  gModeSettingsRecord, 0x70 bytes (the array has 0x68)
 * Every record starts with a u16 checksum of its remaining bytes (see the checksum loops below).
 * gEepromBuffer is the 0x80-byte transfer buffer.
 * Under -g3 the line table steers the scheduling of the unrolled loops, so short loop bodies stay on
 * one line.
 */

/* EEPROM requests, queued in gEepromRequest (argument in gEepromRequestArg) and run by cont_runEepromRequest. */
enum EepromRequest {
    EEPROM_REQ_NONE,
    EEPROM_REQ_LOAD_FILE,     /* save_readFile */
    EEPROM_REQ_SAVE_SLOT,     /* save_writeSlot */
    EEPROM_REQ_ERASE_SLOT,    /* save_eraseSlot */
    EEPROM_REQ_LOAD_2F0,      /* save_readModeSettings */
    EEPROM_REQ_SAVE_2F0,      /* save_writeModeSettings */
    EEPROM_REQ_LOAD_SETTINGS, /* save_readSettings */
    EEPROM_REQ_SAVE_SETTINGS  /* save_writeSettings */
};

static s32 D_80082BB0 = 0xFF000000; // not referenced
s8 gEepromRequest = EEPROM_REQ_NONE; // the pending EEPROM request, run by cont_runEepromRequest
static s32 D_80082BB8 = 0; // not referenced
ModeSettings *gModeSettings = (ModeSettings *)&gModeSettingsRecord[2]; // saved settings, one record per multiplayer mode
u8 gSaveScanning = FALSE; // scanning the EEPROM save files (save_readFile records each in its save slot)

/* EEPROM access (game/Save/EepromTimedWrite/eepromtimedwrite.c, game/Os/EepromLongRead/eepromlongread.c) */
/* Defined in game/Os/EepromLongRead/eepromlongread.c as s32 func_151DD3A0(OSMesgQueue *mq, u8 address, u8 *buffer, int length) (read).
   This file was compiled with a view that returns nothing. */
void func_151DD3A0(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size);

/* Resets the saved game state (zone_resetTaskProgress, every gPlayedCutscenes entry and the values saved with them). */
void save_resetGameState(void) {
    s32 i;

    zone_resetTaskProgress();
    for (i = 0; i < 69; i++) { gPlayedCutscenes[i] = 0; }

    D_800BE3DF = 24;
    gPlayTime = 0;
    D_800D2E45 = 1;
}

/*
 * Loads EEPROM save file `eepromSlot` (0-3) into the game state. A file whose checksum fails resets the
 * state instead. While gSaveScanning is set (scanning all files), it also records the file in its save
 * slot's SaveSlotInfo if it is newer, and keeps the 27 bytes at gTaskDoneBits from the file with the most
 * bits set.
 */
void save_readFile(s32 eepromSlot) {
    u64 mask;
    s32 i;
    s32 j;
    u16 sum;
    u8 *p;
    s32 block;
    u8 byte;
    u8 bit;
    s32 flagCount;
    u8 bits;
    u32 value;
    s32 pad; // matching: unused stack slot (IDO -g3 keeps a slot for every declared local)
    u8 saveSlot;
    s32 sequence;
    u8 backup[27];

    for (i = 0; i < 27; i++) {
        backup[i] = gTaskDoneBits[i];
    }
    if (gSaveEntryBits == NULL) {
        gSaveEntryBits = (u8 *)rom_loadPath(NULL, 1, 7) + 8;
    }
    osWritebackDCache(gEepromBuffer, 0x80);
    osInvalDCache(gEepromBuffer, 0x80);
    block = eepromSlot * 16 + 4;
    func_151DD3A0(&gSiMesgQueue, block, gEepromBuffer, 0x80);
    p = gEepromBuffer;
    sum = 0xCC;
    for (i = 2; i < 0x80; i++) { sum += p[i] << (i & 3); }
    p += 2;
    if (sum != *(u16 *)gEepromBuffer) {
        save_resetGameState();
        if (D_800BE3DC >= 0 && gSaveScanning != 0) {
            for (i = 0; i < 27; i++) {
                gTaskDoneBits[i] = backup[i];
            }
        }
        if (block == 0x34) {
            gSaveScanning = 0;
        }
    } else {
        saveSlot = *p;
        p += 5;
        sequence = p[-4] << 24;
        sequence |= p[-3] << 16;
        sequence |= p[-2] << 8;
        sequence |= p[-1];
        for (i = 0; i < 27; i++) {
            gTaskDoneBits[i] = *p++;
        }
        for (i = 0; i < 9; i++) {
            gSeenTextBits[i] = *p++;
        }
        D_800BE3DF = *p++;
        D_800BE3E0 = *p++;
        D_800BE3E4 = *p++ << 8;
        D_800BE3E4 |= *p++;
        player_setStat(0, 9, D_800BE3E4);
        value = *p++ << 24;
        value |= *p++ << 16;
        value |= *p++ << 8;
        value |= *p++;
        gPlayTime = value;
        byte = *p++;
        bit = 0x80;
        for (i = 0; i < 69; i++) {
            gPlayedCutscenes[i] = 0;
            mask = (u64)1 << (gSaveEntryBits[i] - 1);
            for (j = 0; j < gSaveEntryBits[i]; j++) {
                if (byte & bit) {
                    gPlayedCutscenes[i] |= mask;
                }
                if (bit == 1) {
                    byte = *p++;
                    bit = 0x80;
                } else {
                    bit >>= 1;
                }
                mask >>= 1;
            }
        }
        if (gSaveScanning != 0 && saveSlot < 3) {
            gEepromFileSlots[eepromSlot] = saveSlot;
            if (gSaveSlots[saveSlot].sequence < sequence) {
                flagCount = 0;
                for (i = 0; i < 27; i++) {
                    bits = gTaskDoneBits[i];
                    for (j = 8; j != 0; j--) {
                        if (bits & 1) {
                            flagCount++;
                        }
                        bits >>= 1;
                    }
                }
                gSaveSlots[saveSlot].playTime = gPlayTime;
                gSaveSlots[saveSlot].flagCount = flagCount;
                gSaveSlots[saveSlot].unkC = D_800BE3DF;
                gSaveSlots[saveSlot].unk4 = D_800BE3E4;
                gSaveSlots[saveSlot].sequence = sequence;
                gSaveSlots[saveSlot].eepromSlot = eepromSlot;
                if ((u8)flagCount >= D_800BE3DC) {
                    D_800BE3DC = (u8)flagCount;
                    if (gSeenTextBits[8] & 0x20) {
                        D_800BE3DE = 1;
                    }
                } else {
                    for (i = 0; i < 27; i++) {
                        gTaskDoneBits[i] = backup[i];
                    }
                }
            } else {
                for (i = 0; i < 27; i++) {
                    gTaskDoneBits[i] = backup[i];
                }
            }
        }
        if (block == 0x34) {
            gSaveScanning = 0;
        }
    }
}
/*
 * Writes the game state as save slot `saveSlot` (0-2). The save goes to an EEPROM save file that no
 * other slot uses. Its last block is overwritten with 0xFE first, so an interrupted write fails the
 * checksum. Layout: checksum, slot, sequence, the gTaskDoneBits/gSeenTextBits/D_800BE3DF/D_800BE3E0/
 * D_800BE3E4/gPlayTime values, then each gPlayedCutscenes entry packed MSB-first in gSaveEntryBits[i] bits.
 */
void save_writeSlot(s32 saveSlot) {
    u64 mask;
    s32 i;
    s32 j;
    volatile s32 ret; // matching: the dead save_writeEeprom result is stored to the stack
    u8 *p;
    s32 eepromSlot;
    s32 sequence;
    u8 bit;
    u8 byte;
    u16 sum;

    sequence = (gSaveSlots[saveSlot].sequence + 1) & 0x7FFFFFFF;
    j = -1; /* bit mask of free EEPROM save files; matching: reuses j, a separate local allocates differently */
    for (i = 0; i < 3; i++) {
        if (gSaveSlots[i].sequence >= 0) {
            j &= ~(1 << gSaveSlots[i].eepromSlot);
        }
    }
    eepromSlot = 0;
    for (i = 0; i < 4; i++) {
        if ((1 << i) & j) {
            eepromSlot = i;
        }
    }
    gSaveSlots[saveSlot].eepromSlot = eepromSlot;
    gSaveSlots[saveSlot].sequence = sequence;
    if (gResetPending == 0) {
        for (i = 0; i < 8; i++) { gEepromBuffer[i] = 0xFE; }
        osWritebackDCacheAll();
        osEepromWrite(&gSiMesgQueue, (eepromSlot << 4) + 0x13, gEepromBuffer);
    }
    if (gSaveEntryBits == NULL) {
        gSaveEntryBits = (u8 *)rom_loadPath(NULL, 1, 7) + 8;
    }
    p = (u8 *)(u32)&gEepromBuffer[2]; // matching: without the (u32) round trip the code changes
    *p++ = saveSlot;
    *p++ = (sequence >> 24) & 0xFF;
    *p++ = (sequence >> 16) & 0xFF;
    *p++ = (sequence >> 8) & 0xFF;
    *p++ = sequence & 0xFF;
    for (i = 0; i < 27; i++) { *p++ = gTaskDoneBits[i]; }
    for (i = 0; i < 9; i++) { *p++ = gSeenTextBits[i]; }
    *p++ = D_800BE3DF; // matching: original loads it signed (lb)
    *p++ = D_800BE3E0;
    D_800BE3E4 = player_getStat(0, 6);
    *p++ = (D_800BE3E4 >> 8) & 0xFF;
    *p++ = D_800BE3E4 & 0xFF;
    *p++ = ((u32)gPlayTime >> 24) & 0xFF;
    *p++ = ((u32)gPlayTime >> 16) & 0xFF;
    *p++ = ((u32)gPlayTime >> 8) & 0xFF;
    *p++ = gPlayTime & 0xFF;
    for (i = 0; i < 69; i++) { p[i] = 0; }
    byte = 0;
    bit = 0x80;
    for (i = 0; i < 69; i++) {
        mask = (u64)1 << (gSaveEntryBits[i] - 1);
        for (j = 0; j < gSaveEntryBits[i]; j++) {
            if (gPlayedCutscenes[i] & mask) {
                byte |= bit;
            }
            if (bit == 1) {
                *p++ = byte;
                byte = 0;
                bit = 0x80;
            } else {
                bit >>= 1;
            }
            mask >>= 1;
        }
    }
    sum = 0xCC;
    for (i = 2; i < 0x80; i++) { sum += gEepromBuffer[i] << (i & 3); }
    *(u16 *)gEepromBuffer = sum;
    osWritebackDCacheAll();
    if (gResetPending == 0) {
        ret = save_writeEeprom(&gSiMesgQueue, (eepromSlot << 4) + 4, gEepromBuffer, 0x80);
    }
}
/* Erases every EEPROM save file loaded as save slot `saveSlot` by overwriting its first two blocks. */
void save_eraseSlot(s32 saveSlot) {
    s32 i;
    s32 ret;
    s32 block;
    u8 *buffer = gEepromBuffer;

    for (i = 0; i < 8; i++) { buffer[i] = 0xFF; }
    osWritebackDCacheAll();
    if (gResetPending == 0) {
        for (i = 0; i < 4; i++) {
            if (saveSlot == gEepromFileSlots[i]) {
                block = i * 16 + 4;
                ret = osEepromWrite(&gSiMesgQueue, block, buffer);
                // matching: "+ ret - ret" and "if (ret) {}" keep the unused first result in a saved register
                ret = osEepromWrite(&gSiMesgQueue, block + 1, buffer) + ret - ret;
                if (ret) {}
            }
        }
    }
}
void save_resetModeSettings(void) {
    s32 i;

    for (i = 0; i < 0x68; i++) {
        gModeSettingsRecord[i] = 0;
    }

    for (i = 0; i < 10; i++) {
        gModeSettings[i].unk1 = 3;
        gModeSettings[i].unk3 = 2;
        gModeSettings[i].unk4 = 1;
        gModeSettings[i].unk2 = 10;
        switch (i) {
            case 0:
                gModeSettings[i].unk5 = 3;
                break;
            case 1:
            case 2:
                gModeSettings[i].unk5 = 5;
                break;
            case 7:
                gModeSettings[i].unk5 = 20;
                break;
            case 5:
                gModeSettings[i].unk5 = 20;
                gModeSettings[i].unk2 = 0;
                break;
            case 8:
            case 9:
                gModeSettings[i].unk5 = 3;
                break;
        }
        gModeSettings[i].unk9 = 0;
    }
}
/* Reads the gModeSettingsRecord record from EEPROM; resets it (save_resetModeSettings) if its checksum fails. */
void save_readModeSettings(void) {
    s32 i;
    u16 sum;

    func_151DD3A0(&gSiMesgQueue, 0x44, gModeSettingsRecord, 0x70);
    sum = 0xCC;
    for (i = 2; i < 0x68; i++) { sum += gModeSettingsRecord[i] << (i & 3); }
    if (*(u16 *)gModeSettingsRecord != sum) {
        save_resetModeSettings();
    }
}
/* Checksums the gModeSettingsRecord record and writes it to EEPROM. */
void save_writeModeSettings(void) {
    u16 sum;
    volatile s32 ret; // matching: the dead save_writeEeprom result is stored to the stack
    s32 i;

    sum = 0xCC;
    for (i = 2; i < 0x68; i++) { sum += gModeSettingsRecord[i] << (i & 3); }
    *(u16 *)gModeSettingsRecord = sum;
    if (gResetPending == 0) {
        ret = save_writeEeprom(&gSiMesgQueue, 0x44, gModeSettingsRecord, 0x70);
    }
}
/*
 * Reads the settings record (gSettings) from EEPROM, or sets every entry to -1 if its checksum fails;
 * entry 0x10 keeps its current value either way. save_validateSettings then resets out-of-range entries.
 */
void save_readSettings(void) {
    u16 sum;
    s32 i;
    s8 saved;

    func_151DD3A0(&gSiMesgQueue, 0, gEepromBuffer, 0x20);
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += gEepromBuffer[i] << (i & 3); }
    saved = gSettings[0x10];
    if (sum != *(u16 *)gEepromBuffer) {
        for (i = 0; i < 0x1C; i++) { gSettings[i] = -1; }
    } else {
        for (i = 0; i < 0x1C; i++) { gSettings[i] = gEepromBuffer[i + 2]; }
    }
    gSettings[0x10] = saved;
    save_validateSettings();
    save_checkCodeTamper();
    D_8008FE30 = 1;
}

/* Checksums the settings record (gSettings) and writes it to EEPROM. */
void save_writeSettings(void) {
    u16 sum;
    volatile s32 ret; // matching: the dead save_writeEeprom result is stored to the stack
    s32 i;

    for (i = 0; i < 0x1C; i++) { gEepromBuffer[i + 2] = gSettings[i]; }
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += gEepromBuffer[i] << (i & 3); }
    *(u16 *)gEepromBuffer = sum;
    if (gResetPending == 0) {
        ret = save_writeEeprom(&gSiMesgQueue, 0, gEepromBuffer, 0x20);
    }
}

void func_15007644(void) {
}

/* Requests a settings load (save_readSettings). */
void save_requestReadSettings(void) {
    gEepromRequest = EEPROM_REQ_LOAD_SETTINGS;
    gEepromRequestArg = 0;
}

/* Requests a settings save (save_writeSettings). */
void save_requestWriteSettings(void) {
    gEepromRequest = EEPROM_REQ_SAVE_SETTINGS;
    gEepromRequestArg = 0;
}

/* Requests a gModeSettingsRecord load (save_readModeSettings). */
void save_requestReadModeSettings(void) {
    gEepromRequest = EEPROM_REQ_LOAD_2F0;
    gEepromRequestArg = 0;
}

/* Requests a gModeSettingsRecord save (save_writeModeSettings). */
void save_requestWriteModeSettings(void) {
    gEepromRequest = EEPROM_REQ_SAVE_2F0;
    gEepromRequestArg = 0;
}

/* Requests a load of EEPROM save file `eepromSlot` (save_readFile); a negative one resets the state. */
void save_requestReadFile(s32 eepromSlot) {
    if (eepromSlot < 0) {
        save_resetGameState();
    }
    if (eepromSlot >= 0 && eepromSlot < 4 && gIsMultiplayer == 0) {
        gEepromRequest = EEPROM_REQ_LOAD_FILE;
        gEepromRequestArg = eepromSlot;
    }
}

/* Requests a save to save slot `saveSlot` (save_writeSlot). */
void save_requestWriteSlot(s32 saveSlot) {
    if (saveSlot >= 0 && saveSlot < 3 && gIsMultiplayer == 0) {
        gEepromRequest = EEPROM_REQ_SAVE_SLOT;
        gEepromRequestArg = saveSlot;
    }
}

/* Requests erasing save slot `saveSlot` (save_eraseSlot). */
void save_requestEraseSlot(s32 saveSlot) {
    if (saveSlot >= 0 && saveSlot < 3) {
        gEepromRequest = EEPROM_REQ_ERASE_SLOT;
        gEepromRequestArg = saveSlot;
    }
}

/* Marks all save slots empty and clears the save state, ready to scan the EEPROM save files. */
void save_beginScan(void) {
    s32 i;

    gSaveSlots[0].sequence = -1;
    gSaveSlots[1].sequence = -1;
    gSaveSlots[2].sequence = -1;
    gSaveSlots[0].eepromSlot = -1;
    gSaveSlots[1].eepromSlot = -1;
    gSaveSlots[2].eepromSlot = -1;
    D_800BE3DC = -1;
    D_800BE3DE = 0;
    gSaveScanning = 1;
    for (i = 0; i < 27; i++) { gTaskDoneBits[i] = 0; }
    for (i = 0; i < 9; i++) { gSeenTextBits[i] = 0; }
    for (i = 0; i < 4; i++) { gEepromFileSlots[i] = -1; }
}
