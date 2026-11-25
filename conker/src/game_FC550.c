#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern WorldObject *func_151149AC(u32);
extern void func_15117798(WorldObject *obj);

struct DObj800CC5EC {
    u8 pad0[0x57];
    u8 unk57;
};

extern struct DObj800CC5EC *D_800CC5EC;

void func_150CF0A0(WorldObject *arg0) {
    WorldObject *temp;

    if ((arg0->task_flag & 0x3) != 2) {
        if (arg0->object_status & 0x4) {
            if (D_800CC5EC->unk57 != 0) {
                temp = func_151149AC(0xFE);
                temp->task_flag &= 0xFFFC;
                temp->task_flag |= 0x2;
                temp = func_151149AC(0xFD);
                temp->task_flag &= 0xFFFC;
                temp->task_flag |= 0x2;
            }
        }
    } else {
        func_15117798(arg0);
    }
}
