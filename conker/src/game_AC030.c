#include <ultra64.h>
#include "functions.h"
#include "variables.h"


extern u8 D_800B85A4[];

void func_1507EB80(u8 *buf, s32 *count, u8 c) {
    if (*count + 1 < 40) {
        buf[*count] = c;
        *count = *count + 1;
    }
}

extern u8 *D_80086C24[];
extern u8 D_8009BBF0[];

typedef struct {
    char pad_0[0x3B];
    u8 field_0x3B;
} ActorUniqueIdFields;

void func_1507EBB8(u8 *arg0, s32 *arg1, s32 arg2) {
    u8 *dst = D_80086C24[arg2];
    s32 len = D_8009BBF0[arg2];
    if (*arg1 + len < 40) {
        bcopy(dst, arg0 + *arg1, len);
        *arg1 += len;
    }
}

s32 func_1507EC38(u8 *src, s32 srcLen, u8 *out, s32 *outLen, u8 *used) {
    s32 ret;
    s32 i;
    s32 j;
    s32 found;
    s32 num;
    u8 buf[5];
    s32 n;

    ret = 0;
    *outLen = 0;
    for (i = 0; i < srcLen; i++) {
        found = 0;
        for (j = 0; j < *outLen; j++) {
            if (out[j] == src[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            for (j = 0; j < 5; j++) {
                if (used[j] == src[i]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                out[*outLen] = src[i];
                *outLen = *outLen + 1;
            }
        }
    }

    if (*outLen == 0) {
        num = 0;
        for (i = 0; i < 5; i++) {
            if (used[i] != 0) {
                for (j = 0; j < srcLen; j++) {
                    if (used[i] == src[j]) {
                        buf[num] = used[i];
                        num++;
                        break;
                    }
                }
            }
        }
        if (num == 0) {
            out[0] = 0;
            *outLen = 1;
        } else {
            n = num >> 1;
            if (n == 0) {
                n = 1;
            }
            while (n != 0) {
                num--;
                out[*outLen] = buf[num];
                *outLen = *outLen + 1;
                n--;
            }
            ret = 1;
        }
    }
    return ret;
}

void func_1507EEB8(u8 arg0, u8 *arg1);

void func_1507EE58(s32 arg0, void *arg1) {
    func_1507EEB8(*((u8*)&arg0 + 3), arg1);
    if (*((u8*)&arg0 + 3) == 0x11) {
        func_1507EEB8(0x12, arg1);
    } else if (*((u8*)&arg0 + 3) == 0x12) {
        func_1507EEB8(0x11, arg1);
    }
}

void func_1507EEB8(u8 arg0, u8 *arg1) {
    s32 i;

    for (i = 4; i > 0; i--) {
        arg1[i] = arg1[i - 1];
    }
    arg1[0] = arg0;
}

void func_1507EEF4(void) {
    u8 *state;
    s32 value;

    state = (u8 *)D_800D154C->unk31C;
    value = state[0x64];
    state += 0x58;

    if (value == 0) {
        state[0xC] = 1;
        state[0xD] = 0;
        return;
    }

    if (value == 1) {
        func_150ADA20();
        if (state[0xD] >= 3) {
            state[0xC] = 2;
            state[0xD] = 0;
        }
        return;
    }

    if (state[0xD] >= ((func_150ADA20() & 3) + 8)) {
        state[0xC] = 1;
        state[0xD] = 0;
    }
}

void func_1507EFA0(s32 c, u8 *arg1) {
    s32 i = 4;
    u8 *p = arg1 + 4;
    do {
        if (*p == c) {
            *p = 0;
            return;
        }
        i--;
        p--;
    } while (i >= 0);
}

typedef struct {
    u8 pad0[2];
    u16 timer;
    u8 cur;
    u8 unk5;
    u8 pad6;
    u8 used[5];
    u8 mode;
    u8 count;
} SpeechState;

s32 func_1507FFD8(struct127 *);
s32 func_1507F4C0(s32 arg0);
void func_1507FF94(ActorUniqueIdFields *arg0);
s32 func_1515942C(struct127 *, s32, s32, s32, s32, s32);

void func_1507EFD0(void) {
    SpeechState *state;
    u8 buf[40];
    u8 out[40];
    s32 count;
    s32 outLen;
    s32 idx;
    u8 *used;
    s32 sel;
    s32 ret;
    s32 v;

    state = (SpeechState *)&D_800D154C->unk31C->unk58;
    count = 0;
    if (D_800D154C->pad139[1] != 0) {
        if (D_800D154C->pad139[0] < 0x21) {
            func_1507EB80(buf, &count, D_800D154C->pad139[0]);
        }
    } else if (D_800D18C0 & 2) {
        func_1507EB80(buf, &count, 3);
        D_800D18C0 = 0;
    } else {
        if ((s16)D_800D154C->unk31C->unk66 != 0 && D_800D154C->unk31C->unk78 == 0) {
            v = 0xB;
            if (state->timer >= 0x79) {
                D_800D154C->unk31C->unk66 = 0;
                state->timer = 0;
                state->mode = 0;
                v = 0xC;
                func_1507FF94((ActorUniqueIdFields *)D_800D154C);
            } else {
                v = 0xB;
            }
            func_1507EB80(buf, &count, v);
        } else if (D_800D154C->health < 2) {
            func_1507EB80(buf, &count, 0);
        } else if (func_1515942C(&D_800CC2D0[D_800C3E78], 0, 0, -1, -1, -1) != 0) {
            func_1507EB80(buf, &count, 0);
        } else if (state->timer >= func_1507F4C0(state->mode)) {
            func_1507EEF4();
            sel = -1;
            if (D_800D154C->unk2F8 & 0x200) {
                sel = 2;
            } else if (D_800D154C->unk31C->unk78 == 9) {
                sel = 5;
            } else if (D_800D154C->unk31C->unk78 == 0x23) {
                sel = 9;
            } else if (D_800D154C->unk31C->unk78 == 0x16) {
                sel = 0xA;
            } else if (func_1503195C(D_800D154C, 0x17, 0) != NULL) {
                sel = 6;
            } else if (D_800BE9F0 == 0x31) {
                if (D_800D18C0 & 4) {
                    sel = 8;
                } else {
                    sel = 7;
                }
            } else if (D_800D154C->id == 0x96 || D_800BE9F0 == 0x10 || D_800BE9F0 == 0x1B ||
                       D_800BE9F0 == 0x1E || D_800BE9F0 == 0x26 || D_800BE9F0 == 0x2F ||
                       D_800BE9F0 == 0xE || D_800BE9F0 == 0x28 || D_800BE9F0 == 0x23 ||
                       D_800BE9F0 == 0x1C || D_800BE9F0 == 0x36 || D_800BE9F0 == 0x3A) {
                sel = 2;
            } else if (D_800D18C0 != 0) {
                if (D_800D18C0 & 1) {
                    sel = 4;
                }
                D_800D18C0 = 0;
            }
            if (sel != -1) {
                count = 0;
                func_1507EBB8(buf, &count, sel);
            } else if (state->mode == 1) {
                func_1507EBB8(buf, &count, 0);
            } else {
                func_1507EBB8(buf, &count, 1);
                if (state->mode == 2 && state->count >= 4 && func_1507FFD8(D_800D154C) != 0) {
                    func_1507EBB8(buf, &count, 3);
                }
            }
        } else {
            func_1507EB80(buf, &count, 0);
        }
    }
    used = state->used;
    ret = func_1507EC38(buf, count, out, &outLen, used);
    if (outLen >= 2) {
        idx = func_150ADA20() % (u32)outLen;
    } else {
        idx = 0;
    }
    state->cur = out[idx];
    state->unk5 = 0;
    state->count++;
    if (state->mode != 0) {
        state->timer = 0;
    }
    if (ret) {
        func_1507EFA0(state->cur, used);
    }
    func_1507EE58(state->cur, used);
}

extern u8 *D_80086BA0[];

s32 func_1507F454(void) {
    u8 *state;
    s32 value;
    s32 frame;

    state = (u8 *)D_800D154C->unk31C;
    value = state[0x5C];
    state += 0x58;

    if (value == 0) {
        return 1;
    }
    state[5]++;
    frame = *(state[5] + D_80086BA0[value]);
    if (frame == 0) {
        state[4] = 0;
        state[5] = 0;
        return 1;
    }
    return 0;
}


s32 func_1507F4C0(s32 arg0) {
    s32 pad;
    s32 base;
    u32 mod;

    if (arg0 == 0) {
        base = 0xB4;
        mod = 0x3C;
    } else if (D_800BE9F0 == 0x31) {
        return 0;
    } else if (arg0 == 1) {
        base = 0x3C;
        mod = 0x3C;
    } else {
        base = 0;
        mod = 0x1E;
    }

    return func_150ADA20() % mod + base;
}

void func_1507F54C(struct127 *arg0) {
    s32 idx;
    s32 anim;

    idx = arg0->unk13C;
    if (idx >= 0x64) {
        switch (D_800B85A4[idx * 0x32C]) {
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
        func_1505E650(arg0, anim, 1.0f, 4.0f, 0.0f, 0.0f, 0);
        return;
    }

    func_1505E650(arg0, 0xF, 1.0f, 3.0f, 0.0f, 0.0f, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_AC030/func_1507F640.s")

extern u8 *D_80086BA0[];
extern s32 D_800D18C4;

typedef struct {
    u8 pad0[0x6];
    u8 unk6;
    u8 unk7;
} MoveFlags;

extern MoveFlags D_8009B8B0[];

/* D_800418B0 is really an array of 0x40-byte records; variables.h types it as s32[]. */
typedef struct {
    s32 unk0;
    u8 pad4[0x3C];
} MoveTimer;

void func_1000D96C(s32 arg0, s32 arg1, s32 arg2);
void func_1000DE1C(s32 arg0, s32 arg1);
void func_1000E7A0(u32 arg0, s32 arg1);
void func_1000E8C4(s32 arg0);

void func_1507FC2C(struct127 *arg0) {
    struct197 *sound;
    f32 timer;
    s32 flags;
    s32 prev;
    u8 *state;
    s32 pose;
    MoveFlags *info;
    s32 changed;
    u8 anim;

    state = (u8 *)arg0->unk31C + 0x58;
    if (state[4] != 0) {
        anim = state[4];
        pose = state[5];
        info = &D_8009B8B0[D_80086BA0[anim][pose]];
        flags = info->unk6;
    } else {
        flags = 0;
    }

    if (flags & 0x10) {
        timer = ((MoveTimer *)D_800418B0)[D_800419A0].unk0;
        if (timer >= 0.0f) {
            sound = arg0->unk2D0;
            sound->unk8 = (sound->unk18 * (32768.0f - timer)) / 32768.0f;
        }
    }

    prev = state[6];
    if (flags != prev) {
        changed = flags ^ prev;

        if ((changed & flags & 1) == 1) {
            arg0->unk2F8 |= 1;
        } else if ((changed & prev & 1) == 1) {
            arg0->unk2F8 &= ~1;
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
            func_1000D96C(D_800D18C4, 0, 0);
        } else if ((changed & prev & 4) == 4) {
            func_1000DE1C(D_800D18C4, 0);
        }

        if (((changed & flags & 8) != 8) && ((changed & prev & 8) == 8)) {
            func_100109D0(arg0);
        }

        state[6] = flags;
    }
}

typedef struct GameAC030AnimationState {
    u8 mode;
    u8 pad1;
    u16 timer;
    u8 sequence;
    u8 frame;
    u8 pad6[6];
    u8 field_C;
    u8 field_D;
    s16 delay;
} GameAC030AnimationState;

typedef struct GameAC030NestedState {
    u8 pad0[0x58];
    GameAC030AnimationState animation;
} GameAC030NestedState;

typedef struct GameAC030RootState {
    u8 pad0[0x127];
    u8 field_127;
    u8 pad128[0x12];
    u8 cooldown;
    u8 pad13B[0x1E1];
    GameAC030NestedState *nested;
} GameAC030RootState;

void func_1507FEA0(GameAC030RootState *arg0) {
    GameAC030AnimationState *anim;
    u8 cooldown;
    u16 timer;
    s16 delay;
    s32 limit;

    if ((arg0->nested == NULL) || (arg0->field_127 == 0xFF)) {
        return;
    }
    cooldown = arg0->cooldown;
    anim = &arg0->nested->animation;
    if (cooldown != 0) {
        arg0->cooldown = cooldown - 1;
    }
    if ((anim->mode == 1) && (D_800C35EA != 1)) {
        limit = 0xFFFF - D_800BE9E4;
        timer = anim->timer;
        if (timer < limit) {
            anim->timer = timer + D_800BE9E4;
        }
    } else {
        anim->timer = 0;
        anim->sequence = 0;
        anim->frame = 0;
        anim->field_C = 0;
        anim->field_D = 0;
    }
    delay = arg0->nested->animation.delay;
    if (delay != 0) {
        if (D_800BE9E4 < delay) {
            arg0->nested->animation.delay = delay - D_800BE9E4;
        } else {
            func_1507FF94((ActorUniqueIdFields *)arg0);
            arg0->nested->animation.delay = 0;
        }
    }
    func_1507FC2C((struct127 *)arg0);
}

void func_15191B8C(s32 arg0, u8 arg1);

void func_1507FF94(ActorUniqueIdFields *arg0) {
    struct {
        s32 unk0;
        u8 unk4;
    } sp20;

    sp20.unk0 = (s32)arg0;
    sp20.unk4 = arg0->field_0x3B;
    func_15191B8C((s32)&sp20, 0xD);
    func_151494E0((s32)&sp20, 0xD);
}

extern s32 func_15037698(s32 arg0, s32 arg1, s32 arg2, f32 x, f32 y, f32 z, f32 *out, f32 range, s32 arg8, s32 arg9);
extern s32 func_150A3194(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void *D_800DCE50[];
typedef struct { s32 v[3]; } Types1507FFD8;
extern Types1507FFD8 D_80086C50;

s32 func_1507FFD8(struct127 *arg0) {
    s32 found;
    s32 i;
    struct127 *obj;
    Types1507FFD8 types;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 out[3];
    f32 dx;
    f32 dy;
    f32 dz;
    s32 type;

    found = func_150A3194(4, 5, (s32) arg0->x_position, (s32) arg0->y_position, (s32) arg0->z_position);
    if (found == 0) {
        found = func_150A3194(4, 0xB, (s32) arg0->x_position, (s32) arg0->y_position, (s32) arg0->z_position);
    }
    if (found != 0) {
        types = D_80086C50;
        for (i = 0; i < 3; i++) {
            type = types.v[i];
            obj = D_800DCE50[type];
            while (obj != NULL) {
                x = *(f32 *)((u8 *)obj + 0x98);
                y = *(f32 *)((u8 *)obj + 0x9C);
                z = *(f32 *)((u8 *)obj + 0xA0);
                dist = (x - arg0->x_position) * (x - arg0->x_position) + (y - arg0->y_position) * (y - arg0->y_position) + (z - arg0->z_position) * (z - arg0->z_position);
                if (dist < 1000000.0f &&
                    func_15037698(arg0 - D_800CC2D0, 0, 0, x, y, z, out, 90.0f, 0, 0) != 0) {
                    return 1;
                }
                obj = *(struct127 **)((u8 *)obj + 8);
            }
        }
    }
    return 0;
}
