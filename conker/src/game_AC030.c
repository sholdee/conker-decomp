#include <ultra64.h>

/* This file's .bss, defined before the headers declare it (see init_50A0.c). */
u8 D_800D18C0;
static s32 D_800D18C4;

#include "functions.h"
#include "variables.h"

/*
 * Idle animations: while an actor stands still it plays short "sequences" (lists of frames
 * from D_8009B8B0), picked at random from per-category candidate lists while avoiding the
 * ones played most recently.
 */

/* One frame of an idle sequence. */
typedef struct {
    /* 0x0 */ u16 anim;
    /* 0x2 */ u8 speed;       // in 1/100ths
    /* 0x3 */ u8 blend;       // in 1/10ths
    /* 0x4 */ u8 holdFrames;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 flags;
    /* 0x7 */ u8 unk7;
} IdleFrame;

/* The frames of the idle sequences: anim, speed, blend, hold frames, pad, flags, unk7. */
static const IdleFrame D_8009B8B0[70] = {
    { 0x000F, 100, 70, 0, 0, 0x00, 0 },
    { 0x000F, 100, 100, 0, 0, 0x00, 0 },
    { 0x000F, 100, 70, 0, 0, 0x00, 0 },
    { 0x000F, 100, 70, 0, 2, 0x01, 0 },
    { 0x0004, 100, 100, 1, 0, 0x00, 0 },
    { 0x0004, 100, 70, 0, 0, 0x01, 0 },
    { 0x0003, 100, 70, 0, 0, 0x01, 0 },
    { 0x0025, 100, 70, 0, 3, 0x00, 0 },
    { 0x0025, 100, 70, 0, 3, 0x01, 0 },
    { 0x000D, 100, 70, 0, 0, 0x00, 0 },
    { 0x019F, 100, 70, 5, 0, 0x04, 0 },
    { 0x000E, 100, 70, 0, 0, 0x00, 0 },
    { 0x005B, 100, 70, 5, 2, 0x00, 0 },
    { 0x000F, 100, 70, 0, 1, 0x00, 0 },
    { 0x0072, 100, 70, 0, 0, 0x00, 0 },
    { 0x0052, 100, 70, 0, 0, 0x00, 0 },
    { 0x0061, 100, 70, 0, 0, 0x00, 0 },
    { 0x0025, 100, 70, 0, 0, 0x00, 0 },
    { 0x0171, 100, 70, 0, 0, 0x00, 0 },
    { 0x0172, 100, 70, 0, 0, 0x08, 0 },
    { 0x0173, 100, 70, 0, 0, 0x00, 0 },
    { 0x0174, 100, 70, 0, 0, 0x08, 0 },
    { 0x017A, 100, 70, 0, 0, 0x08, 0 },
    { 0x0011, 100, 70, 0, 0, 0x00, 0 },
    { 0x0026, 100, 70, 0, 0, 0x00, 0 },
    { 0x0029, 100, 70, 0, 0, 0x00, 0 },
    { 0x0055, 100, 70, 0, 0, 0x00, 0 },
    { 0x0057, 120, 70, 0, 0, 0x00, 0 },
    { 0x0056, 100, 70, 0, 0, 0x00, 0 },
    { 0x0049, 100, 70, 0, 0, 0x00, 0 },
    { 0x0047, 100, 70, 0, 0, 0x00, 0 },
    { 0x0081, 100, 70, 0, 0, 0x00, 0 },
    { 0x005E, 100, 70, 0, 0, 0x00, 0 },
    { 0x005F, 100, 70, 0, 0, 0x00, 0 },
    { 0x0060, 100, 70, 0, 0, 0x00, 0 },
    { 0x00B3, 100, 70, 0, 0, 0x00, 0 },
    { 0x00FA, 100, 70, 0, 0, 0x00, 0 },
    { 0x0063, 100, 70, 0, 0, 0x00, 0 },
    { 0x0082, 100, 70, 0, 0, 0x00, 0 },
    { 0x0083, 100, 70, 0, 0, 0x00, 0 },
    { 0x0087, 100, 70, 0, 0, 0x00, 0 },
    { 0x00BB, 100, 70, 0, 0, 0x00, 0 },
    { 0x0088, 100, 70, 2, 0, 0x00, 0 },
    { 0x0089, 100, 70, 0, 0, 0x00, 0 },
    { 0x0087, 100, 70, 0, 0, 0x00, 0 },
    { 0x0088, 100, 70, 0, 0, 0x02, 0 },
    { 0x0089, 100, 70, 0, 0, 0x02, 0 },
    { 0x008A, 100, 70, 0, 0, 0x00, 0 },
    { 0x0067, 100, 70, 0, 0, 0x00, 0 },
    { 0x0073, 100, 70, 5, 0, 0x00, 0 },
    { 0x0086, 100, 70, 0, 0, 0x00, 0 },
    { 0x0091, 100, 70, 0, 0, 0x00, 0 },
    { 0x0094, 100, 20, 0, 0, 0x00, 0 },
    { 0x00BA, 100, 20, 0, 0, 0x01, 0 },
    { 0x009A, 100, 70, 0, 0, 0x00, 0 },
    { 0x009B, 100, 70, 0, 0, 0x08, 0 },
    { 0x00AF, 100, 70, 0, 0, 0x00, 0 },
    { 0x0126, 100, 70, 0, 0, 0x00, 0 },
    { 0x007F, 100, 70, 0, 0, 0x00, 0 },
    { 0x0144, 100, 70, 0, 0, 0x00, 0 },
    { 0x01B9, 100, 70, 0, 0, 0x00, 0 },
    { 0x01BA, 100, 70, 0, 0, 0x00, 0 },
    { 0x01BB, 100, 70, 0, 0, 0x00, 0 },
    { 0x01BC, 100, 70, 0, 0, 0x10, 0 },
    { 0x01CB, 100, 70, 0, 0, 0x10, 0 },
    { 0x01CC, 100, 70, 0, 0, 0x10, 0 },
    { 0x0165, 100, 70, 0, 0, 0x00, 0 },
    { 0x022A, 100, 70, 0, 0, 0x00, 0 },
    { 0x00BD, 100, 70, 0, 0, 0x00, 0 },
    { 0x0229, 100, 70, 0, 0, 0x00, 0 },
};

