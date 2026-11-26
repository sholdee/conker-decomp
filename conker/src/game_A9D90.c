#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Defined in game_1944C0.c. Not in effect.h: game_16F770.c calls it with two more arguments. */
void func_15169040(s32 event, u8 eventType);

/* Puts obj into state 2 with the given timer. */
void func_1507C8E0(Actor *obj, s32 timer) {
    obj->mv->death_State = 2;
    obj->mv->deathTimer = timer;
}

/* Per-frame update of every active player's state machine. */
void func_1507C8FC(void) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    Actor *player;

    if (D_800BEAC0 == 0) {
        player = movement_obj;
        for (i = 0; i < D_8008FD8C; i++, player++) {
            if (player->mv == NULL) {
                continue;
            }
            if (player->mv->death_State == 0) {
                player->mv->deathTime = 0;
                continue;
            }
            if (player->mv->death_State == 1) {
                continue;
            }
            if (i <= D_80082FA0) {
                if (D_800BE616 != 0 && (D_800D18A2 & (1 << i))) {
                    player->mv->death_State = 2;
                    player->mv->deathTimer = 60;
                }
                if (player->mv->death_State == 11) {
                    func_1517EE40(0, 0, 0, 15, 1, i);
                    player->mv->death_State = 12;
                    continue;
                }
                if (player->mv->death_State == 12) {
                    if (func_1517EFAC(i) != 0) {
                        func_1507D4F8(i);
                    }
                    continue;
                }
                if (player->mv->death_State == 2) {
                    if (player->mv->deathTimer > 0) {
                        player->mv->deathTimer -= D_800BE9E4;
                    } else {
                        func_1507CD0C(player);
                    }
                } else if (player->mv->death_State == 7) {
                    if (player->mv->deathTimer > 0) {
                        player->mv->deathTimer -= D_800BE9E4;
                    } else {
                        player->mv->death_State = 8;
                        func_1506160C(player, 2, 0, 60, 0);
                    }
                }
                player->mv->deathTime += D_800BE9E4;
                if ((D_800BE710[i] & 0x8000) || (D_800BE616 != 0 && player->mv->bot == 0) ||
                    func_150859AC(i, 3) < 2 || (player->mv->bot == 1 && player->mv->deathTime >= 181)) {
                    player->mv->respawnReady = TRUE;
                }
                if (D_8008FDBC & 0x80) {
                    player->mv->respawnReady = FALSE;
                }
                func_1507E1D0(player, &x, &y, &z);
                func_1517F814(0, x, y, z, i);
                if (player->mv->respawnReady && player->mv->death_State == 5) {
                    player->mv->death_State = 6;
                }
                if (player->mv->death_State == 9) {
                    func_1507D4F8(i);
                }
            } else if (player->mv->death_State == 11) {
                func_1507D4F8(i);
                continue;
            } else if (player->mv->death_State == 2) {
                if (player->mv->deathTimer > 0) {
                    player->mv->deathTimer -= D_800BE9E4;
                } else {
                    player->mv->death_State = 7;
                }
            } else {
                player->mv->deathTime += D_800BE9E4;
                if (player->unk7 == 0xFF && player->mv->deathTime >= 161) {
                    func_1506160C(player, 2, 0, 60, 0);
                }
                if (player->mv->deathTime >= 181 && player->mv->death_State != 10) {
                    func_1507D4F8(i);
                }
            }
        }
        D_8008FDBC &= ~0x80;
    }
}

/* Puts obj into state 3 and, for a local player slot, calls func_15181D70 on it. */
void func_1507CD0C(Actor *obj) {
    s32 slot = obj - movement_obj;

    obj->mv->death_State = 3;

    if (slot <= D_80082FA0) {
        func_15181D70(slot);
    }
}

