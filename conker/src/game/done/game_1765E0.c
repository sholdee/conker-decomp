#include <ultra64.h>
#include "functions.h"
#include "variables.h"

/* This file's .rodata: the two object types of the timed effects (func_15149130), read by game_131F30.c and
 * game_1D3FC0.c too. */
const s32 D_800A5770[2] = { 0x23, 0x5F };

/*
 * This file's .data. The render setup and reset display lists of the timed effects; their entry in the object
 * type table D_8008B4A8 points to them.
 */
Gfx D_8008A470[] = {
    gsDPPipeSync(),
    gsSPMatrix(&D_80089470, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetConvert(0, 0, 0, 0, 0, 255),
    gsSPEndDisplayList(),
};
Gfx D_8008A498[] = {
    gsDPPipeSync(),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    // gsDPSetConvert(G_CV_K0, G_CV_K1, G_CV_K2, G_CV_K3, G_CV_K4, G_CV_K5) as Rare's gbi.h encoded it: it
    // shifted k2's high bits left (_SHIFTL(k2, 5, 4)) where 2.0L shifts them right
    {{ 0xEC15FDF0, 0x3B78E42A }},
    gsSPEndDisplayList(),
};

/*
 * The callback tables of the timed effects. Each effect kind keeps its own view of the object, so the tables are
 * declared without a prototype, but for the message handlers: func_15149434 passes the message type as a u8.
 */
/* Expire callbacks (TimedEffect.expireCallback). */
static void (*D_8008A4C0[10])() = {
    func_150AE790, func_15193390, func_150DDF88, func_151A18DC, func_151A1EE8, func_150C29F0, func_1519E65C,
    func_1519C9C4, func_150DF334, func_150DEBE0,
};
/* Tick callbacks (TimedEffect.tickCallback), run every frame. */
static void (*D_8008A4E8[98])() = {
    func_150DBC60, func_151A0AF8, func_151A11E4, func_151A1998, func_151A1FB4, func_151A4A38, func_151A5660,
    func_15156B54, func_1519E464, func_1519E754, func_151A4ECC, func_151A084C, func_1515B21C, func_151918BC,
    func_151AB9C8, func_150E6B84, func_150E7C9C, func_1515D130, func_150C2C00, func_151B0B88, func_151929A4,
    func_151B1B34, func_151B2100, func_150CD7F8, func_15192DF0, func_15159C08, func_151094FC, func_15109FB8,
    func_1510A40C, func_15191D54, func_151621B8, func_150E114C, func_150AF328, func_151CE6D0, func_150EFB80,
    func_1519CD64, func_150B54A8, func_151A8CEC, func_151082D0, func_150FC614, func_150E8470, func_150E9E34,
    func_151C4644, func_151C4820, func_150EAB10, func_150D2450, func_150D1C30, func_150D21CC, func_150D2374,
    func_150D26F0, func_150D2924, func_150E8B1C, func_150E8D5C, func_150E88C0, func_150E9178, func_150E93DC,
    func_150D54C8, func_15182748, func_150CF140, func_150CF530, func_15104C44, func_151050B0, func_151072BC,
    func_1515AB88, func_151BDD8C, func_151D4794, func_151D7264, func_150F706C, func_15105BC8, func_150B5950,
    func_150B6110, func_150D1B40, func_150D149C, func_150B1F90, func_151D9EB0, func_150AF7C4, func_150F1170,
    func_150FA1B8, func_151002BC, func_150BDF0C, func_150DFEFC, func_150F5C98, func_150BA55C, func_150C44A4,
    func_150F6178, func_150F64DC, func_150F4DEC, func_150FAAEC, func_150B0D20, func_150EC520, func_150EC6B0,
    func_151DC97C, func_150F2D14, func_150ECB8C, func_150DE7C0, func_151D1138, func_151BF81C, func_151D0128,
};
/* Draw callbacks (TimedEffect.drawCallback): return the advanced display list. */
static Gfx *(*D_8008A670[6])() = {
    func_151B2974, func_151B5328, func_150D22F4, func_15182768, func_151D014C, func_150D0034,
};
/* Destroy handlers, per callback set (TimedEffect.callbackSet). */
static void (*D_8008A688[74])() = {
    func_1514933C, func_1514933C, func_1519E8CC, func_1514933C, func_1519E924, func_1514933C, func_1514933C,
    func_1514933C, func_1514933C, func_1514933C, func_151A5CAC, func_1514933C, func_1514933C, func_1514933C,
    func_1514933C, func_151B1A58, func_1514933C, func_1514933C, func_151B229C, func_151B2FE8, func_1514933C,
    func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C,
    func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_150F02C0, func_1514933C, func_1514933C,
    func_150B57E8, func_1514933C, func_150FC9A4, func_150D227C, func_1514933C, func_150D5A2C, func_150D20B0,
    func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1510764C,
    func_1514933C, func_15141990, func_151BE1B8, func_150F6368, func_1514933C, func_151D7424, func_150F740C,
    func_150F9794, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C,
    func_1514933C, func_150C4B08, func_1514933C, func_1514933C, func_1514933C, func_1514933C, func_1514933C,
    func_1514933C, func_1514933C, func_151D1388, func_151BFBA4,
};
/* Free handlers, per callback set. */
static void (*D_8008A7B0[74])() = {
    func_15149368, func_15149368, func_1519E8F8, func_15149368, func_1519E948, func_15149368, func_15149368,
    func_15149368, func_15149368, func_15149368, func_151A5CEC, func_15149368, func_15149368, func_15149368,
    func_15149368, func_151B1A84, func_15149368, func_15149368, func_151B22C8, func_151B3014, func_15149368,
    func_15149368, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368,
    func_15149368, func_15149368, func_15149368, func_15149368, func_150F02EC, func_15149368, func_15149368,
    func_150B5814, func_15149368, func_150FC9D0, func_150D22A8, func_15149368, func_150D5A4C, func_150D20DC,
    func_15149368, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368, func_15107678,
    func_15149368, func_151419B0, func_151BE1E4, func_150F6394, func_15149368, func_151D7450, func_150F7438,
    func_150F97C0, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368,
    func_15149368, func_150C4B34, func_15149368, func_15149368, func_15149368, func_15149368, func_15149368,
    func_15149368, func_15149368, func_151D13B4, func_151BFBD0,
};
/* Message handlers, per callback set; optional. */
static void (*D_8008A8D8[74])(TimedEffect *object, s32 msg, u8 msgType) = {
    NULL, func_151A4D88, func_1519E570, func_1519E61C, func_1519E818, func_151A4F7C, func_151A09B4, NULL,
    func_15191A84, func_151ABD54, NULL, NULL, NULL, NULL, NULL, func_151B19A4, func_15193234, func_151B1FAC,
    func_151B2F04, func_151B2EC4, NULL, func_151B5A9C, func_151932E0, NULL, NULL, NULL, func_1510A870, NULL,
    func_1510A8CC, func_1519277C, func_151CD3CC, NULL, NULL, NULL, func_151A9024, func_150B5840, func_151087FC,
    func_150FC818, NULL, func_150AEDF8, func_150D596C, func_150D1F6C, func_150D32FC, func_150CF484, func_151A5F70,
    func_15104FF8, func_15105548, func_15105848, func_151076A4, func_151D08F0, func_151419D0, func_151BE0AC,
    func_150F63C0, func_151D4C38, func_151D73A8, func_150F7310, func_150F887C, NULL, func_150F15F8, func_15100230,
    func_150BE150, func_150E02C0, func_150F6138, func_150BA8F0, func_150C4B60, func_150FA468, func_150F6850,
    func_150B0CE0, func_150F3194, func_150ECB4C, func_150FACE4, func_150D00C0, func_151D1328, func_151BFBFC,
};


/* Allocates a timed callback object (0x28 header bytes plus dataSize) and initializes its header. */
void *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags,
                    u8 callbackSet, s32 dataSize, u8 owner, s32 list) {
    TimedEffect *object;
    u8 type;
    s32 size;

    size = dataSize + sizeof(TimedEffect);
    type = (flags & 2) ? 95 : 35;

    object = func_15167A68(type, list, size, 1, owner, 1);
    if (object == NULL) {
        return NULL;
    }
    object->header.state = timer;
    object->expireCallback = expireCallback;
    object->tickCallback = tickCallback;
    object->drawCallback = drawCallback;
    object->header.flags = flags;
    object->callbackSet = callbackSet;
    bzero(object->buffers, sizeof(object->buffers));

    return object;
}

