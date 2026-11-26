#include <ultra64.h>
#include "functions.h"
#include "variables.h"

static f32 D_800DBEC0; // turret's squared distance to the player

/* Turret settings in the data (no code reads this name; a turret object's moveData can point here). */
Turret15112520Cfg D_80089200 = {
    0.1f, 0.05f, 15.0f, 90.0f, -90.0f, 30.0f, 40.0f, -20.0f, 20.0f, 145.0f, -10.0f, 1000.0f, 27, 0,
};

/* Turns a turret toward the player (within its yaw/pitch limits and range) and plays its motor sound. */
void func_15112520(WorldObject *turret) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 yaw;
    f32 pitch;
    f32 delta;
    s32 volume;
    f32 speed;
    Turret15112520Cfg *cfg;
    s32 pad; // matching: unused stack slot
    static s16 D_80089234 = 0; // sound timer: 0 idle, -1 start the sound, >0 cooldown frames

    cfg = turret->moveData.data;
    dx = movement_obj[0].xpos - turret->x;
    dy = (movement_obj[0].ypos + 65.0f) - turret->y;
    dz = movement_obj[0].zpos - turret->z;
    D_800DBEC0 = dx * dx + dy * dy + dz * dz;
    if (cfg->range * cfg->range < D_800DBEC0) {
        pitch = cfg->basePitch;
        yaw = cfg->baseYaw;
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
        speed = cfg->speedFar;
    } else {
        yaw = func_150484A0(dx, dz);
        pitch = func_150484A0(dy, sqrtf(dx * dx + dz * dz));
        yaw *= 57.29577637f;
        pitch *= 57.29577637f;
        if (pitch < 0.0f) {
            pitch += 360.0f;
        } else if (pitch >= 360.0f) {
            pitch -= 360.0f;
        }
        if (yaw < 0.0f) {
            yaw += 360.0f;
        } else if (yaw >= 360.0f) {
            yaw -= 360.0f;
        }
        delta = func_15048A70(cfg->baseYaw, yaw);
        if (pitch > 180.0f) {
            pitch -= 360.0f;
        }
        if (cfg->yawMax + cfg->yawMargin < delta || delta < cfg->yawMin - cfg->yawMargin) {
            yaw = cfg->baseYaw;
            pitch = cfg->basePitch;
            speed = cfg->speedFar;
        } else {
            speed = cfg->speedNear;
            delta = (delta < cfg->yawMin) ? cfg->yawMin : ((cfg->yawMax < delta) ? cfg->yawMax : delta);
            yaw = cfg->baseYaw + delta;
            if (yaw < 0.0f) {
                yaw += 360.0f;
            } else if (yaw >= 360.0f) {
                yaw -= 360.0f;
            }
            if (cfg->pitchMax + cfg->pitchMargin < pitch || pitch < cfg->pitchMin - cfg->pitchMargin) {
                pitch = 0.0f;
            } else {
                pitch = (pitch < cfg->pitchMin) ? cfg->pitchMin : ((cfg->pitchMax < pitch) ? cfg->pitchMax : pitch);
            }
        }
        if (pitch < 0.0f) {
            pitch += 360.0f;
        }
    }
    delta = func_15048A70(turret->u, yaw);
    yaw = fabsf(delta);
    if (cfg->maxStep < yaw) {
        delta = ((delta < 0.0f) ? -1.0f : 1.0f) * cfg->maxStep;
    }
    turret->u += delta * speed;
    if (D_80089234 == 0 && yaw > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->u < 0.0f) {
        turret->u += 360.0f;
    } else if (turret->u >= 360.0f) {
        turret->u -= 360.0f;
    }
    delta = func_15048A70(turret->t, 360.0f - pitch);
    turret->t += delta * speed;
    if (D_80089234 == 0 && fabsf(delta) > 4.0f) {
        D_80089234 = -1;
    }
    if (turret->t < 0.0f) {
        turret->t += 360.0f;
    } else if (turret->t >= 360.0f) {
        turret->t -= 360.0f;
    }
    if (D_80089234 == -1 && cfg->sfxCooldown != 0) {
        volume = 32000.0f - D_800DBEC0 * 0.1000000015f;
        D_80089234 = cfg->sfxCooldown;
        if (volume > 0) {
            func_10010F30(cfg->sfx, 32000, 64, 0, 0);
        }
    } else if (D_80089234 > 0) {
        D_80089234--;
    }
}
