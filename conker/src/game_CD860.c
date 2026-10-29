#include <ultra64.h>
#include "functions.h"


extern s32 D_800D3098;

void func_150A3444(s32, s16, s16, s16);
void func_151C970C(s32, void *);
void func_150A2864(s32, s32);
void func_150A2940(s32, s32, s32);
void func_150A3398(s32, u16, s32, s32, s32, s32);

s32 func_150A03B0(s32 arg0, u32 arg1, void *arg2) {
    s32 temp_v0;
    s32 var_a0;
    s32 var_v0;

    switch (arg1) {
    case 0:
        func_150A2864(arg0, 0);
        return 1;
    case 1:
        func_150A2864(arg0, 1);
        return 1;
    case 2:
        func_150A2940(arg0, *(s32 *)((u8 *)arg2 + 8), *(s32 *)((u8 *)arg2 + 0xC));
        return 1;
    case 3:
        func_150A3444(arg0, *(s16 *)((u8 *)arg2 + 0xA),
                        *(s16 *)((u8 *)arg2 + 0xE),
                        *(s16 *)((u8 *)arg2 + 0x12));
        break;
    case 4:
        func_150A3398(arg0, *(u16 *)((u8 *)arg2 + 0xA),
                        *(s32 *)((u8 *)arg2 + 0xC),
                        *(s32 *)((u8 *)arg2 + 0x10),
                        *(s32 *)((u8 *)arg2 + 0x14),
                        *(s32 *)((u8 *)arg2 + 0x18));
        break;
    case 5:
        temp_v0 = *(s32 *)((u8 *)arg2 + 8);
        if (temp_v0 == 0x3E7) {
            var_a0 = 2;
        } else {
            if (temp_v0 != 0) {
                var_v0 = 0;
            } else {
                var_v0 = 1;
            }
            var_a0 = var_v0;
        }
        func_151C970C(var_a0, (void *)(D_800D3098 + (arg0 * 0x34)));
        break;
    }
    return 0;
}

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 pad6;
    u8 pad8[0x8];
    f32 unk10;
    u8 unk14;
    u8 pad15[2];
    u8 unk17;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x10];
} PlayerStat_CD860; /* 0x34 */

typedef struct {
    u8 pad0[0x78];
    u8 unk78;
} Sub31C_CD860;

typedef struct {
    s32 unk0;
    u8 pad4[0x37];
    u8 unk3B;
    u8 pad3C[0x103];
    u8 unk13F;
    u8 pad140[0x1DC];
    Sub31C_CD860 *unk31C;
    u8 pad320[0xC];
} Obj_CD860; /* 0x32C */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} Msg_CD860;

extern Obj_CD860 D_800CC2D0[];
extern s8 D_8008FD90;
extern u8 D_800C35EA;
extern s32 D_800D3840;
extern Obj_CD860 *func_1505EEF4(s32);
s32 func_150A2D84(s32, s32);
s32 func_150A2EE4(s32, s32);
s32 func_150A2FA4(s32, s32);
s32 func_150A2CA4(s32);
void func_150A3330(s32, s32 *, s32 *, s32 *, s32 *);

s32 func_150A04D8(s32 arg0, s32 arg1, Msg_CD860 *arg2) {
    switch (arg1) {
    case 6:
        {
            s32 v;
            s32 id;
            s32 result;
            Obj_CD860 *obj;
            s32 ret;

            id = arg2->unk8 & 0xFFFF0FFF;
            switch (arg2->unk8 & 0xF000) {
            case 0x2000:
                obj = func_1505EEF4(id);
                if (obj == NULL) {
                    return 0;
                }
                result = func_150A29C8(obj - D_800CC2D0, arg0);
            lbl1:
                break;
            case 0x9000:
                result = func_150A2D84(id, arg0);
                break;
            }
            ret = 0;
            return (result == 0) ? 1 : ret;
        }
    case 8:
        {
            s32 a;
            s32 b;
            s32 c;
            s32 d;

            func_150A3330(arg0, &a, &b, &c, &d);
            if (c == 0) {
                break;
            }
            if (D_800C35EA == 0) {
                return 1;
            }
            break;
        }
    case 9:
        {
            s32 a;
            s32 b;
            s32 c;
            s32 d;

            func_150A3330(arg0, &a, &b, &c, &d);
            if (c == 0) {
                break;
            }
            if (D_800C35EA == 1) {
                return 1;
            }
            break;
        }
    case 10:
        {
            PlayerStat_CD860 *ps;
            s32 hi;
            s32 lo;

            ps = &((PlayerStat_CD860 *)D_800D3098)[arg0];
            hi = ps->unk0;
            lo = ps->unk4;
            return (hi << 16) | (lo & 0xFFFF);
        }
    case 11:
    case 12:
        {
            s32 i;

            for (i = 0; i < 25; i++) {
                if (D_800CC2D0[i].unk0 == (*arg2).unk8) {
                    if (func_150A29C8(i, arg0) == 0) {
                        if (arg1 == 11) {
                            return D_800CC2D0[i].unk13F | 0x2000;
                        }
                        return D_800CC2D0[i].unk3B;
                    }
                }
            }
            return 0;
        }
    case 7:
        return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk14;
    case 13:
        {
            s32 v;
            s32 id;
            s32 result;
            Obj_CD860 *obj;

            id = arg2->unk8 & 0xFFFF0FFF;
            switch (arg2->unk8 & 0xF000) {
            case 0x2000:
                obj = func_1505EEF4(id);
                result = func_150A2EE4(obj - D_800CC2D0, arg0);
                break;
            case 0x9000:
                result = func_150A2FA4(id, arg0);
                break;
            }
            return result;
        }
    case 14:
        return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk2;
    case 15:
        if (func_150A2CA4(arg0) == -1) {
            return 0;
        }
        return 1;
    case 16:
        if (func_150A2CA4(arg0) == -1) {
            s32 i;

            for (i = 0; i < D_8008FD90; i++) {
                if (func_150A2D84(i, arg0) == 0) {
                    if (D_800D3840 == 2) {
                        return 1;
                    }
                    if (arg2->unkC != 0) {
                        if (D_800CC2D0[i].unk31C->unk78 != 3) {
                            if (func_150A2D84(i, arg2->unk8 & 0xFFFF0FFF) == 0) {
                                return 1;
                            }
                        }
                    } else {
                        return 1;
                    }
                }
            }
            return 0;
        }
        return 1;
    case 17:
        {
            s32 v;

            v = arg2->unk8;
            if (v == 0) {
                return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk17;
            } else if (v == 1) {
                return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk18;
            } else if (v == 2) {
                return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk1C;
            } else if (v == 3) {
                return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk20;
            }
            break;
        }
    case 18:
        return ((PlayerStat_CD860 *)D_800D3098)[arg0].unk10;
    }
    return 0;
}
