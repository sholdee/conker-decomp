#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals (see init/Sched/SchedInit/schedinit.c). */
u16 gEliminatedPlayers;
u16 D_800D18A2;
u8 D_800D18A4[4]; // per player: half-size of the square, minus 4
u8 D_800D18A8;
u8 D_800D18A9;

#include "functions.h"
#include "variables.h"

/* Defined in game/Effects/ObjectList/objectlist.c. Not in effect.h: game/Math/MathUtil/mathutil.c calls it with two more arguments. */
void effect_broadcastEvent(s32 event, u8 eventType);

/* Puts obj into state 2 with the given timer. */
void actor_startDeathCountdown(Actor *obj, s32 timer) {
    obj->mv->death_State = 2;
    obj->mv->deathTimer = timer;
}

/* Per-frame update of every active player's state machine. */
void actor_updatePlayerStates(void) {
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    Actor *player;

    if (gPaused == 0) {
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
            if (i <= gScreenLayout) {
                if (gIsMultiplayer != 0 && (D_800D18A2 & (1 << i))) {
                    player->mv->death_State = 2;
                    player->mv->deathTimer = 60;
                }
                if (player->mv->death_State == 11) {
                    screen_startViewFade(0, 0, 0, 15, 1, i);
                    player->mv->death_State = 12;
                    continue;
                }
                if (player->mv->death_State == 12) {
                    if (screen_isViewFadedOut(i) != 0) {
                        actor_finishDeath(i);
                    }
                    continue;
                }
                if (player->mv->death_State == 2) {
                    if (player->mv->deathTimer > 0) {
                        player->mv->deathTimer -= D_800BE9E4;
                    } else {
                        actor_closeDeathIris(player);
                    }
                } else if (player->mv->death_State == 7) {
                    if (player->mv->deathTimer > 0) {
                        player->mv->deathTimer -= D_800BE9E4;
                    } else {
                        player->mv->death_State = 8;
                        actor_fadeCommand(player, 2, 0, 60, 0);
                    }
                }
                player->mv->deathTime += D_800BE9E4;
                if ((gButtonsPressed[i] & 0x8000) || (gIsMultiplayer != 0 && player->mv->bot == 0) ||
                    player_getStat(i, 3) < 2 || (player->mv->bot == 1 && player->mv->deathTime >= 181)) {
                    player->mv->respawnReady = TRUE;
                }
                if (D_8008FDBC & 0x80) {
                    player->mv->respawnReady = FALSE;
                }
                actor_getTrackPoint(player, &x, &y, &z);
                screen_moveIris(0, x, y, z, i);
                if (player->mv->respawnReady && player->mv->death_State == 5) {
                    player->mv->death_State = 6;
                }
                if (player->mv->death_State == 9) {
                    actor_finishDeath(i);
                }
            } else if (player->mv->death_State == 11) {
                actor_finishDeath(i);
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
                    actor_fadeCommand(player, 2, 0, 60, 0);
                }
                if (player->mv->deathTime >= 181 && player->mv->death_State != 10) {
                    actor_finishDeath(i);
                }
            }
        }
        D_8008FDBC &= ~0x80;
    }
}

/* Puts obj into state 3 and, for a local player slot, calls screen_closeIris on it. */
void actor_closeDeathIris(Actor *obj) {
    s32 slot = obj - movement_obj;

    obj->mv->death_State = 3;

    if (slot <= gScreenLayout) {
        screen_closeIris(slot);
    }
}

/* Kills obj with the given cause: plays its sound, resets movement and moves it to a dead state. */
void actor_kill(Actor *obj, s32 cause) {
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
    if (gIsMultiplayer != 0 && !again) {
        if (slot < 16) {
            D_800E0AD0[slot]++;
        }
        if ((D_8008FDBC & 0x20) && obj->team == 0) { // byte at 0x128 (Actor has a u32 pad there)
            player_setStat(1, 10, 1);
        }
    }
    msg.actor = obj;
    msg.uniqueId = obj->unique_id;
    effect_broadcastEvent((s32)&msg, 3);
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
        actor_playAnim(obj, sound, 1.0f, 3.0f, 0.0f, 0.0f, flag);
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
        saved = gCurrentActor;
        gCurrentActor = obj;
        func_1504BAF0(obj);
        gCurrentActor = saved;
    }
    if (mode == 0 || mode == 3) {
        obj->mv->death_State = 1;
    } else {
        actor_startDeathCountdown(obj, 0);
    }
    if (mode == 2) {
        obj->mv->death_State = 11;
    } else if (mode == 3) {
        actor_finishDeath(slot);
    }
    if (slot == 0 && level == 4) {
        r = math_random();
        f = math_randomFloat();
        particle_startScreenBurst(r % 3 + 4, f * 0.1f + 0.3f, 0xFF, 0);
    }
}

