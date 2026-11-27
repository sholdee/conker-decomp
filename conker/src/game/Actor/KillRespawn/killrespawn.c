#include <ultra64.h>

/* This file's .bss, defined before the headers declare the globals (see init/Sched/SchedInit/schedinit.c). */
u16 gEliminatedPlayers;
u16 gTakeoverPlayers;
u8 gEliminatedSquareSizes[4]; // per player: half-size of the square, minus 4
u8 gRespawnPending;
u8 gLastDeathKind;

#include "conker/actor.h"
#include "conker/attach.h"
#include "conker/blood.h"
#include "conker/camera.h"
#include "conker/collision.h"
#include "conker/cont.h"
#include "conker/cutscene.h"
#include "conker/effect.h"
#include "conker/event.h"
#include "conker/game.h"
#include "conker/gfx.h"
#include "conker/model.h"
#include "conker/mp.h"
#include "conker/particle.h"
#include "conker/player.h"
#include "conker/prop.h"
#include "conker/save.h"
#include "conker/screen.h"
#include "conker/script.h"
#include "conker/sight.h"
#include "conker/texture.h"
#include "conker/trail.h"
#include "conker/view.h"

/* Puts actor into state 2 with the given timer. */
void actor_startDeathCountdown(Actor *actor, s32 timer) {
    actor->mv->death_State = 2;
    actor->mv->deathTimer = timer;
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
        for (i = 0; i < gPlayerCount; i++, player++) {
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
                if (gIsMultiplayer != 0 && (gTakeoverPlayers & (1 << i))) {
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
                        player->mv->deathTimer -= gFrameTicks;
                    } else {
                        actor_closeDeathIris(player);
                    }
                } else if (player->mv->death_State == 7) {
                    if (player->mv->deathTimer > 0) {
                        player->mv->deathTimer -= gFrameTicks;
                    } else {
                        player->mv->death_State = 8;
                        actor_fadeCommand(player, 2, 0, 60, 0);
                    }
                }
                player->mv->deathTime += gFrameTicks;
                if ((gButtonsPressed[i] & 0x8000) || (gIsMultiplayer != 0 && player->mv->bot == 0) ||
                    player_getStat(i, 3) < 2 || (player->mv->bot == 1 && player->mv->deathTime >= 181)) {
                    player->mv->respawnReady = TRUE;
                }
                if (gMatchFlags & 0x80) {
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
                    player->mv->deathTimer -= gFrameTicks;
                } else {
                    player->mv->death_State = 7;
                }
            } else {
                player->mv->deathTime += gFrameTicks;
                if (player->alpha == 0xFF && player->mv->deathTime >= 161) {
                    actor_fadeCommand(player, 2, 0, 60, 0);
                }
                if (player->mv->deathTime >= 181 && player->mv->death_State != 10) {
                    actor_finishDeath(i);
                }
            }
        }
        gMatchFlags &= ~0x80;
    }
}

/* Puts actor into state 3 and, for a local player slot, calls screen_closeIris on it. */
void actor_closeDeathIris(Actor *actor) {
    s32 slot = actor - movement_obj;

    actor->mv->death_State = 3;

    if (slot <= gScreenLayout) {
        screen_closeIris(slot);
    }
}

