#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void func_15000AC0(void) {
    D_800D9E64 = 0;
}

/**
 * Sets up the current map's background according to D_800B0DF0->unk8:
 * mode 4 allocates two vertex buffers per view (D_80082FA0 + 1 views), mode 2 builds a
 * 256-entry, four-band colour gradient (chosen by map) plus a quad per view and buffer.
 */
void func_15000AD0(s32 arg0) {
    s32 i;
    s32 j;
    s32 end; // one past the last gradient entry of the current band
    s32 start;
    s32 size;
    s32 from[3];
    s32 to[3];
    s32 cur[3];
    s32 color;
    s32 *buf;
    Vtx *vtx;

    D_800DBEA8[0] = D_800B0DF0->unk5;
    D_800DBEA8[1] = D_800B0DF0->unk6;
    D_800DBEA8[2] = D_800B0DF0->unk7;
    if (D_800BE616 != 0 && D_800B0DF0->unk8 == 4) {
        D_800B0DF0->unk8 = 2;
    }
    if (D_800B0DF0->unk8 == 4) {
        size = (D_80082FA0 != 1) ? 0x130 : 0x1C0;
        for (i = 0; i <= D_80082FA0; i++) {
            vtx = (Vtx *)allocate_memory(size * 32, 1, 2, 0); // two buffers of `size` Vtx
            D_800DBE88[i][0] = vtx;
            D_800DBE88[i][1] = D_800DBE88[i][0] + size;
        }
        func_1502B7F0(&D_800DBE80, 2, 0xD, D_800B0DF0->unk9);
        if (D_80038080 != 0) {
            for (i = 0; i < 360; i++) {
                func_1510D0EC(D_800891BC[D_800B0DF0->unk9][0] + i, 0, 0x3F, 0);
            }
        }
        D_800DBEB0 = allocate_memory((D_80082FA0 + 1) * 0x80, 1, 2, 0);
        if (level == 0x3C) {
            D_800DBEAB = 1;
        } else {
            D_800DBEAB = 0;
        }
    } else if (D_800B0DF0->unk8 == 2) {
        D_800DBE80 = allocate_memory(0x1000, 1, 1, 0);
        buf = D_800DBE80;
        switch (level) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0xBF;
                end = 0xA7;
                to[0] = 0xD0;
                to[1] = 0xD5;
                to[2] = 0xFF;
                break;
            case 43:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                end = 0x85;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x5D;
                break;
            case 36:
                from[0] = 0;
                from[1] = 0;
                from[2] = 0;
                end = 0x7B;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0x1F;
                break;
            default:
                from[0] = 0;
                from[1] = 0x67;
                from[2] = 0x38;
                end = 0x23;
                to[0] = 0xC8;
                to[1] = 0xD5;
                to[2] = 0x2E;
                break;
        }
        for (i = 0; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * i / end + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (level) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB1;
                to[0] = 0x12;
                to[1] = 0x4E;
                to[2] = 0x19;
                break;
            case 43:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0x9C;
                to[0] = 0x1C;
                to[1] = 0x15;
                to[2] = 0x93;
                break;
            case 36:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xA2;
                to[0] = 0x3;
                to[1] = 0x1C;
                to[2] = 0x36;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0x63;
                to[0] = 0xFF;
                to[1] = 0x3E;
                to[2] = 0x12;
                break;
        }
        for (i = start; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (end - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        switch (level) {
            case 0:
            case 41:
            case 45:
            case 48:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xDD;
                to[0] = 0;
                to[1] = 0xC7;
                to[2] = 0;
                break;
            case 43:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB4;
                to[0] = 0xA;
                to[1] = 0x22;
                to[2] = 0xFF;
                break;
            case 36:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xB7;
                to[0] = 0x21;
                to[1] = 0x24;
                to[2] = 0x31;
                break;
            default:
                from[0] = to[0];
                from[1] = to[1];
                from[2] = to[2];
                start = end;
                end = 0xC8;
                to[0] = 0;
                to[1] = 0;
                to[2] = 0;
                break;
        }
        for (i = start; i < end; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = (to[j] - from[j]) * (i - start) / (end - start) + from[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        for (i = end; i < 0x100; i++) {
            for (j = 0; j < 3; j++) {
                cur[j] = to[j];
            }
            color = (cur[0] << 24) | (cur[1] << 16) | (cur[2] << 8);
            buf[i * 4 + 3] = color;
            buf[i * 4 + 2] = color;
            buf[i * 4 + 1] = color;
            buf[i * 4 + 0] = color;
        }
        D_800DBEB0 = allocate_memory((D_80082FA0 + 1) * 0x80, 1, 1, 0);
        for (i = 0; i <= D_80082FA0; i++) {
            vtx = (Vtx *)allocate_memory(2 * 4 * sizeof(Vtx), 1, 1, 0);
            D_800DBE88[i][0] = vtx;
            D_800DBE88[i][1] = vtx + 4;
            for (j = 0; j < 2; j++) {
                D_800DBE88[i][j][0].v.ob[0] = -1750;
                D_800DBE88[i][j][0].v.ob[1] = 1750;
                D_800DBE88[i][j][0].v.ob[2] = -1000;
                D_800DBE88[i][j][0].v.flag = 0;
                D_800DBE88[i][j][0].v.tc[0] = 0x2000;
                D_800DBE88[i][j][1].v.ob[0] = 1750;
                D_800DBE88[i][j][1].v.ob[1] = 1750;
                D_800DBE88[i][j][1].v.ob[2] = -1000;
                D_800DBE88[i][j][1].v.flag = 0;
                D_800DBE88[i][j][1].v.tc[0] = 0x2000;
                D_800DBE88[i][j][2].v.ob[0] = 1750;
                D_800DBE88[i][j][2].v.ob[1] = -1750;
                D_800DBE88[i][j][2].v.ob[2] = -1000;
                D_800DBE88[i][j][2].v.flag = 0;
                D_800DBE88[i][j][2].v.tc[0] = 0x2000;
                D_800DBE88[i][j][3].v.ob[0] = -1750;
                D_800DBE88[i][j][3].v.ob[1] = -1750;
                D_800DBE88[i][j][3].v.ob[2] = -1000;
                D_800DBE88[i][j][3].v.flag = 0;
                D_800DBE88[i][j][3].v.tc[0] = 0x2000;
            }
        }
    } else if (D_800B0DF0->unk8 == 1) {
        func_15111858();
    }
    if (D_800D2E4C[0x14] & 1) {
        D_800DBEAC = 1;
    } else {
        D_800DBEAC = 0;
    }
}