/* Kills obj with the given cause: plays its sound, resets movement and moves it to a dead state. */
void func_1507CD64(Actor *obj, s32 cause) {
    f32 f;
    s32 sound;
    u32 r;
    s32 mode;
    s32 slot;
    ActorRef msg;
    Actor *saved;
    s32 again;
    s32 flag;

    if (obj->mv == NULL) {
        return;
    }
    slot = obj - movement_obj;
    if (slot >= D_8008FD8C) {
        return;
    }
    if (obj->mv->death_State != 0) {
        if (cause != 9 || cause == obj->mv->unk121) {
            return;
        }
        again = TRUE;
    } else {
        again = FALSE;
    }
    obj->mv->unk121 = cause;
    if (D_800BE616 != 0 && !again) {
        if (slot < 16) {
            D_800E0AD0[slot]++;
        }
        if ((D_8008FDBC & 0x20) && obj->team == 0) { // byte at 0x128 (Actor has a u32 pad there)
            func_15085710(1, 10, 1);
        }
    }
    msg.actor = obj;
    msg.uniqueId = obj->unique_id;
    func_15169040((s32)&msg, 3);
    D_800D18A9 = 0;
lbl2: // matching: unused label splits the basic block; as1 then hoists the reload of cause above this store
    sound = -1;
    flag = 0;
    mode = 0;
    if (obj->mv->on_fire) {
        D_800D18A9 = 2;
    }
    switch (cause) {
    case 11:
        sound = 0x2D4;
        break;
    case 12:
        sound = 0x2D6;
        break;
    case 0:
        sound = 0x58;
        break;
    case 1:
        sound = 0x13;
        flag = 1;
        break;
    case 2:
        sound = 7;
        flag = 1;
        break;
    case 3:
        sound = 0x28;
        D_800D18A9 = 1;
        break;
    case 4:
        sound = 0x24;
        mode = 1;
        D_800D18A9 = 1;
        break;
    case 5:
        sound = 0x24;
        mode = 1;
        D_800D18A9 = 3;
        break;
    case 6:
        if (obj->type == 1 || obj->type == 0x15 || obj->drawtype == 3 ||
            obj->attach_char != 0) {
            sound = 0x170;
        }
        mode = 1;
        break;
    case 7:
        sound = 0x16F;
        break;
    case 8:
        sound = 0x170;
        mode = 1;
        D_800D18A9 = 3;
        break;
    case 15:
        sound = 0x170;
        mode = 3;
        D_800D18A9 = 3;
        break;
    case 9:
        mode = 2;
        break;
    case 10:
        sound = 0x2F;
        D_800D18A9 = 3;
        break;
    case 14:
        sound = 0x1E;
        mode = 1;
        break;
    case 13:
        sound = 0x2D8;
        mode = 1;
        break;
    case 16:
        sound = 0xF6;
        mode = 1;
        break;
    }
    if (sound != -1) {
        func_1505E650(obj, sound, 1.0f, 3.0f, 0.0f, 0.0f, flag);
    }
    obj->energy = 0;
    obj->imune = 0xFF;
    obj->stunned = 0xFF;
    obj->norun = 0xFF;
    obj->noshove |= 0x200;
    obj->bodyRadius = 0;
    obj->bodyHeight = 0;
    obj->speed = 0.0f;
    obj->mv->bitten = 0;
    if (obj->lifting != 0) {
        saved = D_800D154C;
        D_800D154C = obj;
        func_1504BAF0(obj);
        D_800D154C = saved;
    }
    if (mode == 0 || mode == 3) {
        obj->mv->death_State = 1;
    } else {
        func_1507C8E0(obj, 0);
    }
    if (mode == 2) {
        obj->mv->death_State = 11;
    } else if (mode == 3) {
        func_1507D4F8(slot);
    }
    if (slot == 0 && level == 4) {
        r = func_150ADA20();
        f = func_150ADA68();
        func_150E7994(r % 3 + 4, f * 0.1f + 0.3f, 0xFF, 0);
    }
}

/* Forwards four arguments to func_1509BFB0(3, ...) keyed by object slot's unk13F | 0x2000. */
void func_1507D158(s32 slot, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 value = movement_obj[slot].listpos;

    func_1509BFB0(3, value | 0x2000, arg1, arg2, arg3, arg4);
}

