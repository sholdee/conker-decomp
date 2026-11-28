#include <ultra64.h>
#include "conker/script.h"

/*
 * A data-only unit: the resource (task) manifest and the handler tables of the script commands.
 *
 * The unit lies between game/Script/CutsceneCmd/cutscenecmd and game/Script/TaskTable/tasktable. Its
 * .rodata (0x242F30, the manifest's dependency lists and names) follows the jump table of cutscenecmd and
 * ends at 0x243B84 with the padding of a unit, before effectcmd's .rodata. Its .data (0x22BEF0) follows
 * that of game/Script/TaskSet/taskset. The handler tables come directly after the manifest (0x22CEE0), and
 * none of the code units after it (tasktable .. game/Script/TaskCmd/taskcmd) defines them, so they are
 * taken as the tail of this unit.
 *
 * The manifest: one ResourceDescriptor for each task (game/Script/TaskTable/tasktable reads it). The names
 * are game text, so the .rodata of this unit stays data that the build extracts from the ROM; the
 * descriptors refer to its lists and names by their address names.
 *
 * The handler tables: bits 12-15 of a command word select the class (entry class - 1), bits 0-11 the
 * command or object id. The interpreter (game/Script/ScriptInterp/scriptinterp.s) runs the start and end
 * handlers of gScriptStartHandlers / gScriptEndHandlers and the query and command handlers of
 * gScriptQueryHandlers / gScriptCommandHandlers; the dispatchers script_query / script_command
 * (game/Script/TaskSet/taskset) call the last two. A null entry is a class that has no handler of that
 * kind. The handlers come from eleven files (game/Script/CameraCmd/cameracmd ..
 * game/Script/ZoneCmd/zonecmd) and have different parameter types, so the tables are declared without a
 * prototype.
 */

/* The dependency lists of the manifest (resource indices, RESOURCE_LIST_END-terminated): extracted .rodata. */
extern const u16 D_8009E470[], D_8009E474[], D_8009E478[], D_8009E47C[], D_8009E480[], D_8009E484[];
extern const u16 D_8009E488[], D_8009E48C[], D_8009E490[], D_8009E498[], D_8009E49C[], D_8009E4A8[];
extern const u16 D_8009E4AC[], D_8009E4B4[], D_8009E4BC[], D_8009E4CC[], D_8009E4D4[], D_8009E4DC[];
extern const u16 D_8009E4E0[], D_8009E4E4[], D_8009E4E8[], D_8009E4F0[], D_8009E4F8[], D_8009E4FC[];
extern const u16 D_8009E504[], D_8009E508[], D_8009E50C[], D_8009E510[], D_8009E514[], D_8009E51C[];
extern const u16 D_8009E528[], D_8009E52C[], D_8009E530[], D_8009E534[], D_8009E540[], D_8009E54C[];
extern const u16 D_8009E550[], D_8009E560[], D_8009E564[], D_8009E568[], D_8009E570[], D_8009E574[];
extern const u16 D_8009E57C[], D_8009E580[], D_8009E584[], D_8009E588[], D_8009E58C[], D_8009E598[];
extern const u16 D_8009E5A4[], D_8009E5B0[], D_8009E5B4[], D_8009E5B8[], D_8009E5C0[], D_8009E5C4[];
extern const u16 D_8009E5C8[], D_8009E5CC[], D_8009E5D0[], D_8009E5D4[], D_8009E5D8[], D_8009E5E0[];
extern const u16 D_8009E5E8[], D_8009E5EC[], D_8009E5F4[], D_8009E5F8[], D_8009E600[], D_8009E608[];
extern const u16 D_8009E60C[], D_8009E618[], D_8009E61C[], D_8009E620[], D_8009E624[], D_8009E628[];
extern const u16 D_8009E62C[], D_8009E630[], D_8009E634[], D_8009E638[], D_8009E63C[], D_8009E640[];
extern const u16 D_8009E648[], D_8009E650[], D_8009E658[], D_8009E660[], D_8009E664[], D_8009E668[];
extern const u16 D_8009E674[], D_8009E67C[], D_8009E684[], D_8009E688[], D_8009E68C[], D_8009E690[];
extern const u16 D_8009E694[], D_8009E698[], D_8009E69C[], D_8009E6A0[], D_8009E6A8[], D_8009E6AC[];
extern const u16 D_8009E6B0[], D_8009E6B4[], D_8009E6BC[], D_8009E6C0[], D_8009E6C4[], D_8009E6C8[];
extern const u16 D_8009E6CC[], D_8009E6D0[], D_8009E6D4[], D_8009E6D8[], D_8009E6DC[], D_8009E6E0[];
extern const u16 D_8009E6E4[], D_8009E6EC[], D_8009E6F0[], D_8009E6F4[], D_8009E6F8[], D_8009E700[];
extern const u16 D_8009E704[], D_8009E708[], D_8009E70C[], D_8009E710[], D_8009E714[], D_8009E720[];
extern const u16 D_8009E728[], D_8009E72C[], D_8009E734[], D_8009E738[], D_8009E73C[], D_8009E740[];
extern const u16 D_8009E744[], D_8009E748[], D_8009E74C[], D_8009E754[], D_8009E758[], D_8009E75C[];
extern const u16 D_8009E760[], D_8009E764[], D_8009E768[], D_8009E76C[], D_8009E770[], D_8009E774[];
extern const u16 D_8009E778[], D_8009E77C[], D_8009E780[], D_8009E784[], D_8009E788[], D_8009E78C[];
extern const u16 D_8009E790[], D_8009E794[], D_8009E798[], D_8009E79C[], D_8009E7A0[], D_8009E7A4[];
extern const u16 D_8009E7A8[], D_8009E7AC[], D_8009E7B4[], D_8009E7B8[], D_8009E7BC[], D_8009E7C0[];
extern const u16 D_8009E7C4[], D_8009E7C8[], D_8009E7CC[], D_8009E7D0[], D_8009E7D4[], D_8009E7DC[];
extern const u16 D_8009E7E0[], D_8009E7E4[], D_8009E7E8[], D_8009E7EC[], D_8009E7F0[], D_8009E7F4[];
extern const u16 D_8009E7F8[], D_8009E7FC[], D_8009E800[], D_8009E804[], D_8009E808[], D_8009E810[];
extern const u16 D_8009E818[], D_8009E820[], D_8009E824[], D_8009E82C[], D_8009E830[];

