#include <ultra64.h>
#include "functions.h"
#include "variables.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

// Camera command playback state (embedded in the camera at 0x6CC).
typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2; // id of the command last started (cmd.unk4)
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ u8 padA[0x6];
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
} CamCmdState;

// Current camera command (embedded in the camera at 0x6FC).
typedef struct {
    /* 0x00 */ u32 type;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ u32 unk8;
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u8 pad16[0x6];
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ u8 pad24[0x10];
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} CamCmd;

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

// This file's view of a camera (struct108, 0x9A0 bytes, array at D_800DBFF0). The shared
// struct108 declaration has different field types at 0x2AC/0x2F8/0x618/0x6CC, so a local
// layout is used until the header is reconciled.
typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0x54];
    /* 0x084 */ s32 unk84;
    /* 0x088 */ u8 pad88[0xAC];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ u8 pad138[0x7C];
    /* 0x1B4 */ s16 unk1B4;
    /* 0x1B6 */ u8 pad1B6[0xE];
    /* 0x1C4 */ s16 unk1C4;
    /* 0x1C6 */ u8 pad1C6[0x1A];
    /* 0x1E0 */ s16 unk1E0;
    /* 0x1E2 */ u8 pad1E2[0xE];
    /* 0x1F0 */ s16 unk1F0;
    /* 0x1F2 */ u8 pad1F2[0x4A];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x23E */ u8 pad23E[0x34];
    /* 0x272 */ s16 unk272;
    /* 0x274 */ s16 unk274;
    /* 0x276 */ u8 pad276[0x2];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ Vec3f unk27C;
    /* 0x288 */ f32 unk288;
    /* 0x28C */ f32 unk28C;
    /* 0x290 */ f32 unk290;
    /* 0x294 */ u8 pad294[0x10];
    /* 0x2A4 */ f32 unk2A4;
    /* 0x2A8 */ f32 unk2A8;
    /* 0x2AC */ f32 unk2AC;
    /* 0x2B0 */ u8 pad2B0[0x48];
    /* 0x2F8 */ Vec3f pos;
    /* 0x304 */ u8 pad304[0x44];
    /* 0x348 */ f32 unk348;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x1C];
    /* 0x36C */ u16 * unk36C;
    /* 0x370 */ f32 unk370;
    /* 0x374 */ f32 unk374;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 unk37C;
    /* 0x380 */ u8 pad380[0x4];
    /* 0x384 */ f32 unk384;
    /* 0x388 */ u8 pad388[0x14];
    /* 0x39C */ f32 unk39C;
    /* 0x3A0 */ u8 pad3A0[0x30];
    /* 0x3D0 */ struct127 * unk3D0;
    /* 0x3D4 */ struct127 * unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x204];
    /* 0x5DC */ f32 unk5DC;
    /* 0x5E0 */ u8 pad5E0[0x10];
    /* 0x5F0 */ s32 unk5F0;
    /* 0x5F4 */ u8 pad5F4[0x20];
    /* 0x614 */ Vec3f * unk614;
    /* 0x618 */ Vec3f unk618;
    /* 0x624 */ u8 pad624[0x4];
    /* 0x628 */ s32 unk628;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ u8 pad630[0x40];
    /* 0x670 */ f32 unk670;
    /* 0x674 */ u8 pad674[0x54];
    /* 0x6C8 */ s32 unk6C8;
    /* 0x6CC */ CamCmdState cam;
    /* 0x6FC */ CamCmd cmd;
    /* 0x734 */ s32 unk734;
    /* 0x738 */ s32 unk738;
    /* 0x73C */ s16 unk73C;
    /* 0x73E */ u8 pad73E[0x76];
    /* 0x7B4 */ f32 unk7B4;
    /* 0x7B8 */ u8 pad7B8[0x138];
    /* 0x8F0 */ f32 unk8F0;
    /* 0x8F4 */ f32 unk8F4;
    /* 0x8F8 */ f32 unk8F8;
    /* 0x8FC */ f32 unk8FC;
    /* 0x900 */ f32 unk900;
    /* 0x904 */ f32 unk904;
    /* 0x908 */ f32 unk908;
    /* 0x90C */ f32 unk90C;
    /* 0x910 */ f32 unk910;
    /* 0x914 */ f32 unk914;
    /* 0x918 */ f32 unk918;
    /* 0x91C */ f32 unk91C;
    /* 0x920 */ u16 unk920;
    /* 0x922 */ u8 pad922[0x2];
    /* 0x924 */ f32 unk924;
    /* 0x928 */ f32 unk928;
    /* 0x92C */ u8 unk92C;
    /* 0x92D */ u8 pad92D[0x1B];
    /* 0x948 */ u8 unk948;
} CameraView;

