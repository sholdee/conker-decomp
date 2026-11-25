#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15000000(void) {
    OSMesgQueue *msgQueue;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        msgQueue = &D_80041E58[i];
        osCreateMesgQueue(msgQueue, &D_80041EA0[i], 8);
        func_10008570(i, msgQueue);
    }

    func_100085A4(0, 6, 1); // stub function
    func_1000E934();
}

void func_15000090(void) {
    func_1000DEC4();
}

// maybe next year...
typedef struct {
    s16 a;
    u16 b;
    u16 c;
    s16 d;
    u16 e;
    u16 f;
    u8 g;
    u8 h;
} Q150000B0;

typedef struct {
    u8 pad[0x2C];
    Q150000B0 q;
} P150000B0;

extern s32 func_1000B548(s32 *arg0);
extern void func_1000E2F4(s32 arg0);
extern void func_1000DE1C(s32 arg0, s32 arg1);
extern void func_1000D96C(s32 arg0, s32 arg1, s32 arg2);

void func_150000B0(void) {
    Q150000B0 q;
    s32 i;
    s32 j;
    s32 arr[3];
    s32 n;
    s32 cnt;
    s32 a2;

    cnt = 0;
    a2 = 0;
    D_80041F00 = 0;
    if (D_800B0DF0) {
        q = ((P150000B0 *)D_800B0DF0)->q;
    } else {
        q.g = 1;
        q.b = 0;
        q.e = 0;
        q.h = 2;
        q.a = 0;
        q.d = 0;
    }
    D_80041F04 = 0;
    D_80041F08 = 0;
    D_80041F0C = 0;
    if (q.a == 1 && (D_800D2E4C->unk4 & 0x80)) {
        q.a = 0x46;
        q.c = 0x10;
    }
    switch (q.g) {
    case 2:
        a2 = 1;
    case 4:
    case 0:
        func_1000DEC4();
        break;
    case 3:
        a2 = 1;
    case 1:
        if (D_800BE9B4 == 2) {
            func_1000DEC4();
        } else if (D_800BE9F8 != 0x22) {
            a2 = 0;
            q.a = -1;
            q.d = -1;
        }
        break;
    }
    i = func_1000B548(arr);
    n = i;
    for (; i < 3; i++) {
        arr[i] = -1;
    }
    if (q.g == 4 && q.d >= 0) {
        for (i = 0; i < 3; i++) {
            if (q.d == arr[i]) {
                if (q.a >= 0) {
                    for (j = 0; j < 3; j++) {
                        if (q.a == arr[j]) {
                            q.d = 0;
                        }
                    }
                }
                if (q.d >= 0) {
                    q.a = q.d;
                    q.c = q.f;
                    q.b = q.e;
                }
            }
        }
        if (q.d >= 0) {
            q.d = 0;
        }
    }
    func_1000E2F4(0);
    if (a2 == 0) {
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                if (q.a == arr[i]) {
                    func_1000E46C(q.a, 0, q.c, 0);
                    func_1000E46C(q.a, 0x64, (q.c ^ 0xFFFF), 0);
                    func_1000E40C(q.a, q.b);
                    func_1000DF68(q.a, 0x7FFF, 0);
                    arr[i] = -1;
                    q.a = -1;
                    n--;
                } else if (q.d == arr[i]) {
                    func_1000E46C(q.d, 0, q.f, 0);
                    func_1000E46C(q.d, 0x64, (q.f ^ 0xFFFF), 0);
                    func_1000E40C(q.d, q.e);
                    func_1000DF68(q.d, 0x7FFF, 0);
                    arr[i] = -1;
                    q.d = -1;
                    n--;
                }
            }
        }
    }
    if (q.a >= 0) {
        cnt = 1;
    }
    if (q.d >= 0) {
        cnt++;
    }
    if (n != 0 && cnt < n) {
        j = n - cnt;
        for (i = 2; i >= 0 && j != 0; i--) {
            if (arr[i] != -1) {
                func_1000DE1C(arr[i], 4);
                arr[i] = -1;
                j--;
            }
        }
    }
    if (q.a >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.a > 0) {
                func_1000D96C(q.a, 0, 0);
            }
        } else {
            if (q.a > 0) {
                func_1000D96C(q.a, arr[j], 5);
            } else {
                func_1000DE1C(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.a > 0) {
            func_1000E46C(q.a, 0, q.c, 0);
            func_1000E40C(q.a, q.b);
        }
    }
    if (q.d >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.d > 0) {
                func_1000D96C(q.d, 0, 0);
            }
        } else {
            if (q.d > 0) {
                func_1000D96C(q.d, arr[j], 5);
            } else {
                func_1000DE1C(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.d > 0) {
            func_1000E46C(q.d, 0, q.f, 0);
            func_1000E40C(q.d, q.e);
        }
    }
    func_10017944(1, q.h);
}

