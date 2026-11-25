#include <ultra64.h>

#include "functions.h"
/* variables.h declares D_800BE3F8 as a pointer; this file uses it as the array of SaveSlotInfo. */
#define D_800BE3F8 D_800BE3F8_ptr_decl_in_variables_h
#include "variables.h"
#undef D_800BE3F8

/*
 * EEPROM save data. The EEPROM is addressed in 8-byte blocks:
 *   blocks 0x00-0x03  options (D_800E0BE0), 0x20 bytes
 *   blocks 0x04-0x43  four save files of 0x80 bytes (16 blocks) each
 *   blocks 0x44-0x51  D_800BE2F0, 0x70 bytes
 * Every record starts with a u16 checksum of its remaining bytes (see the checksum loops below).
 * D_800BE358 is the 0x80-byte transfer buffer.
 */

/* Summary of one of the three save files the player can pick. */
typedef struct SaveSlotInfo {
    /* 0x00 */ s32 unk0;       /* D_800BE3E8 */
    /* 0x04 */ u32 unk4;       /* D_800BE3E4 */
    /* 0x08 */ s32 sequence;   /* incremented on every save, -1 when the slot is empty */
    /* 0x0C */ s8 unkC;        /* D_800BE3DF */
    /* 0x0D */ u8 flagCount;   /* number of bits set in the 27 bytes at D_800D2E4C */
    /* 0x0E */ s8 eepromSlot;  /* which of the four EEPROM save files holds it */
    /* 0x0F */ s8 padF;
} SaveSlotInfo; /* size = 0x10 */

typedef struct Struct33990 {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 unk1;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s8 unk4;
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ u8 unk9;
} Struct33990; /* size = 0xA */

extern SaveSlotInfo D_800BE3F8[3];
extern u8 D_800BE2F0[0x70];
extern u8 D_800BE35A[];
extern s8 D_800E0BE0[0x1C];
static s8 D_800BE3D8[4]; /* save slot loaded from each EEPROM save file, -1 if none */
extern Struct33990 *D_80082BBC;
extern u8 D_8008FE30;
extern u8 *D_800BE3F0; /* bit width of each of the 69 D_800C3A60 entries in a save file */
extern u32 D_800BE3E4;

/* EEPROM access (game_20A3A0.c, game_20A850.c) */
extern void func_151DD3A0(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size); /* read; matching: declared void (returns s32) */
extern s32 func_151DCEF0(OSMesgQueue *mq, u8 block, u8 *buffer, s32 size); /* timed write */
extern s32 func_151DD4E0(OSMesgQueue *mq, u8 block, u8 *buffer);           /* write one block */
extern void func_151DD9E4(void);
extern void func_151E7EF8(void);
extern void *func_1502B5C8();

void func_150064E0(void) {
    s32 i;

    func_15017790();
    for (i = 0; i < 69; i++) { D_800C3A60[i] = 0; }

    D_800BE3DF = 24;
    D_800BE3E8 = 0;
    D_800D2E45 = 1;
}


/*
 * Loads EEPROM save file `eepromSlot` (0-3) into the game state. A file whose checksum fails resets the
 * state instead. While D_80082BC0 is set (scanning all files), it also records the file in its save
 * slot's SaveSlotInfo if it is newer, and keeps the 27 bytes at D_800D2E4C from the file with the most
 * bits set.
 */