/* Re-initialises obj through func_15060F28/func_15082A44 (health back to D_8008726C), keeping a few state bytes. */
void func_1507D1D8(Actor *obj) {
    s32 value;
    s32 saved1AA;
    s8 player;
    s8 saved1B0;
    s8 saved328;
    s8 saved1AF;
    s32 choice;
    s32 slot;

    saved1AA = obj->mv->shotsfired;
    saved1B0 = obj->mv->lastSpawnIndex;
    saved328 = obj->unk328; // signed byte at 0x328 (Actor types it u8)
    saved1AF = obj->mv->unk1AF;
    if (D_800BE616 != 0) {
        player = obj->mv->spawnIndex;
    } else {
        player = -1;
    }
    if (level == 0x2B) {
        slot = obj - movement_obj;
        value = func_150859AC(slot, 1);
        if (value >= 0x11 && value < 0x21) {
            choice = (func_150ADA20() & 0xF) + 0x11;
            func_15085710(slot, 0, choice);
            if (choice < 0x1D) {
                D_800D20FC[slot].type = 0x9C;
            } else {
                D_800D20FC[slot].type = 0x9D;
            }
        }
    }
    slot = obj - movement_obj;
    func_15060F28(obj, 0);
    D_800D18A8 = 1;
    func_15082A44(&D_800D20FC[slot], slot, 0, 0, 0);
    obj->mv->lastSpawnIndex = saved1B0;
    obj->mv->unk1AF = saved1AF;
    obj->unk328 = saved328;
    if (D_80082FA0 >= slot) {
        func_150054C4(&camera_global_Cameras[slot], slot, D_800BE9F2, player);
        func_15181DC8(slot);
        func_1517EE40(0, 0, 0, 0, 1, slot);
        func_1517EE40(0, 0, 0, 30, 0, slot);
    } else {
        func_15085430(obj, player, 0);
    }
    D_800D18A8 = 0;
    obj->mv->death_State = 0;
    obj->mv->respawnReady = FALSE;
    obj->mv->deathTimer = 0;
    obj->mv->shotsfired = saved1AA;
    obj->energy = D_8008726C;
    obj->unk22C |= 0x100;
    obj->mv->context_b = 0;
    obj->unk20F = 0xFF;
    obj->unk210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->intelwalk = 1;
    obj->scriptCmd = NULL;
    obj->mv->spawnIndex = player;
    if (D_800BE616 != 0) {
        obj->imune = 150;
        obj->mv->unk1B1 = obj->imune;
        if (level == 0x33) {
            func_1507D158(slot, 120, 0, 0, 0);
            func_15088D58(obj);
        }
    }
}

/* Ends slot's current run: re-initialises it if func_150859AC(slot, 3) allows, else retires it (state 10). */
void func_1507D4F8(s32 slot) {
    Actor *obj;
    ActorCtl *state;

    if (!(D_8008FDBC & 1)) {
        func_15085710(slot, 4, 1);
    }

    if (func_150859AC(slot, 3) != 0) {
        func_15085710(slot, 5, D_8008726C);
        obj = &movement_obj[slot];
        obj->unkB2 = 0;

        if (D_800BE616 != 0) {
            if (D_800E0C20 == 0) {
                func_1507D1D8(obj);
                return;
            }
            obj->mv->death_State = 10;
            return;
        }

        D_800D18A8 = 1;
        if (!(D_800D2E4C[0x19] & 4) && D_8008FDA8 >= 0) {
            func_1501C730(1, 0x22, 0, 0, 0);
            return;
        }
        func_1501C730(2, D_800BE3DF, D_800BE3E0, 0, 0);
        return;
    }

    if (D_800BE616 == 0) {
        D_800D2E43 = 1;
        func_1509C3A0();
        D_800D18A8 = 1;
        func_15085710(slot, 5, D_8008726C);
        func_15085710(slot, 2, D_80087260);
        func_1501C730(1, 0x18, 0, 0, 0);
    } else {
        D_800D18A0 |= 1 << slot;
    }

    obj = &movement_obj[slot];
    state = obj->mv;
    if (state->bot == 0) {
        // matching: the volatile store and load make IDO reload mv after the decrement, as the original does
        *(volatile s8 *)&D_8008FD94 = D_8008FD94 - 1;
        state = *(ActorCtl *volatile *)&obj->mv;
    }
    state->death_State = 10;
    D_800BE618--;
}


