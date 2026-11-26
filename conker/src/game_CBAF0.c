#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Script command handler (D_800884D4[5]): opcode 0 starts `id` via func_1001263C at volume a[0]
 * (0x7FFF when omitted), 1 passes a[0] (and a[1] when given) to func_151F2D6C, 2 calls func_151F2BA8.
 * Returns 1 when the opcode is handled. */
s32 func_1509E640(s32 id, s32 opcode, ScriptCmd *cmd) {
    switch (opcode) {
    case 0: {
        s32 volume = 0x7FFF;

        if (D_800D3840 >= 3) {
            volume = cmd->a[0].w;
        }
        func_1001263C(id, volume, 64);
        return 1;
    }
    case 1:
        if (D_800D3840 == 4) {
            func_151F2D6C(cmd->a[0].w, cmd->a[1].w);
        } else {
            func_151F2D6C(cmd->a[0].w, 0);
        }
        return 1;
    case 2:
        func_151F2BA8();
        return 1;
    }
    return 0;
}

/* Script query handler (D_80088498[5]): opcode 3 returns whether func_151F2CDC() == 1, else 0. */
s32 func_1509E6F0(s32 id, s32 opcode, ScriptCmd *cmd) {
    if (opcode == 3) {
        return func_151F2CDC() == 1;
    }
    return 0;
}

/* Script command handler (D_800884D4[9]): forwards the low 12 bits of `id` and the parameters to
 * the func_1000Dxxx/func_1000Exxx routine selected by `opcode`. Optional parameters default when
 * the command is short. Returns 0 for an unknown opcode. */
s32 func_1509E730(s32 id, u32 opcode, ScriptCmd *cmd) {
    s32 mode;

    switch (opcode) {
    case 0:
        func_1000D96C(id & 0xFFF, cmd->a[0].w, cmd->a[1].w);
        return 1;
    case 2:
        func_1000DF68(id & 0xFFF, cmd->a[0].w, 0);
        return 1;
    case 5:
        func_1000DF68(id & 0xFFF, cmd->a[0].w, 1);
        return 1;
    case 1:
        func_1000DE1C(id & 0xFFF, cmd->a[0].w);
        return 1;
    case 3:
        if (D_800D3840 == 5) {
            return func_1000E46C(id & 0xFFF, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w);
        }
        return func_1000E46C(id & 0xFFF, cmd->a[0].w, cmd->a[1].w, 60);
    case 4:
        if (D_800D3840 == 5) {
            return func_1000E654(id & 0xFFF, cmd->a[0].w, cmd->a[1].w, cmd->a[2].w & 0xFFF);
        }
        return func_1000E654(id & 0xFFF, cmd->a[0].w, cmd->a[1].w, id & 0xFFF);
    case 6:
        mode = 1;
        if (D_800D3840 == 3) {
            mode = cmd->a[0].w;
        }
        func_1000E054(id & 0xFFF, mode);
        return 1;
    default:
        // matching: a return 0 reached by goto gets its own block; a plain return lets IDO fill the delay slot with it
        goto unknown;
    }
unknown:
    return 0;
}

/* Script query handler (D_80088498[9]): opcode 7 returns func_1000E0F8(id), 8 returns
 * func_1000E8F0(id), anything else 0. */
s32 func_1509E8A0(s32 id, s32 opcode, ScriptCmd *cmd) {
    switch (opcode) {
    case 7:
        return func_1000E0F8(id);
    case 8:
        return func_1000E8F0(id);
    }
    return 0;
}
