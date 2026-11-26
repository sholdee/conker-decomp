#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_800CBFA8[][0x32C];

/* Starts the screen/player effects for player state `state` (some held items force state 27). */
void func_151C9BA0(Camera *player, s32 state) {
    u8 idx;

    func_151C9ED4(player);
    idx = player->CharPtr->attach_char;
    if (idx != 0) {
        if (D_800CBFA8[idx][0] == 0x23 || D_800CBFA8[idx][0] == 0x8A) {
            state = 0x1B;
        }
    }
    switch (state) {
        case 35:
            func_151C9F38(player, 1, 1, -1, 0xFF);
            func_151CA6A0(player, 0);
            break;
        case 31:
        case 41:
            func_151C9F38(player, 2, 1, -1, 0xFF);
            func_151CAD28(player, 0);
            break;
        case 12:
        case 22:
            func_151CAD28(player, 0);
            func_151CCE94(player);
            break;
        case 27:
            if (level == 0x27) {
                func_151C9DE8(player, 1, -1, 0xFF, 1);
            } else {
                if (D_80082FA0 == 0) {
                    func_151CC524(player, 0xFF, 0, 0, 1, 0xFF);
                    func_151CC840(player, 0xFF, 0, 0, 1, 0xFF);
                } else {
                    func_151CCD1C(player, 0xFF, 0, 0);
                }
                func_151C9F38(player, 3, 1, -1, 0xFF);
                func_151CB5FC(player, 1, 1, -1, 0xFF);
            }
            break;
        case 36:
            func_151C9F38(player, 0, 1, -1, 0xFF);
            func_151CA6A0(player, 0);
            break;
        case 18:
        case 24:
        case 56:
        case 59:
            break;
        case 9:
        default:
            func_151CAB78(player, 0);
            break;
    }
}

/* Effects for state 27 when level is 0x27 (see func_151C9BA0). */
void func_151C9DE8(Camera *player, u8 arg1, s8 arg2, u8 arg3, s32 arg4) {
    func_151C9F38(player, 4, arg1, arg2, arg3);
    func_151CC524(player, 0, 50, 0xFF, arg1, arg3);
    func_151CC840(player, 0, 50, 0xFF, arg1, arg3);
    func_151CCF08(player, arg1, arg2, arg3);
    func_150BDE90(player, arg3, arg4);
    func_151CB5FC(player, 2, arg1, arg2, arg3);
    if (level == 0x27) {
        D_8008CD00 = 1;
    }
}

/* Broadcasts message 0x21 (player effects reset) to the effect systems and clears D_8008CD00. */
void func_151C9ED4(Camera *player) {
    Camera *msg;

    msg = player;
    func_15160274((s32)&msg, 0x21, (s32)player);
    func_1515572C((s32)&msg, 0x21);
    func_151A561C((s32)&msg, 0x21);
    func_151494E0((s32)&msg, 0x21);
    D_8008CD00 = 0;
}

extern void (*D_8008FC10[])(SightSprite *, void **, u8);
const SightColor D_800AAC80 = { 255.0f, 0.0f, 0.0f, 255.0f };
const SightColor D_800AAC90[] = {
    { 0.0f, 255.0f, 0.0f, 130.0f },
    { 0.0f, 255.0f, 0.0f, 130.0f },
    { 0.0f, 255.0f, 0.0f, 130.0f },
    { 255.0f, 255.0f, 0.0f, 90.0f },
    { 0.0f, 0.0f, 180.0f, 90.0f },
};
const SightColor D_800AACE0[] = {
    { 0.0f, 255.0f, 0.0f, 65.0f },
    { 0.0f, 255.0f, 0.0f, 65.0f },
    { 0.0f, 255.0f, 0.0f, 65.0f },
    { 255.0f, 0.0f, 0.0f, 65.0f },
    { 0.0f, 0.0f, 180.0f, 65.0f },
};
const f32 D_800AAD30[] = { 2.0f, 2.0f, 2.0f, 1.0f, 1.0f };
const f32 D_800AAD44[] = { 60.0f, 60.0f, 60.0f, 30.0f, 30.0f };
const f32 D_800AAD58[] = { -90.0f, -90.0f, -90.0f, 37.0f, 37.0f };
const f32 D_800AAD6C[][2] = {
    { -50.0f, -62.0f },
    { -38.0f, -46.5f },
    { -38.0f, -46.0f },
    { -25.0f, -31.0f },
    { -25.0f, -31.0f },
};
const f32 D_800AAD94[][2] = {
    { 0.02f, 0.016129032f },
    { 0.02631579f, 0.021505376f },
    { 0.02631579f, 0.02173913f },
    { 0.04f, 0.032258064f },
    { 0.04f, 0.032258064f },
};

