#include <ultra64.h>
#include "functions.h"
#include "variables.h"
#include "effects.h"

/* Ground probe result filled by func_1504715C. */
typedef struct {
    /* 0x00 */ f32 height;
    /* 0x04 */ u8 pad4[0x24 - 0x4];
} Ground;

/* Data of an object that emits particles around an actor while that actor has bones. */
typedef struct {
    /* 0x00 */ Actor *actor;
    /* 0x04 */ u8 actorId; // the actor's unique_id when the emitter was attached
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ f32 spawnCredit; // one particle is emitted per whole unit
} ActorEmitterState;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ ActorEmitterState state;
} ActorEmitter;

void func_1504715C(Ground *, Actor *);
void func_15143794(s16 yaw, s16 pitch, f32 distance, f32 *out);
void func_1515C244(Actor *, f32 *center, f32 *, f32 *);
void func_151D9014(f32 *, f32 *, s32, f32, s32, s32, f32, s32, f32, f32,
                   s32, Ground *, s32, s32, s32, s32);

/* Emitter update: removes itself once its actor is gone or replaced, otherwise emits particles
   from a sphere around the actor's center at a random rate. */
void func_150CF140(ActorEmitter *emitter) {
    ActorEmitterState *state;
    Actor *actor;
    f32 center[3];
    f32 radius;
    Ground ground;
    f32 extentA;
    f32 extentB;
    f32 pos[3];
    f32 vel[3];
    u8 yaw;

    actor = emitter->state.actor;
    state = &emitter->state;
    if ((actor->interaction_state == 0) || (actor->unique_id != state->actorId)) {
        emitter->header.state = -1;
        return;
    }
    if (actor->unk1D4 != NULL) {
        state->spawnCredit += (0.06f + (func_150ADA68() * 0.0704f)) * D_800BE9A4;
        func_1515C244(actor, center, &extentA, &extentB);
        radius = (extentA + extentB) * 0.5f;
        func_1504715C(&ground, actor);
        while (state->spawnCredit > 1.0f) {
            yaw = func_150ADA20() & 0xFF;
            func_15143794(yaw, (s16)((func_150ADA20() % 76U) - 64), radius, pos);
            pos[0] += center[0];
            pos[1] += center[1];
            pos[2] += center[2];
            func_15143794(yaw, (s16)((func_150ADA20() % 54U) - 56), (func_150ADA68() * 1.4430001f) + 2.2680001f, vel);
            func_151D9014(pos, vel, 5, (func_150ADA68() * 0.286f) + -0.78700006f, (func_150ADA20() % 21U) + 30,
                          (func_150ADA20() % 156U) + 100, (func_150ADA68() * 50.0f) + 48.0f, 1, 1.0f, 1.0f, 0,
                          &ground, 1, 0, emitter->header.owner, emitter->header.kind);
            state->spawnCredit -= 1.0f;
        }
    }
}

/* Message payload for func_150CF484: message 0 names an actor that went away, message 0x2D two
   actors that swapped places. */
typedef struct {
    /* 0x00 */ Actor *actorA;
    /* 0x04 */ union {
        Actor *actorB;
        u8 id;
    } u4;
    /* 0x08 */ u8 actorAId;
    /* 0x09 */ u8 actorBId;
} ActorMsg;

/* Emitter message handler: dies with its actor and follows it through a swap. */
void func_150CF484(ActorEmitter *emitter, ActorMsg *msg, u8 type) {
    ActorEmitterState *state = &emitter->state;
    Actor *actorA;

    if (type == 0) {
        actorA = msg->actorA; // matching: loaded before the state field, via a named local
        if ((actorA == state->actor) || (msg->u4.id == state->actorId)) {
            func_1516972C((struct102 *)emitter);
        }
    } else if (type == 0x2D) {
        if (state->actor == msg->actorA) {
            state->actor = msg->u4.actorB;
            state->actorId = msg->actorBId;
        } else if (state->actor == msg->u4.actorB) {
            state->actor = msg->actorA;
            state->actorId = msg->actorAId;
        }
    }
}

void func_151CF898(Actor *, f32, f32);

void func_150CF530(s32 arg0) {
    func_151CF898(&D_800CC2D0[0], D_800CC2D0[0].y_position + 300.0f, -10000.0f);
}

/* Texture scroll offsets; >> 7 gives the 10.2 fixed-point tile coordinate. */
extern s32 D_80088890;
extern s32 D_80088894;
extern s32 D_80088898;
extern s32 D_8008889C;