/* The names of the manifest: extracted .rodata (game text). */
extern const char D_8009E834[], D_8009E84C[], D_8009E860[], D_8009E878[], D_8009E890[], D_8009E8AC[];
extern const char D_8009E8C8[], D_8009E8EC[], D_8009E900[], D_8009E910[], D_8009E928[], D_8009E93C[];
extern const char D_8009E950[], D_8009E960[], D_8009E974[], D_8009E98C[], D_8009E9AC[], D_8009E9B8[];
extern const char D_8009E9C8[], D_8009E9D8[], D_8009E9F4[], D_8009EA04[], D_8009EA20[], D_8009EA30[];
extern const char D_8009EA44[], D_8009EA54[], D_8009EA68[], D_8009EA7C[], D_8009EA90[], D_8009EAA8[];
extern const char D_8009EAB8[], D_8009EAD0[], D_8009EAE8[], D_8009EB00[], D_8009EB10[], D_8009EB24[];
extern const char D_8009EB40[], D_8009EB54[], D_8009EB68[], D_8009EB80[], D_8009EB94[], D_8009EBA8[];
extern const char D_8009EBBC[], D_8009EBC8[], D_8009EBDC[], D_8009EBEC[], D_8009EC04[], D_8009EC14[];
extern const char D_8009EC28[], D_8009EC34[], D_8009EC4C[], D_8009EC5C[], D_8009EC70[], D_8009EC7C[];
extern const char D_8009EC8C[], D_8009ECA0[], D_8009ECB0[], D_8009ECC0[], D_8009ECD4[], D_8009ECE8[];
extern const char D_8009ECFC[], D_8009ED0C[], D_8009ED24[], D_8009ED34[], D_8009ED44[], D_8009ED54[];
extern const char D_8009ED6C[], D_8009ED80[], D_8009ED98[], D_8009EDA8[], D_8009EDB4[], D_8009EDC4[];
extern const char D_8009EDD8[], D_8009EDE8[], D_8009EE00[], D_8009EE18[], D_8009EE28[], D_8009EE3C[];
extern const char D_8009EE54[], D_8009EE64[], D_8009EE78[], D_8009EE88[], D_8009EE9C[], D_8009EEAC[];
extern const char D_8009EEB8[], D_8009EECC[], D_8009EEDC[], D_8009EEEC[], D_8009EF00[], D_8009EF10[];
extern const char D_8009EF1C[], D_8009EF30[], D_8009EF40[], D_8009EF50[], D_8009EF5C[], D_8009EF6C[];
extern const char D_8009EF78[], D_8009EF94[], D_8009EFA4[], D_8009EFB0[], D_8009EFC0[], D_8009EFCC[];
extern const char D_8009EFD4[], D_8009EFE4[], D_8009EFF0[], D_8009EFFC[], D_8009F008[], D_8009F018[];
extern const char D_8009F02C[], D_8009F03C[], D_8009F04C[], D_8009F060[], D_8009F074[], D_8009F098[];
extern const char D_8009F0A4[], D_8009F0B0[];