/* Forwards four arguments to script_command(3, ...) keyed by object slot's unk13F | 0x2000. */
void func_1507D158(s32 slot, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 value = movement_obj[slot].listpos;

    script_command(3, value | 0x2000, arg1, arg2, arg3, arg4);
}

/* Re-initialises obj through actor_remove/collision_spawnObject (health back to gDefaultHealth), keeping a few state bytes. */
void actor_respawn(Actor *obj) {
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
    if (gIsMultiplayer != 0) {
        player = obj->mv->spawnIndex;
    } else {
        player = -1;
    }
    if (level == 0x2B) {
        slot = obj - movement_obj;
        value = player_getStat(slot, 1);
        if (value >= 0x11 && value < 0x21) {
            choice = (math_random() & 0xF) + 0x11;
            player_setStat(slot, 0, choice);
            if (choice < 0x1D) {
                gSpawnRecords[slot].type = 0x9C;
            } else {
                gSpawnRecords[slot].type = 0x9D;
            }
        }
    }
    slot = obj - movement_obj;
    actor_remove(obj, 0);
    D_800D18A8 = 1;
    collision_spawnObject(&gSpawnRecords[slot], slot, 0, 0, 0);
    obj->mv->lastSpawnIndex = saved1B0;
    obj->mv->unk1AF = saved1AF;
    obj->unk328 = saved328;
    if (gScreenLayout >= slot) {
        camera_resetView(&camera_global_Cameras[slot], slot, level, player);
        screen_openIris(slot);
        screen_startViewFade(0, 0, 0, 0, 1, slot);
        screen_startViewFade(0, 0, 0, 30, 0, slot);
    } else {
        player_moveToSpawn(obj, player, 0);
    }
    D_800D18A8 = 0;
    obj->mv->death_State = 0;
    obj->mv->respawnReady = FALSE;
    obj->mv->deathTimer = 0;
    obj->mv->shotsfired = saved1AA;
    obj->energy = gDefaultHealth;
    obj->unk22C |= 0x100;
    obj->mv->context_b = 0;
    obj->unk20F = 0xFF;
    obj->unk210 = 0xFF;
    obj->unk211 = 0xFF;
    obj->intelwalk = 1;
    obj->scriptCmd = NULL;
    obj->mv->spawnIndex = player;
    if (gIsMultiplayer != 0) {
        obj->imune = 150;
        obj->mv->unk1B1 = obj->imune;
        if (level == 0x33) {
            func_1507D158(slot, 120, 0, 0, 0);
            mp_placeAtStart(obj);
        }
    }
}