/* Spawns the two layers of sight ring `type` (colour/size tables indexed by type) for `player`. */
void func_151C9F38(Camera *player, u8 type, u8 arg2, s8 arg3, u8 arg4) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.flashColor = D_800AAC80;
    payload.color = D_800AAC90[type];
    payload.unk4C = D_800AAD44[type] - 1.0f;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.unk8 = 130.0f;
    payload.player = player;
    payload.flags = (s32)((arg3 == -1) ? 0 : 4); // matching: the s32 cast changes how IDO narrows the result
    payload.sourceIndex = arg3;
    desc.timer = 300;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.viewport = player->CNumber;
    desc.x = D_800AAD58[type];
    desc.y = 0.0f;
    desc.halfWidth = D_800AAD30[type];
    desc.halfHeight = D_800AAD44[type];
    desc.texture = 0xFF;
    desc.flags = (1 << (player->CNumber + 11)) | ((arg2 ? 0x40 : 0) | 0x10);
    desc.combineMode = 8;
    desc.unk34 = 0x54;
    payload.rangeMax = 0.0f;
    payload.lastValue = 99999.0f;
    payload.rangeMin = D_800AAD6C[type][D_80082FA0 == 1];
    payload.rangeScale = D_800AAD94[type][D_80082FA0 == 1];
    payload.kind = 3;

    desc.color1.red = D_800AAC90[type].r;
    desc.color1.green = D_800AAC90[type].g;
    desc.color1.blue = D_800AAC90[type].b;
    desc.color1.alpha = D_800AAC90[type].a;
    sprite = func_1515548C(&desc, 8, 0, 0, sizeof(SightSpritePayload), arg4, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    payload.kind = 0;
    desc.halfWidth = D_800AAD30[type] + 2.0f;
    desc.halfHeight = D_800AAD44[type] + 2.0f;
    desc.color1.red = D_800AACE0[type].r;
    desc.color1.green = D_800AACE0[type].g;
    desc.color1.blue = D_800AACE0[type].b;
    desc.color1.alpha = D_800AACE0[type].a;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Spawns the four corner brackets and a ring of 12 dots of the basic sight for `player`. */
void func_151CA6A0(Camera *player, u8 big) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;
    s32 pad[2]; // matching: unused stack space in the original frame

    payload.kind = 0;
    payload.flags = 0;
    desc.halfHeight = 25.0f;
    desc.halfWidth = 25.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        desc.texture = 0x73;
    } else {
        desc.texture = 0x5D;
    }
    desc.timer = 300;
    desc.flags = (1 << (player->CNumber + 11)) | 0x50;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = 0;
    desc.color1.green = 0xFF;
    desc.color1.blue = 0;
    desc.color1.alpha = 0x82;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.viewport = player->CNumber;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.x = 25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    {
        s32 i;

        desc.x = 0.0f;
        desc.y = 0.0f;
        desc.halfHeight = 3.0f;
        desc.halfWidth = 3.0f;
        desc.texture = 0x99;
        desc.flags = (1 << (player->CNumber + 11)) | 0x50;
        desc.combineMode = 8;
        desc.unk34 = 0x44;
        payload.lastValue = 99999.0f;
        if (D_80082FA0 == 1) {
            payload.rangeMin = -62.0f;
        } else {
            payload.rangeMin = -50.0f;
        }
        payload.rangeMax = 0.0f;
        if (D_80082FA0 == 1) {
            payload.rangeScale = 0.01612903178f;
        } else {
            payload.rangeScale = 0.01999999955f;
        }
        i = 0; // matching: the counter is initialised before the angle
        payload.angle = 0.0f;
        for (; i < 12; i++) {
            sprite = func_1515548C(&desc, 7, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
            if (sprite != NULL) {
                memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
            }
            payload.angle += 0.5235987902f;
        }
    }
}

