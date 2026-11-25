#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// Camera path node (D_800D2350); links are node indices, 0xFF = none.
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 pad6[0x3];
    /* 0x9 */ u8 links[5];
    /* 0xE */ u8 padE[0x2];
} CamPathNode;

// Camera point table entry (table base in D_800DC020).
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 pad6[0x12];
} CamPoint;

extern CamPathNode *D_800D2350;
extern CamPoint *D_800DC024;

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
Actor *func_1505EEF4(s32 arg0);
s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15124C38(Camera *arg0, s32 arg1);
void func_151254F4(Camera *arg0, s32 arg1);
void func_151287E0(Camera *arg0, f32 *arg1, f32 *arg2);
void func_1512DD50(Camera *arg0);
void func_1512E140(Camera *arg0);
void func_1512FE00(Camera *camera, s32 action);

// Runs one frame of the camera's current command; cmd.type selects the behaviour.
void func_1512E8E0(Camera *camera) {
    CamCmd *cmd;
    CamCmdState *cam;

    if (camera->mode & 0x100) {
        return;
    }
    if (camera->aimState != 0) {
        return;
    }
    func_151239CC((Camera *)camera, 13);
    cmd = &camera->cmd;
    cam = &camera->cmdState;
    if (camera->mode & 0x40000) {
        return;
    }
    if (cam->cmdId != cmd->id) {
        camera->unk92C = 0;
        cam->cmdId = cmd->id;
        cam->unk6 = 0;
        func_151239CC((Camera *)camera, 12);
        func_151239CC((Camera *)camera, 13);
        func_151239CC((Camera *)camera, 2);
        func_151239CC((Camera *)camera, 7);
    }
    if ((cmd->type != 4) && (camera->playerExt->unk120 != 0) && (camera->unk92C == 0)) {
        cam->unk6 = 1;
    }
    if (cam->unk6 != 0) {
        func_151239CC((Camera *)camera, 12);
    }
    if ((camera->cmdState.unk0 != 0) && (cam->unk6 == 0) && (cam->blending == 0)) {
        cam->lastType = cmd->type;
        if (cmd->type != 10) {
            func_1512FE00(camera, cmd->unk8);
        }
        if (cmd->type == 4) {
            camera->unk272 = -1;
            camera->cmdRunning = 1;
            camera->savedPreset[7] = camera->preset;
            camera->savedPresetMask[7] = camera->presetMask;
            return;
        }
        if (cmd->type == 7) {
            f32 dx;
            s32 i;
            f32 minDist;
            struct17 toCamera;
            struct17 dir;
            struct17 offset;
            struct17 nodePos;
            struct17 linkPos;
            struct17 best;
            struct17 closest;
            f32 dy;
            CamPathNode *node;
            f32 dz;

            // Find the closest point to the camera on the path segments leaving its nearest node.
            node = &D_800D2350[func_15085DF8(camera->eye.x, camera->eye.y, camera->eye.z, 7, 0)];
            minDist = 1e14f;
            if (camera->mode & 0x100) {
                return;
            }
            nodePos.unk0 = node->x;
            nodePos.unk4 = node->y;
            nodePos.unk8 = node->z;
            func_15048F90(&nodePos, (struct17 *)&camera->eye, &toCamera);
            for (i = 0; i < 5; i++) {
                // matching: s32 link read back through (u8) adds a live range uopt needs for the frame layout
                s32 link;

                link = node->links[i];
                if ((u8)link != 0xFF) {
                    linkPos.unk0 = D_800D2350[(u8)link].x;
                    linkPos.unk4 = D_800D2350[(u8)link].y;
                    linkPos.unk8 = D_800D2350[(u8)link].z;
                    func_150491EC(&nodePos, &linkPos, &dir);
                    func_15049148(&dir, func_150AD900(&dir.unk0, &toCamera.unk0), &offset);
                    func_15048F20(&nodePos, &offset, &closest);
                    dx = camera->eye.x - closest.unk0;
                    dy = camera->eye.y - closest.unk4;
                    dz = camera->eye.z - closest.unk8;
                    // matching: dy/dz stay dead and the y/z terms are spelled out; the extra CSE temps size the frame
                    if (((dx * dx) + ((camera->eye.y - closest.unk4) * (camera->eye.y - closest.unk4)) + ((camera->eye.z - closest.unk8) * (camera->eye.z - closest.unk8))) < minDist) {
                        minDist = (dx * dx) + ((camera->eye.y - closest.unk4) * (camera->eye.y - closest.unk4)) + ((camera->eye.z - closest.unk8) * (camera->eye.z - closest.unk8));
                        best = closest;
                    }
                }
            }
            if (func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 12) != 0) {
                camera->cmdRunning = 1;
                camera->flags &= ~0x80000006;
                camera->pathFlags = cmd->unk8;
                camera->unk924 = cmd->unk1C;
                camera->unk928 = cmd->unk20;
                if (camera->pathFlags & 1) {
                    camera->flags |= 4;
                }
                if (camera->pathFlags & 2) {
                    camera->flags |= 2;
                }
                if (camera->pathFlags & 7) {
                    camera->flags |= 0x20000;
                }
                camera->unk92C = 1;
            }
            if (camera->pathFlags & 0x20) {
                camera->flags |= 8;
            } else {
                camera->flags &= ~8;
            }
            camera->unk910 = 1.5f;
            camera->unk914 = 2.5f;
            if ((camera->pathFlags & 2) && (camera->contPad->button & 7)) {
                camera->flags |= 8;
                camera->unk910 *= 0.25f;
                camera->unk914 *= 0.25f;
            } else if (camera->pathFlags & 0x40) {
                if (camera->player->xz_velocity > 30.0f) {
                    camera->unk910 *= 7.0f;
                    camera->unk914 *= 7.0f;
                } else {
                    camera->unk910 *= 4.0f;
                    camera->unk914 *= 4.0f;
                }
                camera->flags |= 0x20000;
            }
            if (camera->snap != 0) {
                camera->unk918 = 0.0f;
                camera->unk908 = camera->unk910;
                camera->unk91C = 0.0f;
                camera->unk90C = camera->unk914;
            } else {
                func_150495B0(&camera->unk908, camera->unk910, &camera->unk918, 1.5f, 2.0f, camera->dt);
                func_150495B0(&camera->unk90C, camera->unk914, &camera->unk91C, 1.5f, 2.0f, camera->dt);
            }
            if (!(camera->pathFlags & 0x10)) {
                if (camera->snap != 0) {
                    camera->unk8F4 = 0.0f;
                    camera->unk904 = 0.0f;
                    camera->eye.x = best.unk0;
                    camera->eye.z = best.unk8;
                } else {
                    func_150495B0(&camera->eye.x, best.unk0, &camera->unk8F4, camera->unk908, camera->unk90C, camera->dt);
                    func_150495B0(&camera->eye.z, best.unk8, &camera->unk904, camera->unk908, camera->unk90C, camera->dt);
                }
            }
            if (!(camera->pathFlags & 8)) {
                if (camera->snap != 0) {
                    camera->eye.y = best.unk4;
                    camera->unk8FC = 0.0f;
                } else {
                    func_150495B0(&camera->eye.y, best.unk4, &camera->unk8FC, camera->unk908, camera->unk90C, camera->dt);
                }
            }
            func_15125594((Camera *)camera);
            camera->unk924 = cmd->unk1C;
            camera->unk928 = cmd->unk20;
            camera->targetDistance = camera->unk924;
            camera->height = camera->unk928;
            if (camera->pathFlags & 4) {
                camera->unk734 = cmd->id | 0x4000;
                camera->unk738 = 1;
            }
            return;
        }
        if (cmd->type == 5) {
            if (func_15123934((Camera *)camera, 0x40, 0, 0, 12) != 0) {
                camera->flags &= ~0xE;
                camera->flags |= 0x200;
                camera->unk27C = camera->eye;
            }
            if ((cmd->unk8 == 5) || (cmd->unk8 == 0x11)) {
                if (camera->unk272 != cmd->unk1C) {
                    camera->unk272 = cmd->unk1C;
                    D_800DC024 = (CamPoint *)D_800DC020;
                    D_800DC024 = &D_800DC024[camera->unk272];
                    camera->unk278 = cmd->unk20;
                    camera->unk288.x = D_800DC024->x;
                    camera->unk288.y = D_800DC024->y;
                    camera->unk288.z = D_800DC024->z;
                    camera->status |= 4;
                    camera->eye.x = camera->unk288.x;
                    camera->unk27C.x = camera->unk288.x;
                    camera->eye.y = camera->unk288.y;
                    camera->unk27C.y = camera->unk288.y;
                    camera->eye.z = camera->unk288.z;
                    camera->unk27C.z = camera->unk288.z;
                    func_15128774((Camera *)camera, camera->player);
                    camera->snap = 1;
                }
            } else {
                camera->unk272 = cmd->unk1C;
                D_800DC024 = (CamPoint *)D_800DC020;
                D_800DC024 = &D_800DC024[camera->unk272];
                camera->unk288.x = D_800DC024->x;
                camera->unk288.y = D_800DC024->y;
                camera->unk288.z = D_800DC024->z;
                camera->unk278 = cmd->unk20;
            }
            camera->unk274 = cmd->unk8;
            camera->cmdRunning = 1;
            return;
        }
        if (cmd->type == 11) {
            cam->minHeight = cmd->unk1C;
            cam->maxHeight = cmd->unk20;
            cam->maxDistance = cmd->unk8;
            return;
        }
        if ((cmd->type == 10) || (cmd->type == 14)) {
            Actor *obj;

            // matching: reusing obj for the id (instead of nesting the calls) drops a cfe temp
            obj = (Actor *)func_15083E0C(cmd->unk1C);
            obj = func_1505EEF4((s32)obj);
            if (obj != NULL) {
                f32 dist;

                if (func_15123934((Camera *)camera, 0x400, 0, camera->unk134, 2) != 0) {
                    if (cmd->type == 14) {
                        func_151254F4((Camera *)camera, obj - D_800CC2D0);
                    }
                    camera->flags &= ~8;
                    camera->unk628 = 0;
                    camera->unk614 = &camera->unk618;
                    camera->cmdRunning = 1;
                    camera->unk62C = 0.0f;
                    camera->unk5DC = 0.0f;
                    dist = sqrtf(((camera->eye.x - obj->x_position) * (camera->eye.x - obj->x_position)) + ((camera->eye.z - obj->z_position) * (camera->eye.z - obj->z_position)));
                    camera->unk618.x = -sinf(camera->yaw * 0.01745329238f) * dist + camera->focus.x;
                    camera->unk618.y = camera->focus.y;
                    camera->unk618.z = -cosf(camera->yaw * 0.01745329238f) * dist + camera->focus.z;
                    camera->unk384 = camera->yaw;
                }
                if (cmd->type == 14) {
                    camera->unk618.x = D_800CC2D0[camera->playerIndex].x_position;
                    camera->unk618.y = D_800CC2D0[camera->playerIndex].y_position;
                    camera->unk618.z = D_800CC2D0[camera->playerIndex].z_position;
                } else if (camera->snap != 0) {
                    camera->unk618.x = obj->x_position;
                    camera->unk618.y = obj->y_position;
                    camera->unk618.z = obj->z_position;
                } else {
                    camera->unk618.x += (obj->x_position - camera->unk618.x) * 0.03999999911f;
                    camera->unk618.y += (obj->y_position - camera->unk618.y) * 0.03999999911f;
                    camera->unk618.z += (obj->z_position - camera->unk618.z) * 0.03999999911f;
                }
                // matching: two separate tests; merging them (or dropping h) changes scheduling
                if (cmd->unk8 != 0) {
                    camera->targetDistance = cmd->unk20;
                }
                if (cmd->unk8 != 0) {
                    f32 h;

                    h = cmd->unk8 - (camera->focus.y - camera->unk618.y);
                    camera->unk34C = h;
                    cmd->unk34 = cmd->unk8;
                }
            }
            return;
        }
        if (cmd->type == 15) {
            if (func_15123934((Camera *)camera, 0x800000, 0, camera->unk134, 2) != 0) {
                camera->unk134 = 0;
                camera->flags |= 0x200;
                camera->flags &= ~8;
                func_1512DD50((Camera *)camera);
            }
            return;
        }
        if (cmd->type == 3) {
            if (func_15123934((Camera *)camera, 0x400, 0, camera->unk134, 2) != 0) {
                camera->unk628 = 0;
                camera->flags |= 8;
                camera->unk614 = &camera->unk618;
                camera->unk62C = 0.0f;
                camera->unk5DC = 0.0f;
                camera->targetDistance = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
                camera->cmdRunning = 1;
            }
            camera->unk618.x = cmd->x;
            camera->unk618.y = cmd->y;
            camera->unk618.z = cmd->z;
            camera->targetDistance = cmd->unk1C;
            return;
        }
        if (cmd->type == 2) {
            f32 shortfall;

            if (func_15123934((Camera *)camera, 0x2000, 0, camera->unk134, 2) != 0) {
                camera->targetDistance = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
            }
            camera->cmdRunning = 1;
            func_1512E140((Camera *)camera);
            camera->distance = sqrtf(((camera->eye.x - camera->focus.x) * (camera->eye.x - camera->focus.x)) + ((camera->eye.z - camera->focus.z) * (camera->eye.z - camera->focus.z)));
            if (camera->distance < camera->targetDistance) {
                shortfall = camera->targetDistance - camera->distance;
                camera->eye.x += shortfall * cam->pushRate * func_150AD78C(camera->yawRad);
                camera->eye.z += shortfall * cam->pushRate * func_150AD780(camera->yawRad);
                cam->pushRate += 0.004999999888f;
                cam->pushRate = (cam->pushRate > 0.03999999911f) ? 0.03999999911f : cam->pushRate;
            }
            func_151287E0((Camera *)camera, NULL, NULL);
            return;
        }
        if ((cmd->type == 1) || (cmd->type == 0)) {
            f32 dist;
            f32 shortfall;

            dist = sqrtf(((camera->eye.x - cmd->x) * (camera->eye.x - cmd->x)) + ((camera->eye.z - cmd->z) * (camera->eye.z - cmd->z)));
            if ((camera->cmdRunning == 0) || (cmd->type == 0)) {
                if (func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 2) != 0) {
                    camera->cmdRunning = 1;
                    camera->targetDistance = cmd->unk1C;
                    camera->unk34C = cmd->unk20;
                    cmd->unk34 = cmd->unk20;
                    camera->flags &= ~4;
                    if (cmd->type == 0) {
                        camera->unk734 = cmd->id | 0x4000;
                        camera->unk738 = 1;
                    }
                }
            }
            camera->targetDistance = cmd->unk1C;
            camera->unk34C = camera->height = cmd->unk20;
            cmd->unk34 = cmd->unk20;
            if ((dist < camera->targetDistance) && (cmd->type == 1)) {
                shortfall = camera->targetDistance - dist;
                camera->eye.x += shortfall * cam->pushRate * func_150AD78C(camera->yawRad);
                camera->eye.z += shortfall * cam->pushRate * func_150AD780(camera->yawRad);
                cam->pushRate += 0.002499999944f;
                cam->pushRate = (cam->pushRate > 0.03999999911f) ? 0.03999999911f : cam->pushRate;
            }
            if (cmd->type == 1) {
                cam->unk18 += (cmd->x - cam->unk18) * 0.03999999911f;
                cam->unk20 += (cmd->z - cam->unk20) * 0.03999999911f;
                camera->focus.x += (cam->unk18 - camera->focus.x) * 0.5f;
                camera->focus.z += (cam->unk20 - camera->focus.z) * 0.5f;
            }
            if (cmd->type != 0) {
                func_1512E140((Camera *)camera);
                func_15124C38((Camera *)camera, 0);
                cam->blend = 0.0f;
            }
            return;
        }
        if (cmd->type == 13) {
            func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 2);
            return;
        }
    }
    cam->minHeight = 0.0f;
    cam->maxHeight = 0.0f;
    cam->maxDistance = 0.0f;
    if (camera->cmdState.unk0 == 0) {
        cam->unk6 = 0;
    }
    if ((camera->cmdRunning == 1) && (cam->lastType == 1)) {
        cam->unk18 = camera->focus.x + (cam->unk18 - camera->focus.x) * 0.5f;
        cam->unk20 = camera->focus.z + (cam->unk20 - camera->focus.z) * 0.5f;
        cam->pushRate = 0.0f;
    }
    if (camera->mode == 0x40) {
        camera->unk272 = -1;
        func_151239CC((Camera *)camera, 12);
    }
    if (camera->cmdRunning == 1) {
        if (cam->lastType == 0) {
            cam->unk18 = camera->focus.x;
            cam->unk20 = camera->focus.z;
        }
        if (cam->lastType == 14) {
            camera->followRate = 0.009999999776f;
            func_151254F4((Camera *)camera, camera->playerIndex);
        }
    }
    if (camera->snap != 0) {
        cam->unk18 = camera->focus.x;
        cam->unk20 = camera->focus.z;
        cam->blend = 1.0f;
    } else {
        cam->unk18 += (camera->focus.x - cam->unk18) * cam->blend;
        cam->unk20 += (camera->focus.z - cam->unk20) * cam->blend;
    }
    camera->focus.x = cam->unk18;
    camera->focus.z = cam->unk20;
    cam->blend += 0.01499999966f;
    cam->blend = (cam->blend > 1.0f) ? 1.0f : cam->blend;
    if ((cam->blend != 1.0f) && (cam->lastType == 1)) {
        cam->blending = 1;
    } else {
        cam->blending = 0;
    }
    if (camera->cmdRunning == 1) {
        camera->unk272 = -1;
        camera->unk614 = NULL;
        camera->cmdRunning = 0;
        cmd->unk34 = 0;
        cmd->type = 13;
        camera->unk8F0 = 0.0f;
        camera->unk8F4 = 0.0f;
        camera->unk8F8 = 0.0f;
        camera->unk8FC = 0.0f;
        camera->unk900 = 0.0f;
        camera->unk904 = 0.0f;
        camera->unk910 = 0.0f;
        camera->unk908 = 0.0f;
        camera->unk918 = 0.0f;
        camera->unk914 = 0.0f;
        camera->unk90C = 0.0f;
        camera->unk91C = 0.0f;
        camera->pathFlags = 0;
        camera->unk92C = 0;
        camera->unk948 = 0;
        camera->unk384 = camera->yaw;
        // matching: double 0.0 literals; 0.0f changes the generated code
        camera->unk924 = 0.0;
        camera->unk928 = 0.0;
        func_151239CC((Camera *)camera, 1);
        func_151239CC((Camera *)camera, 12);
        func_151239CC((Camera *)camera, 13);
        func_151239CC((Camera *)camera, 2);
        func_151239CC((Camera *)camera, 7);
    }
}

