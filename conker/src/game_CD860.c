#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A command/query message: an 8-byte header followed by word parameters. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 params[5];
} Msg_CD860;

/* variables.h declares D_800D3098 as an array, but the code loads it as a pointer to the table. */

void func_150A3444(s32, s16, s16, s16);
void func_151C970C(s32, void *);
void func_150A2864(s32, s32);
void func_150A2940(s32, s32, s32);
void func_150A3398(s32, u16, s32, s32, s32, s32);

/* Applies command `command` with the parameters in `msg` to record `slot`.
 * Returns 1 for commands 0-2 and 0 otherwise. */
s32 func_150A03B0(s32 slot, u32 command, Msg_CD860 *msg) {
    s32 mode;

    switch (command) {
    case 0:
        func_150A2864(slot, 0);
        return 1;
    case 1:
        func_150A2864(slot, 1);
        return 1;
    case 2:
        func_150A2940(slot, msg->params[0], msg->params[1]);
        return 1;
    case 3:
        func_150A3444(slot, msg->params[0], msg->params[1], msg->params[2]);
        break;
    case 4:
        func_150A3398(slot, msg->params[0], msg->params[1], msg->params[2], msg->params[3], msg->params[4]);
        break;
    case 5:
        // matching: `== 0` would compile to sltiu; the inverted test keeps the branchy form
        mode = (msg->params[0] == 999) ? 2 : (msg->params[0] != 0) ? 0 : 1;
        func_151C970C(mode, &D_800D3098[slot]);
        break;
    }
    return 0;
}

extern s32 D_800D3840;
Actor *func_1505EEF4(s32);
s32 func_150A2D84(s32, s32);
s32 func_150A2EE4(s32, s32);
s32 func_150A2FA4(s32, s32);
s32 func_150A2CA4(s32);
void func_150A3330(s32, s32 *, s32 *, s32 *, s32 *);

/* Answers query `query` about record `slot`; `msg` carries the query's parameters.
 * Queries 6 and 13 take an id whose bits 12-15 select its kind: 0x2000 is an object in
 * D_800CC2D0 (looked up by func_1505EEF4), 0x9000 a D_800D3098 record. */
s32 func_150A04D8(s32 slot, s32 query, Msg_CD860 *msg) {
    switch (query) {
    case 6:
        {
            s32 pad; // matching: unused stack slot keeps `result` at the original sp offset
            s32 id;
            s32 result;
            Actor *obj;
            s32 ret = 0;

            id = msg->params[0] & 0xFFFF0FFF;
            switch (msg->params[0] & 0xF000) {
            case 0x2000:
                obj = func_1505EEF4(id);
                if (obj == NULL) {
                    return 0;
                }
                result = func_150A29C8(obj - D_800CC2D0, slot);
            lbl1: // matching: block boundary stops IDO moving the store into the branch delay slot
                break;
            case 0x9000:
                result = func_150A2D84(id, slot);
                break;
            }
            return (result == 0) ? 1 : ret;
        }
    case 8:
        {
            s32 a;
            s32 b;
            s32 c;
            s32 d;

            func_150A3330(slot, &a, &b, &c, &d);
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

            func_150A3330(slot, &a, &b, &c, &d);
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
            TriggerZone *rec;
            s32 hi;
            s32 lo;

            // matching: the no-op cast changes IDO's register numbering
            rec = (TriggerZone *)D_800D3098 + slot; // matching: the (no-op) cast orders the addu
            hi = rec->x;
            lo = rec->z;
            return (hi << 16) | (lo & 0xFFFF);
        }
    case 11:
    case 12:
        {
            s32 i;

            for (i = 0; i < 25; i++) {
                if (D_800CC2D0[i].interaction_state == msg->params[0] && func_150A29C8(i, slot) == 0) {
                    if (query == 11) {
                        return D_800CC2D0[i].unk13F | 0x2000;
                    }
                    return D_800CC2D0[i].unique_id;
                }
            }
            return 0;
        }
    case 7:
        return D_800D3098[slot].disabled;
    case 13:
        {
            s32 pad; // matching: unused stack slot; removing it shifts the frame layout
            s32 id;
            s32 result;
            Actor *obj;

            id = msg->params[0] & 0xFFFF0FFF;
            switch (msg->params[0] & 0xF000) {
            case 0x2000:
                obj = func_1505EEF4(id);
                result = func_150A2EE4(obj - D_800CC2D0, slot);
                break;
            case 0x9000:
                result = func_150A2FA4(id, slot);
                break;
            }
            return result;
        }
    case 14:
        return D_800D3098[slot].y;
    case 15:
        if (func_150A2CA4(slot) == -1) {
            return 0;
        }
        return 1;
    case 16:
        if (func_150A2CA4(slot) == -1) {
            s32 i;

            for (i = 0; i < D_8008FD90; i++) {
                if (func_150A2D84(i, slot) == 0) {
                    if (D_800D3840 == 2) {
                        return 1;
                    }
                    if (msg->params[1] != 0) {
                        if (D_800CC2D0[i].unk31C->unk78 != 3 && func_150A2D84(i, msg->params[0] & 0xFFFF0FFF) == 0) {
                            return 1;
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
            s32 which;

            which = msg->params[0];
            if (which == 0) {
                return D_800D3098[slot].subtype;
            } else if (which == 1) {
                return D_800D3098[slot].unk18;
            } else if (which == 2) {
                return D_800D3098[slot].unk1C;
            } else if (which == 3) {
                return D_800D3098[slot].unk20;
            }
            break;
        }
    case 18:
        return D_800D3098[slot].yaw;
    }
    return 0;
}