void func_150006E0(void) {
    s32 i;
    s32 j;

    D_800D9B20 = (u32)D_800B0DF0->unk2;
    D_800D9B1C = (u32)D_800B0DF0->unk0;
    D_800D3670 = 100.0f - D_800D9B1C;

    func_15000940();

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++) {
            D_800D9B68[j][i] = 0;
            D_800D9B78[j][i] = 0;
        }
    }

    for (i = 0; i < 3; i++) {
        D_800D9B84[i] = 0;
        D_800D9B88[i] = 0;
    }

    D_800D9B8B = 0xFF;
    D_800D9B8C = 0xFF;
    D_800D9E20 = 0xB;

    for (i = 0; i <= D_80082FA0; i++) {
        D_800D9E10[i] = func_1515D480(D_800D9E20);
    }

    for (j = 0; j <= D_80082FA0; j++) {
        for (i = 0; i < 2; i++) {
            D_800D9BD0[j][i][0] = D_800D9BD0[j][i][4] = 0xFF;
            D_800D9BD0[j][i][1] = D_800D9BD0[j][i][5] = 0xFF;
            D_800D9BD0[j][i][2] = D_800D9BD0[j][i][6] = 0xFF;
        }
    }

    for (i = 0; i <= D_80082FA0; i++) {
        D_800D9AC0[i].x = -1.0f;
    }

    D_800D9AF0 = 0;
    for (i = 0; i <= D_80082FA0; i++) {
        D_800D9AF8[i].unk0 = -1.0f;
    }
    D_800D9B18 = 0;
}
// NON-MATCHING: almost only reg remaining
// void func_150006E0(void) {
//     s32 i;
//     s32 j;
//
//     D_800D9B20 = D_800B0DF0->unk2;
//     D_800D9B1C = D_800B0DF0->unk0;
//     D_800D3670 = 100.0f - D_800D9B1C;
//
//     func_15000940();
//
//     // downhill from here!
//     for (i = 0; i < 4; i++) {
//         for (j = 0; j < 3; j++) {
//             D_800D9B68[i][j] = 0;
//             D_800D9B78[i][j] = 0;
//         }
//     }
//
//     for (i = 0; i < 3; i++) {
//         D_800D9B84[i] = 0;
//         D_800D9B88[i] = 0;
//     }
//
//     D_800D9B8B = 0xFF;
//     D_800D9B8C = 0xFF;
//     D_800D9E20 = 0xBU;
//
//     for (i = 0; i <= D_80082FA0; i++) {
//         D_800D9E10[i] = func_1515D480(D_800D9E20);
//     }
//
//     for (i = 0; i <= D_80082FA0; i++) {
//         for (j = 0; j < 2; j++) {
//             D_800D9BD0[i][j][4] = 0xff;
//             D_800D9BD0[i][j][0] = 0xff;
//             D_800D9BD0[i][j][5] = 0xff;
//             D_800D9BD0[i][j][1] = 0xff;
//             D_800D9BD0[i][j][6] = 0xff;
//             D_800D9BD0[i][j][2] = 0xff;
//         }
//     }
//
//     for (i = 0; i <= D_80082FA0; i++) {
//         D_800D9AC0[i].unk0 = -1.0f;
//     }
//
//     D_800D9AF0 = 0;
//     for (i = 0; i <= D_80082FA0; i++) {
//         D_800D9AF8[i].unk0 = -1.0f;
//     }
//     D_800D9B18 = 0;
// }