/* Advances the texture scroll offsets by the frames elapsed. */
void func_150CF578(s32 arg0) {
    D_80088890 += D_800BE9E4 * 26;
    D_80088894 -= D_800BE9E4 * 28;
    D_80088898 -= D_800BE9E4 * 28;
}

/* Sets the scrolled tile sizes of tiles 0 and 1. */
Gfx *func_150CF5E8(Gfx *gfx) {
    gDPSetTileSize(gfx++, 0, D_80088890 >> 7, D_80088894 >> 7, 0x47E, 0x47E);
    gDPSetTileSize(gfx++, 1, D_80088898 >> 7, D_8008889C >> 7, 0x47E, 0x47E);
    return gfx;
}

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

/* Particle motion: while flags & 1, hop from startX to startX + distance along an arc. */
typedef struct {
    /* 0x00 */ u8 flags;
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ f32 phase; // 0 to 1 over the hop
    /* 0x08 */ f32 startX;
    /* 0x0C */ f32 distance;
} HopMotion;

/* Particle motion: fall under gravity and bounce on a floor line (y grows downwards). */
typedef struct {
    /* 0x00 */ Vec2f vel;
    /* 0x08 */ f32 gravity;
    /* 0x0C */ f32 floorY;
} BounceMotion;

typedef struct {
    /* 0x00 */ u8 pad0;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u8 pad2[0xC - 0x2];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 padD[3];
    /* 0x10 */ Vec2f pos;
    /* 0x18 */ u8 pad18[0x70 - 0x18];
    /* 0x70 */ union {
        HopMotion hop;
        BounceMotion bounce;
    } motion;
} Particle;

Particle *func_1515548C(ScreenSpriteInfo *, s32, s32, s32, s32, u8, s32);

/* Spawns the hopping particle, waiting off screen at (-300, 72). */
void func_150CF680(s16 unk12, u8 unkC, s32 unk1) {
    ScreenSpriteInfo spawn;
    HopMotion hop;
    Particle *particle;

    spawn.y = 72.0f;
    spawn.texture = 0xC1;
    spawn.timer = unk12;
    spawn.flags = 0x31;
    spawn.fadeStart = 1;
    spawn.fadeRate = 0xFF;
    spawn.kind = 8;
    spawn.color1.red = 0xFF;
    spawn.color1.green = 0xFF;
    spawn.color1.blue = 0xFF;
    spawn.color1.alpha = 0xFF;
    spawn.color2.red = 0xFF;
    spawn.color2.green = 0xFF;
    spawn.color2.blue = 0xFF;
    spawn.color2.alpha = 0xFF;
    spawn.unk23 = 0xFF;
    spawn.unk28 = 0x200004;

    hop.flags = 0;
    hop.phase = 0.0f;
    spawn.x = -300.0f;
    hop.startX = -300.0f;
    hop.distance = 0.0f;
    spawn.halfHeight = 7.5f;
    spawn.halfWidth = 7.5f;
    spawn.otherModeH = 0;
    spawn.unk2C = 0x9F0601;
    spawn.unk40 = 0;
    spawn.unk41 = 10;
    spawn.combineMode = 0x17;
    spawn.unk34 = 0x44;
    spawn.unk38 = 0x80;
    spawn.unk3C = 0x20;
    spawn.scaleY = 1.0f;
    spawn.scaleX = 1.0f;
    spawn.offsetT = 0.0f;
    spawn.offsetS = 0.0f;
    spawn.viewport = D_80082FA0;

    particle = func_1515548C(&spawn, 12, 0, 0, 0x10, unkC, unk1);
    if (particle != NULL) {
        memcpy(&particle->motion.hop, &hop, sizeof(hop));
    }
}

extern s8 (*D_8008FCD0[])(void);
void func_15143874(s16 angle, f32 distance, f32 *x, f32 *y);

/* Hopping particle update: follows the hop arc and, on landing, kicks up 2 to 5 bouncing
   dust particles. */
