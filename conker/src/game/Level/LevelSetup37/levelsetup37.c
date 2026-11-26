#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15011170(void) {
    gZoneFlag = 0;
    particle_startCameraSpiralEmitter(camera_global_Cameras, 0, 0, 0xFF, 1);
    sound_addDynamicSound(108, -3580,  -546, -6129, 18000, 1111, 1100, sound_chirpCallback, 0, 0, 8, 0);
    sound_addDynamicSound(108, -2929, -1121, -2158, 18000, 1590, 1580, sound_chirpCallback, 0, 0, 8, 0);
    sound_addDynamicSound(108,   497, -1071, -2105, 18000, 2040, 2030, sound_chirpCallback, 0, 0, 8, 0);
    sound_addDynamicSound(1567,   26,     0,     0, 12000, 2500, 1800, func_1000EF40, 0, 0, 72, 0);
    sound_addDynamicSound(1567,   27,     0,     0,  8000, 2000, 1000, func_1000EF40, 0, 0, 72, 0);
}
