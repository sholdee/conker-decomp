#include <ultra64.h>
/* functions.h declares func_150A29C8(u8, u16); golden reads both args as full words. */
#define func_150A29C8 func_150A29C8_hdr
#include "functions.h"
#undef func_150A29C8
/* variables.h gets these types wrong, so they are shadowed here: D_800D2138 is a struct, D_800D2340 (s16)
   a u16 bit mask, and D_800D3098 (`struct178 D_800D3098[73]`) and D_800D3094 (s32) hold a POINTER to the
   trigger-zone table and its (unsigned) length. */
#define D_800D2138 D_800D2138_s32_decl_in_variables_h
#define D_800D2340 D_800D2340_s16_decl_in_variables_h
#define D_800D3094 D_800D3094_s32_decl_in_variables_h
#define D_800D3098 D_800D3098_array_decl_in_variables_h
#include "variables.h"
#undef D_800D2138
#undef D_800D2340
#undef D_800D3094
#undef D_800D3098

/* One trigger zone (struct178 in structs.h, which has everything past 0x6 as a blob). */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ s16 height;
    /* 0x0A */ s16 depth;
    /* 0x0C */ f32 pitch; /* degrees */
    /* 0x10 */ f32 yaw;   /* degrees */
    /* 0x14 */ u8  disabled;
    /* 0x15 */ u8  kind;  /* (type << 2) | shape */
    /* 0x16 */ u8  flags; /* bit 0: only triggers near the ground */
    /* 0x17 */ u8  subtype;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ f32 sinPitch;
    /* 0x28 */ f32 cosPitch;
    /* 0x2C */ f32 sinYaw;
    /* 0x30 */ f32 cosYaw;
} TriggerZone; /* size 0x34 */

#define ZONE_TYPE(zone)  ((s32)(zone)->kind >> 2)
#define ZONE_SHAPE(zone) ((zone)->kind & 3)

typedef struct {
    /* 0x000 */ u8  pad0[0x208];
    /* 0x208 */ u16 unk208; /* = D_800D2340 */
} Struct800D2138;

extern Struct800D2138 D_800D2138;
extern u16 D_800D2340;          /* bit n: actor n's occupancy bits in D_800D30F0 are current */
extern u32 D_800D3094;          /* number of trigger zones */
extern TriggerZone *D_800D3098; /* trigger-zone table */

/* Partial view of Camera (the camera record) covering the fields this file writes. */
typedef struct {
    /* 0x000 */ u8  pad0[0x700];
    /* 0x700 */ s16 unk700;
    /* 0x702 */ u8  pad702[0x236];
    /* 0x938 */ f32 unk938;
    /* 0x93C */ f32 unk93C;
    /* 0x940 */ f32 unk940;
    /* 0x944 */ f32 unk944;
    /* 0x948 */ u8  unk948;
    /* 0x949 */ u8  unk949;
    /* 0x94A */ u8  pad94A[0x56];
} CameraZoneView; /* size 0x9A0, as Camera */

void func_1000FC18(u16 sound, s16 x, s16 y, s16 z, s32 farDistance); /* stops a positional sound */
s32 func_150A1DA0(Actor *actor, TriggerZone *zone, s32 withMargin);
void func_150A23E4(TriggerZone *zone);

/* Per-frame update of the zone state that does not depend on an actor (ticks is unused). */
void func_150A09D0(s32 ticks) {
    u32 countdown;
    s32 i;

    for (i = 0; i < D_800D3094; i++) {
        switch (ZONE_TYPE(&D_800D3098[i])) {
        case 2: /* sound zone: unk20 is 1 while occupied (func_150A23E4 sets it each time); a zone left
                   unoccupied for a whole update gets its looping sound stopped */
            if (D_800D3098[i].unk20 == 1) {
                D_800D3098[i].unk20 ^= 3;
            } else if (D_800D3098[i].unk20 == 2) {
                func_1000FC18(D_800D3098[i].unk1C >> 16,
                              D_800D3098[i].x,
                              D_800D3098[i].y,
                              D_800D3098[i].z,
                              D_800D3098[i].unk18);
                D_800D3098[i].unk20 = 0;
            }
            if (D_800C35EA == 1) {
                if (func_150A1DA0(D_800CC2D0, &D_800D3098[i], 0) == 0) {
                    func_150A23E4(&D_800D3098[i]);
                }
            }
            break;
        case 7: /* music-channel zone (func_1000E46C sets channel volumes) */
            if (ZONE_SHAPE(&D_800D3098[i]) == 3) {
                D_800D3098[i].unk20 = 0;
            } else {
                if (D_800D3098[i].unk20 == 1) {
                    D_800D3098[i].unk20 = 2;
                } else if (D_800D3098[i].unk20 == 2) {
                    if (func_1000E46C(D_800D3098[i].subtype, 0, D_800D3098[i].unk18, 0)) {
                        if (func_1000E46C(D_800D3098[i].subtype, 100, D_800D3098[i].unk1C, 0)) {
                            D_800D3098[i].unk20 = 0;
                        }
                    }
                }
            }
            break;
        case 4:
            if (D_800D3098[i].subtype != 9) {
                /* count down the respawn delay of the object func_150A24C0 spawned */
                countdown = D_800D3098[i].unk1C & 0xFF;
                if (countdown != 0) {
                    if (!D_800D3098[i].disabled) {
                        countdown--;
                    } else {
                        countdown = 0;
                    }
                    D_800D3098[i].unk1C = (D_800D3098[i].unk1C & ~0xFF) | countdown;
                    if (countdown == 0) {
                        countdown = D_800D3098[i].unk1C;
                        D_800DDF5C[(countdown >> 8) & 0xFF] = 0;
                        D_800D3098[i].unk1C &= 0xFFFF0000;
                    }
                }
            }
            /* fallthrough */
        case 0:
            if (D_800D3098[i].subtype == 12) {
                if (!D_800D3098[i].disabled) {
                    s32 j;

                    for (j = 0; j <= D_80082FA0; j++) {
                        CameraZoneView *cam = (CameraZoneView *)&D_800DBFF0[j];

                        cam->unk93C = D_800D3098[i].x;
                        cam->unk940 = D_800D3098[i].y;
                        cam->unk944 = D_800D3098[i].z;
                        cam->unk938 = D_800D3098[i].radius;
                        cam->unk949 = D_800D3098[i].unk18;
                        cam->unk948 = 1;
                    }
                }
            }
            break;
        }
    }
}