s32 func_150CF800(Particle *hopper) {
    f32 phase;
    ScreenSpriteInfo spawn;
    BounceMotion dust;
    s8 count;
    s32 rand;
    s32 bit2;
    Particle *particle;
    HopMotion *hop;

    hop = &hopper->motion.hop;
    if (hop->flags & 1) {
        hop->phase += 0.19000001f * D_800BE9A4;
        phase = hop->phase;
        if (phase >= 1.0f) {
            hop->phase = 1.0f;
            hop->flags &= ~1;
            count = (func_150ADA20() & 3) + 2;
            dust.floorY = hopper->pos.y + 15.0f;
            // matching: x and y are copied as one Vec2f
            *(Vec2f *)&spawn.x = hopper->pos;
            spawn.fadeStart = 12;
            spawn.fadeRate = 0x15;
            spawn.kind = 0;
            spawn.color1.red = 0xFF;
            spawn.color1.green = 0xFF;
            spawn.color1.blue = 0xFF;
            spawn.color2.red = 0xFF;
            spawn.color2.green = 0xFF;
            spawn.color2.blue = 0xFF;
            spawn.color2.alpha = 0xFF;
            spawn.unk23 = 0xFF;
            spawn.otherModeH = 0;
            spawn.unk28 = 0x200004;
            spawn.unk2C = 0x9F0601;
            spawn.unk40 = 0;
            spawn.unk41 = 10;
            spawn.combineMode = 0x17;
            spawn.unk34 = 0x44;
            spawn.unk38 = 0x80;
            spawn.unk3C = 0x20;
            spawn.scaleX = 1.0f;
            spawn.scaleY = 1.0f;
            spawn.offsetT = 0.0f;
            spawn.offsetS = 0.0f;
            spawn.viewport = D_80082FA0;
            for (; count > 0; count--) {
                spawn.color1.alpha = (func_150ADA20() % 101U) + 155;
                spawn.texture = D_8008FCD0[2]();
                rand = func_150ADA20();
                phase = (func_150ADA68() * 2) + 2.5f; // matching: the hop phase's register holds the dust speed
                func_15143874((s16)((rand % 65U) - 160), phase, &dust.vel.x, &dust.vel.y);
                dust.gravity = (func_150ADA68() * 0.333f) + 0.404f;
                spawn.halfWidth = spawn.halfHeight = (func_150ADA68() * 1.79f) + 1.6999999f;
                spawn.timer = (func_150ADA20() % 26U) + 30;
                if (func_150ADA20() & 1) {
                    bit2 = 2;
                } else {
                    bit2 = 0;
                }
                if (func_150ADA20() & 1) {
                    rand = 4; // matching: reusing rand's register for this flag bit
                } else {
                    rand = 0;
                }
                spawn.flags = rand | 1 | bit2 | 8 | 0x10 | 0x20; // matching: folding the constants into 0x39 changes the code
                particle = func_1515548C(&spawn, 13, 0, 0, 0x10, hopper->unkC, hopper->unk1);
                if (particle != NULL) {
                    memcpy(&particle->motion.bounce, &dust, sizeof(dust));
                }
            }
            phase = hop->phase;
        }
        hopper->pos.x = hop->startX + (phase * hop->distance);
        hopper->pos.y = (sinf(hop->phase * 3.1415927f) * -17.5f) + 72.0f;
    }
    return 1;
}

/* Hopping particle message handler: message 0x52 starts a hop to the x given in the payload. */
void func_150CFBEC(Particle *hopper, f32 *targetX, u8 type) {
    HopMotion *hop = &hopper->motion.hop;

    if (type == 0x52) {
        hop->flags |= 1;
        hop->phase = 0.0f;
        hop->startX = hopper->pos.x;
        hop->distance = targetX[0] - hopper->pos.x;
    }
}

void func_1515572C(f32 *, s32);

/* Sends message 0x52: hop to targetX. */
void func_150CFC38(f32 targetX) {
    f32 payload = targetX;

    func_1515572C(&payload, 0x52);
}

/* Bouncing particle update: integrates the motion and bounces off the floor, damped. */
s32 func_150CFC60(Particle *particle) {
    BounceMotion *m = &particle->motion.bounce;

    particle->pos.x += m->vel.x * D_800BE9A4;
    particle->pos.y += (m->vel.y * D_800BE9A4) + (((0.5f * m->gravity) * D_800BE9A4) * D_800BE9A4);
    m->vel.y += m->gravity * D_800BE9A4;
    if (m->floorY < particle->pos.y) {
        particle->pos.y = m->floorY;
        m->vel.x *= 0.3f;
        m->vel.y = fabsf(m->vel.y) * -0.15f;
    }
    return 1;
}

/* Returns the end of the line at str: its 0xBD line break or the string's terminating 0. */
u8 *func_150CFD20(u8 *str) {
    while (*str != 0xBD && *str != 0) {
        str++;
    }
    return str;
}

/* Returns the end (terminating 0) of str. */
u8 *func_150CFD5C(u8 *str) {
    while (*str != 0) {
        str++;
    }
    return str;
}

/* Returns the length of the line at str and stores its end in *lineEnd. */
s32 func_150CFD84(u8 *str, u8 **lineEnd) {
    u8 *end;

    end = func_150CFD20(str);
    *lineEnd = end;
    return end - str;
}