/* The idle sequences: 0-terminated lists of D_8009B8B0 indices. */
static const u8 D_8009BAE0[] = { 0x00 };
static const u8 D_8009BAE4[] = { 0x04, 0x05, 0x06, 0x04, 0x01, 0x00 };
static const u8 D_8009BAEC[] = { 0x0C, 0x00 };
static const u8 D_8009BAF0[] = { 0x07, 0x00 };
static const u8 D_8009BAF4[] = { 0x07, 0x00 };
static const u8 D_8009BAF8[] = { 0x09, 0x0A, 0x0B, 0x00 };
static const u8 D_8009BAFC[] = { 0x11, 0x00 };
static const u8 D_8009BB00[] = { 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x14, 0x00 };
static const u8 D_8009BB08[] = { 0x12, 0x13, 0x13, 0x13, 0x15, 0x13, 0x13, 0x15, 0x13, 0x13, 0x16, 0x13, 0x13, 0x14, 0x00 };
static const u8 D_8009BB18[] = { 0x17, 0x18, 0x19, 0x00 };
static const u8 D_8009BB1C[] = { 0x1A, 0x1B, 0x1C, 0x00 };
static const u8 D_8009BB20[] = { 0x1D, 0x1E, 0x1F, 0x00 };
static const u8 D_8009BB24[] = { 0x20, 0x21, 0x23, 0x24, 0x22, 0x00 };
static const u8 D_8009BB2C[] = { 0x0F, 0x00 };
static const u8 D_8009BB30[] = { 0x10, 0x00 };
static const u8 D_8009BB34[] = { 0x26, 0x00 };
static const u8 D_8009BB38[] = { 0x27, 0x00 };
static const u8 D_8009BB3C[] = { 0x28, 0x2B, 0x29, 0x2A, 0x2F, 0x00 };
static const u8 D_8009BB44[] = { 0x2C, 0x2E, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2E, 0x2E, 0x2E, 0x2E, 0x2F, 0x00 };
static const u8 D_8009BB58[] = { 0x30, 0x31, 0x32, 0x00 };
static const u8 D_8009BB5C[] = { 0x33, 0x34, 0x35, 0x00 };
static const u8 D_8009BB60[] = { 0x36, 0x00 };
static const u8 D_8009BB64[] = { 0x37, 0x00 };
static const u8 D_8009BB68[] = { 0x38, 0x00 };
static const u8 D_8009BB6C[] = { 0x39, 0x00 };
static const u8 D_8009BB70[] = { 0x3A, 0x00 };
static const u8 D_8009BB74[] = { 0x3B, 0x00 };
static const u8 D_8009BB78[] = { 0x42, 0x00 };
static const u8 D_8009BB7C[] = { 0x43, 0x00 };
static const u8 D_8009BB80[] = { 0x44, 0x00 };
static const u8 D_8009BB84[] = { 0x45, 0x00 };
static const u8 D_8009BB88[] = { 0x3C, 0x3D, 0x3D, 0x3E, 0x00 };
static const u8 D_8009BB90[] = { 0x3F, 0x3F, 0x3F, 0x3F, 0x40, 0x40, 0x40, 0x40, 0x3F, 0x3F, 0x3F, 0x3F, 0x41, 0x41, 0x41, 0x41, 0x00 };