/* Caches the sine and cosine of the zone's rotation angles. */
void func_150A0D14(TriggerZone *zone) {
    f32 angle;

    angle = zone->pitch * 0.01745329238f; /* degrees to radians */
    zone->sinPitch = func_150AD78C(angle);
    zone->cosPitch = func_150AD780(angle);
    angle = zone->yaw * 0.01745329238f;
    zone->sinYaw = func_150AD78C(angle);
    zone->cosYaw = func_150AD780(angle);
}

void func_15035808(s32, s32, f32, f32, f32, f32, f32, f32, f32, f32);
s32 func_150AD9A0(s16, s16, s16);

/* Starts the type-6 zone effect (func_15035808) for every active actor inside such a zone. */
void func_150A0D8C(void) {
    u32 i;
    s32 j;
    Actor *obj;
    s16 dy;
    f32 fade;

    for (i = 0; i < D_800D3094; i++) {
        if (!D_800D3098[i].disabled && ZONE_TYPE(&D_800D3098[i]) == 6) {
            for (j = 0; j < 25; j++) {
                obj = &D_800CC2D0[j];
                if (obj->interaction_state == 0) continue;
                if (obj->unk5 != 0 && obj->unk5 != 1) continue;
                if (func_150A1DA0(obj, &D_800D3098[i], 1) != 0) continue;

                dy = obj->y_position - D_800D3098[i].y;
                if (D_800D3098[i].subtype == 0) {
                    if (func_150AD9A0((s16)(obj->x_position - D_800D3098[i].x), dy,
                                      (s16)(obj->z_position - D_800D3098[i].z)) < D_800D3098[i].radius) {
                        fade = D_800D3098[i].radius * 0.2f;
                        if (fade > 100.0f) {
                            fade = 100.0f;
                        }
                        func_15035808(2, j, D_800D3098[i].x, D_800D3098[i].y, D_800D3098[i].z, 0.0f,
                                      D_800D3098[i].yaw, D_800D3098[i].radius - fade, D_800D3098[i].radius,
                                      255.0f);
                    }
                } else {
                    func_15035808(1, j, obj->x_position, obj->unk180, obj->z_position,
                                  0.0f, 0.0f, 100.0f, 150.0f, 255.0f);
                }
            }
        }
    }
}

/* Offsets a value by 0x400; used to hide the cartridge-check address and value in func_150A11C4. */
s32 func_150A1040(s32 value) {
    return value + 0x400;
}

/* Tests the camera position against the given zones and applies the type-1 (camera) ones. */
void func_150A104C(Camera *cam, TriggerZone **zones, s32 count) {
    Actor probe;

    probe.x_position = cam->eye.x;
    probe.y_position = cam->eye.y;
    probe.z_position = cam->eye.z;
    probe.unk180 = probe.y_position;
    probe.old_x_position = cam->prevEye.x;
    probe.old_y_position = cam->prevEye.y;
    probe.old_z_position = cam->prevEye.z;
    cam->unk364 = -10000.0f;
    cam->flags &= ~0x40000;
    while (count != 0) {
        if (!zones) { // matching: this empty test steers IDO's callee-saved register order (mask in s3)
        }
        if (*zones != NULL && func_150A1DA0(&probe, *zones, 0) == 0) {
            if (ZONE_TYPE(*zones) == 1) {
                switch ((*zones)->subtype) {
                case 2:
                    cam->unk364 = (*zones)->y + (*zones)->height;
                    break;
                case 0:
                    cam->flags |= 0x40000;
                    break;
                case 6:
                    cam->flags &= ~0x40000;
                    break;
                }
            }
        }
        count--;
        zones++;
    }
}