/* Returns the length of the longest line in str. */
s32 func_150CFDB8(u8 *str) {
    s32 longest;
    u8 *end;
    u8 *lineEnd;

    longest = 0;
    end = func_150CFD5C(str);
    while (str < end) {
        s32 len = func_150CFD84(str, &lineEnd);
        if (longest < len) {
            longest = len;
        }
        str = lineEnd + 1;
    }
    return longest;
}

/* Text box data (at 0x28 in its object): shows a string one line at a time. */
typedef struct {
    /* 0x00 */ u8 id; // matched against message 0x51's payload
    /* 0x01 */ u8 pad1[3];
    /* 0x04 */ u8 *str;
    /* 0x08 */ u8 flags; // bit 0: a new line is being shown this frame
    /* 0x09 */ u8 pad9[3];
    /* 0x0C */ u8 *line;
    /* 0x10 */ u8 *lineEnd;
    /* 0x14 */ u8 lineLen;
    /* 0x15 */ u8 page; // which of lines[] holds the current line; the other holds the previous one
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ u8 *lines[2]; // NUL-terminated copies of lines, each as long as the longest line
    /* 0x20 */ void *style; // the display style's own state, which follows this struct
    /* 0x24 */ s8 updateFn; // index into D_800888A0, or -1
    /* 0x25 */ u8 msgFn; // index into D_800888B0
    /* 0x26 */ u8 pad26[2];
} TextLines;

typedef struct {
    /* 0x00 */ EffectHeader header;
    /* 0x10 */ u8 pad10[0x18];
    /* 0x28 */ TextLines text;
} TextObj;

/* Copies the current line into lines[page]. */
void func_150CFE3C(TextObj *obj) {
    TextLines *text = &obj->text;

    memcpy(text->lines[text->page], text->line, text->lineLen);
    text->lines[text->page][text->lineLen] = 0;
}

/* Moves on to the next line, if there is one. */
void func_150CFE98(TextObj *obj) {
    TextLines *text = &obj->text;

    if (*text->lineEnd != 0) {
        text->line = text->lineEnd + 1;
        text->lineLen = func_150CFD84(text->lineEnd + 1, &text->lineEnd);
        text->page ^= 1;
        func_150CFE3C(obj);
        text->flags |= 1;
    }
}

/* Creates a text box object for str, with styleSize bytes of style state (left zeroed). */
TextObj *func_150CFF10(u8 id, u8 *str, s16 duration, s32 styleSize, s8 updateFn, u8 msgFn, u8 arg6, s32 arg7) {
    TextLines init;
    TextObj *obj;
    s32 lineSize;
    TextLines *text;

    init.msgFn = msgFn;
    init.updateFn = updateFn;
    init.page = 0;
    init.flags = 1;
    init.str = str;
    init.line = str;
    init.id = id;
    init.lineLen = func_150CFD84(str, &init.lineEnd);
    init.lines[0] = NULL;
    init.lines[1] = NULL;
    init.style = NULL;

    lineSize = func_150CFDB8(str) + 1;

    obj = (TextObj *)func_15149130(duration, -1, -1, 5, 3, 0x47, (sizeof(TextLines) + styleSize + lineSize * 2), arg6, arg7);
    if (obj != NULL) {
        text = &obj->text;
        memcpy(text, &init, sizeof(TextLines));
        text->style = text + 1;
        text->lines[0] = (u8 *)text->style + styleSize;
        text->lines[1] = text->lines[0] + lineSize;
        *text->lines[0] = 0;
        *text->lines[1] = 0;
        func_150CFE3C(obj);
    }
    return obj;
}

extern s32 (*D_800888A0[])(TextObj *);

/* Text box update: runs the style's update function, removing the box when it returns 0. */
s32 func_150D0034(s32 arg0, TextObj *obj, s32 arg2) {
    TextLines *text = &obj->text;

    if ((text->updateFn != -1) && (D_800888A0[text->updateFn](obj) == 0)) {
        obj->header.state = -1;
        return arg0;
    }
    text->flags &= ~1;
    return arg0;
}

extern void (*D_800888B0[])(TextObj *, u8 *, u8);

/* Text box message handler: message 0x51 with this box's id advances the line; the style's
   handler gets every other message. */
void func_150D00C0(TextObj *obj, u8 *payload, u8 type) {
    void (*handler)(TextObj *, u8 *, u8);

    if (type == 0x51) {
        if (obj->text.id == *payload) {
            func_150CFE98(obj);
        }
    } else {
        handler = D_800888B0[obj->text.msgFn];
        if (handler != NULL) {
            handler(obj, payload, type);
        }
    }
}

/* Style 0 state: frames left in the crossfade to a new line. */
typedef struct {
    /* 0x00 */ s8 fadeTimer;
    /* 0x01 */ u8 pad1[7];
} CrossfadeStyle;

