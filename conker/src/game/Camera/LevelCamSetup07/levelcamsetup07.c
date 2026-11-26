#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void camera_levelSetup07(Camera *arg0) {
    s32 i;

    script_command(0, 0x4014, 0);
    for (i = 0 ; i < 3; i++)
    {
      script_command(0, i + 0x400C, 0);
    }
}