// Starts the camera mode for a command's action (cmd.unk8) and sets its flags.
void func_1512FE00(Camera *camera, s32 action) {
    switch (action) {
    case 1:
        func_15123934((Camera *)camera, camera->mode, 0, 0, 7);
        break;
    case 2:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags &= ~8;
        break;
    case 3:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x10000000;
        break;
    case 8:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x80;
        break;
    case 9:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags &= ~8;
        camera->flags |= 0x80;
        break;
    case 11:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x4000;
        break;
    case 12:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x8000;
        break;
    case 13:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x20000;
        break;
    case 16:
        func_15123934((Camera *)camera, 8, 0, camera->unk134, 7);
        break;
    case 14:
        func_15123934((Camera *)camera, 2, 0, camera->unk134, 7);
        break;
    case 18:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x100000;
        break;
    case 19:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x400000;
        break;
    case 21:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->liftTarget = 0.0f;
        break;
    case 22:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x800;
        camera->liftTarget = -30.0f;
        break;
    case 25:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x40;
        break;
    case 23:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x1000000;
        break;
    case 24:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x80000000;
        break;
    case 26:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x10;
        break;
    case 27:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x20;
        break;
    case 28:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x10;
        camera->flags &= ~8;
        break;
    case 29:
        func_15123934((Camera *)camera, camera->mode, 0, camera->unk134, 7);
        camera->flags |= 0x1000010;
        camera->flags &= ~8;
        break;
    case 30:
        func_15128774((Camera *)camera, camera->player);
        break;
    }
}