/* Creates a text box that crossfades between lines (style 0). */
void func_150D0134(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    CrossfadeStyle style;
    TextObj *obj;

    style.fadeTimer = 0;
    obj = func_150CFF10(id, str, duration, 8, 0, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style.fadeTimer, sizeof(style.fadeTimer));
    }
}

void func_15042D94(s32 x, s32 y, s32, u8 *str);

/* Style 0 update: draws the current line fading in over 8 frames and the previous one fading out. */
s32 func_150D01A0(TextObj *obj) {
    s8 *fadeTimer;
    u8 alpha;
    u8 fade;
    TextLines *text;

    text = &obj->text;
    fadeTimer = text->style;
    if (text->flags & 1) {
        *fadeTimer = 8;
    }
    if (*fadeTimer > 0) {
        fade = *fadeTimer * 31;
        alpha = 255 - fade;
        *fadeTimer -= D_800BE9E4;
    } else {
        alpha = 255;
        fade = 0;
    }
    if (alpha) {
        func_1504332C(0xCE, 0xC4, 0x61, alpha);
        func_15042D94(0x92, 0xBE, 0x81, text->lines[text->page]);
    }
    if (fade) {
        func_1504332C(0xCE, 0xC4, 0x61, fade);
        func_15042D94(0x92, 0xBE, 0x81, text->lines[text->page ^ 1]);
    }
    return 1;
}

/* Style 1 state: how much of the line has been typed out. */
typedef struct {
    /* 0x00 */ f32 progress; // fraction of the next character
    /* 0x04 */ s16 shown; // characters typed so far
    /* 0x06 */ u8 pad6[2];
} TypewriterStyle;

/* Creates a text box that types out each line behind a "C:> " prompt (style 1). */
void func_150D02B4(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    TextObj *obj;
    TypewriterStyle style;

    style.shown = 0;
    style.progress = 0.0f;
    obj = func_150CFF10(id, str, duration, 8, 1, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style, sizeof(style));
    }
}

/* Style 1 update: types out the current line and draws it after the prompt, with a cursor. */
s32 func_150D032C(TextObj *obj) {
    TextLines *text;
    TypewriterStyle *style;
    u8 buf[0x50];
    u8 prompt[] = "C:> ";

    text = &obj->text;
    style = text->style;

    if (text->flags & 1) {
        style->shown = 0;
        style->progress = 0.0f;
    }

    if (style->progress < (f32)text->lineLen) {
        style->progress += 0.437f * D_800BE9A4;
        while (style->progress > 1.0f) {
            style->shown++;
            if (text->lineLen < style->shown) {
                style->shown = text->lineLen;
            }
            style->progress -= 1.0f;
        }
    }

    memcpy(buf, prompt, 4);
    memcpy(&buf[4], text->lines[text->page], style->shown);
    buf[style->shown + 4] = ' ';
    buf[style->shown + 5] = 0xBB;
    buf[style->shown + 6] = 0;
    func_1504332C(0, 0xFF, 0, 0x96);
    func_15042D94(15, 0xBE, 0x80, buf);
    return 1;
}

/* Creates a text box that crossfades between lines more slowly, in white (style 2). */
void func_150D04C4(u8 id, u8 *str, s16 duration, u8 arg3, s32 arg4) {
    CrossfadeStyle style;
    TextObj *obj;

    style.fadeTimer = 0;
    obj = func_150CFF10(id, str, duration, 8, 2, 0, arg3, arg4);
    if (obj != NULL) {
        memcpy(obj->text.style, &style.fadeTimer, sizeof(style.fadeTimer));
    }
}

/* Style 2 update: like func_150D01A0 but white and over 20 frames. */
s32 func_150D0534(TextObj *obj) {
    s8 *fadeTimer;
    u8 alpha;
    u8 fade;
    TextLines *text;

    text = &obj->text;
    fadeTimer = text->style;
    if (text->flags & 1) {
        *fadeTimer = 20;
    }
    if (*fadeTimer > 0) {
        fade = *fadeTimer * 12;
        alpha = 255 - fade;
        *fadeTimer -= D_800BE9E4;
    } else {
        alpha = 255;
        fade = 0;
    }
    if (alpha) {
        func_1504332C(0xFF, 0xFF, 0xFF, alpha);
        func_15042D94(20, 20, 0x80, text->lines[text->page]);
    }
    if (fade) {
        func_1504332C(0xFF, 0xFF, 0xFF, fade);
        func_15042D94(20, 20, 0x80, text->lines[text->page ^ 1]);
    }
    return 1;
}
