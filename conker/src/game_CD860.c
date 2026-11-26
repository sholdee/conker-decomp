#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* A command/query message: an 8-byte header followed by word parameters. */
struct Msg_CD860 {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 params[5];
};

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
        func_151C970C(mode, &zones[slot]);
        break;
    }
    return 0;
}

/* Answers query `query` about record `slot`; `msg` carries the query's parameters.
 * Queries 6 and 13 take an id whose bits 12-15 select its kind: 0x2000 is an object in
 * movement_obj (looked up by func_1505EEF4), 0x9000 a zones record. */
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
                result = func_150A29C8(obj - movement_obj, slot);
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
            if (D_800C35EA[0] == 0) {
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
            if (D_800C35EA[0] == 1) {
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
            rec = (TriggerZone *)zones + slot; // matching: the (no-op) cast orders the addu
            hi = rec->x;
            lo = rec->z;
            return (hi << 16) | (lo & 0xFFFF);
        }
    case 11:
    case 12:
        {
            s32 i;

            for (i = 0; i < 25; i++) {
                if (movement_obj[i].type == msg->params[0] && func_150A29C8(i, slot) == 0) {
                    if (query == 11) {
                        return movement_obj[i].listpos | 0x2000;
                    }
                    return movement_obj[i].unique_id;
                }
            }
            return 0;
        }
    case 7:
        return zones[slot].inactive;
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
                result = func_150A2EE4(obj - movement_obj, slot);
                break;
            case 0x9000:
                result = func_150A2FA4(id, slot);
                break;
            }
            return result;
        }
    case 14:
        return zones[slot].y;
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
                        if (movement_obj[i].mv->rt_context_b != 3 && func_150A2D84(i, msg->params[0] & 0xFFFF0FFF) == 0) {
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
                return zones[slot].subTypeData;
            } else if (which == 1) {
                return zones[slot].data1;
            } else if (which == 2) {
                return zones[slot].data2;
            } else if (which == 3) {
                return zones[slot].data3;
            }
            break;
        }
    case 18:
        return zones[slot].yaw;
    }
    return 0;
}