/* The candidate sequences of each category; D_8009BBF0 gives their number. */
static const u8 D_8009BBA4[7] = { 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x14 };
static const u8 D_8009BBAC[9] = { 0x06, 0x07, 0x08, 0x09, 0x0A, 0x13, 0x15, 0x05, 0x04 };
static const u8 D_8009BBB8[1] = { 0x00 };
static const u8 D_8009BBBC[1] = { 0x16 };
static const u8 D_8009BBC0[1] = { 0x1A };
static const u8 D_8009BBC4[6] = { 0x01, 0x01, 0x01, 0x01, 0x01, 0x02 };
static const u8 D_8009BBCC[6] = { 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1E };
static const u8 D_8009BBD4[6] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x20 };
static const u8 D_8009BBDC[10] = { 0x11, 0x12, 0x06, 0x08, 0x09, 0x0A, 0x13, 0x15, 0x05, 0x04 };
static const u8 D_8009BBE8[1] = { 0x1C };
static const u8 D_8009BBEC[2] = { 0x1B, 0x1C };

static const u8 D_8009BBF0[11] = { 7, 9, 1, 1, 1, 6, 10, 1, 2, 6, 6 };

/* This file's .data: the sequences by number, then the candidates by category. */
static const u8 *D_80086BA0[33] = {
    D_8009BAE0, D_8009BB70, D_8009BB74, D_8009BB68, D_8009BB64, D_8009BB60,
    D_8009BB3C, D_8009BB44, D_8009BB58, D_8009BB34, D_8009BB38, D_8009BB2C,
    D_8009BB30, D_8009BB24, D_8009BB1C, D_8009BB20, D_8009BB18, D_8009BB00,
    D_8009BB08, D_8009BAE4, D_8009BAF8, D_8009BAFC, D_8009BB5C, D_8009BAEC,
    D_8009BAF0, D_8009BAF4, D_8009BB6C, D_8009BB88, D_8009BB90, D_8009BB78,
    D_8009BB7C, D_8009BB80, D_8009BB84,
};
static const u8 *D_80086C24[11] = {
    D_8009BBA4, D_8009BBAC, D_8009BBB8, D_8009BBBC, D_8009BBC0, D_8009BBC4,
    D_8009BBDC, D_8009BBE8, D_8009BBEC, D_8009BBCC, D_8009BBD4,
};

/* Appends one sequence to a 40-entry candidate list. */
void func_1507EB80(u8 *list, s32 *count, u8 seq) {
    if (*count + 1 < 40) {
        list[*count] = seq;
        (*count)++;
    }
}

/* Appends every candidate sequence of a category to a 40-entry candidate list. */
void func_1507EBB8(u8 *list, s32 *count, s32 category) {
    const u8 *src = D_80086C24[category];
    s32 len = D_8009BBF0[category];

    if (*count + len < 40) {
        bcopy(src, list + *count, len);
        *count += len;
    }
}