extern CamPathNode *D_800D2350;
extern CamPoint *D_800DC024;

void func_150495B0(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
struct127 *func_1505EEF4(s32 arg0);
s32 func_15085DF8(f32 arg0, f32 arg1, f32 arg2, s32 arg3, s32 arg4);
void func_15124C38(struct108 *arg0, s32 arg1);
void func_151254F4(struct108 *arg0, s32 arg1);
void func_151287E0(struct108 *arg0, f32 *arg1, f32 *arg2);
void func_1512DD50(struct108 *arg0);
void func_1512E140(struct108 *arg0);
void func_1512FE00(struct108 *arg0, s32 arg1);

// Runs one frame of the camera's current command; cmd.type selects the behaviour.
void func_1512E8E0(CameraView *camera) {
    CamCmd *cmd;
    CamCmdState *cam;

    if (camera->unk2C & 0x100) {
        return;
    }
    if (camera->unk73C != 0) {
        return;
    }
    func_151239CC((struct108 *)camera, 13);
    cmd = &camera->cmd;
    cam = &camera->cam;
    if (camera->unk2C & 0x40000) {
        return;
    }
    if (cam->unk2 != cmd->unk4) {
        camera->unk92C = 0;
        cam->unk2 = cmd->unk4;
        cam->unk6 = 0;
        func_151239CC((struct108 *)camera, 12);
        func_151239CC((struct108 *)camera, 13);
        func_151239CC((struct108 *)camera, 2);
        func_151239CC((struct108 *)camera, 7);
    }
    if ((cmd->type != 4) && (camera->unk3D4->unk120 != 0) && (camera->unk92C == 0)) {
        cam->unk6 = 1;
    }
    if (cam->unk6 != 0) {
        func_151239CC((struct108 *)camera, 12);
    }
    if ((camera->cam.unk0 != 0) && (cam->unk6 == 0) && (cam->unk8 == 0)) {
        cam->unk4 = cmd->type;
        if (cmd->type != 10) {
            func_1512FE00((struct108 *)camera, cmd->unk8);
        }
        if (cmd->type == 4) {
            camera->unk272 = -1;
            camera->unk6C8 = 1;
            camera->unk1C4 = camera->unk1B4;
            camera->unk1F0 = camera->unk1E0;
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
            node = &D_800D2350[func_15085DF8(camera->pos.x, camera->pos.y, camera->pos.z, 7, 0)];
            minDist = 1e14f;
            if (camera->unk2C & 0x100) {
                return;
            }
            nodePos.unk0 = node->x;
            nodePos.unk4 = node->y;
            nodePos.unk8 = node->z;
            func_15048F90(&nodePos, (struct17 *)&camera->pos, &toCamera);
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
                    dx = camera->pos.x - closest.unk0;
                    dy = camera->pos.y - closest.unk4;
                    dz = camera->pos.z - closest.unk8;
                    // matching: dy/dz stay dead and the y/z terms are spelled out; the extra CSE temps size the frame
                    if (((dx * dx) + ((camera->pos.y - closest.unk4) * (camera->pos.y - closest.unk4)) + ((camera->pos.z - closest.unk8) * (camera->pos.z - closest.unk8))) < minDist) {
                        minDist = (dx * dx) + ((camera->pos.y - closest.unk4) * (camera->pos.y - closest.unk4)) + ((camera->pos.z - closest.unk8) * (camera->pos.z - closest.unk8));
                        best = closest;
                    }
                }
            }
            if (func_15123934((struct108 *)camera, camera->unk2C, 0, camera->unk134, 12) != 0) {
                camera->unk6C8 = 1;
                camera->unk84 &= ~0x80000006;
                camera->unk920 = cmd->unk8;
                camera->unk924 = cmd->unk1C;
                camera->unk928 = cmd->unk20;
                if (camera->unk920 & 1) {
                    camera->unk84 |= 4;
                }
                if (camera->unk920 & 2) {
                    camera->unk84 |= 2;
                }
                if (camera->unk920 & 7) {
                    camera->unk84 |= 0x20000;
                }
                camera->unk92C = 1;
            }
            if (camera->unk920 & 0x20) {
                camera->unk84 |= 8;
            } else {
                camera->unk84 &= ~8;
            }
            camera->unk910 = 1.5f;
            camera->unk914 = 2.5f;
            if ((camera->unk920 & 2) && (*camera->unk36C & 7)) {
                camera->unk84 |= 8;
                camera->unk910 *= 0.25f;
                camera->unk914 *= 0.25f;
            } else if (camera->unk920 & 0x40) {
                if (camera->unk3D0->xz_velocity > 30.0f) {
                    camera->unk910 *= 7.0f;
                    camera->unk914 *= 7.0f;
                } else {
                    camera->unk910 *= 4.0f;
                    camera->unk914 *= 4.0f;
                }
                camera->unk84 |= 0x20000;
            }
            if (camera->unk23C != 0) {
                camera->unk918 = 0.0f;
                camera->unk908 = camera->unk910;
                camera->unk91C = 0.0f;
                camera->unk90C = camera->unk914;
            } else {
                func_150495B0(&camera->unk908, camera->unk910, &camera->unk918, 1.5f, 2.0f, camera->unk7B4);
                func_150495B0(&camera->unk90C, camera->unk914, &camera->unk91C, 1.5f, 2.0f, camera->unk7B4);
            }
            if (!(camera->unk920 & 0x10)) {
                if (camera->unk23C != 0) {
                    camera->unk8F4 = 0.0f;
                    camera->unk904 = 0.0f;
                    camera->pos.x = best.unk0;
                    camera->pos.z = best.unk8;
                } else {
                    func_150495B0(&camera->pos.x, best.unk0, &camera->unk8F4, camera->unk908, camera->unk90C, camera->unk7B4);
                    func_150495B0(&camera->pos.z, best.unk8, &camera->unk904, camera->unk908, camera->unk90C, camera->unk7B4);
                }
            }
            if (!(camera->unk920 & 8)) {
                if (camera->unk23C != 0) {
                    camera->pos.y = best.unk4;
                    camera->unk8FC = 0.0f;
                } else {
                    func_150495B0(&camera->pos.y, best.unk4, &camera->unk8FC, camera->unk908, camera->unk90C, camera->unk7B4);
                }
            }
            func_15125594((struct108 *)camera);
            camera->unk924 = cmd->unk1C;
            camera->unk928 = cmd->unk20;
            camera->unk374 = camera->unk924;
            camera->unk348 = camera->unk928;
            if (camera->unk920 & 4) {
                camera->unk734 = cmd->unk4 | 0x4000;
                camera->unk738 = 1;
            }
            return;
        }
        if (cmd->type == 5) {
            if (func_15123934((struct108 *)camera, 0x40, 0, 0, 12) != 0) {
                camera->unk84 &= ~0xE;
                camera->unk84 |= 0x200;
                camera->unk27C = camera->pos;
            }
            if ((cmd->unk8 == 5) || (cmd->unk8 == 0x11)) {
                if (camera->unk272 != cmd->unk1C) {
                    camera->unk272 = cmd->unk1C;
                    D_800DC024 = (CamPoint *)D_800DC020;
                    D_800DC024 = &D_800DC024[camera->unk272];
                    camera->unk278 = cmd->unk20;
                    camera->unk288 = D_800DC024->x;
                    camera->unk28C = D_800DC024->y;
                    camera->unk290 = D_800DC024->z;
                    camera->unk5F0 |= 4;
                    camera->pos.x = camera->unk288;
                    camera->unk27C.x = camera->unk288;
                    camera->pos.y = camera->unk28C;
                    camera->unk27C.y = camera->unk28C;
                    camera->pos.z = camera->unk290;
                    camera->unk27C.z = camera->unk290;
                    func_15128774((struct108 *)camera, camera->unk3D0);
                    camera->unk23C = 1;
                }
            } else {
                camera->unk272 = cmd->unk1C;
                D_800DC024 = (CamPoint *)D_800DC020;
                D_800DC024 = &D_800DC024[camera->unk272];
                camera->unk288 = D_800DC024->x;
                camera->unk28C = D_800DC024->y;
                camera->unk290 = D_800DC024->z;
                camera->unk278 = cmd->unk20;
            }
            camera->unk274 = cmd->unk8;
            camera->unk6C8 = 1;
            return;
        }
        if (cmd->type == 11) {
            cam->unk24 = cmd->unk1C;
            cam->unk28 = cmd->unk20;
            cam->unk2C = cmd->unk8;
            return;
        }
        if ((cmd->type == 10) || (cmd->type == 14)) {
            struct127 *obj;

            // matching: reusing obj for the id (instead of nesting the calls) drops a cfe temp
            obj = (struct127 *)func_15083E0C(cmd->unk1C);
            obj = func_1505EEF4((s32)obj);
            if (obj != NULL) {
                f32 dist;

                if (func_15123934((struct108 *)camera, 0x400, 0, camera->unk134, 2) != 0) {
                    if (cmd->type == 14) {
                        func_151254F4((struct108 *)camera, obj - D_800CC2D0);
                    }
                    camera->unk84 &= ~8;
                    camera->unk628 = 0;
                    camera->unk614 = &camera->unk618;
                    camera->unk6C8 = 1;
                    camera->unk62C = 0.0f;
                    camera->unk5DC = 0.0f;
                    dist = sqrtf(((camera->pos.x - obj->x_position) * (camera->pos.x - obj->x_position)) + ((camera->pos.z - obj->z_position) * (camera->pos.z - obj->z_position)));
                    camera->unk618.x = -sinf(camera->unk37C * 0.01745329238f) * dist + camera->unk2A4;
                    camera->unk618.y = camera->unk2A8;
                    camera->unk618.z = -cosf(camera->unk37C * 0.01745329238f) * dist + camera->unk2AC;
                    camera->unk384 = camera->unk37C;
                }
                if (cmd->type == 14) {
                    camera->unk618.x = D_800CC2D0[camera->unk23D].x_position;
                    camera->unk618.y = D_800CC2D0[camera->unk23D].y_position;
                    camera->unk618.z = D_800CC2D0[camera->unk23D].z_position;
                } else if (camera->unk23C != 0) {
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
                    camera->unk374 = cmd->unk20;
                }
                if (cmd->unk8 != 0) {
                    f32 h;

                    h = cmd->unk8 - (camera->unk2A8 - camera->unk618.y);
                    camera->unk34C = h;
                    cmd->unk34 = cmd->unk8;
                }
            }
            return;
        }
        if (cmd->type == 15) {
            if (func_15123934((struct108 *)camera, 0x800000, 0, camera->unk134, 2) != 0) {
                camera->unk134 = 0;
                camera->unk84 |= 0x200;
                camera->unk84 &= ~8;
                func_1512DD50((struct108 *)camera);
            }
            return;
        }
        if (cmd->type == 3) {
            if (func_15123934((struct108 *)camera, 0x400, 0, camera->unk134, 2) != 0) {
                camera->unk628 = 0;
                camera->unk84 |= 8;
                camera->unk614 = &camera->unk618;
                camera->unk62C = 0.0f;
                camera->unk5DC = 0.0f;
                camera->unk374 = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
                camera->unk6C8 = 1;
            }
            camera->unk618.x = cmd->x;
            camera->unk618.y = cmd->y;
            camera->unk618.z = cmd->z;
            camera->unk374 = cmd->unk1C;
            return;
        }
        if (cmd->type == 2) {
            f32 shortfall;

            if (func_15123934((struct108 *)camera, 0x2000, 0, camera->unk134, 2) != 0) {
                camera->unk374 = cmd->unk1C;
                camera->unk34C = cmd->unk20;
                cmd->unk34 = cmd->unk20;
            }
            camera->unk6C8 = 1;
            func_1512E140((struct108 *)camera);
            camera->unk370 = sqrtf(((camera->pos.x - camera->unk2A4) * (camera->pos.x - camera->unk2A4)) + ((camera->pos.z - camera->unk2AC) * (camera->pos.z - camera->unk2AC)));
            if (camera->unk370 < camera->unk374) {
                shortfall = camera->unk374 - camera->unk370;
                camera->pos.x += shortfall * cam->unk14 * func_150AD78C(camera->unk39C);
                camera->pos.z += shortfall * cam->unk14 * func_150AD780(camera->unk39C);
                cam->unk14 += 0.004999999888f;
                cam->unk14 = (cam->unk14 > 0.03999999911f) ? 0.03999999911f : cam->unk14;
            }
            func_151287E0((struct108 *)camera, NULL, NULL);
            return;
        }
        if ((cmd->type == 1) || (cmd->type == 0)) {
            f32 dist;
            f32 shortfall;

            dist = sqrtf(((camera->pos.x - cmd->x) * (camera->pos.x - cmd->x)) + ((camera->pos.z - cmd->z) * (camera->pos.z - cmd->z)));
            if ((camera->unk6C8 == 0) || (cmd->type == 0)) {
                if (func_15123934((struct108 *)camera, camera->unk2C, 0, camera->unk134, 2) != 0) {
                    camera->unk6C8 = 1;
                    camera->unk374 = cmd->unk1C;
                    camera->unk34C = cmd->unk20;
                    cmd->unk34 = cmd->unk20;
                    camera->unk84 &= ~4;
                    if (cmd->type == 0) {
                        camera->unk734 = cmd->unk4 | 0x4000;
                        camera->unk738 = 1;
                    }
                }
            }
            camera->unk374 = cmd->unk1C;
            camera->unk34C = camera->unk348 = cmd->unk20;
            cmd->unk34 = cmd->unk20;
            if ((dist < camera->unk374) && (cmd->type == 1)) {
                shortfall = camera->unk374 - dist;
                camera->pos.x += shortfall * cam->unk14 * func_150AD78C(camera->unk39C);
                camera->pos.z += shortfall * cam->unk14 * func_150AD780(camera->unk39C);
                cam->unk14 += 0.002499999944f;
                cam->unk14 = (cam->unk14 > 0.03999999911f) ? 0.03999999911f : cam->unk14;
            }
            if (cmd->type == 1) {
                cam->unk18 += (cmd->x - cam->unk18) * 0.03999999911f;
                cam->unk20 += (cmd->z - cam->unk20) * 0.03999999911f;
                camera->unk2A4 += (cam->unk18 - camera->unk2A4) * 0.5f;
                camera->unk2AC += (cam->unk20 - camera->unk2AC) * 0.5f;
            }
            if (cmd->type != 0) {
                func_1512E140((struct108 *)camera);
                func_15124C38((struct108 *)camera, 0);
                cam->unk10 = 0.0f;
            }
            return;
        }
        if (cmd->type == 13) {
            func_15123934((struct108 *)camera, camera->unk2C, 0, camera->unk134, 2);
            return;
        }
    }
    cam->unk24 = 0.0f;
    cam->unk28 = 0.0f;
    cam->unk2C = 0.0f;
    if (camera->cam.unk0 == 0) {
        cam->unk6 = 0;
    }
    if ((camera->unk6C8 == 1) && (cam->unk4 == 1)) {
        cam->unk18 = camera->unk2A4 + (cam->unk18 - camera->unk2A4) * 0.5f;
        cam->unk20 = camera->unk2AC + (cam->unk20 - camera->unk2AC) * 0.5f;
        cam->unk14 = 0.0f;
    }
    if (camera->unk2C == 0x40) {
        camera->unk272 = -1;
        func_151239CC((struct108 *)camera, 12);
    }
    if (camera->unk6C8 == 1) {
        if (cam->unk4 == 0) {
            cam->unk18 = camera->unk2A4;
            cam->unk20 = camera->unk2AC;
        }
        if (cam->unk4 == 14) {
            camera->unk670 = 0.009999999776f;
            func_151254F4((struct108 *)camera, camera->unk23D);
        }
    }
    if (camera->unk23C != 0) {
        cam->unk18 = camera->unk2A4;
        cam->unk20 = camera->unk2AC;
        cam->unk10 = 1.0f;
    } else {
        cam->unk18 += (camera->unk2A4 - cam->unk18) * cam->unk10;
        cam->unk20 += (camera->unk2AC - cam->unk20) * cam->unk10;
    }
    camera->unk2A4 = cam->unk18;
    camera->unk2AC = cam->unk20;
    cam->unk10 += 0.01499999966f;
    cam->unk10 = (cam->unk10 > 1.0f) ? 1.0f : cam->unk10;
    if ((cam->unk10 != 1.0f) && (cam->unk4 == 1)) {
        cam->unk8 = 1;
    } else {
        cam->unk8 = 0;
    }
    if (camera->unk6C8 == 1) {
        camera->unk272 = -1;
        camera->unk614 = NULL;
        camera->unk6C8 = 0;
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
        camera->unk920 = 0;
        camera->unk92C = 0;
        camera->unk948 = 0;
        camera->unk384 = camera->unk37C;
        // matching: double 0.0 literals; 0.0f changes the generated code
        camera->unk924 = 0.0;
        camera->unk928 = 0.0;
        func_151239CC((struct108 *)camera, 1);
        func_151239CC((struct108 *)camera, 12);
        func_151239CC((struct108 *)camera, 13);
        func_151239CC((struct108 *)camera, 2);
        func_151239CC((struct108 *)camera, 7);
    }
}

