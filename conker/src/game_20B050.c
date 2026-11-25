#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern u8 D_800D2E40;
extern u8 D_8008FDA4;
void func_1501C730(s32, s32, s32, s32, s32);
void func_151E557C(void);
void func_1000E934(void);
void func_1000F1A8(void);

/* Requests a switch to map 0x1D (func_1501C730 mode 6) and resets this mode's state flags. */
void func_151DDBA0(void) {
    D_800D2E40 = 0;
    func_1501C730(6, 0x1D, 0, 0, 1);
    D_800E0B94 = 3;
    D_8008FDA4 = 0;
    D_800BEAC1 = 0;
    func_151E557C();
    func_1000F1A8();
    func_1000E934();
    D_8008FD8C = 1;
    D_8008FD90 = 1;
}