/*
 * Copies the distinct candidates not among the recent sequences to out. If none remain, falls
 * back to (up to half of) the recent sequences that are candidates, oldest first, and returns 1;
 * with no candidates at all, out is { 0 }.
 */
s32 func_1507EC38(u8 *candidates, s32 numCandidates, u8 *out, s32 *outLen, u8 *recent) {
    s32 reused;
    s32 i;
    s32 j;
    s32 found;
    s32 numReusable;
    u8 reusable[5];
    s32 take;

    reused = FALSE;
    *outLen = 0;
    for (i = 0; i < numCandidates; i++) {
        found = FALSE;
        for (j = 0; j < *outLen; j++) {
            if (out[j] == candidates[i]) {
                found = TRUE;
                break;
            }
        }
        if (!found) {
            for (j = 0; j < 5; j++) {
                if (recent[j] == candidates[i]) {
                    found = TRUE;
                    break;
                }
            }
            if (!found) {
                out[*outLen] = candidates[i];
                (*outLen)++;
            }
        }
    }

    if (*outLen == 0) {
        numReusable = 0;
        for (i = 0; i < 5; i++) {
            if (recent[i] != 0) {
                for (j = 0; j < numCandidates; j++) {
                    if (recent[i] == candidates[j]) {
                        reusable[numReusable] = recent[i];
                        numReusable++;
                        break;
                    }
                }
            }
        }
        if (numReusable == 0) {
            out[0] = 0;
            *outLen = 1;
        } else {
            take = numReusable >> 1;
            if (take == 0) {
                take = 1;
            }
            while (take != 0) {
                numReusable--;
                out[*outLen] = reusable[numReusable];
                (*outLen)++;
                take--;
            }
            reused = TRUE;
        }
    }
    return reused;
}

/* Records seq as the newest recent sequence; 0x11 and 0x12 are recorded as a pair. */
void func_1507EE58(seq, recent)
    u8 seq;
    u8 *recent;
{
    func_1507EEB8(seq, recent);
    if (seq == 0x11) {
        func_1507EEB8(0x12, recent);
    } else if (seq == 0x12) {
        func_1507EEB8(0x11, recent);
    }
}

/* Pushes seq onto the front of the 5-entry recent list, dropping the oldest. */
void func_1507EEB8(u8 seq, u8 *recent) {
    s32 i;

    for (i = 4; i > 0; i--) {
        recent[i] = recent[i - 1];
    }
    recent[0] = seq;
}

/* Advances the current actor's idle phase: 0 -> 1, 1 -> 2 after 3 sequences, 2 -> 1 after 8-11. */
void func_1507EEF4(void) {
    IdleState *idle;
    s32 phase;

    idle = &gCurrentActor->mv->idle;
    phase = idle->phase;

    if (phase == 0) {
        idle->phase = 1;
        idle->phaseCount = 0;
        return;
    }

    if (phase == 1) {
        func_150ADA20_u8();
        if (idle->phaseCount >= 3) {
            idle->phase = 2;
            idle->phaseCount = 0;
        }
        return;
    }

    if (idle->phaseCount >= (func_150ADA20_u8() & 3) + 8) {
        idle->phase = 1;
        idle->phaseCount = 0;
    }
}

/* Clears the oldest occurrence of seq from the recent list. */
void func_1507EFA0(s32 seq, u8 *recent) {
    s32 i;

    for (i = 4; i >= 0; i--) {
        if (recent[i] == seq) {
            recent[i] = 0;
            return;
        }
    }
}