// Starts the camera mode for a command's action (cmd.unk8) and sets its flags.
void func_1512FE00(struct108 *camera, s32 action) {
    switch (action) {
    case 1:
        func_15123934(camera, camera->unk2C, 0, 0, 7);
        break;
    case 2:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 &= ~8;
        break;
    case 3:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x10000000;
        break;
    case 8:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x80;
        break;
    case 9:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 &= ~8;
        camera->unk84 |= 0x80;
        break;
    case 11:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x4000;
        break;
    case 12:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x8000;
        break;
    case 13:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x20000;
        break;
    case 16:
        func_15123934(camera, 8, 0, camera->unk134, 7);
        break;
    case 14:
        func_15123934(camera, 2, 0, camera->unk134, 7);
        break;
    case 18:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x100000;
        break;
    case 19:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x400000;
        break;
    case 21:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk190 = 0.0f;
        break;
    case 22:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x800;
        camera->unk190 = -30.0f;
        break;
    case 25:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x40;
        break;
    case 23:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x1000000;
        break;
    case 24:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x80000000;
        break;
    case 26:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x10;
        break;
    case 27:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x20;
        break;
    case 28:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x10;
        camera->unk84 &= ~8;
        break;
    case 29:
        func_15123934(camera, camera->unk2C, 0, camera->unk134, 7);
        camera->unk84 |= 0x1000010;
        camera->unk84 &= ~8;
        break;
    case 30:
        func_15128774(camera, camera->unk3D0);
        break;
    }
}