/* Bounding box of zone i (D_800D309C[i]) plus the set of zones that overlap it. */
typedef struct {
    /* 0x00 */ u64 mask[3]; /* bit n: zone n overlaps this box */
    /* 0x18 */ u8  unk18;
    /* 0x19 */ u8  pad19;
    /* 0x1A */ s16 xmin;
    /* 0x1C */ s16 xmax;
    /* 0x1E */ s16 ymin;
    /* 0x20 */ s16 ymax;
    /* 0x22 */ s16 zmin;
    /* 0x24 */ s16 zmax;
    /* 0x26 */ u8  pad26[2];
} ZoneBounds; /* size 0x28 */

#define BOX_CONTAINS(b, px, py, pz) ((b)->xmin < (px) && (px) < (b)->xmax && (b)->ymin < (py) && \
                                     (py) < (b)->ymax && (b)->zmin < (pz) && (pz) < (b)->zmax)

extern TriggerZone **D_800D3270;
extern s32 D_800D3274;
extern s32 D_800D3278;
extern ZoneBounds *D_800D309C;
extern u64 D_800D30A0[3];     /* zones that are always tested */
extern u64 D_800D30B8[3];     /* default candidate zones */
extern u16 D_800D30D0[16];    /* per actor: 1 + index of the last box it was found in, 0 for none */
extern u64 D_800D30F0[16][3]; /* per actor: zones it was inside last time */
extern u8 D_800DF0E0;
extern u8 D_800DF0E1;

void func_150806A8(s32);
void func_150829D8(Actor *);
void func_1512D560(Camera *, s32, TriggerZone *);
void func_15055C88(Actor *arg0);
void func_1000D96C(s32 newId, s32 oldId, s32 transition);
void func_15080500(Actor *, TriggerZone *, s32, s32, s32);
void func_151AE06C(Actor *, TriggerZone *);
void func_1501D348(s32 map, s32 cutscene, s32 scene, s32 callback, s32 callbackArg);
s32 __osPiGetAccess(void);
void __osPiRelAccess(void);
s32 osPiRawReadIo(u32, u32 *);
void func_150A24C0(TriggerZone *zone, s32 unk127);
void func_15172E7C(Actor *actor, s32 arg1, s32 arg2);
void func_150A25D4(TriggerZone *zone, Actor *actor, s32 unk127, s64 prevBits);

