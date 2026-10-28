#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern void *func_15167A68(s32, s32, s32, s32, s32, s32);

struct sub150C0AC0 {
    s32 unk0;
    s16 unk4;
    s16 unk6;
};

struct s150C0AC0_arg {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s16 unk14;
    s16 unk16;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
};

struct s150C0AC0 {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    struct s150C0AC0_arg unk18;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    struct sub150C0AC0 *unk40;
    s16 unk44;
    s16 unk46;
};


void func_150C09B0(s32 arg0, s32 arg1, s32 arg2) {
    func_15169850(arg1, (u8)arg2, arg0 + 0x20, arg0 + 0x24, arg0);
}

extern void func_150C0A48(struct102 *);

void func_150C09F0(struct102 *arg0) {
    func_150C0A48(arg0);
    func_15169804(arg0);
}

extern void func_15169824(struct102 *);

void func_150C0A1C(struct102 *arg0) {
    func_150C0A48(arg0);
    func_15169824(arg0);
}

struct sub150C0A48 {
    struct102 *unk0;
    s16        unk4;
};

struct s150C0A48 {
    u8 pad0[0x40];
    struct sub150C0A48 *unk40;
    s16 unk44;
};

void func_150C0A48(struct102 *arg0x) {
    struct s150C0A48 *arg0 = (struct s150C0A48 *)arg0x;
    s16 i;

    i = arg0->unk44;
    while (i != -1) {
        func_1516972C(arg0->unk40[i].unk0);
        i = arg0->unk40[i].unk4;
    }
}

struct s150C0AC0 *func_150C0AC0(struct s150C0AC0_arg *arg0, s32 arg1, s32 arg2) {
    struct s150C0AC0 *ret;
    s32 size;
    s16 i;
    s32 *ptr;

    size = (arg0->unk14 * 8) + 0x48;
    if ((arg0->unk18 & 2) == 0) {
        ptr = (s32 *)arg0->unk8;
        if (ptr == NULL) {
            return NULL;
        }
        if (*ptr == 0) {
            return NULL;
        }
    }

    ret = func_15167A68(0x25, arg2, size, 1, (u8)arg1, 1);
    if (ret == NULL) {
        return NULL;
    }

    memcpy(&ret->unk18, arg0, 0x1C);
    ret->unk40 = (struct sub150C0AC0 *)((u8 *)ret + 0x48);
    if (arg0->unk18 & 2) {
        ptr = (s32 *)ret->unk18.unk0;
        ret->unk34 = *(f32 *)((u8 *)ptr + 0);
        ret->unk38 = *(f32 *)((u8 *)ptr + 8);
    } else {
        ptr = (s32 *)ret->unk18.unk8;
        ret->unk34 = *(f32 *)((u8 *)ptr + 0x14);
        ret->unk38 = *(f32 *)((u8 *)ptr + 0x1C);
    }
    ret->unk3C = 0.0f;
    ret->unk44 = -1;
    ret->unk46 = 0;
    for (i = 0; i < arg0->unk14 - 1; i++) {
        ret->unk40[i].unk4 = i + 1;
    }
    ret->unk40[arg0->unk14 - 1].unk4 = -1;
    ret->unk10 = 1;
    ret->unk14 = 0;
    return ret;
}

struct s150C0C38_obj {
    s32 unk0;
    u8 pad4[0x10];
    f32 unk14;
    u8 pad18[4];
    f32 unk1C;
    u8 pad20[0x1B];
    u8 unk3B;
    f32 unk3C;
    vertex unk40;
    u8 pad4C[0x38];
    u16 unk84;
    u8 pad86[0xFA];
    f32 unk180;
};

struct s150C0C38_ent {
    struct s150C0C38_obj *unk0;
    s16 unk4;
    s16 unk6;
};

struct s150C0C38 {
    u8 pad0;
    u8 unk1;
    u8 pad2[0xA];
    u8 unkC;
    u8 padD[0xB];
    vertex *unk18;
    u8 pad1C[4];
    struct s150C0C38_obj *unk20;
    u8 unk24;
    u8 pad25[3];
    f32 unk28;
    u8 pad2C[2];
    s16 unk2E;
    u8 unk30;
    u8 pad31[3];
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    struct s150C0C38_ent *unk40;
    s16 unk44;
    s16 unk46;
};

struct s150C0C38_spawn {
    s32 unk00;
    s32 unk04;
    u16 unk08;
    s16 unk0A;
    s32 unk0C;
    s32 unk10;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
    u8 unk1B;
    u8 unk1C;
    u8 unk1D;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    vertex unk30;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    s32 unk58;
    u8 pad5C[4];
    u8 unk60;
    u8 unk61;
    s8 unk62;
    s8 unk63;
    s8 unk64;
    s8 unk65;
    u8 pad66[0x2];
};

extern void *func_15130280(void *, u8, s32, s32, u8, s32);