/* func_15149130 without a draw callback. */
void *func_151491F4(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize,
                    u8 owner, s32 list) {
    return func_15149130(timer, expireCallback, tickCallback, -1, flags, callbackSet, dataSize, owner, list);
}

/* Per-frame update: counts the timer down (flag 1), runs the tick callback, then the expire callback and
 * removal once the timer goes negative. */
void func_15149264(TimedEffect *object) {
    if (object->header.flags & 1) {
        object->header.state -= D_800BE9E4;
    }
    if (object->tickCallback != -1) {
        D_8008A4E8[object->tickCallback](object);
    }
    if (object->header.state < 0) {
        if (object->expireCallback != -1) {
            D_8008A4C0[object->expireCallback](object);
        }
    }
    if (object->header.state < 0) {
        func_1516972C(&object->header);
    }
}

void func_15149318(TimedEffect *object) {
    func_151D5E30(object->buffers);
}

void func_1514933C(TimedEffect *object) {
    func_15149318(object);
    func_15169804(&object->header);
}

void func_15149368(TimedEffect *object) {
    func_15149318(object);
    func_15169824(&object->header);
}

/* Dispatches through the object's callback set (out-of-range sets fall back to set 0). */
void func_15149394(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A688[set](object);
}

void func_151493E4(TimedEffect *object) {
    s32 set = object->callbackSet;

    if (set < 0) {
        set = 0;
    } else if (set >= 74) {
        set = 0;
    }
    D_8008A7B0[set](object);
}

void func_15149434(TimedEffect *object, s32 arg1, u8 arg2) {
    s32 set = object->callbackSet;

    if (set < 0 || set >= 74) {
        set = 0;
    }
    if (D_8008A8D8[set] != NULL) {
        D_8008A8D8[set](object, arg1, arg2);
    }
}

/* Runs the object's draw callback, if any; returns the advanced display list. */
Gfx *func_15149490(Gfx *gfx, TimedEffect *object, s16 view) {
    s32 drawCallback = object->drawCallback;

    if (drawCallback != -1) {
        gfx = D_8008A670[drawCallback](gfx, object, view);
    }
    return gfx;
}

void func_151494E0(s32 arg0, u8 arg1) {
    func_15169260(D_800A5770, 2, arg0, arg1);
}

/* func_15169850 for effects: tracks or retargets the effect's owner on actor events. */
void func_15149514(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj) {
    func_15169850(event, eventType, owner, ownerId, obj);
}