/* Finds the zones that contain the actor (narrowed through the zone bounding boxes) and applies them. */
void func_150A11C4(Actor *actor) {
    u32 i;
    u32 base;
    u64 *word;
    u64 *prev; // matching: also holds the actor's y (s16) until the box scan (one variable, one register)
    u32 bestUnk18;
    s32 unk127;
    s32 x;
    s32 y; // matching: unused (y lives in prev) but it shifts the -g3 stack frame
    s16 z;
    Camera *cam;
    ZoneBounds *box;
    u64 mask[3];
    s32 found;
    s32 k;
    u64 bits;
    u64 prevBits;
    s32 idx;

    bestUnk18 = 1000;
    unk127 = actor->unk127;
    cam = actor->camera;
    if (cam == NULL) {
        unk127 = 0xFF;
    } else if (D_800D3274 != 0) {
        func_150A104C(cam, D_800D3270, D_800D3274);
    }
    x = (s16)actor->x_position;
    prev = (u64 *)(s16)actor->y_position;
    z = actor->z_position;
    if (actor->unk127 != 0xFF) {
        func_150806A8(actor->unk127);
    }
    func_150829D8(actor);
    *(s16 *)&actor->unk120 = -10000; /* structs.h splits this s16 into bytes */
    D_800D3278 = 0;
    idx = actor - D_800CC2D0;
    found = 0;
    if (idx < 16) {
        if (D_800D30D0[idx] > 0) {
            box = &D_800D309C[D_800D30D0[idx] - 1];
            if (BOX_CONTAINS(box, x, (s32)prev, z)) {
                for (i = 0; i < 3; i++) {
                    mask[i] = box->mask[i];
                }
                found = 1;
            }
        }
        D_800D30D0[idx] = 0;
    }
    if (!found) {
        for (i = 0; i < 3; i++) {
            mask[i] = D_800D30B8[i];
        }
    }

    /* Keep only the candidates whose box contains the actor, narrowed by each box's overlap set. */
    word = mask;
    bits = *word;
    for (i = 0, base = 0; i < D_800D3094; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            bits = *++word;
            if (i >= D_800D3094) {
                continue;
            }
        }
        if (bits & 1) {
            box = &D_800D309C[i];
            if (BOX_CONTAINS(box, x, (s32)prev, z)) {
                for (k = 0; k < 3; k++) {
                    mask[k] &= box->mask[k];
                }
                if (idx < 16 && box->unk18 < bestUnk18) {
                    D_800D30D0[idx] = i + 1;
                    bestUnk18 = box->unk18;
                }
            } else {
                *word ^= 1LL << (i - base);
            }
        }
        bits >>= 1;
    }

    /* Apply every candidate zone the actor is inside; prevBits bit 0 says it was inside last time. */
    word = mask;
    base = 0;
    for (i = 0; i < 3; i++) {
        mask[i] |= D_800D30A0[i];
    }
    if (idx < 16) {
        prev = D_800D30F0[idx];
        prevBits = *prev;
        *prev = 0;
    } else {
        prev = NULL;
    }
    bits = *word;
    for (i = 0; i < D_800D3094; i++) {
        if (bits == 0) {
            base += 64;
            i = base;
            if (i >= D_800D3094) {
                continue;
            }
            bits = *++word;
            if (prev != NULL) {
                prev++;
                prevBits = *prev;
                *prev = 0;
            }
        }
        if (bits & 1) {
            if (func_150A1DA0(actor, &D_800D3098[i], 0) == 0) {
                switch (ZONE_TYPE(&D_800D3098[i])) {
                case 0:
                    if (cam != NULL && D_800D3098[i].subtype != 12) {
                        func_1512D560(cam, 1, &D_800D3098[i]);
                        ((CameraZoneView *)cam)->unk700 = i;
                    }
                    break;
                case 1:
                    switch (D_800D3098[i].subtype) {
                    case 0:
                        actor->unkF8 &= ~4;
                        break;
                    case 1:
                        actor->unkF8 &= ~2;
                        break;
                    case 2:
                        *(s16 *)&actor->unk120 = D_800D3098[i].y + D_800D3098[i].height;
                        break;
                    case 3:
                        actor->unkF8 |= 2;
                        break;
                    case 4:
                        func_15055C88(actor);
                        break;
                    case 5: {
                        u16 angle;
                        s32 flag;
                        f32 height;

                        /* unk95 is read as u8 here (s8 in structs.h); yaw * 65536/360 is a binary angle */
                        if (*(u8 *)&actor->unk31C->unk95 == 0 && actor->stunned == 0 && actor->health != 0 &&
                            actor->disable_run == 0 &&
                            (actor->unk31C->unk11A == 0 || D_800BE9F0 == 6) && actor->unk44 > 8.0f) {
                            height = D_800D3098[i].y + D_800D3098[i].height;
                            flag = 0;
                            angle = (u32)(D_800D3098[i].yaw * 182.0444489f);
                            angle -= actor->unk31C->unk4C;
                            if (height < actor->y_position + 250.0f && actor->unk28 == 0.0f) {
                                angle -= 0x8000;
                                flag = 1;
                            }
                            if (angle > 0x2000 && angle < 0x6000) {
                                actor->unk31C->unk97 = flag;
                                actor->unk31C->unk96 = 0;
                                actor->disable_jump = 18;
                                actor->unk31C->unk95 = i + 1;
                                if (D_800D3098[i].unk18 != 0) {
                                    *(u8 *)&actor->unk31C->unk95 |= 0x80;
                                }
                            }
                        }
                    }
                        break;
                    case 6:
                        actor->unkF8 |= 4;
                        break;
                    case 7:
                        actor->unkF4 |= 0x100000;
                        break;
                    case 8:
                        if (D_800D3098[i].unk18 != 0) {
                            D_800DF0E1 |= 1 << idx;
                        } else {
                            D_800DF0E0 |= 1 << idx;
                        }
                        break;
                    case 9:
                        actor->unkF8 |= 0x8000000;
                        break;
                    }
                    break;
                case 2:
                    func_150A23E4(&D_800D3098[i]);
                    break;
                case 7:
                    if (D_800D3098[i].unk20 == 0) {
                        if (func_1000E46C(D_800D3098[i].subtype, 100, D_800D3098[i].unk18, 0)) {
                            if (func_1000E46C(D_800D3098[i].subtype, 0, D_800D3098[i].unk1C, 0)) {
                                D_800D3098[i].unk20 = 1;
                            }
                        }
                    } else {
                        D_800D3098[i].unk20 = 1;
                    }
                    break;
                case 14:
                    if (unk127 != 0xFF && !(prevBits & 1)) {
                        func_1000D96C(D_800D3098[i].subtype, D_800D3098[i].unk18, D_800D3098[i].unk1C & 0xFF);
                        if (D_800D3098[i].subtype) {
                            func_1000E46C(D_800D3098[i].subtype, 0, D_800D3098[i].unk20 & 0xFFFF, 0);
                        }
                    }
                    break;
                case 25:
                    if (D_800D3098[i].subtype == 0) {
                        D_80041FD8 = D_800D3098[i].unk18;
                    }
                    break;
                case 4:
                    func_150A24C0(&D_800D3098[i], unk127);
                    break;
                case 8: {
                    u32 raw;

                    /* Cartridge check: the ROM word at 0xB0000454 must be 0x1090827. */
                    __osPiGetAccess();
                    osPiRawReadIo(func_150A1040(0xB0000054), &raw);
                    __osPiRelAccess();
                    if (func_150A1040(raw) == 0x1090C2B && D_800C35EA == 0 && actor->health != 0) {
                        func_15172E7C(actor, D_800D3098[i].subtype, D_800D3098[i].unk18);
                        if (D_800BE9F0 != D_800D3098[i].subtype) {
                            actor->immune = 0xFF;
                            actor->unk1CC = actor->y_position;
                        }
                    }
                }
                    break;
                case 11:
                    func_15080500(actor, &D_800D3098[i], 1, D_800D3098[i].subtype, D_800D3098[i].unk18);
                    break;
                case 12:
                    func_150A25D4(&D_800D3098[i], actor, unk127, prevBits);
                    break;
                case 3:
                    if (D_800D3098[i].subtype == 5) {
                        func_151AE06C(actor, &D_800D3098[i]);
                    }
                    break;
                case 13:
                    if (unk127 != 0xFF && !(prevBits & 1) && D_800E0B94 == 0 && actor->health != 0 &&
                        D_800C35EA == 0) {
                        if (D_800D3098[i].subtype == 0) {
                            D_800D3098[i].disabled = 1;
                        }
                        func_1501D348(D_800BE9F0, D_800D3098[i].unk18, 0, 0, 0);
                        D_800D3098[i].unk1C++;
                    }
                    break;
                }
                if (prev != NULL) {
                    *prev |= 1LL << (i - base);
                }
            }
        }
        bits >>= 1;
        prevBits >>= 1;
    }
}