void func_150C0C38(struct s150C0C38 *arg0) {
    struct s150C0C38 *self[1];
    volatile u8 kill;
    vertex pos;
    struct s150C0C38_obj *obj;
    s16 i;
    s32 bit;
    struct s150C0C38_spawn sp80;
    struct s150C0C38 *sp7C;

    self[0] = arg0;
    kill = 0;
    if (!(arg0->unk30 & 2)) {
        obj = arg0->unk20;
        if (obj->unk0 == 0) {
            kill = 1;
        }
        if (obj->unk3B != arg0->unk24) {
            kill = 1;
        }
        pos.x = obj->unk14;
        pos.y = arg0->unk20->unk180;
        pos.z = arg0->unk20->unk1C;
    } else {
        memcpy(&pos, arg0->unk18, 0xC);
        pos.y -= 65.0f;
    }
    if (kill == 0) {
        s32 ok;

        ok = 1;
        for (i = arg0->unk44; i != -1; i = arg0->unk40[i].unk4) {
            arg0->unk40[i].unk0->unk40 = pos;
        }
        if (arg0->unk30 & 4) {
            if ((arg0->unk20->unk84 != 0x1D) && (arg0->unk20->unk84 != 0x1E)) {
                ok = 0;
            }
        }
        if (arg0->unk20->unk3C < 3.0f) {
            ok = 0;
        }
        if (((pos.x != arg0->unk34) || (pos.z != arg0->unk38)) && ok) {
            arg0->unk3C += arg0->unk28 * D_800BE9A4;
            if (arg0->unk3C > 1.0f) {
                s16 idx;
                u8 angle;
                f32 sinA;
                f32 cosA;
                f32 speed;

                sp80.unk1D = 0x6C;
                sp80.unk08 = 0x5103;
                sp80.unk00 = 0x200005;
                sp80.unk04 = 0x1F0600;
                sp80.unk0C = 0;
                sp80.unk10 = 0;
                sp80.unk17 = 0xFF;
                sp80.unk1C = 0xFF;
                sp80.unk48 = 0.0f;
                sp80.unk4C = 0.0f;
                sp80.unk50 = 0.0f;
                sp80.unk54 = 0.0f;
                sp80.unk1E = 0x1E;
                sp80.unk20 = 8;
                sp80.unk58 = 0x80DE01;
                sp80.unk60 = 8;
                sp80.unk61 = 6;
                sp80.unk62 = -1;
                sp80.unk63 = 0;
                sp80.unk64 = -1;
                sp80.unk65 = 0;
                sp7C = arg0;
                if ((arg0->unk3C > 1.0f) && (arg0->unk46 != -1)) {
                    do {
                        idx = arg0->unk46;
                        angle = func_150ADA20() & 0xFFFF;
                        sinA = func_151423D8(angle - 0x40);
                        cosA = func_151423D8(angle);
                        speed = (func_150ADA68() * 33.0f) + 40.0f;
                        sp80.unk18 = 0x52;
                        sp80.unk19 = 0x6B;
                        sp80.unk1A = 0x97;
                        sp80.unk14 = 0x87;
                        sp80.unk15 = 0x6B;
                        sp80.unk16 = 0x97;
                        sp80.unk1B = (func_150ADA20() % 121U) + 0x50;
                        sp80.unk28 = sp80.unk2C = (func_150ADA68() * 17.0f) + 67.0f;
                        sp80.unk30 = pos;
                        sp80.unk3C = speed * sinA;
                        sp80.unk40 = 0.0f;
                        sp80.unk44 = speed * cosA;
                        sp80.unk58 &= ~0xC0;
                        if (func_150ADA20() & 1) {
                            bit = 0x80;
                        } else {
                            bit = 0;
                        }
                        sp80.unk58 |= (u32)(((func_150ADA20() & 1) ? 0x40 : 0) | bit);
                        sp80.unk0A = sp80.unk22 = (func_150ADA20() % 22U) + 0x46;
                        sp80.unk24 = 1.035226941f;
                        arg0->unk40[idx].unk0 = func_15130280(&sp80, 1, 0, 4, arg0->unkC, arg0->unk1);
                        if (arg0->unk40[idx].unk0 != NULL) {
                            memcpy((u8 *)arg0->unk40[idx].unk0 + 0xA8, &sp7C, 4);
                            arg0->unk46 = arg0->unk40[idx].unk4;
                            arg0->unk40[idx].unk4 = arg0->unk44;
                            arg0->unk44 = idx;
                        }
                        arg0->unk3C -= 1.0f;
                    } while ((arg0->unk3C > 1.0f) && (self[0]->unk46 != -1));
                }
                while (arg0->unk3C > 1.0f) {
                    arg0->unk3C -= 1.0f;
                }
            }
        }
        arg0->unk34 = pos.x;
        arg0->unk38 = pos.z;
    }
    if (arg0->unk30 & 1) {
        arg0->unk2E -= D_800BE9E4;
        if (arg0->unk2E < 0) {
            kill = 1;
        }
    }
    if (kill != 0) {
        func_1516972C((struct102 *)arg0);
    }
}

struct s150C1198 {
    u8 pad0[0xA8];
    struct s150C1198List *unkA8;
};

struct s150C1198List {
    u8 pad0[0x40];
    struct sub150C0A48 *unk40;
    s16 unk44;
    s16 unk46;
};

void func_150C1198(struct s150C1198 *arg0) {
    struct s150C1198List *temp;
    s32 found;
    s16 prev;
    s16 idx;

    temp = arg0->unkA8;
    found = 0;
    prev = -1;
    idx = temp->unk44;
    while ((idx != -1) && (found == 0)) {
        if ((struct102 *)arg0 == temp->unk40[idx].unk0) {
            found = 1;
        } else {
            prev = idx;
            idx = temp->unk40[idx].unk4;
        }
    }

    if (found) {
        if (idx == temp->unk44) {
            temp->unk44 = temp->unk40[idx].unk4;
        } else {
            temp->unk40[prev].unk4 = temp->unk40[idx].unk4;
        }
        temp->unk40[idx].unk4 = temp->unk46;
        temp->unk46 = idx;
    }
}
