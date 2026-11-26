#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static const Vec3f D_80096550 = { -21.0f, 1631.0f, -1535.0f };

void func_150121C0(LightEffect *effect) {
    ModelParticle *ret;
    s32 i;
    struct {
        LightEffect *saved; // matching: kept below the spawn template on the stack
        ParticleSpawn78 payload;
    } stack;

    stack.payload.radius = 1.0f;
    stack.payload.elasticity = 1.0f;
    stack.payload.sizeXZ = 1.0f;
    stack.payload.sizeY = 1.0f;
    stack.payload.rot.x = 0.0f;
    stack.payload.rot.y = 0.0f;
    stack.payload.rot.z = 0.0f;
    stack.payload.scale.x = 1.0f;
    stack.payload.scale.y = 1.0f;
    stack.payload.scale.z = 1.0f;
    stack.payload.pos.x = 0.0f;
    stack.payload.pos.y = 0.0f;
    stack.payload.pos.z = 0.0f;
    stack.payload.vel.x = 0.0f;
    stack.payload.vel.y = 0.0f;
    stack.payload.vel.z = 0.0f;
    stack.payload.rotVel.x = 0.0f;
    stack.payload.rotVel.y = 0.0f;
    stack.payload.rotVel.z = 0.0f;
    stack.payload.gravity = 0.0f;
    stack.payload.flags = 0xC0500;
    stack.payload.lifetime = 0x12C;
    stack.payload.model = 0x21;
    stack.saved = effect;
    stack.payload.variant = 0;

    collision_selectMap(0);
    stack.payload.unk5C = collision_findZoneLeaf(D_80096550.x, D_80096550.z);
    stack.payload.alpha = 0xFF;
    stack.payload.updateFn = 0;
    stack.payload.update2Fn = 6;
    stack.payload.floorHitFn = 0;
    stack.payload.floorType3Fn = 0;
    stack.payload.wallHitFn = 0;
    stack.payload.destroyFn = 0;
    stack.payload.squashFn = 0;
    stack.payload.matrixFn = 3;
    stack.payload.unk6A = 0;
    stack.payload.owner = NULL;
    stack.payload.ownerId = 0;
    stack.payload.fadeStart = 1;
    stack.payload.fadeRate = 0xFF;

    ret = func_15132A4C(&stack.payload, 0, 0, 4, 0xFF, 1);
    if (ret != NULL) {
        memcpy(ret + 1, &stack.saved, sizeof(stack.saved));
        for (i = 0; i < 2; i = (i + 1) & 0xFF) {
            memcpy(&ret->mtx[i], &D_80089470, sizeof(Mtx));
        }
    }
}