s32 func_150AD960(s32, s32, s32, s32);

/*
 * Returns 0 if the actor is inside the zone, 1 if not (or the zone is disabled).  withMargin widens the
 * zone by the actor's unkE4.  Shapes: 0 cylinder (optionally rotated), 1 sphere, 2 rotated box,
 * 3 rotated gate crossed by the move from the old to the current position.
 */
s32 func_150A1DA0(Actor *actor, TriggerZone *zone, s32 withMargin) {
    s16 dx;
    s16 dy;
    s16 dz;
    s16 margin;
    s16 odx;
    s16 ody;
    s16 odz;
    s16 w;
    f32 c;
    f32 s;
    f32 a;
    f32 b;
    f32 r;
    f32 a2;
    f32 b2;

    if (zone->disabled == 1 || actor == NULL || zone == NULL) {
        return 1;
    }
    dx = actor->x_position - zone->x;
    dy = actor->y_position - zone->y;
    dz = actor->z_position - zone->z;
    margin = withMargin ? actor->unkE4 : 0;
    if (!(zone->flags & 1) || ((f32)(s32)actor->y_position - actor->unk180 <= 5.0f)) {
        switch (ZONE_SHAPE(zone)) {
        case 0:
            if (0.01f < zone->pitch || 0.01f < zone->yaw) {
                s = zone->sinYaw;
                c = zone->cosYaw;
                a = dx;
                b = dz;
                dx = a * c - b * s;
                r = b * c + a * s;
                c = zone->cosPitch;
                s = zone->sinPitch;
                a = dy;
                dz = r * c - a * s;
                dy = a * c + r * s;
            }
            if (func_150AD960(dx, dz, 0, 0) < zone->radius + margin && dy > 0 && dy < zone->height) {
                return 0;
            }
            break;
        case 1:
            if (func_150AD9A0(dx, dy, dz) < zone->radius + margin) {
                return 0;
            }
            break;
        case 2:
            c = zone->cosYaw;
            s = zone->sinYaw;
            a = dx;
            b = dz;
            dx = a * c - b * s;
            if (dx < zone->radius + margin && -zone->radius - margin < dx) {
                r = b * c + a * s;
                c = zone->cosPitch;
                s = zone->sinPitch;
                a = dy;
                w = r * c - a * s;
                if (w < zone->depth + margin && -zone->depth - margin < w) {
                    dy = a * c + r * s;
                    if (dy < zone->height && dy > 0) {
                        return 0;
                    }
                }
            }
            break;
        case 3:
            odx = actor->old_x_position - zone->x;
            ody = actor->old_y_position - zone->y;
            odz = actor->old_z_position - zone->z;
            c = zone->cosYaw;
            s = zone->sinYaw;
            a = dx;
            b = dz;
            dx = a * c - b * s;
            a2 = odx;
            b2 = odz;
            odx = a2 * c - b2 * s;
            b = b * c + a * s;
            b2 = b2 * c + a2 * s;
            c = zone->cosPitch;
            s = zone->sinPitch;
            a = dy;
            dz = b * c - a * s;
            a2 = ody;
            odz = b2 * c - a2 * s;
            /* crossed from behind the gate plane to in front of it */
            if (odz < 0 && dz >= 0) {
                c = zone->cosPitch;
                s = zone->sinPitch;
                dy = a * c + b * s;
                b = (f32)odz / (f32)(odz - dz);
                if (fabsf(odx + b * (dx - odx)) < zone->radius) {
                    ody = a2 * c + b2 * s;
                    if (ody + b * (dy - ody) >= 0.0f && ody + b * (dy - ody) < zone->height) {
                        return 0;
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return 1;
}

/* Type-2 zone occupied: starts its looping positional sound (func_1000FA64) if it is not playing yet. */
void func_150A23E4(TriggerZone *zone) {
    s32 flags;
    s32 farFlag;

    if (zone->unk20 == 0) {
        farFlag = 0;
        flags = 8;
        if (ZONE_SHAPE(zone) == 0) {
            farFlag = 0x8000;
        }
        if (zone->subtype >> 7) {
            flags = 0x108;
        }
        func_1000FA64(zone->unk1C >> 16, zone->x, zone->y, zone->z, (zone->subtype & 0x7F) * 327,
                      zone->unk18 | farFlag, zone->unk1C & 0xFFFF,
                      NULL, NULL, NULL, flags, 0);
        zone->unk20 = 1;
    fence:; // matching: keeps as1 from moving the store above into the branch delay slot
    } else {
        zone->unk20 = 1;
    }
}

s32 func_15183290(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* A TriggerZone with unk18 and unk1C viewed as halves and bytes. */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius;
    /* 0x08 */ u8  pad8[0xF];
    /* 0x17 */ u8  subtype;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ u8  unk1A;
    /* 0x1B */ u8  unk1B;
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  unk1E;
    /* 0x1F */ u8  unk1F;
    /* 0x20 */ u32 unk20;
} TriggerZoneBytes;

/* Type-4 zone: spawns its object (func_15183290) once the countdown in the low byte of unk1C is 0;
   while the zone stays occupied the countdown is held at 2 or more. */
void func_150A24C0(TriggerZone *zone, s32 unk127) {
    s32 handle;
    s32 countdown;
    s32 subtype;
    s32 lo18;
    s32 hi18;
    s32 b1A;
    s32 b1C;
    s32 b1D;
    s32 unk20;

    if (unk127 != 0xFF) {
        if ((zone->unk1C & 0xFF) == 0 || zone->subtype == 9) {
            /* matching: each argument goes through its own local, in this order */
            subtype = zone->subtype;
            lo18 = zone->unk18 & 0xFF;
            hi18 = ((TriggerZoneBytes *)zone)->unk18;
            b1A = ((TriggerZoneBytes *)zone)->unk1A;
            b1C = ((TriggerZoneBytes *)zone)->unk1C;
            b1D = ((TriggerZoneBytes *)zone)->unk1D;
            unk20 = zone->unk20;
            handle = func_15183290(zone->x, zone->y + b1D * 16, zone->z, lo18, subtype, zone->radius, unk127,
                                   hi18, b1A, b1C, unk20);
            if (handle != -1 && zone->subtype != 9) {
                zone->unk1C = (handle << 8) | 0xF0 | (zone->unk1C & 0xFFFF0000);
            }
        } else {
            countdown = zone->unk1C & 0xFF;
            if (countdown < 2) {
                countdown = 2;
            }
            zone->unk1C = (zone->unk1C & ~0xFF) | countdown;
        }
    }
}

extern u8 D_800D3090;
extern u8 D_800D9EB8[][3];
void func_150E7290(s32 arg0, s32 owner, s32 kind);
void func_150E68B0(Actor *actor, s32 arg1, TriggerZone *zone, u8 sourceIndex);
void func_151CE51C(Actor *actor, TriggerZone *zone);

/* Type-12 zone: subtype-specific effect on the actor inside it; prevBits bit 0 is "was inside last time". */
void func_150A25D4(TriggerZone *zone, Actor *actor, s32 unk127, s64 prevBits) {
    switch (zone->subtype) {
    case 0:
        break;
    case 1:
        if (unk127 != 0xFF) {
            func_150E7290(unk127, 0xFF, 0);
        }
        break;
    case 2:
        if (unk127 != 0xFF) {
            /* colour: one byte each from unk18, unk1C, unk20 */
            D_800D9EB8[unk127][0] = zone->unk18;
            D_800D9EB8[unk127][1] = zone->unk1C;
            D_800D9EB8[unk127][2] = zone->unk20;
        }
        break;
    case 3:
        if (unk127 != 0xFF) {
            func_150E68B0(actor, unk127, zone, ((TriggerZoneBytes *)zone)->unk1B);
        }
        break;
    case 6: {
        u8 *timer = &D_800D3090;
        if (!(prevBits & 1)) {
            *timer = 30;
            func_10010344(0x66F, actor, 0x5DC0, 0x190, 0x1F4);
        }
        *timer += D_800BE9E4;
        if (*timer >= 31) {
            if (actor->unkB2 > 1440) {
                actor->unkB2 = 1200;
            } else if (actor->unkB2 < 241) {
                actor->unkB2 = 90;
            } else {
                actor->unkB2 -= 240;
            }
            *timer = 0;
        }
    }
        break;
    case 8:
        func_151C8674((s32)actor, (s32)zone);
        break;
    case 9:
        func_1515F270((s32)actor, (s32 *)zone);
        break;
    case 10:
        func_151CE51C(actor, zone);
        break;
    }
}

/* Applies the type-8 zones containing the actor (func_15172E7C), unless its unk65 is set. */
void func_150A278C(Actor *actor) {
    s32 i;

    if (actor->unk65 != 0) {
        return;
    }
    for (i = 0; i < D_800D3094; i++) {
        if (ZONE_TYPE(&D_800D3098[i]) == 8) {
            if (func_150A1DA0(actor, &D_800D3098[i], 0) == 0) {
                func_15172E7C(actor, D_800D3098[i].subtype, D_800D3098[i].unk18);
            }
        }
    }
}

/* Enables (0) or disables (1) zone id; disabling also clears it from every actor's occupancy bits. */
void func_150A2864(u32 id, s32 disabled) {
    s32 i;

    id &= 0xFFF;
    if (id < D_800D3094) {
        D_800D3098[id].disabled = disabled;
    }
    if (disabled == 1) {
        for (i = 0; i < 16; i++) {
            D_800D30F0[i][id >> 6] &= ~(1ULL << (id & 0x3F));
        }
    }
}

/* Sets zone id's unk18 and subtype (-1 leaves one unchanged) and clears its unk1C. */
void func_150A2940(s32 id, s32 unk18, s32 subtype) {
    id &= 0xFFF;
    if (id < D_800D3094) {
        if (unk18 != -1) {
            D_800D3098[id].unk18 = unk18;
        }
        if (subtype != -1) {
            D_800D3098[id].subtype = subtype;
        }
        D_800D3098[id].unk1C = 0;
    }
}

/* Returns 0 if actor `actor` is inside zone id, else 1 (cached occupancy bits when they are current). */
s32 func_150A29C8(u32 actor, u32 id) {
    id &= 0xFFF;
    if (D_800D3094 < id) {
        return 1;
    }
    if (actor < 16 && (D_800D2340 & (1 << actor))) {
        if ((D_800D30F0[actor][id >> 6] >> (id & 0x3F)) & 1) {
            return 0;
        }
        return 1;
    }
    return func_150A1DA0(&D_800CC2D0[actor], &D_800D3098[id], 1);
}

/* Returns the first of the count zone ids in ids[] that contains actor `actor`, or -1. */
s32 func_150A2AEC(u32 actor, s32 count, s32 *ids) {
    s32 i;
    u32 id;

    if (actor < 16 && (D_800D2340 & (1 << actor))) {
        for (i = 0; i < count; i++) {
            id = ids[i];
            id &= 0xFFF;
            if (id < D_800D3094) {
                if ((D_800D30F0[actor][id >> 6] >> (id & 0x3F)) & 1) {
                    return id;
                }
            }
        }
    } else {
        for (i = 0; i < count; i++) {
            id = ids[i];
            id &= 0xFFF;
            if (id < D_800D3094) {
                if (func_150A1DA0(&D_800CC2D0[actor], &D_800D3098[id], 1) == 0) {
                    return id;
                }
            }
        }
    }
    return -1;
}

/* Returns the first actor whose cached occupancy bits say it is inside zone id, or -1. */
s32 func_150A2CA4(u32 id) {
    s32 i;

    id &= 0xFFF;
    if (D_800D3094 < id) {
        return -1;
    }
    for (i = 0; i != 16; i++) {
        if (D_800D2138.unk208 & (1 << i)) {
            if ((D_800D30F0[i][id >> 6] >> (id & 0x3F)) & 1) {
                return i;
            }
        }
    }
    return -1;
}

/* Returns 0 if camera `camIndex`'s position is inside zone id, else 1. */
s32 func_150A2D84(s32 camIndex, s32 id) {
    Camera *cam;
    Actor probe;

    cam = &D_800DBFF0[camIndex];
    id &= 0xFFF;
    if (D_800D3094 < id) {
        return 1;
    }
    probe.x_position = cam->eye.x;
    probe.y_position = cam->eye.y;
    probe.z_position = cam->eye.z;
    probe.unk180 = probe.y_position;
    probe.old_x_position = cam->prevEye.x;
    probe.old_y_position = cam->prevEye.y;
    probe.old_z_position = cam->prevEye.z;
    return func_150A1DA0(&probe, &D_800D3098[id], 0);
}

/* Returns 1 if the point is behind (or on) the zone's rotated plane, else 0.
 * matching: the dead u/z computations reproduce the original's register allocation
 * (u reserves $f16 so t lands in $f18) and block IDO's (a - b <= 0) -> (a <= b) fold. */
s32 func_150A2E4C(TriggerZone *zone, f32 x, f32 y, f32 z) {
    f32 s;
    f32 c;
    f32 t;
    f32 u;
    f32 w;

    x -= zone->x;
    y -= zone->y;
    z = y - zone->z;
    s = zone->sinYaw;
    c = zone->cosYaw;
    u = z * s;
    t = z * c + x * s;
    s = zone->sinPitch;
    c = zone->cosPitch;
    u = u + t;
    w = t * c - y * s;
    z = y * c + t * s;
    u = u * u;
    if (w <= 0.0f) {
        return 1;
    }
    return 0;
}

/* Returns whether actor `actor` is behind gate zone id (0 for zones that are not gates, 1 for bad ids). */
s32 func_150A2EE4(s32 actor, s32 id) {
    id &= 0xFFF;
    if (D_800D3094 < id) {
        return 1;
    }
    if (ZONE_SHAPE(&D_800D3098[id]) == 3) {
        return func_150A2E4C(&D_800D3098[id], D_800CC2D0[actor].x_position, D_800CC2D0[actor].y_position,
                             D_800CC2D0[actor].z_position);
    }
    return 0;
}

/* As func_150A2EE4, for camera `camIndex`'s position. */
s32 func_150A2FA4(s32 camIndex, s32 id) {
    Camera *cam;

    cam = &D_800DBFF0[camIndex];
    id &= 0xFFF;
    if (D_800D3094 < id) {
        return 1;
    }
    if (ZONE_SHAPE(&D_800D3098[id]) == 3) {
        return func_150A2E4C(&D_800D3098[id], cam->eye.x, cam->eye.y, cam->eye.z);
    }
    return 0;
}

/* Returns 1 if the point is inside an enabled type-3 zone of the given subtype. */
s32 func_150A3058(s32 subtype, s16 x, s16 y, s16 z) {
    u8 pad[4]; // matching: unused local of the original (frame layout)
    Actor probe;
    s32 i;

    probe.x_position = x;
    probe.old_x_position = x;
    probe.y_position = y;
    probe.unk180 = y;
    probe.old_y_position = y;
    probe.z_position = z;
    probe.old_z_position = z;

    for (i = 0; i < D_800D3094; i++) {
        if (!D_800D3098[i].disabled) {
            if (ZONE_TYPE(&D_800D3098[i]) == 3) {
                if (D_800D3098[i].subtype == subtype) {
                    if (func_150A1DA0(&probe, &D_800D3098[i], 0) == 0) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/* Returns the first enabled zone of the given type and subtype that contains the point, or NULL. */
TriggerZone *func_150A3194(s32 type, s32 subtype, s32 x, s32 y, s32 z) {
    s32 i;
    Actor probe;

    probe.x_position = x;
    probe.old_x_position = x;
    probe.y_position = y;
    probe.unk180 = y;
    probe.old_y_position = y;
    probe.z_position = z;
    probe.old_z_position = z;

    for (i = 0; i < D_800D3094; i++) {
        if (!D_800D3098[i].disabled) {
            if (type == ZONE_TYPE(&D_800D3098[i])) {
                if (subtype == D_800D3098[i].subtype) {
                    if (func_150A1DA0(&probe, &D_800D3098[i], 0) == 0) {
                        return &D_800D3098[i];
                    }
                }
            }
        }
    }
    return NULL;
}

/* Returns 1 if the point is inside the zone. */
s32 func_150A32B4(TriggerZone *zone, s32 x, s32 y, s32 z) {
    Actor probe;
    u8 pad[8]; // matching: unused local of the original (frame layout)

    probe.x_position = x;
    probe.old_x_position = x;
    probe.y_position = y;
    probe.unk180 = y;
    probe.old_y_position = y;
    probe.z_position = z;
    probe.old_z_position = z;
    if (func_150A1DA0(&probe, zone, 0) == 0) {
        return 1;
    }
    return 0;
}

/* Reads zone id's subtype and parameters. */
void func_150A3330(s32 id, s32 *subtype, s32 *unk18, s32 *unk1C, s32 *unk20) {
    *subtype = D_800D3098[id].subtype;
    *unk18 = D_800D3098[id].unk18;
    *unk1C = D_800D3098[id].unk1C;
    *unk20 = D_800D3098[id].unk20;
}

/* Writes the zone id fields selected by mask (1: subtype, 2: unk18, 4: unk1C, 8: unk20). */
void func_150A3398(s32 id, u16 mask, s32 subtype, s32 unk18, s32 unk1C, s32 unk20) {
    if (mask & 1) {
        D_800D3098[id].subtype = subtype;
    }
    if (mask & 2) {
        D_800D3098[id].unk18 = unk18;
    }
    if (mask & 4) {
        D_800D3098[id].unk1C = unk1C;
    }
    if (mask & 8) {
        D_800D3098[id].unk20 = unk20;
    }
}

/* Moves zone id to (x, y, z). */
void func_150A3444(s32 id, s16 x, s16 y, s16 z) {
    D_800D3098[id].x = x;
    D_800D3098[id].y = y;
    D_800D3098[id].z = z;
}
