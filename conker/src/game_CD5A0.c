#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* 12-byte slot record. The first word is a bitfield: golden reads it whole (`lw` + `srl 31`)
   for the guard and updates the top byte in place (`lbu`/`sb`) for each field write, which is
   exactly what these bitfield assignments produce. */
typedef struct Rec150A0264 {
    u32 active : 1;  /* claimed by the start handler */
    u32 running : 1; /* counting (command 0 sets it, command 1 clears it) */
    u32 unit : 4;    /* index into D_80088510 / D_8008851C */
    u32 unk0_b6 : 2;
    u32 unk0_rest : 24;
    u32 count; /* units counted, wrapped by D_8008851C[unit] */
    u32 ticks; /* ticks towards the next count */
} Rec150A0264;

struct Src150A0264 {
    char pad0[4];
    s32 unit;  /* the timer unit (Rec150A0264.unit) of the start command */
    s32 value; /* the count that command 2 sets */
};

static Rec150A0264 D_800D3010[10];

/* Defined here (not extern): as1 then knows the array's alignment and shares one `lui at`
   between stores in the same 8-byte window of the peeled records. */
void func_150A00F0(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        D_800D3010[i].active = 0;
        D_800D3010[i].count = 0;
        D_800D3010[i].ticks = 0;
    }
}

/* Per unit (indexed by Rec150A0264.unit): the ticks that make one count, and the count's wrap
   (0: none). The unit-4 period 0x5180 is 86400 cut to 16 bits. */
static u16 D_80088510[] = { 2, 6, 60, 3600, 0x5180, 0 };
static u16 D_8008851C[] = { 0, 10, 60, 60, 24, 0 };

void func_150A019C(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if ((D_800D3010[i].active != 0) && (D_800D3010[i].running == 1)) {
            D_800D3010[i].ticks = D_800D3010[i].ticks + D_800BE9E4;
            if (D_800D3010[i].ticks >= D_80088510[D_800D3010[i].unit]) {
                D_800D3010[i].ticks = D_800D3010[i].ticks - D_80088510[D_800D3010[i].unit];
                D_800D3010[i].count = D_800D3010[i].count + 1;
                if (D_8008851C[D_800D3010[i].unit] != 0) {
                    D_800D3010[i].count = D_800D3010[i].count % D_8008851C[D_800D3010[i].unit];
                }
            }
        }
    }
}


s32 func_150A0264(s32 arg0, Src150A0264 *arg1) {
    Rec150A0264 *r = &D_800D3010[arg0];

    if (r->active != 0) {
        return 0;
    }
    r->active = 1;
    r->running = 0;
    r->unit = arg1->unit;
    r->count = 0;
    return 1;
}

s32 func_150A02D0(s32 arg0, s32 arg1, Src150A0264 *arg2) {
    switch (arg1) {
    case 0:
        D_800D3010[arg0].running = 1;
        return 1;
    case 1:
        D_800D3010[arg0].running = 0;
        return 1;
    case 2:
        D_800D3010[arg0].count = arg2->value;
        return 1;
    }
    return 0;
}

s32 func_150A0374(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 == 3) {
        return D_800D3010[arg0].count;
    }
    return 0;
}