/* Death handler: dispatches on obj->id; returns 1 or 2 when the object was handled specially, else 0. */
/* Runs the per-id handler for obj (mostly id handler(obj, 0xFF, 1)); returns 1 or 2 for ids handled specially, else 0. */
s32 func_1507D754(Actor *obj) {
    WorldObject *spawned;
    void *handle;

    switch (obj->character_type) {
    case 0x7F:
        func_150B0348(obj, 0xFF, 1);
        break;
    case 0x10:
    case 0x38:
    case 0x91:
        func_150BFFE0(obj);
        break;
    case 0x3A:
    case 0x3D:
        func_150D85AC(obj);
        break;
    case 0x4A:
        func_150F1D10(obj, 0xFF, 1);
        break;
    case 0x47:
        func_151C9740(obj, 0xFF, 1);
        break;
    case 0x25:
        if (obj->taskint[2] != 0 || level == 6 || level == 0x3B) {
            func_151CFA4C(obj, 0xFF, 1);
        }
        return 0;
    case 0x79:
        func_150BE494(obj, 0xFF, 1);
        break;
    case 0x05:
    case 0xAD:
    case 0xAE:
    case 0xAF:
        obj->unk328 = 0;
        func_1507DB44(obj, 0);
        func_150AEB9C(obj);
        return 1;
    case 0x53:
        if (D_800BE616 != 0) {
            func_1507CD64(obj, 10);
            return 2;
        }
        break;
    case 0x90:
        if (D_800BE616 != 0 && level == 0x33) {
            func_1507CD64(obj, 8);
            return 2;
        }
        break;
    case 0x28:
    case 0x77:
        spawned = NULL;
        func_151BA518(obj, 0xFF, 1);
        func_15145A50(obj);
        if (D_800BE616 != 0) {
            func_1507CD64(obj, 6);
            spawned = func_151149AC((obj->playernumber + 11));
        } else {
            if (level == 0x23) {
                spawned = func_151149AC(1);
            }
            handle = func_1507515C(obj);
            if (handle != NULL && D_800C35EA[0] != 1) {
                func_15136C3C(handle, 1, 1, 1, 1, 0, 0xFF, 1);
            }
        }
        if (spawned != NULL) {
            spawned->inactive = 0;
            spawned->x = obj->xpos;
            spawned->y = obj->ypos;
            spawned->z = obj->zpos;
            spawned->t = obj->vtilt;
            spawned->u = obj->theta;
            spawned->v = obj->lean_angle;
            spawned->alpha = 0xFF;
            spawned->touch.i.a = 0;
            spawned->touch.i.b = 0;
            spawned->touch.i.c = 0;
        }
        if (D_800BE616 != 0) {
            return 2;
        }
        break;
    case 0x0D:
        func_150B7B40(obj, 0xFF, 1);
        break;
    case 0x20:
        if (obj->yspd < -20.0f) {
            func_150BC488(obj);
        } else {
            func_150BBB5C(obj);
        }
        break;
    case 0x4F:
    case 0x83:
        func_151A6C90(obj, 0xFF, 1);
        break;
    case 0x34:
        func_150E0870(obj, 0xFF, 1);
        break;
    case 0x44:
        func_151D22B0(obj, 0xFF, 1);
        break;
    }
    return 0;
}

void func_1507DB34(void) {
}

void func_1507DB3C(void) {
}

void func_1507DB44(Actor *obj, s32 arg1) {
    func_1503DE70(obj, arg1, -1);
}

void func_1507DB64(void) {
}

extern Gfx D_80086B80[];
extern u8 D_D0F;

/*
 * Draws a 32x32 RGBA32 textured square centred on player's view (coordinates in 10.2 fixed point). While the
 * player's D_800D18A2 bit is set the square shrinks each frame and, once gone, the bit is cleared; otherwise it
 * grows back to its full size.
 */
Gfx *func_1507DB6C(Gfx *gfx, s32 player) {
    s32 texture;
    s32 size;
    RenderView *view;
    s32 right, left, bottom, top;
    s32 scale;

    gSPDisplayList(gfx++, D_80086B80);
    texture = func_1510D0EC((s32)&D_D0F, NULL, 3, 0);
    gDPSetTextureImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 1, texture);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 0, 0, G_TX_LOADTILE, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPLoadSync(gfx++);
    gDPLoadBlock(gfx++, G_TX_LOADTILE, 0, 0, 32 * 32 - 1, 0);
    gDPPipeSync(gfx++);
    gDPSetTile(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_32b, 8, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 0, 0, G_TX_CLAMP, 0, 0);
    gDPSetTileSize(gfx++, G_TX_RENDERTILE, 0, 0, 31 << G_TEXTURE_IMAGE_FRAC, 31 << G_TEXTURE_IMAGE_FRAC);
    gDPSetOtherMode(gfx++,
                    0xF /* blend mask bits */ | G_AD_DISABLE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE |
                        G_TL_TILE | G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_OPA_SURF | G_RM_OPA_SURF2);
    if (D_800D18A2 & (1 << player)) {
        if (D_800D18A4[player] >= 4) {
            D_800D18A4[player] -= 4;
        } else {
            D_800D18A2 &= ~(1 << player);
            func_15181DC8(player);
            func_1517EE40(0, 0, 0, 120, 0, player);
            return gfx;
        }
    } else {
        D_800D18A4[player] += 4;
        if (D_800D18A4[player] > 60) {
            D_800D18A4[player] = 60;
        }
    }
    size = D_800D18A4[player] + 4;
    // matching: indexing through the (no-op) cast orders the addu as the original
    view = &((RenderView *)D_800BE628)[player]; // matching: the no-op cast changes the code
    left = right = (s32)(view->right + view->left) * 2;
    top = bottom = (s32)(view->bottom + view->top) * 2;
    left -= size;
    right += size;
    top -= size;
    bottom += size;
    scale = 0x20000 / (right - left);
    gSPTextureRectangle(gfx++, left, top, right, bottom, G_TX_RENDERTILE, 0, 0, scale, scale);
    return gfx;
}