/* Ends slot's current run: re-initialises it if player_getStat(slot, 3) allows, else retires it (state 10). */
void actor_finishDeath(s32 slot) {
    Actor *obj;
    ActorCtl *state;

    if (!(D_8008FDBC & 1)) {
        player_setStat(slot, 4, 1);
    }

    if (player_getStat(slot, 3) != 0) {
        player_setStat(slot, 5, gDefaultHealth);
        obj = &movement_obj[slot];
        obj->underwaterTime = 0;

        if (gIsMultiplayer != 0) {
            if (gMatchResult == 0) {
                actor_respawn(obj);
                return;
            }
            obj->mv->death_State = 10;
            return;
        }

        D_800D18A8 = 1;
        if (!(gTaskDoneBits[0x19] & 4) && gCurrentSaveSlot >= 0) {
            game_requestLevelChange(1, 0x22, 0, 0, 0);
            return;
        }
        game_requestLevelChange(2, D_800BE3DF, D_800BE3E0, 0, 0);
        return;
    }

    if (gIsMultiplayer == 0) {
        gAutoSavePending = 1;
        script_autoSave();
        D_800D18A8 = 1;
        player_setStat(slot, 5, gDefaultHealth);
        player_setStat(slot, 2, D_80087260);
        game_requestLevelChange(1, 0x18, 0, 0, 0);
    } else {
        gEliminatedPlayers |= 1 << slot;
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
s32 actor_runDeathEffect(Actor *obj) {
    WorldObject *spawned;
    void *handle;

    switch (obj->character_type) {
    case 0x7F:
        particle_spawnObjDebrisBurst(obj, 0xFF, 1);
        break;
    case 0x10:
    case 0x38:
    case 0x91:
        func_150BFFE0(obj);
        break;
    case 0x3A:
    case 0x3D:
        attach_spawnFootImpact(obj);
        break;
    case 0x4A:
        beam_spawnActorExplosion(obj, 0xFF, 1);
        break;
    case 0x47:
        sight_explodeAtActor(obj, 0xFF, 1);
        break;
    case 0x25:
        if (obj->taskint[2] != 0 || level == 6 || level == 0x3B) {
            trail_breakApart(obj, 0xFF, 1);
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
        actor_breakIntoParts(obj, 0);
        effect_spawnAttachedBursts(obj);
        return 1;
    case 0x53:
        if (gIsMultiplayer != 0) {
            actor_kill(obj, 10);
            return 2;
        }
        break;
    case 0x90:
        if (gIsMultiplayer != 0 && level == 0x33) {
            actor_kill(obj, 8);
            return 2;
        }
        break;
    case 0x28:
    case 0x77:
        spawned = NULL;
        effect_spawnTankExplosion(obj, 0xFF, 1);
        func_15145A50(obj);
        if (gIsMultiplayer != 0) {
            actor_kill(obj, 6);
            spawned = gfx_findObjectById((obj->playernumber + 11));
        } else {
            if (level == 0x23) {
                spawned = gfx_findObjectById(1);
            }
            handle = event_getFirstLiveOwnedObject(obj);
            if (handle != NULL && gCutsceneStates[0] != 1) {
                blood_spawnHitEffects(handle, 1, 1, 1, 1, 0, 0xFF, 1);
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
        if (gIsMultiplayer != 0) {
            return 2;
        }
        break;
    case 0x0D:
        effect_spawnBreakUp(obj, 0xFF, 1);
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

void actor_breakIntoParts(Actor *obj, s32 arg1) {
    model_breakIntoParts(obj, arg1, -1);
}

void func_1507DB64(void) {
}

/* Sets up the textured square of func_1507DB6C. */
static Gfx D_80086B80[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};
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
    texture = texture_getAddress((s32)&D_D0F, NULL, 3, 0);
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
            screen_openIris(player);
            screen_startViewFade(0, 0, 0, 120, 0, player);
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
    view = &((RenderView *)gViews)[player]; // matching: the no-op cast changes the code
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


/* Sets bits in obj->unk9C (after re-running collision_removePartSet 0x44/0x23 when type is 1). */
void func_1507DE4C(Actor *obj) {
    if (obj->type == 1) {
        collision_removePartSet(obj, 0x44);
        collision_removePartSet(obj, 0x23);
        obj->unk9C |= 0xF000;
        collision_removePartSet(obj, 0x44);
        collision_removePartSet(obj, 0x23);
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

/* Looks up resource's position and facing via player_getSpawnPoint and calls cutscene_start(0x25, ...) from there. */
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

    if (gCutsceneStates[0] != 1) {
        player_getSpawnPoint(0, resource, 1, pos, &vec, &unk4F, &angle, &unk44, &unk48, 1, &type);
        yaw = (angle - 64) * (360.0f / 256.0f) + 180.0f;
        if (type == 0) {
            kind = 1;
        } else if (type == 1) {
            kind = 3;
        } else {
            return;
        }
        cutscene_setAnchor(pos[0], pos[1], pos[2], yaw);
        savedMode = level;
        level = 0x25;
        D_800C3671 = 1;
        cutscene_start(0x25, kind, 0, 0, 0);
        D_800C3670 = 1;
        level = savedMode;
    }
}

/* Returns 0, or 1 + the type player_getSpawnPoint reports for D_800BE3E0 (after setting gNextLevel from D_800BE3DF). */
s32 func_1507E114(s32 *arg0) {
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
    mode = gNextLevel;
    if (mode == 0x22 || mode == 0x18) {
        return 0;
    }
    gNextLevel = D_800BE3DF;
    player_getSpawnPoint(0, D_800BE3E0, 1, pos, &vec, &unk43, &unk42, &unk44, &unk3C, 1, &type);
    return type + 1;
}

/* Gets obj's tracking point: 30 * scale_y up through bone 0 (12 or 3 in some states), or its position. */
void actor_getTrackPoint(Actor *obj, f32 *x, f32 *y, f32 *z) {
    if (obj->ob_matrix_ptr != NULL) {
        MtxF *bone;
        Vec3f in;
        Vec3f out;

        in.x = 0.0f;
        in.y = obj->scale_y * 30.0f;
        in.z = 0.0f;
        bone = obj->ob_matrix_ptr;
        if (obj->type == 1) {
            bone += 12;
        } else if (obj->type == 0x1E) {
            bone += 3;
        }
        math_transformOffset(&in, &out, bone);
        *x = out.x;
        *y = out.y;
        *z = out.z;
    } else {
        *x = obj->xpos;
        *y = obj->ypos;
        *z = obj->zpos;
    }
}