/* The manifest: { name, packed id, level, packed spawn point, dependencies, related dependencies }. */
ResourceDescriptor gTaskManifest[RESOURCE_COUNT] = {
    /*   0 */ { D_8009E834, 0xA0000002, 0x29, 0xD000, D_8009E474, D_8009E470 },
    /*   1 */ { NULL, 0x60000000, 0x29, 0xD000, D_8009E480, D_8009E470 },
    /*   2 */ { NULL, 0x60000000, 0x29, 0xD000, D_8009E484, D_8009E470 },
    /*   3 */ { NULL, 0x60000000, 0x29, 0xD000, D_8009E488, D_8009E470 },
    /*   4 */ { D_8009E84C, 0x20000003, 0x29, 0xD000, D_8009E478, D_8009E47C },
    /*   5 */ { D_8009E860, 0x20000004, 0x29, 0xD000, D_8009E48C, D_8009E490 },
    /*   6 */ { D_8009E878, 0x20000005, 0x29, 0xD006, D_8009E498, D_8009E49C },
    /*   7 */ { NULL, 0x60000000, 0x29, 0xD006, D_8009E4A8, D_8009E470 },
    /*   8 */ { NULL, 0x60000000, 0x29, 0xD000, D_8009E470, D_8009E470 },
    /*   9 */ { D_8009E890, 0x20000006, 0x29, 0xD000, D_8009E470, D_8009E470 },
    /*  10 */ { D_8009E8AC, 0xA0000007, 0x06, 0xD000, D_8009E4AC, D_8009E470 },
    /*  11 */ { D_8009E8C8, 0x20000008, 0x06, 0xD000, D_8009E4B4, D_8009E4BC },
    /*  12 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4DC, D_8009E470 },
    /*  13 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4CC, D_8009E470 },
    /*  14 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4D4, D_8009E470 },
    /*  15 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4E0, D_8009E470 },
    /*  16 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4E4, D_8009E470 },
    /*  17 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E470, D_8009E470 },
    /*  18 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4E8, D_8009E470 },
    /*  19 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E470, D_8009E470 },
    /*  20 */ { D_8009E8EC, 0xA0000009, 0x06, 0xD00B, D_8009E470, D_8009E4F0 },
    /*  21 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E4F8, D_8009E470 },
    /*  22 */ { NULL, 0x60000000, 0x06, 0xD00B, D_8009E4FC, D_8009E470 },
    /*  23 */ { NULL, 0x60000000, 0x06, 0xD006, D_8009E504, D_8009E470 },
    /*  24 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E508, D_8009E470 },
    /*  25 */ { D_8009E900, 0x0000000A, 0x06, 0xD000, D_8009E470, D_8009E470 },
    /*  26 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E50C, D_8009E470 },
    /*  27 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E510, D_8009E470 },
    /*  28 */ { D_8009E910, 0x20000010, 0x06, 0xD006, D_8009E514, D_8009E51C },
    /*  29 */ { NULL, 0x60000000, 0x06, 0xD006, D_8009E528, D_8009E470 },
    /*  30 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E52C, D_8009E470 },
    /*  31 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E570, D_8009E470 },
    /*  32 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E570, D_8009E470 },
    /*  33 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E574, D_8009E470 },
    /*  34 */ { D_8009E928, 0xA000002F, 0x19, 0x0000, D_8009E57C, D_8009E470 },
    /*  35 */ { D_8009E93C, 0xA000000B, 0x06, 0xD000, D_8009E470, D_8009E5A4 },
    /*  36 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E580, D_8009E470 },
    /*  37 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E584, D_8009E470 },
    /*  38 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E588, D_8009E470 },
    /*  39 */ { D_8009E950, 0x2000000C, 0x06, 0xD000, D_8009E58C, D_8009E598 },
    /*  40 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E470, D_8009E470 },
    /*  41 */ { NULL, 0x60000000, 0x06, 0xD000, D_8009E470, D_8009E470 },
    /*  42 */ { D_8009E960, 0x2000005D, 0x3B, 0xD002, D_8009E530, D_8009E470 },
    /*  43 */ { D_8009E974, 0xA0000043, 0x27, 0x0000, D_8009E534, D_8009E540 },
    /*  44 */ { D_8009E98C, 0x20000044, 0x27, 0x0000, D_8009E54C, D_8009E470 },
    /*  45 */ { D_8009E9AC, 0xA0000015, 0x0C, 0xD000, D_8009E5E0, D_8009E5E8 },
    /*  46 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E5B0, D_8009E470 },
    /*  47 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E470, D_8009E470 },
    /*  48 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E5B4, D_8009E470 },
    /*  49 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E470, D_8009E470 },
    /*  50 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E470, D_8009E470 },
    /*  51 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E5B8, D_8009E470 },
    /*  52 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E470, D_8009E470 },
    /*  53 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E5C0, D_8009E470 },
    /*  54 */ { D_8009E9B8, 0x20000017, 0x0C, 0xD000, D_8009E5C4, D_8009E470 },
    /*  55 */ { D_8009E9C8, 0x20000018, 0x0C, 0xD000, D_8009E5C8, D_8009E5CC },
    /*  56 */ { D_8009E9D8, 0x20000019, 0x0C, 0xD000, D_8009E5D0, D_8009E470 },
    /*  57 */ { NULL, 0x60000000, 0x0C, 0xD000, D_8009E470, D_8009E470 },
    /*  58 */ { NULL, 0x60000000, 0x0C, 0xD005, D_8009E5D4, D_8009E470 },
    /*  59 */ { D_8009E9F4, 0xA0000016, 0x0C, 0xD005, D_8009E5D8, D_8009E470 },
    /*  60 */ { D_8009EA04, 0xA000001A, 0x01, 0xD000, D_8009E5EC, D_8009E470 },
    /*  61 */ { D_8009EA20, 0x2000001B, 0x01, 0xD001, D_8009E5F8, D_8009E470 },
    /*  62 */ { NULL, 0x60000000, 0x01, 0xD001, D_8009E600, D_8009E470 },
    /*  63 */ { D_8009EA30, 0x2000001C, 0x01, 0xD001, D_8009E608, D_8009E470 },
    /*  64 */ { NULL, 0x60000000, 0x01, 0xD000, D_8009E5F4, D_8009E470 },
    /*  65 */ { D_8009EA44, 0x2000001D, 0x01, 0xD000, D_8009E60C, D_8009E470 },
    /*  66 */ { D_8009EA54, 0x20000020, 0x01, 0xD002, D_8009E620, D_8009E470 },
    /*  67 */ { D_8009EA68, 0x2000001E, 0x10, 0xD000, D_8009E618, D_8009E470 },
    /*  68 */ { D_8009EA7C, 0x2000001F, 0x10, 0xD000, D_8009E61C, D_8009E470 },
    /*  69 */ { D_8009EA90, 0xA0000021, 0x07, 0xD000, D_8009E668, D_8009E470 },
    /*  70 */ { D_8009EAA8, 0x20000025, 0x07, 0xD001, D_8009E658, D_8009E660 },
    /*  71 */ { NULL, 0x60000000, 0x07, 0xD001, D_8009E664, D_8009E470 },
    /*  72 */ { D_8009EAB8, 0x2000002E, 0x07, 0xD002, D_8009E674, D_8009E67C },
    /*  73 */ { NULL, 0x60000000, 0x07, 0xD002, D_8009E684, D_8009E688 },
    /*  74 */ { NULL, 0x60000000, 0x07, 0xD000, D_8009E470, D_8009E68C },
    /*  75 */ { D_8009EAD0, 0x20000026, 0x12, 0xD000, D_8009E624, D_8009E470 },
    /*  76 */ { D_8009EAE8, 0x20000027, 0x12, 0xD000, D_8009E628, D_8009E470 },
    /*  77 */ { D_8009EB00, 0x20000028, 0x12, 0xD000, D_8009E62C, D_8009E470 },
    /*  78 */ { NULL, 0x60000000, 0x12, 0xD000, D_8009E630, D_8009E470 },
    /*  79 */ { D_8009EB10, 0x2000002D, 0x12, 0xD002, D_8009E638, D_8009E470 },
    /*  80 */ { D_8009EB24, 0x20000029, 0x17, 0xD000, D_8009E634, D_8009E470 },
    /*  81 */ { D_8009EB40, 0x20000023, 0x0B, 0xD000, D_8009E640, D_8009E470 },
    /*  82 */ { NULL, 0x60000000, 0x0B, 0xD000, D_8009E650, D_8009E470 },
    /*  83 */ { D_8009EB54, 0x20000024, 0x0B, 0xD000, D_8009E470, D_8009E648 },
    /*  84 */ { NULL, 0x60000000, 0x0B, 0xD000, D_8009E470, D_8009E470 },
    /*  85 */ { D_8009EB68, 0x20000022, 0x0B, 0xD000, D_8009E63C, D_8009E470 },
    /*  86 */ { NULL, 0x60000000, 0x0B, 0xD000, D_8009E470, D_8009E470 },
    /*  87 */ { D_8009EB80, 0x20000030, 0x02, 0xD000, D_8009E690, D_8009E470 },
    /*  88 */ { NULL, 0x60000000, 0x02, 0xD000, D_8009E694, D_8009E470 },
    /*  89 */ { D_8009EB94, 0x20000033, 0x02, 0xD005, D_8009E698, D_8009E470 },
    /*  90 */ { NULL, 0x60000000, 0x02, 0xD005, D_8009E698, D_8009E470 },
    /*  91 */ { NULL, 0x60000000, 0x02, 0xD000, D_8009E6AC, D_8009E470 },
    /*  92 */ { NULL, 0x60000000, 0x02, 0xD000, D_8009E6B0, D_8009E470 },
    /*  93 */ { NULL, 0x60000000, 0x02, 0xD000, D_8009E69C, D_8009E470 },
    /*  94 */ { D_8009EBA8, 0x20000034, 0x02, 0xD007, D_8009E6A0, D_8009E470 },
    /*  95 */ { NULL, 0x60000000, 0x02, 0xD000, D_8009E6A0, D_8009E470 },
    /*  96 */ { D_8009EBBC, 0x20000036, 0x02, 0xD002, D_8009E6A8, D_8009E470 },
    /*  97 */ { NULL, 0x60000000, 0x02, 0xD003, D_8009E6C8, D_8009E470 },
    /*  98 */ { D_8009EBC8, 0x2000003C, 0x02, 0xD006, D_8009E6CC, D_8009E470 },
    /*  99 */ { NULL, 0x60000000, 0x02, 0xD004, D_8009E470, D_8009E470 },
    /* 100 */ { NULL, 0x60000000, 0x02, 0xD004, D_8009E6D0, D_8009E470 },
    /* 101 */ { NULL, 0x60000000, 0x42, 0x0000, D_8009E470, D_8009E470 },
    /* 102 */ { D_8009EBDC, 0x20000031, 0x43, 0xD001, D_8009E6EC, D_8009E470 },
    /* 103 */ { D_8009EBEC, 0x20000032, 0x43, 0xD001, D_8009E6F0, D_8009E470 },
    /* 104 */ { D_8009EC04, 0x20000035, 0x43, 0xD000, D_8009E6F4, D_8009E470 },
    /* 105 */ { D_8009EC14, 0x2000003D, 0x43, 0xD000, D_8009E6F8, D_8009E470 },
    /* 106 */ { NULL, 0x60000000, 0x44, 0x0000, D_8009E470, D_8009E470 },
    /* 107 */ { D_8009EC28, 0x20000038, 0x31, 0xD000, D_8009E6B4, D_8009E470 },
    /* 108 */ { D_8009EC34, 0x20000037, 0x31, 0xD000, D_8009E6B4, D_8009E470 },
    /* 109 */ { NULL, 0x60000000, 0x31, 0xD000, D_8009E470, D_8009E470 },
    /* 110 */ { D_8009EC4C, 0x20000039, 0x31, 0xD000, D_8009E6BC, D_8009E470 },
    /* 111 */ { NULL, 0x60000000, 0x31, 0xD000, D_8009E6C0, D_8009E470 },
    /* 112 */ { D_8009EC5C, 0x2000003A, 0x31, 0xD000, D_8009E6C4, D_8009E470 },
    /* 113 */ { D_8009EC70, 0xA0000040, 0x14, 0xD000, D_8009E708, D_8009E470 },
    /* 114 */ { D_8009EC7C, 0x20000041, 0x14, 0xD000, D_8009E70C, D_8009E470 },
    /* 115 */ { D_8009EC8C, 0x20000042, 0x14, 0xD000, D_8009E710, D_8009E470 },
    /* 116 */ { NULL, 0x60000000, 0x37, 0xD002, D_8009E710, D_8009E470 },
    /* 117 */ { D_8009ECA0, 0x8000000D, 0x2C, 0xD000, D_8009E714, D_8009E720 },
    /* 118 */ { NULL, 0x40000000, 0x2C, 0xD000, D_8009E728, D_8009E470 },
    /* 119 */ { NULL, 0x60000000, 0x2C, 0xD001, D_8009E470, D_8009E470 },
    /* 120 */ { D_8009ECB0, 0x2000000F, 0x2C, 0xD001, D_8009E72C, D_8009E470 },
    /* 121 */ { NULL, 0x60000000, 0x2C, 0xD001, D_8009E470, D_8009E470 },
    /* 122 */ { D_8009ECC0, 0xA0000011, 0x35, 0xD000, D_8009E550, D_8009E470 },
    /* 123 */ { D_8009ECD4, 0x20000012, 0x35, 0xD000, D_8009E560, D_8009E470 },
    /* 124 */ { D_8009ECE8, 0x20000013, 0x35, 0xD000, D_8009E564, D_8009E470 },
    /* 125 */ { NULL, 0x60000000, 0x35, 0xD000, D_8009E470, D_8009E470 },
    /* 126 */ { D_8009ECFC, 0xA0000014, 0x37, 0xD004, D_8009E568, D_8009E470 },
    /* 127 */ { NULL, 0x60000000, 0x19, 0x0000, D_8009E470, D_8009E470 },
    /* 128 */ { NULL, 0x60000000, 0x19, 0x0000, D_8009E470, D_8009E470 },
    /* 129 */ { NULL, 0x60000000, 0x19, 0x0000, D_8009E470, D_8009E470 },
    /* 130 */ { D_8009ED0C, 0xA000003E, 0x32, 0x0000, D_8009E700, D_8009E470 },
    /* 131 */ { D_8009ED24, 0x2000003F, 0x32, 0x0000, D_8009E704, D_8009E470 },
    /* 132 */ { D_8009ED34, 0x0000000E, 0x00, 0xD000, D_8009E734, D_8009E470 },
    /* 133 */ { NULL, 0x40000000, 0x00, 0xD000, D_8009E734, D_8009E738 },
    /* 134 */ { D_8009ED44, 0x2000002A, 0x0A, 0xD000, D_8009E73C, D_8009E470 },
    /* 135 */ { D_8009ED54, 0x2000002B, 0x0A, 0xD000, D_8009E740, D_8009E470 },
    /* 136 */ { D_8009ED6C, 0x2000002C, 0x0A, 0xD004, D_8009E744, D_8009E470 },
    /* 137 */ { NULL, 0x60000000, 0x0A, 0xD004, D_8009E748, D_8009E470 },
    /* 138 */ { D_8009ED80, 0x2000004C, 0x04, 0xD000, D_8009E74C, D_8009E470 },
    /* 139 */ { NULL, 0x60000000, 0x04, 0xD001, D_8009E754, D_8009E470 },
    /* 140 */ { NULL, 0x60000000, 0x23, 0xD002, D_8009E758, D_8009E470 },
    /* 141 */ { D_8009ED98, 0x2000005C, 0x04, 0x0000, D_8009E75C, D_8009E470 },
    /* 142 */ { D_8009EDA8, 0x2000004D, 0x1B, 0xD000, D_8009E760, D_8009E470 },
    /* 143 */ { NULL, 0x60000000, 0x1B, 0xD004, D_8009E764, D_8009E470 },
    /* 144 */ { D_8009EDB4, 0x2000004E, 0x1E, 0xD000, D_8009E768, D_8009E470 },
    /* 145 */ { D_8009EDC4, 0x20000050, 0x26, 0xD000, D_8009E76C, D_8009E470 },
    /* 146 */ { D_8009EDD8, 0x20000051, 0x26, 0xD000, D_8009E770, D_8009E470 },
    /* 147 */ { D_8009EDE8, 0x20000052, 0x1C, 0xD000, D_8009E790, D_8009E470 },
    /* 148 */ { D_8009EE00, 0x20000053, 0x1C, 0xD000, D_8009E794, D_8009E470 },
    /* 149 */ { NULL, 0x60000000, 0x1C, 0xD000, D_8009E798, D_8009E470 },
    /* 150 */ { D_8009EE18, 0x20000054, 0x1C, 0xD002, D_8009E79C, D_8009E7A0 },
    /* 151 */ { D_8009EE28, 0x20000055, 0x1C, 0xD002, D_8009E7A4, D_8009E7A8 },
    /* 152 */ { NULL, 0x60000000, 0x0E, 0xD000, D_8009E7AC, D_8009E7B4 },
    /* 153 */ { NULL, 0x60000000, 0x0E, 0xD000, D_8009E7B8, D_8009E470 },
    /* 154 */ { D_8009EE3C, 0x20000056, 0x0E, 0xD000, D_8009E7BC, D_8009E470 },
    /* 155 */ { D_8009EE54, 0x20000058, 0x0E, 0xD000, D_8009E7C0, D_8009E470 },
    /* 156 */ { D_8009EE64, 0x20000057, 0x28, 0x0000, D_8009E7C4, D_8009E470 },
    /* 157 */ { NULL, 0x60000000, 0x28, 0x0000, D_8009E7C8, D_8009E470 },
    /* 158 */ { D_8009EE78, 0xA000004A, 0x13, 0x0000, D_8009E774, D_8009E778 },
    /* 159 */ { D_8009EE88, 0x2000004B, 0x13, 0x0000, D_8009E774, D_8009E77C },
    /* 160 */ { NULL, 0x60000000, 0x13, 0x0000, D_8009E780, D_8009E470 },
    /* 161 */ { D_8009EE9C, 0x20000059, 0x23, 0x0000, D_8009E784, D_8009E470 },
    /* 162 */ { D_8009EEAC, 0x2000005A, 0x23, 0x0000, D_8009E788, D_8009E470 },
    /* 163 */ { D_8009EEB8, 0x2000005B, 0x23, 0xD002, D_8009E78C, D_8009E470 },
    /* 164 */ { D_8009EECC, 0x2000004F, 0x2F, 0xD000, D_8009E7CC, D_8009E470 },
    /* 165 */ { NULL, 0x60000000, 0x2F, 0xD000, D_8009E7D0, D_8009E470 },
    /* 166 */ { D_8009EEDC, 0xA0000045, 0x41, 0x0000, D_8009E7D4, D_8009E470 },
    /* 167 */ { D_8009EEEC, 0x20000046, 0x41, 0xD004, D_8009E7DC, D_8009E470 },
    /* 168 */ { NULL, 0x60000000, 0x3D, 0x0000, D_8009E7E0, D_8009E470 },
    /* 169 */ { D_8009EF00, 0x20000047, 0x3C, 0xD005, D_8009E7E0, D_8009E470 },
    /* 170 */ { NULL, 0x60000000, 0x3C, 0xD005, D_8009E7E4, D_8009E470 },
    /* 171 */ { D_8009EF10, 0x20000048, 0x3C, 0x0000, D_8009E7E8, D_8009E470 },
    /* 172 */ { NULL, 0x60000000, 0x3C, 0x0000, D_8009E7E8, D_8009E470 },
    /* 173 */ { D_8009EF1C, 0x20000049, 0x3C, 0x0000, D_8009E7EC, D_8009E470 },
    /* 174 */ { NULL, 0x60000000, 0x41, 0x0000, D_8009E7F0, D_8009E470 },
    /* 175 */ { D_8009EF30, 0xA000005E, 0x39, 0xD001, D_8009E7F4, D_8009E470 },
    /* 176 */ { NULL, 0x60000000, 0x39, 0xD001, D_8009E7F8, D_8009E470 },
    /* 177 */ { D_8009EF40, 0x2000005F, 0x36, 0x0000, D_8009E7FC, D_8009E470 },
    /* 178 */ { D_8009EF50, 0x20000060, 0x3A, 0x0000, D_8009E800, D_8009E470 },
    /* 179 */ { D_8009EF5C, 0x2000003B, 0x38, 0x0000, D_8009E6D4, D_8009E470 },
    /* 180 */ { D_8009EF6C, 0x20000061, 0x40, 0x0000, D_8009E6D8, D_8009E470 },
    /* 181 */ { D_8009EF78, 0x20000062, 0x40, 0xD003, D_8009E6DC, D_8009E470 },
    /* 182 */ { D_8009EF94, 0x20000063, 0x40, 0xD001, D_8009E6E0, D_8009E470 },
    /* 183 */ { D_8009EFA4, 0xA0000065, 0x1A, 0x0000, D_8009E470, D_8009E470 },
    /* 184 */ { D_8009EFB0, 0x20000066, 0x24, 0x0000, D_8009E470, D_8009E470 },
    /* 185 */ { D_8009EFC0, 0x20000067, 0x2B, 0x0000, D_8009E470, D_8009E470 },
    /* 186 */ { D_8009EFCC, 0x20000068, 0x2D, 0x0000, D_8009E470, D_8009E470 },
    /* 187 */ { D_8009EFD4, 0x20000069, 0x30, 0x0000, D_8009E470, D_8009E470 },
    /* 188 */ { D_8009EFE4, 0x2000006A, 0x33, 0x0000, D_8009E470, D_8009E470 },
    /* 189 */ { D_8009EFF0, 0x2000006B, 0x34, 0x0000, D_8009E470, D_8009E470 },
    /* 190 */ { D_8009EFFC, 0x2000006C, 0x3F, 0x0000, D_8009E470, D_8009E470 },
    /* 191 */ { D_8009F008, 0x20000001, 0x18, 0x0000, D_8009E470, D_8009E470 },
    /* 192 */ { D_8009F018, 0x2000006E, 0x29, 0x0009, D_8009E808, D_8009E470 },
    /* 193 */ { D_8009F02C, 0x2000006F, 0x06, 0xD000, D_8009E810, D_8009E470 },
    /* 194 */ { D_8009F03C, 0x20000070, 0x06, 0xD000, D_8009E818, D_8009E470 },
    /* 195 */ { D_8009F04C, 0xA000006D, 0x39, 0x0000, D_8009E804, D_8009E470 },
    /* 196 */ { D_8009F060, 0x20000071, 0x35, 0x0000, D_8009E820, D_8009E470 },
    /* 197 */ { NULL, 0x60000000, 0x06, 0xD01A, D_8009E824, D_8009E470 },
    /* 198 */ { NULL, 0x60000000, 0x39, 0xD001, D_8009E82C, D_8009E470 },
    /* 199 */ { D_8009F074, 0x20000064, 0x40, 0x0000, D_8009E6E4, D_8009E470 },
    /* 200 */ { NULL, 0x60000000, 0x1D, 0x0000, D_8009E470, D_8009E470 },
    /* 201 */ { D_8009F098, 0xA0000000, 0x21, 0x0000, D_8009E470, D_8009E470 },
    /* 202 */ { D_8009F0A4, 0x20000072, 0x22, 0x0000, D_8009E470, D_8009E470 },
    /* 203 */ { D_8009F0B0, 0x20000073, 0x18, 0x0000, D_8009E830, D_8009E470 },
};