/* Message handler shared by sight sprites: removes the sprite when its player's effects are
 * reset (0x21) or when message 0 names an actor controlled by that player, then runs the
 * handler for the sprite's kind. */
void func_151CAACC(SightSprite *sprite, void **msg, u8 msgId) {
    void (*handler)(SightSprite *, void **, u8);
    Actor *actor;

    if (msgId == 0x21) {
        if (sprite->payload.player == *(Camera **)msg) {
            func_1516972C(&sprite->header);
        // matching: an empty labelled statement keeps IDO from merging this branch's exit
        label:
            ;
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->CameraPtr == sprite->payload.player) {
            func_1516972C(&sprite->header);
        }
    }
    handler = D_8008FC10[sprite->payload.kind];
    if (handler != NULL) {
        handler(sprite, msg, msgId);
    }
}

/* Spawns the plain dot sight for `player`. */
void func_151CAB78(Camera *player, u8 big) {
    ScreenSpriteInfo desc;
    s32 pad; // matching: unused stack slot
    SightSpritePayload payload;
    SightSprite *sprite;
    s32 kind;
    f32 scale;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    if (D_80082FA0 > 0) {
        scale = 2.0f;
    } else {
        scale = 1.0f;
    }

    desc.texture = 100;
    desc.timer = 300;
    desc.halfHeight = scale * 12.0f;
    desc.halfWidth = desc.halfHeight;
    desc.flags = (1 << (player->CNumber + 11)) | 0x50;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = 0xFF;
    desc.color1.green = 200;
    desc.color1.blue = 0;
    desc.color1.alpha = 130;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.viewport = player->CNumber;

    if (big) {
        kind = 4;
    } else {
        kind = 0;
    }

    sprite = func_1515548C(&desc, kind, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Spawns the corner brackets of the zoom sight and, unless D_800BE616 is set, its centre lens. */
void func_151CAD28(Camera *player, u8 big) {
        ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.kind = 0;
    payload.flags = 0;
    desc.halfHeight = 25.0f;
    desc.halfWidth = 25.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.unk8 = 130.0f;
    if (D_80082FA0 > 0) {
        desc.texture = 0x72;
    } else {
        desc.texture = 0x65;
    }
    desc.timer = 300;
    desc.flags = (1 << (player->CNumber + 11)) | 0x50;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = 0;
    desc.color1.green = 0xFF;
    desc.color1.blue = 0;
    desc.color1.alpha = 130;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.viewport = player->CNumber;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.x = 25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -25.0f;
    desc.y = 25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 25.0f;
    desc.y = -25.0f;
    sprite = func_1515548C(&desc, big ? 4 : 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    if (D_800BE616 == 0) {
        EffectDesc spawn;
        SightLens lens;
        SightLensObject *lensObj;

        lens.player = player;
        lens.view = &D_800BE628[player->CNumber];
        lens.left = lens.view->centerX - 12.0f;
        lens.topOffset = (u32)(s32)(lens.view->centerY - 12.0f) * D_800BE620;
        spawn.flags = 0xE;
        spawn.updateIndex = -1;
        spawn.drawIndex = 1;
        spawn.lifetime = 300;
        spawn.messageIndex = 1;
        lensObj = (SightLensObject *)func_1515FF74(&spawn, 0x498, 0xFF, 1);
        if (lensObj != NULL) {
            lens.pixels = lensObj->pixels;
            memcpy(&lensObj->lens, &lens, sizeof(SightLens));
        }
    }
}

/* Draws the zoom lens: saves the 24x24 framebuffer block under it, then writes it back
 * magnified towards the centre, mirrored into all four quadrants. */
void func_151CB110(SightLensObject *obj, u16 *framebuffer) {
    RenderView *view;
    s32 row;
    s32 lensOffset;
    s32 screenOffset;
    f32 x;
    f32 y;
    f32 radius;
    f32 invRadius;
    f32 strength;
    s32 screenPos[2][2];
    s32 pad; // matching: unused stack slot
    s32 lensPos[2][2];

    if (D_800BEAC0 != 0 || D_800BEAC1 != 0) {
        return;
    }
    if (D_800BEAC2 != 0) {
        return;
    }
    row = 0;
    view = obj->lens.view;
    {
        SightLens *lens = &obj->lens;
        screenOffset = obj->lens.left + obj->lens.topOffset;
        lensOffset = 0;
        do {
            // matching: byte-pointer form gives IDO's operand order for the address add
            memcpy(lens->pixels + lensOffset, (u8 *)framebuffer + screenOffset * 2, 24 * sizeof(u16));
            row++;
            lensOffset += 24;
            screenOffset += D_800BE620;
        } while (row != 24);

        {
            s32 unused; // matching: a block-scope declaration here steers IDO's scheduling
            for (y = -1.0f; y < 0.0f; y += 0.08333333582f) {
                radius = func_15145A0C(fabsf(y), 1.0f, 1.0f);
                if (radius) {
                    screenPos[0][1] = (s32)(view->centerY + y * 12.0f) * D_800BE620;
                    screenPos[1][1] = (s32)(view->centerY - y * 12.0f) * D_800BE620;
                    invRadius = 1.0f / radius;
                    strength = 1.0f - (1.0f - radius * 0.6693121791f);
                    x = -radius;
                    while (x < 0.0f) {
                        f32 scale;
                        scale = func_15145A0C(fabsf(x), radius, invRadius);
                        screenPos[0][0] = (s32)(view->centerX + x * 12.0f);
                        screenPos[1][0] = (s32)(view->centerX - x * 12.0f);
                        scale = 1.0f - scale * strength;
                        lensPos[0][0] = (s32)(x * scale * 12.0f + 12.0f);
                        lensPos[0][1] = (s32)(y * scale * 12.0f + 12.0f) * 24;
                        lensPos[1][0] = (s32)(12.0f - x * scale * 12.0f);
                        lensPos[1][1] = (s32)(12.0f - y * scale * 12.0f) * 24;
                        *(framebuffer + screenPos[0][1] + screenPos[0][0]) = *(lens->pixels + lensPos[0][0] + lensPos[0][1]);
                        *(framebuffer + screenPos[0][1] + screenPos[1][0]) = *(lens->pixels + lensPos[1][0] + lensPos[0][1]);
                        *(framebuffer + screenPos[1][1] + screenPos[1][0]) = *(lens->pixels + lensPos[1][0] + lensPos[1][1]);
                        *(framebuffer + screenPos[1][1] + screenPos[0][0]) = *(lens->pixels + lensPos[0][0] + lensPos[1][1]);
                        x += 0.08333333582f;
                    }
                }
            }
        }
    }
}

/* Message handler of the lens: removed like the sight sprites (see func_151CAACC). */
void func_151CB49C(SightLensObject *obj, void **msg, u8 msgId) {
    Actor *actor;

    if (msgId == 0x21) {
        if (obj->lens.player == *(Camera **)msg) {
            func_1516972C(&obj->header);
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->CameraPtr == obj->lens.player) {
            func_1516972C(&obj->header);
        }
    }
}

/* Fades the sprite towards payload.unk8 while the player's 0x2000 input flag is held
 * (and unk3D4->unk19E is clear), and towards 0 otherwise. */
void func_151CB510(SightSprite *sprite) {
    Camera *player;
    SightSpritePayload *payload;
    f32 target;

    player = sprite->payload.player;
    payload = &sprite->payload;
    if ((player->PadPtr->button & 0x2000) && player->mv->ReloadTimer == 0) {
        target = payload->unk8;
    } else {
        target = 0.0f;
    }
    payload->unk4 += (target - payload->unk4) * 0.400000006f;
    sprite->desc.color1.alpha = payload->unk4;
}

/* Spawns the four pieces (two sides, two layers each) of side panel `kind` for `player`. */
void func_151CB5FC(Camera *player, u8 kind, u8 arg2, s8 arg3, u8 arg4) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.unk4 = 255.0f;
    payload.unk8 = 255.0f;
    payload.kind = kind;
    payload.player = player;
    payload.flags = (s32)((arg3 == -1) ? 0 : 4); // matching: see func_151C9F38
    payload.sourceIndex = arg3;
    payload.fade = 255.0f;
    desc.timer = 300;
    desc.halfWidth = 49.0f;
    payload.bouncePhase = 0.0f;
    desc.halfHeight = 18.0f;
    desc.flags = (1 << (player->CNumber + 11)) | ((arg2 ? 0x40 : 0) | 0x32);
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = 0xFF;
    desc.color1.green = 0xFF;
    desc.color1.blue = 0xFF;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 7;
    desc.unk34 = 0x22;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 7;
    desc.viewport = player->CNumber;
    payload.side = 0;
    desc.color1.alpha = 0xFF;
    desc.texture = 0x7B;
    payload.rangeMax = 0.0f;
    desc.x = 88.0f;
    desc.y = 91.0f;
    payload.rangeMin = D_800AAD6C[3][D_80082FA0 == 1];
    payload.rangeScale = D_800AAD94[3][D_80082FA0 == 1];
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.color1.alpha = 0;
    desc.texture = 0x7C;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.flags &= ~2;
    desc.color1.alpha = 0xFF;
    desc.texture = 0x7B;
    payload.side = 1;
    desc.x = -88.0f;
    desc.y = 91.0f;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
    desc.color1.alpha = 0;
    desc.texture = 0x7C;
    sprite = func_1515548C(&desc, 5, 0, 0, sizeof(SightSpritePayload), arg4, 0);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Message 0x37 for a side panel: restart its bounce when `side` and `player` match. */
void func_151CB918(SightSprite *sprite, SightPanelMsg *msg, u8 msgId) {
    SightSpritePayload *payload = &sprite->payload;

    if (msgId == 0x37) {
        if (payload->side == msg->side) {
            if (payload->player == msg->player) {
                payload->bouncePhase = 0.0f;
                payload->flags |= 1;
                payload->fade = 0.0f;
            }
        }
    }
}

/* Side panel update: height follows the sight value, optional bounce, fade-in. */
s32 func_151CB970(SightSprite *sprite) {
    SightSpritePayload *payload;
    f32 value;
    f32 range;
    f32 offset;
    f32 baseY;
    u8 alpha;

    payload = &sprite->payload;
    value = func_151CC1D4(sprite);
    offset = (1.0f - ((value - payload->rangeMin) * payload->rangeScale)) * 75.0f;

    if (payload->flags & 1) {
        value = sinf(payload->bouncePhase);
        baseY = 91.0f + offset;
        range = (112.0f + offset) - baseY;
        sprite->desc.y = (value * range) + baseY;
        payload->bouncePhase += 0.5049999952f * D_800BE9A4;
        if (3.141592741f <= payload->bouncePhase) {
            sprite->desc.y = baseY;
            payload->flags &= ~1;
        }
    } else {
        sprite->desc.y = 91.0f + offset;
    }

    if (payload->kind == 2) {
        sprite->desc.y -= 20.0f;
    }

    alpha = (u32)payload->fade;
    if (sprite->desc.texture == 0x7C) {
        sprite->desc.color1.alpha = 255 - alpha;
    } else {
        sprite->desc.color1.alpha = alpha;
    }

    payload->fade += (255.0f - payload->fade) * 0.6760000587f;
    return 1;
}

/* Message handler of the effect spawned by func_151CCE94 (owner player at +0x20). */
void func_151CBB6C(SightOwnedEffect *effect, void **msg, u8 msgId) {
    Actor *actor;

    if (msgId == 0x21) {
        if (effect->player == *(Camera **)msg) {
            func_1516972C(&effect->header);
        }
    } else if (msgId == 0) {
        actor = *msg;
        if (actor->CameraPtr == effect->player) {
            func_1516972C(&effect->header);
        }
    }
}

/* Moves the sprite around a circle of radius 55 as the sight value changes. */
s32 func_151CBBE0(SightSprite *sprite) {
    SightSpritePayload *payload;
    f32 scaled;
    f32 value;

    payload = &sprite->payload;
    value = func_151CC1D4(sprite);
    if (value != payload->lastValue) {
        payload->lastValue = value;
        scaled = ((value - payload->rangeMin) * payload->rangeScale) * 6.283185959f;
        func_1514373C(payload->angle + scaled, 55.0f, &sprite->desc.x, &sprite->desc.y);
    }
    return 1;
}

/* Scales the sprite's height with the sight value and fades its flash colour back to normal. */
s32 func_151CBC60(SightSprite *sprite) {
    SightSpritePayload *payload = &sprite->payload;
    f32 value;
    f32 base;

    value = func_151CC1D4(sprite);
    if (value != payload->lastValue) {
        base = payload->unk4C;
        payload->lastValue = value;
        sprite->desc.halfHeight = base - (value - payload->rangeMin) * payload->rangeScale * base + 1.0f;
    }
    if (payload->flags & 2) {
        payload->flashTimer -= D_800BE9A4;
        value = payload->flashTimer;
        if (value <= 0.0f) {
            sprite->desc.color1.red = payload->color.r;
            sprite->desc.color1.green = payload->color.g;
            sprite->desc.color1.blue = payload->color.b;
            sprite->desc.color1.alpha = payload->color.a;
            payload->flags &= ~2;
        } else {
            sprite->desc.color1.red = payload->color.r + (payload->flashColor.r - payload->color.r) * (value * 0.028571429f);
            sprite->desc.color1.green = payload->color.g + (payload->flashColor.g - payload->color.g) * (payload->flashTimer * 0.028571429f);
            sprite->desc.color1.blue = payload->color.b + (payload->flashColor.b - payload->color.b) * (payload->flashTimer * 0.028571429f);
            sprite->desc.color1.alpha = payload->color.a + (payload->flashColor.a - payload->color.a) * (payload->flashTimer * 0.028571429f);
        }
    }
    return 1;
}

/* Current sight value of a sprite, clamped to [rangeMin, rangeMax]: either from the
 * D_800C3958 entry chosen at spawn (flag 4) or the mean of the player's unk19C/unk1A0. */
f32 func_151CC1D4(SightSprite *sprite) {
    SightSpritePayload *payload;
    f32 value;

    payload = &sprite->payload;
    if (payload->flags & 4) {
        f32 t = D_800C3958[0][payload->sourceIndex].unk28 * 0.009999999776f;

        value = payload->rangeMin + (payload->rangeMax - payload->rangeMin) * t;
    } else {
        value = (payload->player->FOVX + payload->player->FOVY) * 0.5f;
    }
    if (value < payload->rangeMin) {
        value = payload->rangeMin;
    } else if (payload->rangeMax < value) {
        value = payload->rangeMax;
    }
    return value;
}

/* Broadcasts message 0x46 (flash the sight sprites of `player`). */
void func_151CC290(s32 player) {
    s32 msg;

    msg = player;
    func_1515572C((s32)&msg, 0x46);
}

/* Message 0x46 handler: starts the 35-frame colour flash. */
void func_151CC2BC(SightSprite *sprite, s32 *msg, u8 msgId) {
    SightSpritePayload *payload;

    payload = &sprite->payload;
    // matching: reading payload->player as *(s32 *)payload keeps IDO's load order
    if ((msgId == 0x46) && (*msg == *(s32 *)payload)) {
        payload->flags |= 2;
        payload->flashTimer = 35.0f;
        sprite->desc.color1.red = (u32)payload->flashColor.r;
        sprite->desc.color1.green = (u32)payload->flashColor.g;
        sprite->desc.color1.blue = (u32)payload->flashColor.b;
        sprite->desc.color1.alpha = (u32)payload->flashColor.a;
    }
}

/* Spawns the two bars of the sight's cross (colour arg1..arg3) for `player`. */
void func_151CC524(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.offsetS = 0.0f;
    desc.offsetT = 0.0f;
    desc.timer = 300;
    desc.flags = (1 << (player->CNumber + 11)) | ((arg4 ? 0x40 : 0) | 0x10 | 0x80 | 0x100 | 0x200);
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = red;
    desc.color1.green = green;
    desc.color1.blue = blue;
    desc.color1.alpha = 0x82;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.viewport = player->CNumber;
    desc.scaleX = 0.200000003f;
    desc.scaleY = 1.0f;
    desc.halfWidth = 28.0f;
    desc.halfHeight = 4.199999809f;
    desc.y = -37.0f;
    desc.texture = 0x9A;
    desc.x = 0.0f;
    sprite = func_1515548C(&desc, 9, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9B;
    desc.y = 0.0f;
    desc.scaleX = 1.0f;
    desc.scaleY = 0.200000003f;
    desc.halfWidth = 4.199999809f;
    desc.halfHeight = 28.0f;
    desc.x = -37.0f;
    sprite = func_1515548C(&desc, 9, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Cross-bar update: shifts the bar sideways (texture 0x9A) or vertically by the aim angles. */
s32 func_151CC77C(SightSprite *sprite) {
    // matching: the angle pointers are taken (in registers) before the NULL check
    f32 *pitchp;
    f32 *yawp;
    struct {
        f32 yaw;
        f32 pitch;
        s32 pad[2];
    } angles;
    ActorCtl *aimer;
    Actor *state;

    // matching: one variable first holds the player, then the aimer (as IDO allocated it)
    aimer = (ActorCtl *)sprite->payload.player;
    state = ((Camera *)aimer)->CharPtr;
    if (state == NULL) {
        return 0;
    }
    aimer = state->mv;
    pitchp = &angles.pitch;
    yawp = &angles.yaw;
    if (aimer == NULL) {
        return 0;
    }
    func_15145974(&aimer->aimDir, pitchp, yawp);
    if (sprite->desc.texture == 0x9A) {
        sprite->desc.offsetS = -((angles.pitch * 0.002777777845f) * 2560.0f) * 2.0f;
    } else {
        sprite->desc.offsetT = ((angles.yaw * 0.002777777845f) * 2560.0f) * 2.0f;
    }
    return 1;
}

/* Spawns the centre dot, side ticks and diagonal corners of the cross sight for `player`. */
void func_151CC840(Camera *player, u8 red, u8 green, u8 blue, u8 arg4, u8 arg5) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.timer = 300;
    desc.flags = (1 << (player->CNumber + 11)) | ((arg4 ? 0x40 : 0) | 0x10);
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = red;
    desc.color1.green = green;
    desc.color1.blue = blue;
    desc.color1.alpha = 0xB4;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.viewport = player->CNumber;
    desc.texture = 0x64;
    desc.halfHeight = 12.0f;
    desc.halfWidth = 12.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9D;
    desc.flags &= ~6;
    desc.flags |= 0;
    desc.halfHeight = 7.0f;
    desc.halfWidth = 7.0f;
    desc.x = 55.0f;
    desc.y = 0.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = -55.0f;
    desc.y = 0.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9C;
    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = 0.0f;
    desc.y = 55.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = 0.0f;
    desc.y = -55.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.texture = 0x9E;
    desc.flags &= ~6;
    desc.flags |= 6;
    desc.halfHeight = 6.0f;
    desc.halfWidth = 6.0f;
    desc.x = 44.0f;
    desc.y = 44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 4;
    desc.x = -44.0f;
    desc.y = 44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 0;
    desc.x = -44.0f;
    desc.y = -44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }

    desc.flags &= ~6;
    desc.flags |= 2;
    desc.x = 44.0f;
    desc.y = -44.0f;
    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), arg5, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* Spawns the large ring of the cross sight (used instead of the cross in multiplayer). */
void func_151CCD1C(Camera *player, u8 red, u8 green, u8 blue) {
    ScreenSpriteInfo desc;
    s32 pad; // matching: unused stack slot
    SightSpritePayload payload;
    SightSprite *sprite;

    payload.player = player;
    payload.unk4 = 0.0f;
    payload.kind = 0;
    payload.flags = 0;
    payload.unk8 = 130.0f;

    desc.timer = 300;
    desc.flags = (1 << (player->CNumber + 11)) | 0x50;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = red;
    desc.color1.green = green;
    desc.color1.blue = blue;
    desc.color1.alpha = 0xB4;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x44;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 10;
    desc.x = 0.0f;
    desc.y = 0.0f;
    desc.viewport = player->CNumber;
    desc.texture = 0xB4;
    desc.halfWidth = 65.0f;
    desc.halfHeight = 65.0f;

    sprite = func_1515548C(&desc, 0, 0, 0, sizeof(SightSpritePayload), 0xFF, 1);
    if (sprite != NULL) {
        memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
    }
}

/* When D_800BE616 is set, spawns the func_151A4FD0 effect for `player` (see func_151CBB6C). */
void func_151CCE94(Camera *player) {
    SightOwnedEffect *effect;
    Camera *owner;

    if (D_800BE616 != 0) {
        owner = player;
        effect = (SightOwnedEffect *)func_151A4FD0(0, 0, 0, 0xFF, 0, player->CNumber, 1, 4);
        if (effect != NULL) {
            memcpy(&effect->player, &owner, sizeof(owner));
        }
    }
}

/* Texture ids of the four strips in a scope frame row. */
typedef struct { u8 b[4]; } SightTextureRow;
extern SightTextureRow D_8008FC28;

/* Spawns the two rows of four texture strips (top and bottom) of the scope frame for `player`. */
void func_151CCF08(Camera *player, u8 arg1, s8 arg2, u8 arg3) {
    ScreenSpriteInfo desc;
    SightSprite *sprite;
    SightSpritePayload payload;
    u8 i;
    SightTextureRow textures;
    u8 j;

    payload.rangeMax = 0.0f;
    payload.player = player;
    payload.unk4 = 0.0f;
    payload.rangeMin = D_800AAD6C[4][D_80082FA0 == 1];
    payload.rangeScale = D_800AAD94[4][D_80082FA0 == 1];
    payload.unk8 = 130.0f;
    payload.flags = (s32)((arg2 == -1) ? 0 : 4); // matching: see func_151C9F38
    payload.sourceIndex = arg2;

    desc.timer = 300;
    desc.flags = (arg1 ? 0x40 : 0) | 0x20 | 0x10;
    desc.fadeStart = 1;
    desc.fadeRate = 0xFF;
    desc.kind = 7;
    desc.color1.red = 0xFF;
    desc.color1.green = 0xFF;
    desc.color1.blue = 0xFF;
    desc.color1.alpha = 0xFF;
    desc.color2.red = 0xFF;
    desc.color2.green = 0xFF;
    desc.color2.blue = 0xFF;
    desc.color2.alpha = 0xFF;
    desc.unk23 = 0xFF;
    desc.otherModeH = 0;
    desc.unk28 = 0x200004;
    desc.unk2C = 0x1F0601;
    desc.combineMode = 8;
    desc.unk34 = 0x13;
    desc.unk38 = 0x80;
    desc.unk3C = 0x20;
    desc.unk40 = 0;
    desc.unk41 = 0;
    desc.viewport = player->CNumber;
    desc.offsetS = 0.0f;
    desc.offsetT = 0.0f;
    desc.scaleX = 1.0f;
    desc.scaleY = 1.0f;
    desc.halfWidth = 35.75f;
    desc.halfHeight = 16.0f;

    for (j = 0; j < 2; j++) {
        textures = D_8008FC28;
        desc.y = j ? 92.0f : -90.0f;
        desc.flags |= j ? 4 : 0;
        payload.kind = j ? 5 : 4;
        for (i = 0; i < 4; i++) {
            desc.x = ((i * 71.5f) + 35.75f) - 143.0f;
            desc.texture = textures.b[i];
            sprite = func_1515548C(&desc, 0xB, 0, 0, sizeof(SightSpritePayload), arg3, 1);
            if (sprite != NULL) {
                memcpy(&sprite->payload, &payload, sizeof(SightSpritePayload));
            }
        }
    }
}

/* Scope frame strip update: the top (kind 5) and bottom (kind 4) rows slide with the sight value. */
void func_151CD224(SightSprite *sprite) {
    SightSpritePayload *payload = &sprite->payload;
    f32 value = func_151CC1D4(sprite);
    f32 offset = (1.0f - (value - payload->rangeMin) * payload->rangeScale) * 75.0f;
    if (payload->kind == 5) {
        sprite->desc.y = 92.0f + offset;
    } else if (payload->kind == 4) {
        sprite->desc.y = -92.0f - offset;
    }
}
