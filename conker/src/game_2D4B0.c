#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15000000(void) {
    OSMesgQueue *msgQueue;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        msgQueue = &D_80041E58[i];
        osCreateMesgQueue(msgQueue, D_80041EA0[i], 8);
        func_10008570(i, msgQueue);
    }

    func_100085A4(0, 6, 1); // stub function
    func_1000E934();
}

void func_15000090(void) {
    func_1000DEC4();
}

void func_150000B0(void) {
    LevelMusic q;
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
        q = D_800B0DF0->music;
    } else {
        q.mode = 1;
        q.unk2 = 0;
        q.unk8 = 0;
        q.unkD = 2;
        q.song = 0;
        q.song2 = 0;
    }
    D_80041F04 = 0;
    D_80041F08 = 0;
    D_80041F0C = 0;
    if (q.song == 1 && (D_800D2E4C[0x4] & 0x80)) {
        q.song = 0x46;
        q.unk4 = 0x10;
    }
    switch (q.mode) {
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
            q.song = -1;
            q.song2 = -1;
        }
        break;
    }
    i = func_1000B548(arr);
    n = i;
    for (; i < 3; i++) {
        arr[i] = -1;
    }
    if (q.mode == 4 && q.song2 >= 0) {
        for (i = 0; i < 3; i++) {
            if (q.song2 == arr[i]) {
                if (q.song >= 0) {
                    for (j = 0; j < 3; j++) {
                        if (q.song == arr[j]) {
                            q.song2 = 0;
                        }
                    }
                }
                if (q.song2 >= 0) {
                    q.song = q.song2;
                    q.unk4 = q.unkA;
                    q.unk2 = q.unk8;
                }
            }
        }
        if (q.song2 >= 0) {
            q.song2 = 0;
        }
    }
    func_1000E2F4(0);
    if (a2 == 0) {
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                if (q.song == arr[i]) {
                    func_1000E46C(q.song, 0, q.unk4, 0);
                    func_1000E46C(q.song, 0x64, (q.unk4 ^ 0xFFFF), 0);
                    func_1000E40C(q.song, q.unk2);
                    func_1000DF68(q.song, 0x7FFF, 0);
                    arr[i] = -1;
                    q.song = -1;
                    n--;
                } else if (q.song2 == arr[i]) {
                    func_1000E46C(q.song2, 0, q.unkA, 0);
                    func_1000E46C(q.song2, 0x64, (q.unkA ^ 0xFFFF), 0);
                    func_1000E40C(q.song2, q.unk8);
                    func_1000DF68(q.song2, 0x7FFF, 0);
                    arr[i] = -1;
                    q.song2 = -1;
                    n--;
                }
            }
        }
    }
    if (q.song >= 0) {
        cnt = 1;
    }
    if (q.song2 >= 0) {
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
    if (q.song >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.song > 0) {
                func_1000D96C(q.song, 0, 0);
            }
        } else {
            if (q.song > 0) {
                func_1000D96C(q.song, arr[j], 5);
            } else {
                func_1000DE1C(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.song > 0) {
            func_1000E46C(q.song, 0, q.unk4, 0);
            func_1000E40C(q.song, q.unk2);
        }
    }
    if (q.song2 >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.song2 > 0) {
                func_1000D96C(q.song2, 0, 0);
            }
        } else {
            if (q.song2 > 0) {
                func_1000D96C(q.song2, arr[j], 5);
            } else {
                func_1000DE1C(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.song2 > 0) {
            func_1000E46C(q.song2, 0, q.unkA, 0);
            func_1000E40C(q.song2, q.unk8);
        }
    }
    func_10017944(1, q.unkD);
}

void func_150006E0() {
    s32 i;
    s32 j;

    D_800D9B20 = D_800B0DF0->unk2;
    D_800D9B1C = D_800B0DF0->unk0;
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
            D_800D9BD0[j][i].l.col[0] = D_800D9BD0[j][i].l.colc[0] = 0xFF;
            D_800D9BD0[j][i].l.col[1] = D_800D9BD0[j][i].l.colc[1] = 0xFF;
            D_800D9BD0[j][i].l.col[2] = D_800D9BD0[j][i].l.colc[2] = 0xFF;
        }
    }

    for (i = 0; i <= D_80082FA0; i++) {
        D_800D9AC0[i].x = -1.0f;
    }

    D_800D9AF0 = 0;
    for (i = 0; i <= D_80082FA0; i++) {
        gViewClips[i].near = -1.0f;
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
//         gViewClips[i].near = -1.0f;
//     }
//     D_800D9B18 = 0;
// }