/* Start handlers, by class: (id, record). */
s32 (*gScriptStartHandlers[15])() = {
    NULL,           /* 1 */
    script_spawnActor,  /* 2: game/Script/ActorCmd/actorcmd */
    NULL,           /* 3 */
    NULL,           /* 4 */
    NULL,           /* 5 */
    NULL,           /* 6 */
    script_playSound,  /* 7: game/Script/ObjectCmd/objectcmd */
    script_spawnSwarm,  /* 8: game/Script/SwarmCmd/swarmcmd */
    NULL,           /* 9 */
    NULL,           /* 10 */
    script_startTimer,  /* 11: game/Script/TimerCmd/timercmd */
    NULL,           /* 12 */
    NULL,           /* 13 */
    script_startEffect,  /* 14: game/Script/EffectCmd/effectcmd */
    script_initSpringValue,  /* 15: game/Script/MiscCmd/misccmd */
};

/* End handlers, by class: (id, record). */
s32 (*gScriptEndHandlers[15])() = {
    NULL,
    script_removeActor,
    NULL,
    NULL,
    NULL,
    NULL,
    script_stopSound,
    script_destroySwarm,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    script_endEffect,
    script_emptyMiscEnd,
};

/* Query handlers, by class: (id, opcode, record). */
s32 (*gScriptQueryHandlers[15])() = {
    script_cutsceneQuery,  /* 1: game/Script/CutsceneCmd/cutscenecmd */
    script_actorQuery,  /* 2: game/Script/ActorCmd/actorcmd */
    script_worldObjectQuery,  /* 3: game/Script/ObjectCmd/objectcmd */
    script_zoneQuery,  /* 4: game/Script/ZoneCmd/zonecmd */
    script_taskQuery,  /* 5: game/Script/TaskCmd/taskcmd */
    script_streamQuery,  /* 6: game/Script/MusicCmd/musiccmd */
    script_soundQuery,  /* 7: game/Script/ObjectCmd/objectcmd */
    script_swarmQuery,  /* 8: game/Script/SwarmCmd/swarmcmd */
    script_cameraQuery,  /* 9: game/Script/CameraCmd/cameracmd */
    script_musicQuery,  /* 10: game/Script/MusicCmd/musiccmd */
    script_timerQuery,  /* 11: game/Script/TimerCmd/timercmd */
    script_emptyQuery12,  /* 12: game/Script/ObjectCmd/objectcmd */
    NULL,           /* 13 */
    script_effectQuery,  /* 14: game/Script/EffectCmd/effectcmd */
    script_miscQuery,  /* 15: game/Script/MiscCmd/misccmd */
};

/* Command handlers, by class: (id, opcode, record). */
s32 (*gScriptCommandHandlers[15])() = {
    script_cutsceneCommand,
    script_actorCommand,
    script_worldObjectCommand,
    script_zoneCommand,
    script_taskCommand,
    script_streamCommand,
    script_actorSoundCommand,
    script_swarmCommand,
    script_cameraCommand,
    script_musicCommand,
    script_timerCommand,
    script_trapCommand12,
    NULL,
    script_effectCommand,
    script_miscCommand,
};
