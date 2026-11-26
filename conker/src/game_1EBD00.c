#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Extension block copied into the spawned effect at 0x120. */
typedef struct {
    /* 0x00 */ f32 angle;
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 count;
} Extra151BE850;


/*
 * Spawns an effect (variant 0-2) at `pos`, facing `angle` degrees. In one mode (D_800D2E4C[0x19] bit 2,
 * or variant 2) it is a model effect 39 units ahead of and 40 below `pos`; otherwise func_151BEEE0 makes it.
 * The callers also pass the usual effect owner and list, which it does not use.
 */
void *func_151BE850(Vec3f *pos, f32 angle, u8 count, u8 arg3, u8 variant, u8 owner, s32 list) {
    void *fx;
    ZoneNode *zone;
    EffectParams params;
    Extra151BE850 extra;

    func_1510F800(0);
    zone = func_1510FD20(pos->x, pos->z);
    if (variant >= 3) {
        return NULL;
    }
    if ((D_800D2E4C[0x19] & 4) || (variant == 2)) {
        u8 variantIds[3] = { 3, 1, 2 };
        Vec3f offset;
        s32 flag;

        extra.angle = angle;
        params.owner = NULL;
        params.ownerId = 0;
        params.unk41 = 0;
        params.pos = *pos;
        extra.count = count;
        {
            s32 pad; // matching: stack slot

            func_151436B4(angle * 0.01745329238f, 0.0f, 39.0f, &offset);
            extra.pos.x = pos->x + offset.x;
            extra.pos.y = (pos->y + offset.y) - 40.0f;
            extra.pos.z = pos->z + offset.z;
        }
        params.unk38 = variantIds[variant];
        // matching: the (u8) narrowing of a separate local steers IDO's temp-register order
        flag = arg3 ? 0x10 : 0;
        params.flags = (u8)flag | 0xE;
        params.updateFunc = 1;
        if (level == 7) {
            params.updateFunc2 = 6;
        } else {
            params.updateFunc2 = 2;
        }
        params.preDrawFunc = 0;
        params.postDrawFunc = -1;
        params.lifetime = 300;
        params.model = 0xA2;
        params.unkC = 8;
        params.render.unk0 = 0;
        params.render.setGeometryMode = 0x620405;
        params.render.clearGeometryMode = 0x40200;
        params.render.renderMode = 20;
        params.render.unk10 = 0x37;
        params.render.unk14 = 0x80;
        params.render.unk18 = 0x20;
        params.render.primMode = 0;
        params.render.envMode = 8;
        params.color0.red = 0xFF;
        params.color0.green = 0xFF;
        params.color0.blue = 0xFF;
        params.color0.alpha = 0xFF;
        params.color1.red = 0xFF;
        params.color1.green = 0xFF;
        params.color1.blue = 0xFF;
        params.color1.alpha = 0xFF;
        params.mask = zone;
        fx = func_15157010(&params, 0, 1.0f, 3, 0xFF, 0x18, 0xFF, 1);
        if (fx != NULL) {
            // copies the whole 0x18-byte extension (extraSize above); a 0x18-byte Extra151BE850 grows the frame
            memcpy((u8 *)fx + 0x120, &extra, 0x18);
        }
    } else {
        fx = func_151BEEE0(angle, pos, 3, 0xFF, variant, arg3, zone, 0xFF, 1);
    }
    return fx;
}