void func_15006590(s32 eepromSlot) {
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
        backup[i] = ((u8 *)D_800D2E4C)[i];
    }
    if (D_800BE3F0 == NULL) {
        D_800BE3F0 = (u8 *)func_1502B5C8(NULL, 1, 7) + 8;
    }
    osWritebackDCache(D_800BE358, 0x80);
    osInvalDCache(D_800BE358, 0x80);
    block = eepromSlot * 16 + 4;
    func_151DD3A0(&D_800BE900, block, D_800BE358, 0x80);
    p = D_800BE358;
    sum = 0xCC;
    for (i = 2; i < 0x80; i++) { sum += p[i] << (i & 3); }
    p += 2;
    if (sum != *(u16 *)D_800BE358) {
        func_150064E0();
        if (D_800BE3DC >= 0 && D_80082BC0 != 0) {
            for (i = 0; i < 27; i++) {
                ((u8 *)D_800D2E4C)[i] = backup[i];
            }
        }
        if (block == 0x34) {
            D_80082BC0 = 0;
        }
    } else {
        saveSlot = *p;
        p += 5;
        sequence = p[-4] << 24;
        sequence |= p[-3] << 16;
        sequence |= p[-2] << 8;
        sequence |= p[-1];
        for (i = 0; i < 27; i++) {
            ((u8 *)D_800D2E4C)[i] = *p++;
        }
        for (i = 0; i < 9; i++) {
            D_800D2E60[i] = *p++;
        }
        D_800BE3DF = *p++;
        D_800BE3E0 = *p++;
        D_800BE3E4 = *p++ << 8;
        D_800BE3E4 |= *p++;
        func_15085710(0, 9, D_800BE3E4);
        value = *p++ << 24;
        value |= *p++ << 16;
        value |= *p++ << 8;
        value |= *p++;
        D_800BE3E8 = value;
        byte = *p++;
        bit = 0x80;
        for (i = 0; i < 69; i++) {
            D_800C3A60[i] = 0;
            mask = (u64)1 << (D_800BE3F0[i] - 1);
            for (j = 0; j < D_800BE3F0[i]; j++) {
                if (byte & bit) {
                    D_800C3A60[i] |= mask;
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
        if (D_80082BC0 != 0 && saveSlot < 3) {
            D_800BE3D8[eepromSlot] = saveSlot;
            if (D_800BE3F8[saveSlot].sequence < sequence) {
                flagCount = 0;
                for (i = 0; i < 27; i++) {
                    bits = ((u8 *)D_800D2E4C)[i];
                    for (j = 8; j != 0; j--) {
                        if (bits & 1) {
                            flagCount++;
                        }
                        bits >>= 1;
                    }
                }
                D_800BE3F8[saveSlot].unk0 = D_800BE3E8;
                D_800BE3F8[saveSlot].flagCount = flagCount;
                D_800BE3F8[saveSlot].unkC = *(s8 *)&D_800BE3DF; // matching: original loads it signed (lb)
                D_800BE3F8[saveSlot].unk4 = D_800BE3E4;
                D_800BE3F8[saveSlot].sequence = sequence;
                D_800BE3F8[saveSlot].eepromSlot = eepromSlot;
                if ((u8)flagCount >= D_800BE3DC) {
                    D_800BE3DC = (u8)flagCount;
                    if (D_800D2E68 & 0x20) {
                        D_800BE3DE = 1;
                    }
                } else {
                    for (i = 0; i < 27; i++) {
                        ((u8 *)D_800D2E4C)[i] = backup[i];
                    }
                }
            } else {
                for (i = 0; i < 27; i++) {
                    ((u8 *)D_800D2E4C)[i] = backup[i];
                }
            }
        }
        if (block == 0x34) {
            D_80082BC0 = 0;
        }
    }
}
/*
 * Writes the game state as save slot `saveSlot` (0-2). The save goes to an EEPROM save file that no
 * other slot uses. Its last block is overwritten with 0xFE first, so an interrupted write fails the
 * checksum. Layout: checksum, slot, sequence, the D_800D2E4C/D_800D2E60/D_800BE3DF/D_800BE3E0/
 * D_800BE3E4/D_800BE3E8 values, then each D_800C3A60 entry packed MSB-first in D_800BE3F0[i] bits.
 */
void func_15006BEC(s32 saveSlot) {
    u64 mask;
    s32 i;
    s32 j;
    volatile s32 ret; // matching: the dead func_151DCEF0 result is stored to the stack
    u8 *p;
    s32 eepromSlot;
    s32 sequence;
    u8 bit;
    u8 byte;
    u16 sum;

    sequence = (D_800BE3F8[saveSlot].sequence + 1) & 0x7FFFFFFF;
    j = -1; /* free EEPROM save files */
    for (i = 0; i < 3; i++) {
        if (D_800BE3F8[i].sequence >= 0) {
            j &= ~(1 << D_800BE3F8[i].eepromSlot);
        }
    }
    eepromSlot = 0;
    for (i = 0; i < 4; i++) {
        if ((1 << i) & j) {
            eepromSlot = i;
        }
    }
    D_800BE3F8[saveSlot].eepromSlot = eepromSlot;
    D_800BE3F8[saveSlot].sequence = sequence;
    if (D_8002AC5C == 0) {
        for (i = 0; i < 8; i++) { D_800BE358[i] = 0xFE; }
        osWritebackDCacheAll();
        func_151DD4E0(&D_800BE900, (eepromSlot << 4) + 0x13, D_800BE358);
    }
    if (D_800BE3F0 == NULL) {
        D_800BE3F0 = (u8 *)func_1502B5C8(NULL, 1, 7) + 8;
    }
    p = (u8 *)(u32)D_800BE35A; // matching: &D_800BE358[2] or a plain D_800BE35A changes codegen
    *p++ = saveSlot;
    *p++ = (sequence >> 24) & 0xFF;
    *p++ = (sequence >> 16) & 0xFF;
    *p++ = (sequence >> 8) & 0xFF;
    *p++ = sequence & 0xFF;
    for (i = 0; i < 27; i++) { *p++ = ((u8 *)D_800D2E4C)[i]; }
    for (i = 0; i < 9; i++) { *p++ = D_800D2E60[i]; }
    *p++ = *(s8 *)&D_800BE3DF;
    *p++ = D_800BE3E0;
    D_800BE3E4 = func_150859AC(0, 6);
    *p++ = (D_800BE3E4 >> 8) & 0xFF;
    *p++ = (D_800BE3E4) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 24) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 16) & 0xFF;
    *p++ = ((u32)D_800BE3E8 >> 8) & 0xFF;
    *p++ = (D_800BE3E8) & 0xFF;
    for (i = 0; i < 69; i++) { p[i] = 0; }
    byte = 0;
    bit = 0x80;
    for (i = 0; i < 69; i++) {
        mask = (u64)1 << (D_800BE3F0[i] - 1);
        for (j = 0; j < D_800BE3F0[i]; j++) {
            if (D_800C3A60[i] & mask) {
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
    for (i = 2; i < 0x80; i++) { sum += D_800BE358[i] << (i & 3); }
    *(u16 *)D_800BE358 = sum;
    osWritebackDCacheAll();
    if (D_8002AC5C == 0) {
        ret = func_151DCEF0(&D_800BE900, (eepromSlot << 4) + 4, D_800BE358, 0x80);
    }
}
/* Erases every EEPROM save file loaded as save slot `saveSlot` by overwriting its first two blocks. */
void func_1500707C(s32 saveSlot) {
    s32 i;
    s32 ret;
    s32 block;
    u8 *buffer = D_800BE358;

    for (i = 0; i < 8; i++) { buffer[i] = 0xFF; }
    osWritebackDCacheAll();
    if (D_8002AC5C == 0) {
        for (i = 0; i < 4; i++) {
            if (saveSlot == D_800BE3D8[i]) {
                block = i * 16 + 4;
                ret = func_151DD4E0(&D_800BE900, block, buffer);
                // matching: "+ ret - ret" and "if (ret) {}" keep the unused first result in a saved register
                ret = func_151DD4E0(&D_800BE900, block + 1, buffer) + ret - ret;
                if (ret) {}
            }
        }
    }
}
void func_15007168(void) {
    s32 i;

    for (i = 0; i < 0x68; i++) {
        D_800BE2F0[i] = 0;
    }

    for (i = 0; i < 10; i++) {
        D_80082BBC[i].unk1 = 3;
        D_80082BBC[i].unk3 = 2;
        D_80082BBC[i].unk4 = 1;
        D_80082BBC[i].unk2 = 10;
        switch (i) {
            case 0:
                D_80082BBC[i].unk5 = 3;
                break;
            case 1:
            case 2:
                D_80082BBC[i].unk5 = 5;
                break;
            case 7:
                D_80082BBC[i].unk5 = 20;
                break;
            case 5:
                D_80082BBC[i].unk5 = 20;
                D_80082BBC[i].unk2 = 0;
                break;
            case 8:
            case 9:
                D_80082BBC[i].unk5 = 3;
                break;
        }
        D_80082BBC[i].unk9 = 0;
    }
}
// PERMUTER CANDIDATE best 1165: loop structure byte-exact; remaining diff is a clean 3-register
// rotation {sum,i,p}->{v0,v1,a0} (target sum=v0,i=v1,p=a0) plus seed 0xCC reassociation.
// void func_1500727C(void) {
//     u16 sum;
//     s32 i;
//     u8 *p;
//     func_151DD3A0(&D_800BE900, 0x44, &D_800BE2F0, 0x70);
//     sum = (D_800BE2F2 << 2) + 0xCC + (D_800BE2F3 << 3);
//     p = D_800BE2F4;
//     i = 4;
//     do {
//         sum += *p++ << (i & 3);
//         i++;
//     } while (i != 0x68);
//     if (sum != D_800BE2F0) {
//         func_15007168();
//     }
// }
void func_1500727C(void) {
    s32 i;
    u16 sum;

    func_151DD3A0(&D_800BE900, 0x44, (s16 *)D_800BE2F0, 0x70);
    sum = 0xCC;
    i = 2;
    do {
        sum += D_800BE2F0[i] << (i & 3);
        i++;
    } while (i < 0x68);
    if (*(u16 *)D_800BE2F0 != sum) {
        func_15007168();
    }
}
// PERMUTER CANDIDATE best 1265: same unrolled-checksum 3-register rotation {sum,i,p}->{v0,v1,a0}
// as func_1500727C, plus target keeps the (dead) func_151DCEF0 return in a stack slot (frame -0x20
// vs my -0x18) which my build DCEs. Reconstruction:
// void func_15007360(void) {
//     u16 sum; s32 i; u8 *p; s32 ret;
//     sum = (D_800BE2F2 << 2) + 0xCC + (D_800BE2F3 << 3);
//     p = D_800BE2F4; i = 4;
//     do { sum += *p++ << (i & 3); i++; } while (i != 0x68);
//     D_800BE2F0 = sum;
//     if (D_8002AC5C == 0) { ret = func_151DCEF0(&D_800BE900, 0x44, &D_800BE2F0, 0x70); }
// }
void func_15007360(void) {
    u16 sum;
    volatile s32 ret;
    s32 i;

    sum = 0xCC;
    for (i = 2; i < 0x68; i++) { sum += D_800BE2F0[i] << (i & 3); }
    *(u16 *)D_800BE2F0 = sum;
    if (D_8002AC5C == 0) {
        ret = func_151DCEF0(&D_800BE900, 0x44, D_800BE2F0, 0x70);
    }
}
/* Keep each loop body on one line: under -g3 the line table changes the unrolled store order. */
void func_15007440(void) {
    u16 sum;
    s32 i;
    s8 saved;

    func_151DD3A0(&D_800BE900, 0, &D_800BE358, 0x20);
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += D_800BE358[i] << (i & 3); }
    saved = D_800E0BE0[0x10];
    if (sum != *(u16 *)D_800BE358) {
        for (i = 0; i < 0x1C; i++) { D_800E0BE0[i] = -1; }
    } else {
        for (i = 0; i < 0x1C; i++) { D_800E0BE0[i] = D_800BE358[i + 2]; }
    }
    D_800E0BE0[0x10] = saved;
    func_151DD9E4();
    func_151E7EF8();
    D_8008FE30 = 1;
}

/* The dead func_151DCEF0 result is kept on the stack via a volatile local (sw v0,0x18(sp)). */
void func_15007558(void) {
    u16 sum;
    volatile s32 ret;
    s32 i;

    for (i = 0; i < 0x1C; i++) { D_800BE358[i + 2] = D_800E0BE0[i]; }
    sum = 0xCC;
    for (i = 2; i < 0x1E; i++) { sum += D_800BE358[i] << (i & 3); }
    *(u16 *)D_800BE358 = sum;
    if (D_8002AC5C == 0) {
        ret = func_151DCEF0(&D_800BE900, 0, D_800BE358, 0x20);
    }
}

void func_15007644(void) {
}

void func_1500764C(void) {
    D_80082BB4 = 6;
    D_800BE3EC = 0;
}

void func_15007668(void) {
    D_80082BB4 = 7;
    D_800BE3EC = 0;
}

void func_15007684(void) {
    D_80082BB4 = 4;
    D_800BE3EC = 0;
}

void func_150076A0(void) {
    D_80082BB4 = 5;
    D_800BE3EC = 0;
}

void func_150076BC(s32 arg0) {
    if (arg0 < 0) {
        func_150064E0();
    }
    if ((arg0 >= 0) && (arg0 < 4) && (D_800BE616 == 0)) {
        D_80082BB4 = 1;
        D_800BE3EC = arg0;
    }
}

void func_15007718(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3) && (D_800BE616 == 0)) {
        D_80082BB4 = 2;
        D_800BE3EC = arg0;
    }
}

void func_15007750(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3)) {
        D_80082BB4 = 3;
        D_800BE3EC = arg0;
    }
}

void func_15007778(void) {
    s32 i;
    ((struct162 *)&D_800BE3F8)->unk8 = -1;
    ((struct162 *)&D_800BE3F8)->unk18 = -1;
    ((struct162 *)&D_800BE3F8)->unk28 = -1;
    ((struct162 *)&D_800BE3F8)->unkE = -1;
    ((struct162 *)&D_800BE3F8)->unk1E = -1;
    ((struct162 *)&D_800BE3F8)->unk2E = -1;
    D_800BE3DC = -1;
    D_800BE3DE = 0;
    D_80082BC0 = 1;
    for (i = 0; i < 27; i++) { ((u8 *)D_800D2E4C)[i] = 0; }
    for (i = 0; i < 9; i++) { D_800D2E60[i] = 0; }
    for (i = 0; i < 4; i++) { D_800BE3D8[i] = -1; }
}

