#include <ultra64.h>
#include "conker/sprite.h"
#include "conker/texture.h"

/* Shows a sprite that fades in over `fadeIn` frames, holds for `hold` frames and
 * fades out over `fadeOut` frames. */
void sprite_showFadeSprite(s32 fadeIn, s32 hold, s32 fadeOut) {
    Sprite2DParams desc;
    s32 total;

    desc.texture = &D_800917EC;
    desc.behavior = 10;
    desc.width = 64;
    desc.height = 32;
    desc.scaleX = 0x18FC;
    desc.scaleY = 0x1A12;
    desc.renderMode = 7;
    total = fadeIn + hold + fadeOut;
    desc.flags = 0;
    desc.data.fade.fadeInHold = (fadeIn << 16) | hold;
    desc.data.fade.fadeOutTotal = (fadeOut << 16) | total;
    desc.data.fade.unk10 = 10000;
    desc.timer = total;
    desc.frameSpeed = 0;
    desc.sOffset = 0;
    desc.extraWidth = 0;
    desc.frameCount = 1;
    desc.red = 0xFF;
    desc.green = 0xFF;
    desc.blue = 0xFF;
    desc.combiner = 0x11;
    desc.alpha = 0;
    desc.frame = 0;
    desc.x = 228.0f;
    desc.y = 252.59259033203125f;
    sprite_newSprite2DDefault(&desc);
}