/* Picks the current actor's next idle sequence from the candidates its situation allows. */
void func_1507EFD0(void) {
    IdleState *idle;
    u8 candidates[40];
    u8 choices[40];
    s32 count;
    s32 numChoices;
    s32 pick;
    u8 *recent;
    s32 category;
    s32 reused;
    s32 seq;

    idle = &gCurrentActor->mv->idle;
    count = 0;
    /* unk139: a requested sequence, unk13A: its countdown */
    if (gCurrentActor->unk13A != 0) {
        if (gCurrentActor->unk139 < 0x21) {
            func_1507EB80(candidates, &count, gCurrentActor->unk139);
        }
    } else if (D_800D18C0 & 2) {
        func_1507EB80(candidates, &count, 3);
        D_800D18C0 = 0;
    } else if (gCurrentActor->mv->idle.delay != 0 && gCurrentActor->mv->rt_context_b == 0) {
        seq = 0xB;
        if (idle->timer >= 121) {
            gCurrentActor->mv->idle.delay = 0;
            idle->timer = 0;
            idle->phase = 0;
            seq = 0xC;
            func_1507FF94(gCurrentActor);
        } else {
            seq = 0xB; // matching: the redundant store keeps the else-branch block
        }
        func_1507EB80(candidates, &count, seq);
    } else if (gCurrentActor->energy < 2) {
        func_1507EB80(candidates, &count, 0);
    } else if (effect_matchWatcherState(&movement_obj[dc_glbl_CharNum], 0, 0, -1, -1, -1) != 0) {
        func_1507EB80(candidates, &count, 0);
    } else if (idle->timer >= func_1507F4C0(idle->phase)) {
        func_1507EEF4();
        category = -1;
        if (gCurrentActor->unk2F8 & 0x200) {
            category = 2;
        } else if (gCurrentActor->mv->rt_context_b == 9) {
            category = 5;
        } else if (gCurrentActor->mv->rt_context_b == 0x23) {
            category = 9;
        } else if (gCurrentActor->mv->rt_context_b == 0x16) {
            category = 10;
        } else if (attach_find(gCurrentActor, 0x17, 0) != NULL) {
            category = 6;
        } else if (level == 0x31) {
            if (D_800D18C0 & 4) {
                category = 8;
            } else {
                category = 7;
            }
        } else if (gCurrentActor->character_type == 0x96 || level == 0x10 || level == 0x1B ||
                   level == 0x1E || level == 0x26 || level == 0x2F ||
                   level == 0xE || level == 0x28 || level == 0x23 ||
                   level == 0x1C || level == 0x36 || level == 0x3A) {
            category = 2;
        } else if (D_800D18C0 != 0) {
            if (D_800D18C0 & 1) {
                category = 4;
            }
            D_800D18C0 = 0;
        }
        if (category != -1) {
            count = 0;
            func_1507EBB8(candidates, &count, category);
        } else if (idle->phase == 1) {
            func_1507EBB8(candidates, &count, 0);
        } else {
            func_1507EBB8(candidates, &count, 1);
            if (idle->phase == 2 && idle->phaseCount >= 4 && func_1507FFD8(gCurrentActor) != 0) {
                func_1507EBB8(candidates, &count, 3);
            }
        }
    } else {
        func_1507EB80(candidates, &count, 0);
    }
    recent = idle->recent;
    reused = func_1507EC38(candidates, count, choices, &numChoices, recent);
    if (numChoices >= 2) {
        pick = func_150ADA20_u8() % (u32)numChoices;
    } else {
        pick = 0;
    }
    idle->sequence = choices[pick];
    idle->frame = 0;
    idle->phaseCount++;
    if (idle->phase != 0) {
        idle->timer = 0;
    }
    if (reused) {
        func_1507EFA0(idle->sequence, recent);
    }
    func_1507EE58(idle->sequence, recent);
}

/* Steps the current actor's idle sequence; returns 1 when it has none or it just ended. */
s32 func_1507F454(void) {
    IdleState *idle;
    s32 sequence;
    s32 next;

    idle = &gCurrentActor->mv->idle;
    sequence = idle->sequence;

    if (sequence == 0) {
        return TRUE;
    }
    idle->frame++;
    next = D_80086BA0[sequence][idle->frame];
    if (next == 0) {
        idle->sequence = 0;
        idle->frame = 0;
        return TRUE;
    }
    return FALSE;
}

/* Returns a random idle time to wait in phase before picking the next sequence. */
s32 func_1507F4C0(s32 phase) {
    s32 pad; // matching: unused stack slot
    s32 base;
    u32 range;

    if (phase == 0) {
        base = 180;
        range = 60;
    } else if (level == 0x31) {
        return 0;
    } else if (phase == 1) {
        base = 60;
        range = 60;
    } else {
        base = 0;
        range = 30;
    }

    return func_150ADA20_u8() % range + base;
}