/* Sets bits in obj->unk9C (after re-running func_150836CC 0x44/0x23 when type is 1). */
void func_1507DE4C(Actor *obj) {
    if (obj->type == 1) {
        func_150836CC(obj, 0x44);
        func_150836CC(obj, 0x23);
        obj->unk9C |= 0xF000;
        func_150836CC(obj, 0x44);
        func_150836CC(obj, 0x23);
        return;
    }

    switch (obj->character_type) {
    case 0x9F:
    case 0xA0:
        obj->unk9C |= 0xF000;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        obj->unk9C |= 0xFF8;
        break;
    }
}

/* Sets obj's unk94/unk9C flags and taskint[0] (1, 2, 4 or 8) for part 4..9. */
void func_1507DF10(Actor *obj, s32 part) {
    switch (part) {
    case 9:
        obj->unk94 |= 0x20;
        obj->unk9C |= 0x78;
        obj->taskint[0] = 1;
        return;
    case 8:
        obj->unk94 |= 0x40;
        obj->unk94 &= ~0x200;
        obj->unk9C |= 0xF00;
        obj->taskint[0] = 2;
        return;
    case 6:
    case 7:
        obj->unk94 |= 0xE;
        obj->unk94 &= ~0x410;
        obj->unk9C |= 0xEE0000;
        obj->taskint[0] = 4;
        return;
    case 4:
    case 5:
        obj->unk94 |= 0x80;
        obj->unk94 &= ~0x500;
        obj->taskint[0] = 8;
        return;
    }
}

/* Looks up resource's position and facing via func_15084D70 and calls func_1501D348(0x25, ...) from there. */
void func_1507DFE4(s32 resource, s32 arg1) {
    s32 savedMode;
    s16 pos[3];
    Vec3f vec;
    u8 unk4F;
    u8 angle;
    s32 unk48;
    s32 unk44;
    f32 yaw;
    s32 kind;
    s32 type;

    if (D_800C35EA[0] != 1) {
        func_15084D70(0, resource, 1, pos, &vec, &unk4F, &angle, &unk44, &unk48, 1, &type);
        yaw = (angle - 64) * (360.0f / 256.0f) + 180.0f;
        if (type == 0) {
            kind = 1;
        } else if (type == 1) {
            kind = 3;
        } else {
            return;
        }
        func_15022190(pos[0], pos[1], pos[2], yaw);
        savedMode = level;
        level = 0x25;
        D_800C3671 = 1;
        func_1501D348(0x25, kind, 0, 0, 0);
        D_800C3670 = 1;
        level = savedMode;
    }
}

/* Returns 0, or 1 + the type func_15084D70 reports for D_800BE3E0 (after setting D_800BE9F4 from D_800BE3DF). */
s32 func_1507E114(s32 arg0) {
    s32 type;
    s16 pos[3];
    Vec3f vec;
    s32 unk44;
    u8 unk43;
    u8 unk42;
    s32 unk3C;
    s32 mode;

    if (D_800D18A8 == 0) {
        return 0;
    }
    mode = D_800BE9F4;
    if (mode == 0x22 || mode == 0x18) {
        return 0;
    }
    D_800BE9F4 = D_800BE3DF;
    func_15084D70(0, D_800BE3E0, 1, pos, &vec, &unk43, &unk42, &unk44, &unk3C, 1, &type);
    return type + 1;
}

/* Gets obj's tracking point: 30 * scale_y up through bone 0 (12 or 3 in some states), or its position. */
void func_1507E1D0(Actor *obj, f32 *x, f32 *y, f32 *z) {
    if (obj->ob_matrix_ptr != NULL) {
        MtxF *bone;
        Vec3f in;
        f32 out[3];

        in.x = 0.0f;
        in.y = obj->scale_y * 30.0f;
        in.z = 0.0f;
        bone = obj->ob_matrix_ptr;
        if (obj->type == 1) {
            bone += 12;
        } else if (obj->type == 0x1E) {
            bone += 3;
        }
        func_15143134(&in, out, bone);
        *x = out[0];
        *y = out[1];
        *z = out[2];
    } else {
        *x = obj->xpos;
        *y = obj->ypos;
        *z = obj->zpos;
    }
}