/* Kills actor with the given cause: plays its sound, resets movement and moves it to a dead state. */
void actor_kill(Actor *actor, s32 cause) {
    f32 f;
    s32 sound;
    u32 r;
    s32 mode;
    s32 slot;
    ActorRef msg;
    Actor *saved;
    s32 again;
    s32 flag;

    if (actor->mv == NULL) {
        return;
    }
    slot = actor - movement_obj;
    if (slot >= gPlayerCount) {
        return;
    }
    if (actor->mv->death_State != 0) {
        if (cause != 9 || cause == actor->mv->unk121) {
            return;
        }
        again = TRUE;
    } else {
        again = FALSE;
    }
    actor->mv->unk121 = cause;
    if (gIsMultiplayer != 0 && !again) {
        if (slot < 16) {
            D_800E0AD0[slot]++;
        }
        if ((gMatchFlags & 0x20) && actor->team == 0) { // byte at 0x128 (Actor has a u32 pad there)
            player_setStat(1, 10, 1);
        }
    }
    msg.actor = actor;
    msg.uniqueId = actor->unique_id;
    effect_broadcastEvent((s32)&msg, 3);
    gLastDeathKind = 0;
lbl2: // matching: unused label splits the basic block; as1 then hoists the reload of cause above this store
    sound = -1;
    flag = 0;
    mode = 0;
    if (actor->mv->on_fire) {
        gLastDeathKind = 2;
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
        gLastDeathKind = 1;
        break;
    case 4:
        sound = 0x24;
        mode = 1;
        gLastDeathKind = 1;
        break;
    case 5:
        sound = 0x24;
        mode = 1;
        gLastDeathKind = 3;
        break;
    case 6:
        if (actor->type == 1 || actor->type == 0x15 || actor->drawtype == 3 ||
            actor->attach_char != 0) {
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
        gLastDeathKind = 3;
        break;
    case 15:
        sound = 0x170;
        mode = 3;
        gLastDeathKind = 3;
        break;
    case 9:
        mode = 2;
        break;
    case 10:
        sound = 0x2F;
        gLastDeathKind = 3;
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
        actor_playAnim(actor, sound, 1.0f, 3.0f, 0.0f, 0.0f, flag);
    }
    actor->energy = 0;
    actor->imune = 0xFF;
    actor->stunned = 0xFF;
    actor->norun = 0xFF;
    actor->noshove |= 0x200;
    actor->bodyRadius = 0;
    actor->bodyHeight = 0;
    actor->speed = 0.0f;
    actor->mv->bitten = 0;
    if (actor->lifting != 0) {
        saved = gCurrentActor;
        gCurrentActor = actor;
        actor_dropLiftedObject(actor);
        gCurrentActor = saved;
    }
    if (mode == 0 || mode == 3) {
        actor->mv->death_State = 1;
    } else {
        actor_startDeathCountdown(actor, 0);
    }
    if (mode == 2) {
        actor->mv->death_State = 11;
    } else if (mode == 3) {
        actor_finishDeath(slot);
    }
    if (slot == 0 && level == LEVEL_W2_OUTSIDE_MOLEHOLE) {
        r = math_random();
        f = math_randomFloat();
        particle_startScreenBurst(r % 3 + 4, f * 0.1f + 0.3f, 0xFF, 0);
    }
}

/* Sends script command listpos | 0x2000 of actor `slot` with argument `arg` and three extra words (script_command). */
void func_1507D158(s32 slot, s32 arg, s32 extra0, s32 extra1, s32 extra2) {
    s32 value = movement_obj[slot].listpos;

    script_command(3, value | 0x2000, arg, extra0, extra1, extra2);
}

/* Re-initialises actor through actor_remove/collision_spawnObject (health back to gDefaultHealth), keeping a few state bytes. */
void actor_respawn(Actor *actor) {
    s32 value;
    s32 saved1AA;
    s8 player;
    s8 saved1B0;
    s8 saved328;
    s8 saved1AF;
    s32 choice;
    s32 slot;

    saved1AA = actor->mv->shotsfired;
    saved1B0 = actor->mv->lastSpawnIndex;
    saved328 = actor->radarColor;
    saved1AF = actor->mv->unk1AF;
    if (gIsMultiplayer != 0) {
        player = actor->mv->spawnIndex;
    } else {
        player = -1;
    }
    if (level == LEVEL_WMP_BEACH) {
        slot = actor - movement_obj;
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
    slot = actor - movement_obj;
    actor_remove(actor, 0);
    gRespawnPending = 1;
    collision_spawnObject(&gSpawnRecords[slot], slot, 0, 0, 0);
    actor->mv->lastSpawnIndex = saved1B0;
    actor->mv->unk1AF = saved1AF;
    actor->radarColor = saved328;
    if (gScreenLayout >= slot) {
        camera_resetView(&camera_global_Cameras[slot], slot, level, player);
        screen_openIris(slot);
        screen_startViewFade(0, 0, 0, 0, 1, slot);
        screen_startViewFade(0, 0, 0, 30, 0, slot);
    } else {
        player_moveToSpawn(actor, player, 0);
    }
    gRespawnPending = 0;
    actor->mv->death_State = 0;
    actor->mv->respawnReady = FALSE;
    actor->mv->deathTimer = 0;
    actor->mv->shotsfired = saved1AA;
    actor->energy = gDefaultHealth;
    actor->unk22C |= 0x100;
    actor->mv->context_b = 0;
    actor->pathNode = 0xFF;
    actor->prevPathNode = 0xFF;
    actor->pathGoal = 0xFF;
    actor->intelwalk = 1;
    actor->scriptCmd = NULL;
    actor->mv->spawnIndex = player;
    if (gIsMultiplayer != 0) {
        actor->imune = 150;
        actor->mv->unk1B1 = actor->imune;
        if (level == LEVEL_WMP_VOLCANO_RACE) {
            func_1507D158(slot, 120, 0, 0, 0);
            mp_placeAtStart(actor);
        }
    }
}

/* Ends slot's current run: re-initialises it if player_getStat(slot, 3) allows, else retires it (state 10). */
void actor_finishDeath(s32 slot) {
    Actor *actor;
    ActorCtl *state;

    if (!(gMatchFlags & 1)) {
        player_setStat(slot, 4, 1);
    }

    if (player_getStat(slot, 3) != 0) {
        player_setStat(slot, 5, gDefaultHealth);
        actor = &movement_obj[slot];
        actor->underwaterTime = 0;

        if (gIsMultiplayer != 0) {
            if (gMatchResult == 0) {
                actor_respawn(actor);
                return;
            }
            actor->mv->death_State = 10;
            return;
        }

        gRespawnPending = 1;
        if (!(gTaskDoneBits[0x19] & 4) && gCurrentSaveSlot >= 0) {
            game_requestLevelChange(1, LEVEL_WFRONTEND_HELL, 0, 0, 0);
            return;
        }
        game_requestLevelChange(2, gCheckpointLevel, gCheckpointSpawn, 0, 0);
        return;
    }

    if (gIsMultiplayer == 0) {
        gAutoSavePending = 1;
        script_autoSave();
        gRespawnPending = 1;
        player_setStat(slot, 5, gDefaultHealth);
        player_setStat(slot, 2, D_80087260);
        game_requestLevelChange(1, LEVEL_W5_THRONE_ROOM, 0, 0, 0);
    } else {
        gEliminatedPlayers |= 1 << slot;
    }

    actor = &movement_obj[slot];
    state = actor->mv;
    if (state->bot == 0) {
        // matching: the volatile store and load make IDO reload mv after the decrement, as the original does
        *(volatile s8 *)&gHumansRemaining = gHumansRemaining - 1;
        state = *(ActorCtl *volatile *)&actor->mv;
    }
    state->death_State = 10;
    D_800BE618--;
}


/* Death handler: dispatches on actor->id; returns 1 or 2 when the object was handled specially, else 0. */
/* Runs the per-id handler for actor (mostly id handler(actor, 0xFF, 1)); returns 1 or 2 for ids handled specially, else 0. */
s32 actor_runDeathEffect(Actor *actor) {
    WorldObject *spawned;
    void *handle;

    switch (actor->character_type) {
    case 0x7F:
        particle_spawnObjDebrisBurst(actor, 0xFF, 1);
        break;
    case 0x10:
    case 0x38:
    case 0x91:
        func_150BFFE0(actor);
        break;
    case 0x3A:
    case 0x3D:
        attach_spawnFootImpact(actor);
        break;
    case 0x4A:
        effect_spawnActorExplosion(actor, 0xFF, 1);
        break;
    case 0x47:
        sight_explodeAtActor(actor, 0xFF, 1);
        break;
    case 0x25:
        if (actor->taskint[2] != 0 || level == LEVEL_THE_WINDMILL || level == LEVEL_W1_WINDMILL_EXPLODED) {
            trail_breakApart(actor, 0xFF, 1);
        }
        return 0;
    case 0x79:
        func_150BE494(actor, 0xFF, 1);
        break;
    case 0x05:
    case 0xAD:
    case 0xAE:
    case 0xAF:
        actor->radarColor = 0;
        actor_breakIntoParts(actor, 0);
        effect_spawnAttachedBursts(actor);
        return 1;
    case 0x53:
        if (gIsMultiplayer != 0) {
            actor_kill(actor, 10);
            return 2;
        }
        break;
    case 0x90:
        if (gIsMultiplayer != 0 && level == LEVEL_WMP_VOLCANO_RACE) {
            actor_kill(actor, 8);
            return 2;
        }
        break;
    case 0x28:
    case 0x77:
        spawned = NULL;
        effect_spawnTankExplosion(actor, 0xFF, 1);
        func_15145A50(actor);
        if (gIsMultiplayer != 0) {
            actor_kill(actor, 6);
            spawned = gfx_findObjectById((actor->playernumber + 11));
        } else {
            if (level == LEVEL_W2_WARBOSS) {
                spawned = gfx_findObjectById(1);
            }
            handle = event_getFirstLiveOwnedObject(actor);
            if (handle != NULL && gCutsceneStates[0] != 1) {
                blood_spawnHitEffects(handle, 1, 1, 1, 1, 0, 0xFF, 1);
            }
        }
        if (spawned != NULL) {
            spawned->inactive = 0;
            spawned->x = actor->xpos;
            spawned->y = actor->ypos;
            spawned->z = actor->zpos;
            spawned->t = actor->vtilt;
            spawned->u = actor->theta;
            spawned->v = actor->lean_angle;
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
        effect_spawnBreakUp(actor, 0xFF, 1);
        break;
    case 0x20:
        if (actor->yspd < -20.0f) {
            func_150BC488(actor);
        } else {
            func_150BBB5C(actor);
        }
        break;
    case 0x4F:
    case 0x83:
        func_151A6C90(actor, 0xFF, 1);
        break;
    case 0x34:
        func_150E0870(actor, 0xFF, 1);
        break;
    case 0x44:
        effect_spawnDeathDebrisBurst(actor, 0xFF, 1);
        break;
    }
    return 0;
}

void func_1507DB34(void) {
}

void func_1507DB3C(void) {
}

void actor_breakIntoParts(Actor *actor, s32 kind) {
    model_breakIntoParts(actor, kind, -1);
}

void func_1507DB64(void) {
}

/* Sets up the textured square of func_1507DB6C. */
static Gfx gEliminatedSquareDl[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPEndDisplayList(),
};

/*
 * Draws a 32x32 RGBA32 textured square centred on player's view (coordinates in 10.2 fixed point). While the
 * player's gTakeoverPlayers bit is set the square shrinks each frame and, once gone, the bit is cleared; otherwise it
 * grows back to its full size.
 */
Gfx *func_1507DB6C(Gfx *gfx, s32 player) {
    s32 texture;
    s32 size;
    RenderView *view;
    s32 right, left, bottom, top;
    s32 scale;

    gSPDisplayList(gfx++, gEliminatedSquareDl);
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
    if (gTakeoverPlayers & (1 << player)) {
        if (gEliminatedSquareSizes[player] >= 4) {
            gEliminatedSquareSizes[player] -= 4;
        } else {
            gTakeoverPlayers &= ~(1 << player);
            screen_openIris(player);
            screen_startViewFade(0, 0, 0, 120, 0, player);
            return gfx;
        }
    } else {
        gEliminatedSquareSizes[player] += 4;
        if (gEliminatedSquareSizes[player] > 60) {
            gEliminatedSquareSizes[player] = 60;
        }
    }
    size = gEliminatedSquareSizes[player] + 4;
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


/* Sets bits in actor->hiddenBones (after re-running collision_removePartSet 0x44/0x23 when type is 1). */
void actor_applyHeadShot(Actor *actor) {
    if (actor->type == 1) {
        collision_removePartSet(actor, 0x44);
        collision_removePartSet(actor, 0x23);
        actor->hiddenBones |= 0xF000;
        collision_removePartSet(actor, 0x44);
        collision_removePartSet(actor, 0x23);
        return;
    }

    switch (actor->character_type) {
    case 0x9F:
    case 0xA0:
        actor->hiddenBones |= 0xF000;
        break;
    case 0x5A:
    case 0x74:
    case 0x7A:
        actor->hiddenBones |= 0xFF8;
        break;
    }
}

/* Sets actor's hiddenParts/hiddenBones flags and taskint[0] (1, 2, 4 or 8) for part 4..9. */
void actor_applyPartHit(Actor *actor, s32 part) {
    switch (part) {
    case 9:
        actor->hiddenParts |= 0x20;
        actor->hiddenBones |= 0x78;
        actor->taskint[0] = 1;
        return;
    case 8:
        actor->hiddenParts |= 0x40;
        actor->hiddenParts &= ~0x200;
        actor->hiddenBones |= 0xF00;
        actor->taskint[0] = 2;
        return;
    case 6:
    case 7:
        actor->hiddenParts |= 0xE;
        actor->hiddenParts &= ~0x410;
        actor->hiddenBones |= 0xEE0000;
        actor->taskint[0] = 4;
        return;
    case 4:
    case 5:
        actor->hiddenParts |= 0x80;
        actor->hiddenParts &= ~0x500;
        actor->taskint[0] = 8;
        return;
    }
}

/* Looks up resource's position and facing via player_getSpawnPoint and calls cutscene_start(LEVEL_GENERIC_CUTSCENES, ...) from there. */
void actor_startSpawnCutscene(s32 resource, s32 arg1) {
    s32 savedLevel;
    s16 pos[3];
    Vec3f cameraEye;
    u8 tilt;
    u8 angle;
    s32 doorWalkTime;
    s32 unk14;
    f32 yaw;
    s32 kind;
    s32 type;

    if (gCutsceneStates[0] != 1) {
        player_getSpawnPoint(0, resource, 1, pos, &cameraEye, &tilt, &angle, &unk14, &doorWalkTime, 1, &type);
        yaw = (angle - 64) * (360.0f / 256.0f) + 180.0f;
        if (type == 0) {
            kind = 1;
        } else if (type == 1) {
            kind = 3;
        } else {
            return;
        }
        cutscene_setAnchor(pos[0], pos[1], pos[2], yaw);
        savedLevel = level;
        level = LEVEL_GENERIC_CUTSCENES;
        gSpawnCutsceneActive = 1;
        cutscene_start(LEVEL_GENERIC_CUTSCENES, kind, 0, 0, 0);
        gCutsceneIgnorePlayed = 1;
        level = savedLevel;
    }
}

/* Returns 0, or 1 + the type player_getSpawnPoint reports for gCheckpointSpawn (after setting gNextLevel from gCheckpointLevel). */
s32 actor_resumeAtCheckpoint(s32 *arg0) {
    s32 type;
    s16 pos[3];
    Vec3f cameraEye;
    s32 unk14;
    u8 tilt;
    u8 angle;
    s32 doorWalkTime;
    s32 next;

    if (gRespawnPending == 0) {
        return 0;
    }
    next = gNextLevel;
    if (next == LEVEL_WFRONTEND_HELL || next == LEVEL_W5_THRONE_ROOM) {
        return 0;
    }
    gNextLevel = gCheckpointLevel;
    player_getSpawnPoint(0, gCheckpointSpawn, 1, pos, &cameraEye, &tilt, &angle, &unk14, &doorWalkTime, 1, &type);
    return type + 1;
}

/* Gets actor's tracking point: 30 * scale_y up through bone 0 (12 or 3 in some states), or its position. */
void actor_getTrackPoint(Actor *actor, f32 *x, f32 *y, f32 *z) {
    if (actor->ob_matrix_ptr != NULL) {
        MtxF *bone;
        Vec3f in;
        Vec3f out;

        in.x = 0.0f;
        in.y = actor->scale_y * 30.0f;
        in.z = 0.0f;
        bone = actor->ob_matrix_ptr;
        if (actor->type == 1) {
            bone += 12;
        } else if (actor->type == 0x1E) {
            bone += 3;
        }
        math_transformOffset(&in, &out, bone);
        *x = out.x;
        *y = out.y;
        *z = out.z;
    } else {
        *x = actor->xpos;
        *y = actor->ypos;
        *z = actor->zpos;
    }
}