/* Plays the actor's idle animation; when unk13C names an object slot (100 and up), one that
 * suits that object's id. */
void func_1507F54C(Actor *actor) {
    s32 slot;
    s32 anim;

    slot = actor->lifting;
    if (slot >= 100) {
        switch (movement_obj[slot - 100].character_type) {
        case 0x57:
            anim = 0x115;
            break;
        case 0x8C:
            anim = 0x1A5;
            break;
        case 0xA8:
        case 0xA9:
            anim = 0x1AF;
            break;
        case 0x89:
        case 0xBA:
            anim = 0x1FA;
            break;
        default:
            anim = 0xD8;
            break;
        }
        actor_playAnim(actor, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        return;
    }

    actor_playAnim(actor, 0xF, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

/* Per-frame idle animation update for the current actor (gCurrentActor). */
void func_1507F640(void) {
    s32 type;
    const IdleFrame *entry;
    IdleState *idle;
    ActorCtl *owner;
    f32 speed;
    f32 blend;
    s32 anim;
    s32 override;
    s32 frame;
    s32 sequence;

    speed = 1.0f;
    blend = 4.0f;
    if (D_800C35EA[0] == 1) {
        return;
    }
    if (gCurrentActor->drawtype == 4) {
        return;
    }
    if (dc_glbl_CharNum >= D_8008FD8C) {
        return;
    }
    idle = &gCurrentActor->mv->idle;
    idle->active = TRUE;
    if (gCurrentActor->unk2F8 & 0x20) {
        return;
    }
    if (gCurrentActor->mv->death_State) {
        return;
    }
    if (idle->holdFrames > 0) {
        idle->holdFrames--;
        idle->timer = 0;
        return;
    }
    if (gCurrentActor->energy == 0) {
        actor_kill(gCurrentActor, 0);
        return;
    }
    anim = 0x4F;
    if (gCurrentActor->norun >= 250) {
        gCurrentActor->norun = 0;
    }
    if (gCurrentActor->nojump == 0xFF) {
        gCurrentActor->nojump = 0;
    }
    gCurrentActor->count5 = 0;
    gCurrentActor->unkAA = 0;
    gCurrentActor->angle = gCurrentActor->langle;
    gCurrentActor->unk100 &= 0xFE;
    gCurrentActor->unk2F8 &= ~7;
    gCurrentActor->unk2F8 |= 0; // matching: a second store, as from setting a 3-bit field to 0
    owner = gCurrentActor->mv;
    if (owner->env_suit) {
        s32 pad; // matching: unused stack slot
        if (func_150ADA20_u8() & 3) {
            anim = 0x4D;
        }
        actor_playAnim(gCurrentActor, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        return;
    }
    type = owner->rt_context_b;
    override = -1;
    switch (type) {
    case 2:
    case 19:
        override = 0x3E;
        break;
    case 56:
        override = 0x1DF;
        break;
    case 57:
        override = 0x1EA;
        break;
    case 55:
        override = 0x1F3;
        break;
    case 59:
        override = 0x221;
        break;
    case 38:
        override = 0x177;
        break;
    case 58:
        override = 0x1FA;
        break;
    case 37:
        override = 0xD8;
        break;
    case 45:
    case 46:
        override = 0x1AF;
        break;
    case 21:
        override = 0xE2;
        break;
    case 12:
        override = 0xBD;
        break;
    case 20:
    case 63:
        override = 0xF9;
        break;
    case 34:
        override = 0x15A;
        break;
    case 33:
        override = 0x122;
        break;
    case 25:
    case 64:
        override = 0x10F;
        break;
    case 18:
        override = 0x10A;
        break;
    case 24:
    case 65:
        override = 0xC5;
        break;
    case 36:
        override = 0x153;
        break;
    }
    if (owner->unk1B2 && type == 22) {
        override = 0x2C5;
    }
    if (owner->transport == 2) {
        override = 5;
    }
    if (owner->pissed_up) {
        blend = 8.0f;
        speed = 1.5f;
        override = 0xAB;
    }
    if (owner->injured) {
        override = 0x63;
        blend = 8.0f;
    }
    if (owner->walk_attrib & 2) {
        override = 0x6B;
        blend = 8.0f;
    } else if (owner->walk_attrib & 1) {
        override = 0xAF;
        blend = 8.0f;
    }
    if (override != -1) {
        actor_playAnim(gCurrentActor, override, speed, blend, 0.0f, 0.0f, 0);
        return;
    }
    if (gCurrentActor->lifting >= 100) {
        func_1507F54C(gCurrentActor);
        return;
    }
    if (gIsMultiplayer && type != 35 && type != 22 && type != 9) {
        actor_playAnim(gCurrentActor, 0xF, 1.0f, 3.0f, 0.0f, 0.0f, 0);
        return;
    }
    if (func_1507F454()) {
        owner = gCurrentActor->mv;
        if (owner->AimState && idle->sequence == 0) {
            switch (owner->rt_context_b) {
            case 3:
                override = 0x2C;
                break;
            case 22:
                override = 0xBD;
                break;
            case 9:
                override = 0x7F;
                break;
            case 35:
                override = 0x165;
                break;
            default:
                func_1507F54C(gCurrentActor);
                return;
            }
            if (override != -1) { // matching: always true here, but the compare is in the asm
                actor_playAnim(gCurrentActor, override, 1.0f, 4.0f, 0.0f, 0.0f, 0);
                return;
            }
        }
        func_1507EFD0();
    }
    sequence = idle->sequence;
    frame = idle->frame;
    override = D_80086BA0[sequence][frame];
    entry = &D_8009B8B0[override];
    if (override == 0) {
        owner = gCurrentActor->mv;
        sequence = -1;
        switch (owner->rt_context_b) {
        case 9:
            sequence = 0x3A;
            break;
        case 35:
            sequence = 0x42;
            break;
        case 22:
            sequence = 0x44;
            break;
        default:
            if (gCurrentActor->energy == 1) {
                sequence = 0xE;
            } else if (owner->injured) {
                sequence = 0x25;
            }
            break;
        }
        if (sequence != -1) {
            entry = &D_8009B8B0[sequence];
        }
    }
    actor_playAnim(gCurrentActor, entry->anim, entry->speed * 0.01f, entry->blend * 0.1f, 0.0f,
                  0.0f, 0);
    idle->holdFrames = entry->holdFrames;
    idle->active = TRUE;
}

/* Applies the side effects of the actor's current idle frame flags, on their rising/falling edges. */
void func_1507FC2C(Actor *actor) {
    AnimInstance *sound;
    f32 intensity;
    s32 flags;
    s32 prev;
    IdleState *idle;
    s32 frame;
    const IdleFrame *entry;
    s32 changed;
    u8 sequence;

    idle = &actor->mv->idle;
    if (idle->sequence != 0) {
        sequence = idle->sequence;
        frame = idle->frame;
        entry = &D_8009B8B0[D_80086BA0[sequence][frame]];
        flags = entry->flags;
    } else {
        flags = 0;
    }

    if (flags & 0x10) {
        intensity = gMusicEventLevels[gLastMarkerChannel][0];
        if (intensity >= 0.0f) {
            sound = actor->unk2D0;
            sound->frame[0] = (sound->numFrames[0] * (32768.0f - intensity)) / 32768.0f;
        }
    }

    prev = idle->frameFlags;
    if (flags != prev) {
        changed = flags ^ prev;

        if ((changed & flags & 1) == 1) {
            actor->unk2F8 |= 1;
        } else if ((changed & prev & 1) == 1) {
            actor->unk2F8 &= ~1;
        }

        if ((changed & flags & 2) == 2) {
            func_1000E7A0(1, 0);
        } else if ((changed & prev & 2) == 2) {
            func_1000E8C4(1);
        }

        if ((changed & flags & 4) == 4) {
            if (D_800D18C4 == 0) {
                D_800D18C4 = 0x15;
            } else if (D_800D18C4++ == 0x17) {
                D_800D18C4 = 0x15;
            }
            music_playTune(D_800D18C4, 0, 0);
        } else if ((changed & prev & 4) == 4) {
            music_stopTune(D_800D18C4, 0);
        }

        if ((changed & flags & 8) != 8 && (changed & prev & 8) == 8) {
            sound_stopActorSound2(actor);
        }

        idle->frameFlags = flags;
    }
}

/* Per-frame idle bookkeeping for an actor: counts down unk13A, advances the idle timer while
 * idle (resetting the state otherwise), runs the delay, then applies the frame flags. */
void func_1507FEA0(Actor *actor) {
    IdleState *idle;
    u8 countdown;
    u16 timer;
    s16 delay;
    s32 limit;

    if (actor->mv == NULL || actor->playernumber == 0xFF) {
        return;
    }
    countdown = actor->unk13A;
    idle = &actor->mv->idle;
    if (countdown != 0) {
        actor->unk13A = countdown - 1;
    }
    if (idle->active == TRUE && D_800C35EA[0] != 1) {
        limit = 0xFFFF - D_800BE9E4;
        timer = idle->timer;
        if (timer < limit) {
            idle->timer = timer + D_800BE9E4;
        }
    } else {
        idle->timer = 0;
        idle->sequence = 0;
        idle->frame = 0;
        idle->phase = 0;
        idle->phaseCount = 0;
    }
    delay = actor->mv->idle.delay; // matching: re-read through actor, not idle
    if (delay != 0) {
        if (D_800BE9E4 < delay) {
            actor->mv->idle.delay = delay - D_800BE9E4;
        } else {
            func_1507FF94(actor);
            actor->mv->idle.delay = 0;
        }
    }
    func_1507FC2C(actor);
}

/* Sends event 0xD about actor (pointer plus unique id) to both event handlers. */
void func_1507FF94(Actor *actor) {
    ActorRef event;

    event.actor = actor;
    event.uniqueId = actor->unique_id;
    particle_sendDripMsg((s32)&event, 0xD);
    effect_sendEventToTimedEffects((s32)&event, 0xD);
}

/* A node of the gObjectLists per-type object lists. */
typedef struct ListObject {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ struct ListObject *next;
    /* 0x0C */ u8 padC[0x8C];
    /* 0x98 */ f32 x;
    /* 0x9C */ f32 y;
    /* 0xA0 */ f32 z;
} ListObject;

typedef struct {
    s32 type[3];
} ObjTypeList;

/* The object types that count as watchers. */
static ObjTypeList D_80086C50 = { { 0x15, 0x16, 0x17 } };

/*
 * Returns 1 when the actor stands in a trigger zone (type 4, subtype 5 or 11) and one of the
 * objects of the three D_80086C50 types is within 1000 units and inside its 90-degree view.
 */
s32 func_1507FFD8(Actor *actor) {
    TriggerZone *inZone;
    s32 i;
    ListObject *obj;
    ObjTypeList types;
    f32 x;
    f32 y;
    f32 z;
    f32 distSq;
    f32 out[3];
    f32 pad[3]; // matching: unused stack space (frame size)
    s32 type;

    inZone = zone_findAt(4, 5, actor->xpos, actor->ypos, actor->zpos);
    if (!inZone) {
        inZone = zone_findAt(4, 11, actor->xpos, actor->ypos, actor->zpos);
    }
    if (inZone) {
        types = D_80086C50;
        for (i = 0; i < 3; i++) {
            type = types.type[i];
            obj = (ListObject *)gObjectLists[0][type]; // the lists are indexed flat: type runs over both rows
            while (obj != NULL) {
                x = obj->x;
                y = obj->y;
                z = obj->z;
                distSq = (x - actor->xpos) * (x - actor->xpos) +
                         (y - actor->ypos) * (y - actor->ypos) +
                         (z - actor->zpos) * (z - actor->zpos);
                if (distSq < 1000000.0f &&
                    func_15037698(actor - movement_obj, 0, 0, x, y, z, out, 90.0f, 0, 0) != 0) {
                    return TRUE;
                }
                obj = obj->next;
            }
        }
    }
    return FALSE;
}
