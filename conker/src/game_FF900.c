#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* Sets the colour passed to func_1515D4D4 from the object's colour wave and advances each channel's phase. */
void func_150D2450(ColourWaveEffect *object) {
    ColourWave *wave;
    f32 pad; // matching: unused stack slot; IDO places sine[] after it
    f32 sine[2]; // red and green; the blue sine is used straight away

    wave = &object->wave;
    sine[0] = sinf(wave->phaseR);
    sine[1] = sinf(wave->phaseG);

    func_1515D4D4(
        (u8)(wave->baseR + (sine[0] * wave->amplitudeR)),
        (u8)(wave->baseG + (sine[1] * wave->amplitudeG)),
        (u8)((sinf(wave->phaseB) * wave->amplitudeB) + wave->baseB),
        0);

    wave->phaseR = func_15144B68(wave->phaseR + (wave->speedR * gFrameDelta));
    wave->phaseG = func_15144B68(wave->phaseG + (wave->speedG * gFrameDelta));
    wave->phaseB = func_15144B68(wave->phaseB + (wave->speedB * gFrameDelta));
}
