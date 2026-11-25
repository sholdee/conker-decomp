#include <ultra64.h>
#include "functions.h"
#include "variables.h"


/* Billboard expiry hook D_8008D5D0[1]: gives the particle 300 more frames, a fade of 10 and half its size
   step, packed into both halves of tmpl.unk4 (width and height steps). */
void func_151927C0(BillboardParticle *p) {
    s16 step;
    step = (p->tmpl.unk4 & 0xFF) >> 1;
    p->tmpl.life = 300;
    p->tmpl.fadeStep = 10;
    p->tmpl.unk4 = (step << 16) + step;
    p->tmpl.unk2B = 0;
}
