#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct struct12 struct12;
typedef struct struct25 struct25;
typedef struct struct27 struct27;
typedef struct struct31 struct31;
typedef struct struct54 struct54;
typedef struct struct56 struct56;
typedef struct struct119 struct119;
typedef struct struct124 struct124;
typedef struct Actor Actor;
typedef struct Camera Camera;
struct Rope;
struct BoneChain;
typedef struct struct216 struct216;
typedef struct struct255 struct255;
typedef struct struct256 struct256;

/*
  union convention
  ================
  b  -> (signed) byte (s8)
  ub -> unsigned byte (u8)
  h  -> (signed) short (s16)
  uh -> unsigned short (u16)
  w  -> (signed) word (s16)
  uw -> unsigned word (u16)
*/

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;   // used
    s32 unk10;
    s32 unk14;
    s32 unk18;  // used
} struct00; // size 0x1C

typedef struct {
    s16 unk0;   // used
    s16 unk2;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;  // used
    s32 unk18;  // used
    s32 unk1C;
    s32 unk20;
    u16 unk24;  // used
    s16 unk26;
    s16 unk28;  // used
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s8  unk30;
    s8  unk31;
    s8  unk32;
    s8  unk33;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    s32 unk44;
    s32 unk48;  // used
    s32 unk4C;  // used
    s32 unk50;  // used
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    u8  unk64;
    u8  unk65;
} struct04;

typedef struct {
    u16 unk0;   // used
    u16 unk2;   // used
    u16 unk4;   // used
    u16 unk6;   // used
} struct05;

struct struct12 {
    s32 unk0;
    s32 unk4;
    u8  unk8;
    u8  unk9;
    u8  padA[0x2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

typedef struct {
    s32 unk0;
} struct36;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
} struct37;

typedef struct {
    s32 unk0;     // used
    s32 unk4;     // used
    s32 unk8;     // used
    s32 unkC;     // used
    s32 unk10;    // used
    s32 unk14;    // used
    s32 unk18;    // used
    s32 unk1C;    // used
    s32 unk20;    // used
    s32 unk24;    // used
    s32 unk28;    // used
    void (*unk2C)(s32 arg0);
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    struct36 *unk40;
    s32 unk44;    // used
    struct37 *unk48; // used
    s32 unk4C;    // used
    s32 unk50;    // used
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
} struct07;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct10;

typedef struct {
    s16 unk0;
    s16 unk2;
    struct10 unk4[8];
} struct8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s16 unkC;
    u8  unkE;
} struct9;

typedef struct {
    u16 unk0;
    u8  unk2;
    u8  unk3;       // used
    s32 unk4;
    s32 unk8;
    s16 unkC;
    s16 unkE;       // used
    u32 unk10[16];  // used
} struct11;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} struct13;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} struct14; // size 0x10

typedef struct {
    u32 unk0;
    s32 unk4;
    u32 unk8;
    s32 unkC;
    s32 unk10;  // used
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u16 unk24;
    u16 unk26;
    s32 unk28;
    s32 unk2C;  // used
} struct15; // 0x30 size

typedef struct {
    u32 unk0;
    u16 unk4;
    u8  unk6;
    u8  unk7;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  unkC;
    u8  unkD;
    u8  unkE;
    u8  unkF;
    u32 unk10;
    u8  unk14;
    u8  unk15;
    u8  unk16;
    u8  unk17;
    f32 unk18;
    // padding
    u32 unk1C;
    u32 pad20;
    u32 pad24;
    u32 pad28;
    u32 pad2C;
    u32 pad30;
    u32 pad34;
    u32 pad38;
} struct16; // 0x3C

/* Three-float vector: positions, directions, velocities, scales. */
typedef struct Vec3f {
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
    /* 0x8 */ f32 z;
} Vec3f; // size 0xC

/* 4x4 float matrix (row-major, the guMtxF2L layout) for bone and camera transforms. */
typedef f32 MtxF[4][4];

/* Three-short vector: packed positions, angles. */
typedef struct Vec3s {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
} Vec3s; // size 0x6

/* Three-int vector. */
typedef struct Vec3i {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
    /* 0x8 */ s32 z;
} Vec3i; // size 0xC

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    s32 unkC;
} struct23;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s16 unk34;
    s8  unk36;
} struct24;

struct struct27 {
    s32 unk0;
    struct27 *unk4;
    s32 unk8;
};

struct struct25 {
    struct25 *unk0; // ALInstrument
    u16 unk4;
    u8 unk6;
    u8 unk7;
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
    u8 unkC;
    u8 unkD;
    u8 unkE;
    u8 unkF;
    f32 unk10;
    u8  unk14;
    s8  unk15;
    u8  unk16;
    u8  unk17;
    f32 unk18;
    u32 unk1C;
    u32 unk20;
    struct27 *unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3A;
    u8 unk3B;
}; // 0x3C

typedef struct {
    s32 *unk0;
    u16 unk4;
    u8  unk6;
    u8  unk7;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  unkB;
    u8  unkC;
    u8  unkD;
    u8  unkE;
    u8  unkF;
    f32 unk10;
    u32 unk14;
    f32 unk18;
    u32 unk1C;
    u32 unk20;
    struct27 *unk24;
    u32 unk28;
    u32 unk2C;
    u32 unk30;
    u8  unk34;
    u8  unk35;
    u8  unk36;
    u8  unk37;
    u8  unk38;
    u8  unk39;
    u8  unk3A;
    u8  unk3B;
} struct29;

// size 0x44
typedef struct {
    s32 unk0;
    s32 unk4;
    u8  unk8;
    u8  unk9;
    u16 unkA;
    struct15 *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8  pad[0x28];
} struct30;

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    u32 unk1C;
    u32 unk20;
    u32 unk24;
    s32 unk28;
    u32 unk2C;
    u16 unk30;
    u16 unk32;
    u8  unk34;
    u8  unk35;
    u8  unk36;
    u8  unk37;
    u16 unk38;
    u16 unk3A;
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u32 unk4C;
    u32 unk50;
    u32 unk54;
    u32 unk58;
    u32 unk5C;
    struct25 *unk60;
    struct25 *unk64;
    s32 unk68;
    struct30 *unk6C;
    u32 unk70;
    u32 unk74;
    u32 unk78;
    f32 unk7C;
    f32 unk80;
    s32 unk84; // address of mesgQueue?
    s32 unk88;
    u8  unk8C;
    u8  unk8D;
} struct26;

typedef struct {
    s32 unk0;
    s32 unk4;
} struct32;

struct struct31 {
    struct31 *unk0;     // used
    struct31 **unk4;    // used
    s32 unk8;
    s32 unkC;           // used
    s32 unk10;
    u8 pad[0x1C];
    f32 unk30;          // used
    s32 unk34;
    struct31** unk38;   // used
    s32 unk3C;          // used
    s32 unk40;          // used
    s32 unk44;
    s32 unk48;
    s16 unk4C;          // used
    s8  unk4E;          // used
    s16 unk50;
    s8  unk52;
    u8  unk53;          // used
    u8  unk54;          // used
};

typedef struct {
    s16 unk0;   // used
    s16 unk2;
    s16 unk4;   // used
    s16 unk6;
    s16 unk8;
    s16 unkA;
    u8  unkC;
    u8  unkD;
    s16 unkE;
} struct39;

typedef struct {
    s16 unk0;   // used
    s16 unk2;   // used
    s32 unk4;
    u16 *unk8; // used
    s32 unkC;
    s32 unk10;
    s32 unk14; // used
    s32 unk18; // used
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    struct25 *unk28; // probably not this struct
    s32 unk2C;
    s32 unk30; // used
    s32 unk34; // used
    s32 unk38;
    s32 unk3C;
    s32 unk40; // used
    s32 unk44; // used
    s32 unk48; // used
    f32 unk4C; // used
    s32 unk50; // used
    f32 unk54; // used
    s32 unk58; // used
    s32 unk5C; // used
    u16 unk60; // used
    u16 unk62; // used
    u16 unk64; // used
    u16 unk66; // used
    s16 unk68; // used
    s16 unk6A; // used
    u16 unk6C; // used
    u16 unk6E; // used
    u16 unk70; // used
    u16 unk72;
    u16 unk74;
    u16 unk76; // used
    s32 unk78; // used
    s32 unk7C; // used
    s32 unk80; // used
    s32 unk84; // used
    s32 unk88; // used
    s32 unk8C; // used
    s32 unk90;
    s16 unk94;
    s16 unk96;
    u8  unk98;
    u8  unk99; // used
    u16 unk9A;
    s16 unk9C;
    s16 unk9E;
    u16 unkA0; // used
    u16 unkA2; // used
    s16 unkA4;
    s16 unkA6;
    s32 unkA8;
    s32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s32 *unkB8; // used
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8; // used
    s32 unkCC; // used
} struct42;

typedef struct {
    s16 unk0;   // used
    s16 unk2;   // used
    s16 unk4;
    s16 unk6;
    s16 unk8;   // used
    s16 unkA;   // used
    u16 unkC;   // used
    s16 unkE;
    s32 unk10;
    s16 unk14;
    u8  unk16;
    u8  unk17;
    s32 unk18;  // used
    s16 pad1C;
    s8  unk1E;
    u8  unk1F;
} struct47;

typedef struct {
    s16 unk0;
    s8  unk2;
    s8  unk3;
} struct49;

typedef struct {
    s32 unk0;   // used
    s32 unk4;   // used
    s16 unk8;   // used
    s16 unkA;
    s32 unkC;
    s32 unk10;  // used
    s32 unk14;
    s32 unk18;
    s32 unk1C;  // used
    s32 unk20;  // used
    s32 unk24;
    s32 unk28;  // used
    s32 unk2C;  // used
    s32 unk30;  // used
    s32 unk34;  // used
    s32 unk38;  // used
    s32 unk3C;
    s32 unk40;  // used
    s32 unk44;  // used
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;  // used
    s32 unk5C;  // used
    s32 unk60;  // used
    s32 unk64;  // used
    s32 unk68;  // used
    s32 unk6C;  // used
    s32 unk70;  // used
} struct50;

typedef struct {
    s32 unk0;
    s32 unk4;
    s16 unk8;
} struct51;

typedef struct {
    s32 unk0;
    f32 unk4;
} struct52;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
} struct53; // used as OSMesg

struct struct54 {
    struct54 *unk0;   // used
    struct54 *unk4;
    u32 unk8;   // used
    s32 unkC;   // used
    s32 unk10;
    s8 unk14;   // used
    u8 unk15;   // used
    u8 unk16;   // used
};

typedef struct {
    s16 unk0;
    s16 unk2; // used
    s32 unk4;
    s32 unk8;
    u8 pad[0x1C];
    s32 unk28; // used
    s32 *unk2C; // used
} struct55;

struct struct56 {
    s32 unk0;
    s32 unk4;
    struct56 *unk8;
};

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;  // used
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u16 unk24;  // used
} struct57;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} struct100;

typedef struct {
    u8  unk0;
    u8  unk1;   // used
    u8  unk2;
    u8  unk3;
    u8  unk4;
    u8  unk5;
    u8  unk6;
    u8  unk7;
    s32 unk8;
    s16 unkC;
    u8  unkE;
    u8  unkF;
    s16 unk10;
    u8  unk12;  // used
    u8  unk13;
    u8  unk14;
    u8  pad15[0x3];
    u8  unk18;  // used
    u8  unk19;  // used
    u8  pad1A[0x2];
    s16 unk1C;
    u8  pad1E[0xA];
    u8  unk28;
    u8  unk29[0x3];
    s32 unk2C;
    s32 unk30;
    u8  pad34[0x6];
    u8  unk3A;
    u8  pad3B[0x2];
    u8  unk3D;
} struct102;

typedef struct {
    u16 unk0;
    u16 unk2;  // used
    u8  unk4;
    u8  unk5;
    u8  unk6;
    u8  unk7;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    u8  padB[0x3];
    u8  unkE;
    u8  padF;
    u8  unk10;
    u8  pad11;
    u8  unk12;
    u8  unk13;
    u8  unk14;
    u8  pad15;
    s16 unk16;
    u8  pad18[0x2];
    u16 unk1A;
    u8  pad1C[0xD];
    u8  unk29;
    u8  pad2A[0x1F];
    u8  unk49;
    u8  unk4A;
} struct104;

typedef struct {
    s32 unk0;  // used
    s32 unk4;  // used
    s32 unk8;  // used
    s32 unkC;  // used
} struct105;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s8  pad[0x3];
    u8  unkF;
} struct106;


typedef struct {
    u8 pad[0xEC];
} struct150; // size 0xEC

typedef struct {
    f32 unk20;
    f32 unk24;
    s16 unk28;
    u16 unk2A;
    s16 unk2C;
    s16 unk2E;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54; //?
    f32 unk58;
    f32 unk5C;
    u16 unk60;
    u16 unk62;
    f32 unk64;
    f32 unk68;
    u16 unk6C;
    u16 unk6E;
    s32 unk70;
    s32 unk74;
} struct109;

typedef struct {
    u8  unk0;
    u8  pad1;
    u8  unk2;
    u8  pad3;
    u8  unk4;
    u8  pad5;
    s16 unk6;
    u8  pad8[0x4];
    u8  unkC;
    u8  padD[0x3];
    u8  unk10;
    u8  pad11[0x5];
    u8  unk16;
    u8  pad17[0x4];
    u8  unk1B;
} struct110;

typedef struct {
    u8 pad0[0x18];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    u8 pad1[0x18];
    s16 unk38;
    s16 unk3A;
    s16 unk3C;
    s16 unk3E;
} struct113;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;  // used
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s16 unk28;
    s16 unk2A;  // used
    s32 unk2C;
    s32 unk30;
    s16 unk34;  // used
    s16 unk36;  // used
    s16 unk38;
    s8  unk3A;  // used
    u8  unk3B;
} struct114;

typedef struct {
    void (*unk0)(struct102 *arg0); // used
    u8 pad0[0x14];
    void (*unk18)(struct102 *arg0); // used
    u8 pad1[0x18];
} struct115;

typedef struct {
    u8 pad[0x20];
} struct117; // used as OSMesg

typedef struct {
    u8  pad0[0x14];
    s32 unk14;
    f32 unk18;
    u8  pad1C[0xfc];
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    s32 unk124;
} struct118;

struct struct119 {
    s16 unk0;
    u8  pad2[0x16E];
    struct119 *unk170;
};

typedef struct {
    u16 unk0;       // used
    u16 unk2;
    u16 unk4;       // used
    u16 unk6;
    struct31 *unk8; // used
} struct120;

typedef struct {
    s16 unk0;
    u8  unk2;
    u8  unk3;
    s32 unk4;
    s32 unk8;
    s32 unkC[8];
} struct121;

typedef struct {
    s32 unk0;
} struct123;

struct FLOAT {
    f32 unk0;
};

struct SHORTS {
    s16 unk0;
    s16 unk2;
};

struct UBYTES4 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
};

struct WORD {
    s32 unk0;
};

struct struct124 {
    struct124* unk0;
    u8 unk4;
    s8 unk5;
    s8 unk6;
    s8 unk7;
    s32 unk8;
    s16 unkC;
    s16 unkE;
    s32 unk10;
    u8  pad14[0x3];
    u8  unk17;
    s16 unk18;
    s16 unk1A;  // used
    s16 unk1C;  // used
    s16 unk1E;  // used
    s16 unk20;
    s16 unk22;  // used
    s16 unk24;  // used
    s16 unk26;  // used
    union {
        struct UBYTES4 i;
        struct FLOAT f;
    } data;
}; // size 0x2C

typedef struct {
    s16 unk0;
    s16 unk2;       // used
    s32 unk4;
    s16 unk8[0x10]; // used
    s32 unk28;      // used
} struct125;

/* Idle animation state of an ActorCtl (ActorCtl.idle, 0x58). While an actor stands still it plays
 * short "sequences" of frames, picked at random while avoiding the ones played most recently. */
typedef struct {
    /* 0x0 */ u8  active;
    /* 0x1 */ u8  holdFrames;  // frames left before the next sequence frame
    /* 0x2 */ u16 timer;       // time spent idle in the current phase
    /* 0x4 */ u8  sequence;    // index into D_80086BA0, 0 = none
    /* 0x5 */ u8  frame;       // position within the sequence
    /* 0x6 */ u8  frameFlags;  // flags of the frame applied last
    /* 0x7 */ u8  recent[5];   // most recently played sequences, newest first
    /* 0xC */ u8  phase;
    /* 0xD */ u8  phaseCount;  // sequences played in the current phase
    /* 0xE */ s16 delay;
} IdleState; // size 0x10

/* Per-character control block (Actor.unk31C, 0x1C0 bytes, allocated for player-controlled and
 * scripted characters): movement state, controller input, rope riding, idle animation and the
 * first-person aim state. */
typedef struct {
    /* 0x000 */ f32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s16 unk8;
    /* 0x00A */ u8  padA;
    /* 0x00B */ u8  unkB;
    /* 0x00C */ u8  unkC;
    /* 0x00D */ u8  unkD;
    /* 0x00E */ u16 unkE;
    /* 0x010 */ s16 unk10;
    /* 0x012 */ s16 unk12;
    /* 0x014 */ u16 unk14;
    /* 0x016 */ u8  unk16;
    /* 0x017 */ u8  unk17;
    /* 0x018 */ s16 unk18;
    /* 0x01A */ u8  unk1A;
    /* 0x01B */ u8  unk1B;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8  unk20;
    /* 0x021 */ u8  unk21;
    /* 0x022 */ s8  unk22;
    /* 0x023 */ s8  unk23;
    /* 0x024 */ s16 unk24;
    /* 0x026 */ u8  unk26;
    /* 0x027 */ u8  unk27;
    /* 0x028 */ f32 unk28;
    /* 0x02C */ f32 unk2C;
    /* 0x030 */ s8  unk30;
    /* 0x031 */ u8  unk31;
    /* 0x032 */ u16 unk32;
    /* 0x034 */ u16 unk34;
    /* 0x036 */ u8  unk36;
    /* 0x037 */ u8  unk37;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ u8  unk3C;
    /* 0x03D */ u8  pad3D;
    /* 0x03E */ u16 unk3E;
    /* 0x040 */ f32 unk40;
    /* 0x044 */ s8  unk44;
    /* 0x045 */ u8  pad45;
    /* 0x046 */ s16 unk46;
    /* 0x048 */ s16 unk48;
    /* 0x04A */ s8  chased;
    /* 0x04B */ s8  chasing;
    /* 0x04C */ u16 unk4C;
    /* 0x04E */ u8  unk4E;
    /* 0x04F */ u8  unk4F;
    /* 0x050 */ s8  unk50;
    /* 0x051 */ u8  unk51;
    /* 0x052 */ u8  unk52;
    /* 0x053 */ u8  unk53;
    /* 0x054 */ s8  unk54;
    /* 0x055 */ s8  unk55;
    /* 0x056 */ u8  unk56;
    /* 0x057 */ u8  unk57;
    /* 0x058 */ IdleState idle;
    /* 0x068 */ u16 unk68;
    /* 0x06A */ u8  pad6A;
    /* 0x06B */ u8  unk6B;      // grab state
    /* 0x06C */ s16 holdX;      // held position (s16 world units)
    /* 0x06E */ s16 holdY;
    /* 0x070 */ s16 holdZ;
    /* 0x072 */ u16 unk72;
    /* 0x074 */ u8  unk74;
    /* 0x075 */ u8  unk75;
    /* 0x076 */ u8  unk76;
    /* 0x077 */ u8  unk77;
    /* 0x078 */ u8  unk78;
    /* 0x079 */ u8  unk79;
    /* 0x07A */ u8  unk7A;
    /* 0x07B */ u8  unk7B;
    /* 0x07C */ u8  unk7C;
    /* 0x07D */ u8  matrix_physics;
    /* 0x07E */ u16 unk7E;
    /* 0x080 */ s16 unk80;
    /* 0x082 */ s16 unk82;
    /* 0x084 */ u8  unk84;
    /* 0x085 */ s8  stick_x;    // this frame's controller input
    /* 0x086 */ s8  stick_y;
    /* 0x087 */ u8  pad87;
    /* 0x088 */ u16 prevButtons;  // buttons of the previous frame
    /* 0x08A */ u16 buttons;
    /* 0x08C */ u16 unk8C;
    /* 0x08E */ s8  unk8E;
    /* 0x08F */ s8  unk8F;
    /* 0x090 */ f32 unk90;
    /* 0x094 */ u8  unk94;
    /* 0x095 */ u8  unk95;
    /* 0x096 */ u8  unk96;
    /* 0x097 */ u8  unk97;
    /* 0x098 */ u8  ropeId;
    /* 0x099 */ u8  pad99[3];
    /* 0x09C */ struct Rope *rope;     // rope being ridden, or NULL
    /* 0x0A0 */ f32 ropeProgress;      // 0..1 along the rope
    /* 0x0A4 */ u8  padA4[4];
    /* 0x0A8 */ f32 ropeSwingForce;
    /* 0x0AC */ u8  ropeSwingAngle;
    /* 0x0AD */ u8  ropeSwingTimer;
    /* 0x0AE */ u8  ropeNode;
    /* 0x0AF */ u8  unkAF;
    /* 0x0B0 */ Actor *unkB0;
    /* 0x0B4 */ u8  padB4[4];
    /* 0x0B8 */ f32 unkB8;
    /* 0x0BC */ f32 unkBC;
    /* 0x0C0 */ f32 unkC0;
    /* 0x0C4 */ u8  padC4[0x18];
    /* 0x0DC */ Vec3f unkDC;
    /* 0x0E8 */ Vec3f unkE8;      // a direction (func_15145974 turns it into heading/elevation)
    /* 0x0F4 */ u8  padF4[0x14];
    /* 0x108 */ u8  unk108;
    /* 0x109 */ u8  unk109;
    /* 0x10A */ u8  pad10A[0xA];
    /* 0x114 */ s16 height;      // cached size from func_1507C3E0
    /* 0x116 */ s16 width;
    /* 0x118 */ s16 depth;
    /* 0x11A */ u8  unk11A;
    /* 0x11B */ u8  spawnIndex;
    /* 0x11C */ struct BoneChain *boneChain;
    /* 0x120 */ u8  unk120;
    /* 0x121 */ u8  unk121;
    /* 0x122 */ u8  unk122;
    /* 0x123 */ u8  unk123;
    /* 0x124 */ s16 unk124;
    /* 0x126 */ s16 unk126;
    /* 0x128 */ u8  unk128;
    /* 0x129 */ s8  unk129;
    /* 0x12A */ s8  unk12A;
    /* 0x12B */ u8  pad12B;
    /* 0x12C */ u16 unk12C;
    /* 0x12E */ u8  pad12E[2];
    /* 0x130 */ Vec3f aimDir;
    /* 0x13C */ Vec3f aimEye;
    /* 0x148 */ Vec3f aimTarget;
    /* 0x154 */ u8  pad154[0xC];
    /* 0x160 */ Vec3f unk160;
    /* 0x16C */ f32 aimYaw;
    /* 0x170 */ f32 aimPitch;
    /* 0x174 */ f32 aimYawTarget;
    /* 0x178 */ f32 aimPitchTarget;
    /* 0x17C */ f32 aimYawVel;
    /* 0x180 */ f32 aimPitchVel;
    /* 0x184 */ f32 aimZoom;
    /* 0x188 */ f32 aimZoomY;
    /* 0x18C */ f32 unk18C;
    /* 0x190 */ s32 aimZone;
    /* 0x194 */ u8  aimReset;
    /* 0x195 */ s8  unk195;
    /* 0x196 */ s8  unk196;
    /* 0x197 */ u8  aimState;      // nonzero while aiming in first person
    /* 0x198 */ u8  unk198;
    /* 0x199 */ u8  unk199;
    /* 0x19A */ u8  grenade_count;
    /* 0x19B */ u8  unk19B;
    /* 0x19C */ u16 unk19C;
    /* 0x19E */ u16 unk19E;
    /* 0x1A0 */ u16 unk1A0;
    /* 0x1A2 */ u16 unk1A2;
    /* 0x1A4 */ u16 unk1A4;
    /* 0x1A6 */ u16 unk1A6;
    /* 0x1A8 */ u8  unk1A8;
    /* 0x1A9 */ u8  unk1A9;
    /* 0x1AA */ s16 unk1AA;
    /* 0x1AC */ u8  unk1AC;
    /* 0x1AD */ u8  unk1AD;
    /* 0x1AE */ s8  unk1AE;
    /* 0x1AF */ s8  unk1AF;
    /* 0x1B0 */ s8  lastSpawnIndex;   // last multiplayer spawn point used
    /* 0x1B1 */ u8  unk1B1;
    /* 0x1B2 */ u8  unk1B2;
    /* 0x1B3 */ u8  unk1B3;
    /* 0x1B4 */ u8  unk1B4;
    /* 0x1B5 */ u8  pad1B5[3];
    /* 0x1B8 */ f32 unk1B8;
    /* 0x1BC */ f32 unk1BC;
} ActorCtl; // size 0x1C0

/* One 0x30-byte entry of the level's spawn table D_800D20FC; Actor.spawn points at the one it came from. */
typedef struct SpawnRecord {
    /* 0x00 */ u16 flags;
    /* 0x02 */ u8  unk2;
    /* 0x03 */ u8  interaction_state;
    /* 0x04 */ u8  type;         // model id, 0xFF for none
    /* 0x05 */ u8  unk5;
    /* 0x06 */ s16 x;
    /* 0x08 */ s16 y;
    /* 0x0A */ s16 z;
    /* 0x0C */ u8  angle;        // high byte of the facing angle
    /* 0x0D */ u8  unkD;
    /* 0x0E */ u8  unkE;
    /* 0x0F */ u8  unkF;
    /* 0x10 */ u8  unk10;
    /* 0x11 */ u8  unk11;
    /* 0x12 */ u8  unk12;
    /* 0x13 */ u8  unk13;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ u8  pad16[0x2];
    /* 0x18 */ s32 objFlags;     // copied to the object's unkF8
    /* 0x1C */ u8  unk1C;
    /* 0x1D */ u8  unk1D;
    /* 0x1E */ u8  pad1E[0x2];
    /* 0x20 */ f32 xz_scale;
    /* 0x24 */ f32 y_scale;
    /* 0x28 */ u8  unique_id;
    /* 0x29 */ u8  partSet;      // 1-based index into D_80086CC4, 0 for none
    /* 0x2A */ u16 unk2A;        // cull radius, 0 = always drawn (func_150623F4)
    /* 0x2C */ u16 unk2C;
    /* 0x2E */ u8  keepTime;     // seconds (x30 frames) to keep the model's D_800D2040 counter; 0xFF = never set
    /* 0x2F */ u8  unk2F;
} SpawnRecord; // size 0x30

/* One point of a level path (8 bytes). D_800D2104[n] points at path n: a centre point followed by
 * D_800D2108[n] - 1 waypoints. */
typedef struct PathPoint {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u16 param; /* point 0: radius / 8 of the path's area; later points: speed */
} PathPoint;

/* D_800D2138 (0x20C bytes, cleared by func_15017300): match state. D_800D213C (per-player stats) and
 * D_800D2340 (= unk208) are separately named views into it. */
typedef struct MatchState {
    /* 0x000 */ s32 unk0;      /* indexes D_80087280; 1 = two views */
    /* 0x004 */ u8  pad4[0x204];
    /* 0x208 */ u16 unk208;    /* bit per player */
    /* 0x20A */ u8  pad20A[2];
} MatchState; // size 0x20C

/* Text resource loaded by func_1502B6BC: the text, then the sound effects voiced one per word. */
typedef struct TextResource {
    /* 0x0 */ char *text;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u16 *sounds;
    /* 0xC */ u32 soundsSize; /* in bytes */
} TextResource;

/* Text box setup: a message-table entry, or one supplied by the caller (see func_1516A7B0). */
typedef struct TextBoxConfig {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 text;            /* message table index, or the string itself when supplied */
    /* 0x08 */ u8 numPages;         /* strings in the text, separated by NULs */
    /* 0x09 */ u8 objIndex;         /* D_800CC2D0 object the box belongs to */
    /* 0x0A */ u8 unkA;
    /* 0x0B */ u8 padB;
    /* 0x0C */ f32 worldX;          /* projected to screen when not TEXTBOX_SCREEN_POS */
    /* 0x10 */ f32 worldY;
    /* 0x14 */ f32 worldZ;
    /* 0x18 */ f32 *screenXPercent; /* TEXTBOX_SCREEN_POS placement */
    /* 0x1C */ f32 *screenYPercent;
    /* 0x20 */ f32 *anglePercent;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ u16 duration;
    /* 0x28 */ u8 r;
    /* 0x29 */ u8 g;
    /* 0x2A */ u8 b;
    /* 0x2B */ u8 unk2B;            /* box is removed unless D_800C35EA == 1 */
    /* 0x2C */ void (*callback)(struct TextBoxConfig *config, s32 page, s32 state);
} TextBoxConfig;

/* Object type OBJTYPE_TEXTBOX, 0x74 bytes. */
typedef struct {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad1[0xC];
    /* 0x0D */ u8 source;           /* 0: D_800DD254 text, 1: D_800DD258 text, 2: caller string */
    /* 0x0E */ u8 visible;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ TextBoxConfig *config;
    /* 0x14 */ u8 unk14;            /* set for caller-supplied configs; disables button input */
    /* 0x15 */ u8 state;            /* TEXTBOX_STATE_* */
    /* 0x16 */ s16 timer;
    /* 0x18 */ s16 fadeTimer;
    /* 0x1A */ u8 textAlpha;
    /* 0x1B */ u8 bubbleAlpha[3];
    /* 0x1E */ u8 bubbleIndex;
    /* 0x1F */ u8 placed;           /* screen position taken from the world point */
    /* 0x20 */ s8 page;
    /* 0x21 */ u8 pad21[3];
    /* 0x24 */ u8 *text;            /* current page */
    /* 0x28 */ f32 scale;
    /* 0x2C */ f32 scaleSpeed;
    /* 0x30 */ f32 halfTextWidth;
    /* 0x34 */ f32 halfTextHeight;
    /* 0x38 */ f32 width;
    /* 0x3C */ f32 height;
    /* 0x40 */ f32 x;               /* anchor (tip of the tail) */
    /* 0x44 */ f32 y;
    /* 0x48 */ f32 bodyX;           /* scaled body position */
    /* 0x4C */ f32 bodyY;
    /* 0x50 */ f32 centerX;         /* unscaled body position: the text origin */
    /* 0x54 */ f32 centerY;
    /* 0x58 */ f32 angle;           /* direction from the anchor to the body, radians */
    /* 0x5C */ f32 anchorX;
    /* 0x60 */ f32 anchorY;
    /* 0x64 */ f32 baseX;
    /* 0x68 */ f32 baseY;
    /* 0x6C */ f32 yaw;             /* object heading relative to the camera, degrees */
    /* 0x70 */ f32 bubbleX;
} TextBox;

typedef struct {
    u8  pad0[0x4];
    u16 unk4;
    u8  pad6[0x2];
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8  pad14[0x4];
    f32 unk18;
} struct197;

/* Defined by the files that use them. */
struct Attachment;
struct CamData;
struct TimedEvent;

/* Aim and trail points an actor keeps at 0x1A4 (filled in by func_1503A08C). */
typedef struct ActorAimPoints {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 topY;
    /* 0x08 */ s16 side[2][3]; // points pushed out to either side (trails are emitted from them)
    /* 0x14 */ s8  dir[3];     // facing direction in 1/256 units
} ActorAimPoints; // size 0x18

/* Per-actor vertex morph state (Actor.morph): blends from keyframe `prev` to keyframe `cur`. */
typedef struct VtxMorphState {
    /* 0x0 */ f32 blend; // 0.0f (prev) .. 1.0f (cur)
    /* 0x4 */ f32 rate;  // blend increase per tick
    /* 0x8 */ u8  prev;
    /* 0x9 */ u8  cur;
    /* 0xA */ u8  hold;  // counts down once the blend has reached 1.0f
} VtxMorphState; // size 0xC

/* The game actor: the 0x32C-byte entries of D_800CC2D0. */
struct Actor {
    /* 0x00 */  s32 interaction_state;
    /* 0x04 */  u8  id;
    u8  unk5;
    u8  unk6;
    u8  unk7;
    u8  unk8;
    u8  unk9;
    u8  unkA;
    /* 0x0B */  u8  unkB[4];  // one per player
    /* 0x0F */  u8  unkF;     // per-player bits (<< 4: seen by player i)
    /* 0x10 */  u16 unk10;    // view range
    /* 0x12 */  u16 unk12;    // frame counter
    /* 0x14 */  f32 x_position;
    /* 0x18 */  f32 y_position;
    /* 0x1C */  f32 z_position;
    /* 0x20 */  f32 y_velocity;
    /* 0x24 */  f32 gravity;
    f32 unk28;
    /* 0x2C */  f32 old_x_position;
    /* 0x30 */  f32 old_y_position;
    /* 0x34 */  f32 old_z_position;
    /* 0x38 */  s16 unk38;
    s8  unk3A;
    /* 0x3B */  u8  unique_id;
    /* 0x3C */  f32 xz_velocity;
    f32 unk40;  // used
    f32 unk44;
    f32 unk48;
    /* 0x4C */  f32 animation_speed;
    u32 pad50;
    /* 0x54 */  f32 unk54;
    /* 0x58 */  s32 unk58;
    s32 unk5C;  // used
    /* 0x60 */  s16 unk60;    // texture scroll
    /* 0x62 */  s16 unk62;    // its speed
    s8  unk64;
    u8  unk65;  // used
    /* 0x66 */  u8  unk66;      // flags
    u8  pad67;
    /* 0x68 */  u8  unk68[2];    // texture frame of each animated part (eyes), from eyeCode
    /* 0x6A */  u8  eyeClosed[2]; // per eye, toggled by blinking
    /* 0x6C */  u8  eyeCode[2];   // >= 10: set by the current expression
    /* 0x6E */  u8  blinkTimer;
    /* 0x6F */  u8  expression;   // current expression index
    /* 0x70 */  u8  defaultExpression;
    /* 0x71 */  u8  expressionPriority;
    /* 0x72 */  u16 expressionTimer; // 0xFFFF = no timeout, 0xFFFE = frozen
    u8  unk74;
    s8  pad75;
    u16 unk76;  // moving_angle
    u16 unk78;
    u16 unk7A;  // facing angle
    u16 unk7C;
    u8  unk7E;
    u8  unk7F;  // counter
    u8  unk80;  // counter
    u8  unk81;  // counter
    u8  unk82;  // counter
    u8  unk83;  // used
    union {
        u16 uh;
        u8  ub[2];
    }   unk84;
    u8  unk86;
    u8  unk87;
    u8  unk88;
    /* 0x89 */  u8  disable_run;   // slowdown?
    /* 0x8A */  u8  disable_jump; // unk8A;
    u8  pad8B;
    u16 unk8C;  // used
    u16 unk8E;  // used
    /* 0x90 */  s16 unk90;    // saved unkE4
    u16 unk92;
    s32 unk94;
    /* 0x98 */  s32 unk98;     // blacked-out display-list mask
    u32 unk9C;
    /* 0xA0 */  u32 fogColor;  // packed ARGB; in fog mode 1 a pointer to an f32 percentage
    /* 0xA4 */  u8  fogMode;
    /* 0xA5 */  u8  fogLevel;  // fade level, or pulse phase in modes 4/5
    /* 0xA6 */  u8  fogRate;
    /* 0xA7 */  u8  fogFade;   // pulse fade-out, 0xFF = full
    u8  unkA8;
    u8  unkA9;
    u8  unkAA;
    u8  unkAB;
    /* 0xAC */  u8  unkAC;
    /* 0xAD */  u8  in_water;
    u8  unkAE;
    u8  padAF;
    s8  unkB0;
    u8  padB1;
    u16 unkB2;  // (oxygen?)
    /* 0xB4 */  f32 lastEventTime; // animation time of the last timed event (func_1506AD30)
    f32 unkB8;  // used
    /* 0xBC */  f32 unkBC;
    /* 0xC0 */  f32 target_speed;
    f32 unkC4;
    /* 0xC8 */  f32 unkC8;
    s16 unkCC;
    s16 unkCE;
    u8  unkD0;
    s8  padD1;
    s16 unkD2;    // used
    s16 unkD4;    // used
    s16 unkD6;    // used
    s16 unkD8;
    s16 unkDA;
    f32 unkDC;    // used
    f32 unkE0;    // used
    s16 unkE4;    // used
    s16 unkE6;    // used
    u16 unkE8;    // used
    u16 unkEA;
    f32 unkEC;
    f32 unkF0;
    s32 unkF4;
    s32 unkF8;
    s32 unkFC;
    u8  unk100;   // used
    u8  unk101;
    u8  unk102;
    u8  unk103;
    /* 0x104 */ u8  stunned; //unk104;
    u8  unk105;
    u8  unk106;
    u8  unk107;
    u8  pad108;
    u8  unk109;
    u8  pad10A;
    u8  unk10B;
    s16 unk10C;
    u8  unk10E;
    u8  unk10F;
    /* 0x110 */ u8  unk110;
    u8  pad111;
    /* 0x112 */ u16 unk112;
    f32 unk114;
    f32 unk118;
    f32 unk11C;
    /* 0x120 */ s16 unk120;
    u8  unk122;
    u8  unk123;
    u8  unk124;
    /* 0x125 */ u8  immune; // used
    u8  pad126;
    u8  unk127;
    /* 0x128 */ u8  team;
    /* 0x129 */ u8  unk129;
    /* 0x12A */ u8  unk12A;
    u8  pad12B;
    /* 0x12C */ s16 unk12C;
    u16 pad12E;
    u32 pad130;
    /* 0x134 */ u8  unk134;
    /* 0x135 */ u8  morphDuration;
    /* 0x136 */ u8  unk136;  // sprite texture
    /* 0x137 */ u8  unk137;
    u8  unk138;
    /* 0x139 */ u8  unk139;  // requested sequence
    /* 0x13A */ u8  unk13A;  // its countdown
    /* 0x13B */ u8  unk13B;
    u8  unk13C; // used
    u8  unk13D;
    u8  unk13E;
    /* 0x13F */ u8  unk13F; // index of its spawn record in D_800D20FC, 0xFF = none
    /* 0x140 */ u8  unk140;
    u8  pad141[0x3];
    /* 0x144 */ SpawnRecord *spawn; // the spawn record this actor came from
    f32 unk148;
    /* 0x14C */ f32 xz_scale;
    /* 0x150 */ f32 y_scale;
    f32 unk154;
    f32 unk158;
    f32 unk15C;
    /* 0x160 */ u16 unk160;  // shadow size, scaled by xz_scale (game_13AE20)
    /* 0x162 */ u8  unk162;
    u8  pad163;
    f32 unk164;
    f32 unk168;
    f32 unk16C;
    f32 unk170;
    /* 0x174 */ f32 unk174;
    /* 0x178 */ f32 unk178;
    /* 0x17C */ f32 ceilingY;
    /* 0x180 */ f32 floorY;
    u32 unk184;
    /* 0x188 */ void *unk188;     // floor triangle found by the floor probe: its three vertex pointers (Vec3s *[3], game_13AE20)
    /* 0x18C */ s16 floorVerts[3][3]; // the floor triangle under the actor
    /* 0x19E */ u16 floorTriIndex;
    /* 0x1A0 */ u16 propNum;   // D_800DBEF4 index + 1 of the prop stood on, 0 = none
    /* 0x1A2 */ s16 floorType;
    /* 0x1A4 */ ActorAimPoints aim;
    u32 pad1BC;
    u32 pad1C0;
    /* 0x1C4 */ struct TimedEvent *events; // timed event list
    /* 0x1C8 */ u8  lod;       // current level of detail
    /* 0x1C9 */ u8  lodRequest; // requested LOD + 1, 0xFF = reset to 0
    /* 0x1CA */ u8  health;
    u8  unk1CB;
    f32 unk1CC;
    s8  unk1D0;
    s8  unk1D1;
    u16 pad1D2;
    /* 0x1D4 */ MtxF *unk1D4;   // this frame's bone matrices
    /* 0x1D8 */ MtxF *prevBones; // last frame's copy (platforms)
    u8  pad1DC;
    /* 0x1DD */ u8  unk1DD[3];
    /* 0x1E0 */ u8  unk1E0[3];
    /* 0x1E3 */ u8  unk1E3;
    u8  unk1E4;
    u8  unk1E5;
    u8  unk1E6;
    u8  unk1E7;
    u8  unk1E8;
    u8  pad1E9;
    u16 unk1EA;
    u16 unk1EC;
    u8  pad1EE[0x2];
    u32 pad1F0;
    /* 0x1F4 */ f32 unk1F4;
    /* 0x1F8 */ f32 unk1F8;
    u8  unk1FC;
    u8  unk1FD;
    /* 0x1FE */ u8  level;       // 0..255, moved by levelStep each update (func_15034860)
    /* 0x1FF */ u8  levelMode;   // 0: fall, 2: rise, else random walk (3: until levelTimer runs out)
    /* 0x200 */ u8  levelTimer;
    /* 0x201 */ u8  unk201;
    /* 0x202 */ s8  unk202;
    /* 0x203 */ s8  levelStep;
    /* 0x204 */ s16 level2;      // -255..255 (direction set by the sign of unk208)
    /* 0x206 */ u8  level2Mode;  // same encoding as levelMode
    /* 0x207 */ u8  level2Timer;
    /* 0x208 */ s8  unk208;
    /* 0x209 */ s8  level2Step;
    /* 0x20A */ u8  unk20A;
    /* 0x20B */ u8  unk20B[4];   // script variables (command 0x59)
    u8  unk20F;
    /* 0x210 */ u8  unk210;
    u8  unk211;
    u8  unk212[0x2];
    s16 unk214;
    /* 0x216 */ s16 unk216;      // distance bias (game_C4DC0)
    struct216 *unk218; // struct216 is 5 bytes long
    u16 unk21C;
    u8  unk21E;
    u8  unk21F;
    u8  unk220;
    s8  unk221;
    u8  unk222;
    u8  unk223;
    s16 unk224;
    u8  unk226;
    u8  pad227;
    u8  unk228;
    u8  unk229;
    u8  pad22A;
    u8  unk22B;
    u16 unk22C;
    s16 unk22E;
    u8  pad230;
    u8  unk231;
    u8  unk232; // used
    u8  unk233;
    u8  unk234;
    u8  unk235;
    u8  unk236;
    u8  unk237;
    u8  unk238;
    s8  unk239; // used
    u8  unk23A;
    u8  unk23B;
    /* 0x23C */ u8  unk23C;
    u8  unk23D;
    u8  unk23E;
    u8  unk23F;
    u8  unk240;
    u8  unk241;
    u8  unk242;
    u8  pad243;
    s16 unk244;
    u8  unk246;
    u8  unk247;
    u8  unk248;
    u8  unk249;
    u8  unk24A;
    u8  pad24B;
    u8  unk24C;
    u8  unk24D;
    u8  unk24E;
    s8  unk24F;
    u8  unk250;
    u8  unk251;
    u8  unk252;
    u8  unk253;
    u8  unk254;
    u8  unk255;
    u8  unk256;
    u8  unk257;
    u8  unk258;
    u8  pad259[0x3];
    s32 unk25C;
    /* 0x260 */ VtxMorphState *morph; // vertex morph (game_1A8060), NULL when the model has none
    /* 0x264 */ s32 unk264;
    u8  pad268[0x8];
    /* 0x270 */ f32 unk270;
    /* 0x274 */ u8  objectNum; // D_800CC2D0 index + 1 of the object stood on, 0 = none
    u8  pad275;
    /* 0x276 */ u8  unk276;      // head-turn effect kind, 0-6 (func_1503D9B4)
    u8  pad277;
    /* 0x278 */ u16 unk278;      // effect state
    /* 0x27A */ s16 unk27A[2];   // yaw of head parts 1 and 0
    /* 0x27E */ s16 unk27E[2];   // pitch of head parts 1 and 0
    /* 0x282 */ u16 unk282;      // effect frames left, 0xFFFF = endless
    /* 0x284 */ s8  unk284;      // effect offset for unk27A[0]
    /* 0x285 */ s8  unk285;      // ... for unk27E[0]
    /* 0x286 */ s8  unk286;      // ... for unk27A[1]
    /* 0x287 */ s8  unk287;      // ... for unk27E[1]
    /* 0x288 */ u8  unk288;
    u8  pad289[0x3];
    /* 0x28C */ u8 *modelData[7][2]; // per model in modelIds, per frame buffer
    /* 0x2C4 */ u8 *modelIds;
    /* 0x2C8 */ u8  lodCount;   // models in modelIds that are LODs
    /* 0x2C9 */ u8  modelCount; // lodCount + extra sub-models
    s8  unk2CA;
    s8  unk2CB;
    s32 unk2CC;
    struct197 *unk2D0;
    /* 0x2D4 */ struct102 *unk2D4;
    /* 0x2D8 */ f32 unk2D8[3];   // launch velocity (y, gravity, xz) for func_15071FDC, set by script command 0x22
    s32 unk2E4;
    s32 unk2E8;
    s32 unk2EC;
    u32 pad2F0;
    /* 0x2F4 */ struct Attachment *attachments; // head of the attachment list
    u16 unk2F8; // wait animation?
    u8  unk2FA;
    u8  unk2FB;
    /* 0x2FC */ u8  unk2FC;
    /* 0x2FD */ u8  unk2FD;
    /* 0x2FE */ u8  unk2FE;
    u8  unk2FF;
    u8  unk300;
    /* 0x301 */ u8  unk301;
    /* 0x302 */ u8  unk302;
    /* 0x303 */ u8  unk303;
    /* 0x304 */ s32 unk304[4]; // one per view
    /* 0x314 */ u8 (*unk314)[8]; // one per frame buffer
    /* 0x318 */ Camera *camera;
    /* 0x31C */ ActorCtl *unk31C;
    /* 0x320 */ u32 unk320;
    /* 0x324 */ struct CamData *camData; // aim easing state, one entry per aim slot
    /* 0x328 */ s8  unk328;
    u8  pad329[0x3];
}; // size 0x32C => 812 bytes game_object

/*
 * A world object: one 0xA0-byte entry of the level's object array D_800DBEF4 (D_800DBEF0 entries),
 * built by func_150039E0 (game_30E90.c) from the level's placement lists. Owner type 1 for effects and trails.
 */
typedef struct Shadow Shadow; // blob shadow under an object, see game_13AE20.c

typedef struct WorldObject WorldObject;
struct WorldObject {
    /* 0x00 */ f32 rotX; // degrees
    /* 0x04 */ f32 rotY;
    /* 0x08 */ f32 rotZ;
    /* 0x0C */ void *model;          // loaded model asset, if any
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ u16 numVertices;
    /* 0x18 */ f32 yOffset;
    /* 0x1C */ Gfx *displayList;
    /* 0x20 */ Vtx *vertices[2];     // per frame buffer (D_800BE9C0); both srcVertices unless state bit 0
    /* 0x28 */ Vtx *srcVertices;     // the model's own vertices
    /* 0x2C */ f32 scaleX;
    /* 0x30 */ f32 scaleY;
    /* 0x34 */ f32 scaleZ;
    /* 0x38 */ void (*update)(WorldObject *obj);
    /* 0x3C */ union {
        void *data;                  // per-kind data, e.g. a Turret15112520Cfg
        s32 word;
        struct {
            s16 hi;
            s16 lo;
        } half;
        struct {
            u16 hi;
            u16 lo;
        } uhalf;
        u8 bytes[4];
    } param;                         // the placement's parameter word
    /* 0x40 */ s32 material;         // collision material, used when materials is NULL
    /* 0x44 */ s32 *materials;       // per-triangle materials, from firstTri on
    /* 0x48 */ u8 pad48[4];
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ u8 attach;            // < 3: own transform, 3: none, 101..124: rides actor D_800CC2D0[attach - 100]
    /* 0x4F */ u8 flags;
    /* 0x50 */ u16 unk50;            // bounding radius, see func_15004AAC
    /* 0x52 */ u16 unk52;
    /* 0x54 */ u16 modelId;          // | 0x8000 for objects without a loaded model
    /* 0x56 */ u16 numTris;          // collision triangles, see func_15003668
    /* 0x58 */ u16 firstTri;
    /* 0x5A */ s16 moveX;            // translation this frame
    /* 0x5C */ s16 moveY;
    /* 0x5E */ s16 moveZ;
    /* 0x60 */ f32 spinX;            // rotation this frame (degrees)
    /* 0x64 */ f32 spinY;
    /* 0x68 */ f32 spinZ;
    /* 0x6C */ u16 effectFlags;
    /* 0x6E */ u8 inactive;          // 1: not updated (finished or removed), no floor probe
    /* 0x6F */ u8 viewFlags;         // bits 0-3: visible in view N, 0x40: update this frame, 0x80: hidden
    /* 0x70 */ u8 state;             // 1: vertices transformed into per-frame copies, 2: drawn by func_150A50C0,
                                     // 8: removed, high nibble: objects following in its group
    /* 0x71 */ u8 index;             // own index in D_800DBEF4
    /* 0x72 */ u8 id;                // found by func_151149AC
    /* 0x73 */ u8 behaviorState;     // per-behaviour state bits
    /* 0x74 */ u16 sound;            // handle from func_15114D24
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ void (*onContact)(WorldObject *obj, Actor *actor);
    /* 0x7C */ union {
        s32 words[3];
        f32 floats[3];
        struct {
            /* 0x7C */ s32 unk7C;
            /* 0x80 */ s32 tri;      // floor probe results, see func_150045C4
            /* 0x84 */ f32 y;
        } floor;
    } work;                          // per-behaviour scratch state
    /* 0x88 */ u16 fadeDistance;     // fades out when the camera is closer than this
    /* 0x8A */ u8 alpha;
    /* 0x8B */ u8 viewAlpha[4];
    /* 0x8F */ u8 pad8F;
    /* 0x90 */ u16 drawDistance;     // 0: default (D_800D9B1C), 1: 40000
    /* 0x92 */ u8 unk92;
    /* 0x93 */ u8 pad93;
    /* 0x94 */ Shadow *shadow;       // created and freed by func_15113C88
    /* 0x98 */ s32 unk98;            // with unk9C, passed by address to func_1503F800
    /* 0x9C */ s32 unk9C;
}; // size 0xA0

typedef struct {
    u8  pad0[0x48];
    u8  unk48;  // used
    u8  pad49;
    u8  pad4A;
    u8  pad4B;
    s32 unk4C; // used
} struct132;

typedef struct {
    u8 pad[0x1b];
    u8 unk1B;
} struct133;

typedef struct {
    s16 unk0;
    u16 unk2;
    s16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    s32 unkC;
    s32 unk10;
    u16 unk14;
    u8  unk16;
    u8  unk17;
    s32 unk18;
    s32 unk1C; // struct ptr?
    s32 unk20;
    s32 unk24;
} struct134;

typedef struct {
    struct134 *unk0;
    struct134 *unk4;
    s32 unk8;
    struct134 *unkC;
    f32 unk10;
    f32 unk14;
    s32 unk18;
    s8  unk1C;
    s8  pad1D;
    s8  pad1E;
    s8  pad1F;
} struct135; // size 0x34 (memcpy) ?

typedef struct struct137 struct137;

struct struct137 {
    s32 unk0; // used
    s32 unk4; // used
    s32 unk8; // used
    s32 unkC; // used
    struct137 *unk10; // used
    s32 pad14;
    s32 pad18;
    s32 pad1C;
    s32 pad20;
    s32 pad24;
    s32 pad28;
    s32 unk2C; // used
    u32 unk30; // used
    s32 pad34;
    s32 pad38;
    s32 pad3C;
    s32 pad40;
    s32 pad44;
    s32 pad48;
    u16 unk4C; // used
    u16 unk4E; // used
    u16 pad50;
    u16 unk52; // used
    u16 unk54; // used
    u16 pad56;
    u16 unk58; // used
    u16 unk5A; // used
    u16 pad5C;
    u16 pad5E;
    u16 pad60;
    u16 pad62;
}; // 0x64 (100) bytes

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
} struct138;

typedef struct {
    s16 unk0;
    s16 unk2;
    s32 unk4;
    s16 unk8[16];
} struct139;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    u8  pad14[0x20];
    f32 unk34;
    f32 unk38;
    u8  pad3C[0x8];
    s32 unk44;
    s32 unk4C;
    struct14 unk50[3]; // 3 * 0x10 structs
    u8  pad80[0x4];
    s32 unk84;
    u8  pad88[0xf4];
} struct140; // 0x180

typedef struct {
    u16 unk0;
    u16 unk2;
    s16 unk4;
    u16 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
    s32 unk28;
    f32 unk2C;
    f32 unk30; // sp70
    f32 unk34;
    s16 unk38;
    s16 unk3A;
    f32 unk3C;
    f32 unk40; // sp80
    s16 unk44;
    s16 unk46;
    s32 unk48;
    s32 unk4C;
} struct145;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
} struct146; // dummy 0x18 length

typedef struct {
    u8 unk0;  // used
    u8 unk1;
    u8 unk2;
    u8 unk3;
    s32 unk4; // used but might be u8
    s32 unk8; // used
    struct54 *unkC;
    struct54 *unk10;
    s32 unk14;
} struct147;

typedef struct {
    u32 unk0;
    u32 unk4;
} struct148;

typedef struct {
    s32 pad0;
    u8  unk4;
    s8  unk5; // used
} struct152;

typedef struct {
    s32 pad0;
    struct152 *unk4;
} struct153;

typedef struct {
    s32 pad0;
    s32 pad4;
    s32 pad8;
    struct153 *unkC;
    s32 pad10;
    s32 pad14;
    s32 pad18;
    s32 pad1C;
    s32 pad20;
    s32 pad24;
    s32 pad28;
    s32 pad2C;
    s32 pad30;
    f32 unk34;  // used
} struct154;

typedef struct {
    u8 pad[0x68];
} struct156; // size 0x68

typedef struct {
    s32 unk0;
    s16 unk4;
    s8  unk6;
    s8  unk7;
    s32 unk8;
    s32 unkC;
    s8  unk10;
    s8  unk11;
    s8  unk12;
    s8  unk13;
    s8  unk14;
    s8  unk15;
    s8  unk16;
    s8  unk17;
    s32 unk18;
    s32 unk1C;
    s8  unk20;
    s8  pad21;
    s16 unk22;
    s16 unk24;
} struct157;

/* Camera command playback state (Camera.cmdState, at 0x6CC). */
typedef struct CamCmdState {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cmdId;                            /* id (cmd.id) of the command last started */
    /* 0x04 */ s16 lastType;                         /* cmd.type of the last command run */
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 blending;                         /* still easing back (blend < 1) after a type 1 command */
    /* 0x0A */ u8 padA[0x6];
    /* 0x10 */ f32 blend;                            /* 0..1 easing factor */
    /* 0x14 */ f32 pushRate;                         /* 0..0.04, scales the minimum-distance push */
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 minHeight;
    /* 0x28 */ f32 maxHeight;
    /* 0x2C */ f32 maxDistance;
} CamCmdState; // size 0x30

/* Current camera command (Camera.cmd, at 0x6FC). Type 15 is a rail (see CamRail). */
typedef struct CamCmd {
    /* 0x00 */ u32 type;
    /* 0x04 */ s16 id;
    /* 0x06 */ u8 pad6[0x2];
    /* 0x08 */ u32 unk8;                             /* type 7: path options (copied to Camera.pathFlags); type 15: rail height offset */
    /* 0x0C */ u8 padC[0x4];
    /* 0x10 */ s16 x;                                /* type 15: rail midpoint */
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 halfLength;                       /* type 15: half the rail length */
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u32 unk1C;                            /* type 15: rail offset mode (copied to rail.offsetMode) */
    /* 0x20 */ u32 unk20;                            /* type 15: rail offset (copied to rail.offset) */
    /* 0x24 */ f32 unk24;                            /* type 15: unk24/unk28 split halfLength into horizontal/vertical parts, */
    /* 0x28 */ f32 unk28;                            /* unk2C/unk30 split the horizontal part into x/z */
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 pad36[0x2];
} CamCmd; // size 0x38

/* Aim (first-person swing) state (Camera.aim, at 0x740). */
typedef struct CamAim {
    /* 0x00 */ Vec3f orbitPos;                      /* eye position on the orbit around the target */
    /* 0x0C */ Vec3f offset;                        /* sideways shift of the aim ray (x and z only) */
    /* 0x18 */ f32 yawVel;
    /* 0x1C */ f32 pitchVel;
    /* 0x20 */ f32 savedYaw;                         /* view to return to on exit */
    /* 0x24 */ f32 savedTargetDistance;
    /* 0x28 */ f32 savedHeight;
    /* 0x2C */ f32 savedUnk134;
    /* 0x30 */ f32 savedUnk354;
    /* 0x34 */ f32 targetYaw;
    /* 0x38 */ f32 targetPitch;
    /* 0x3C */ f32 pitch;
    /* 0x40 */ f32 signedPitch;                      /* pitch in -180..180 */
    /* 0x44 */ f32 distance;
    /* 0x48 */ f32 turnDistance;                     /* swing round once Camera.distance passes this */
    /* 0x4C */ u8 useOverride;                       /* take pitch, yaw offset and distance from the fields below */
    /* 0x4D */ u8 unk4D;
    /* 0x4E */ s8 approachDir;                       /* -1: distance grows towards turnDistance, 1: it shrinks */
    /* 0x4F */ u8 pad4F[0x1];
    /* 0x50 */ f32 overridePitch;
    /* 0x54 */ f32 overrideYawOffset;
    /* 0x58 */ f32 overrideDistance;
    /* 0x5C */ f32 offsetXVel;
    /* 0x60 */ f32 offsetZVel;
    /* 0x64 */ s32 stickLock;                        /* bit 0: stick_x held at <= 0, bit 1: at >= 0 */
} CamAim; // size 0x68

/* Smooth transition of the camera to a new target/yaw (func_15128030; Camera.transition, at 0x7F4). */
typedef struct CamTransition {
    /* 0x00 */ u16 state;
    /* 0x02 */ u8 pad2[0x2];
    /* 0x04 */ Vec3f startTarget;
    /* 0x10 */ Vec3f endTarget;
    /* 0x1C */ f32 startDistance;
    /* 0x20 */ f32 startYaw;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 endDistance;
    /* 0x2C */ f32 endYaw;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 endUnk5E8;
    /* 0x38 */ f32 endUnk3A8;
    /* 0x3C */ f32 progress;
    /* 0x40 */ u16 duration;
    /* 0x42 */ u8 pad42[0x2];
    /* 0x44 */ f32 endHeight;
    /* 0x48 */ f32 startHeight;
    /* 0x4C */ s16 timer;
    /* 0x4E */ u8 pad4E[0x2];
} CamTransition; // size 0x50

/* Camera rail: a line segment the eye slides along (Camera.rail, at 0x870; set up by a type 15 command). */
typedef struct CamRail {
    /* 0x00 */ u8 offsetMode;                        /* which side of the rail the eye sits on (0..3) */
    /* 0x01 */ u8 pad1[0x3];
    /* 0x04 */ Vec3f start;
    /* 0x10 */ Vec3f dir;                           /* unit direction from start to end */
    /* 0x1C */ f32 invLength;
    /* 0x20 */ f32 offset;                           /* horizontal distance of the eye from the rail */
    /* 0x24 */ f32 heightOffset;
    /* 0x28 */ f32 eyeEaseA;                         /* easing parameters for the eye, eased toward 10.0 */
    /* 0x2C */ f32 eyeEaseB;
    /* 0x30 */ f32 eyeEaseAVel;
    /* 0x34 */ f32 eyeEaseBVel;
    /* 0x38 */ Vec3f eyeGoal;
} CamRail; // size 0x44

/* One player camera (Actor.camera; array at D_800DBFF0). Mode state 0x000..0x236 is a stack of 21 save slots (func_15123934 / func_151239CC). */
struct Camera {
    /* 0x000 */ u16 modeIndex;                       /* bit index of mode */
    /* 0x002 */ u16 savedModeIndex[21];
    /* 0x02C */ s32 mode;                            /* one-hot mode bit; 0x100 is the R-trigger camera */
    /* 0x030 */ s32 savedMode[21];
    /* 0x084 */ s32 flags;
    /* 0x088 */ s32 savedFlags[21];
    /* 0x0DC */ s32 unkDC;
    /* 0x0E0 */ s32 savedUnkDC[21];
    /* 0x134 */ s32 unk134;
    /* 0x138 */ s32 savedUnk134[21];
    /* 0x18C */ f32 lift;                            /* vertical offset of eye and target */
    /* 0x190 */ f32 liftTarget;
    /* 0x194 */ f32 side;                            /* sideways offset of eye and target */
    /* 0x198 */ f32 sideTarget;
    /* 0x19C */ f32 unk19C;
    /* 0x1A0 */ f32 unk1A0;
    /* 0x1A4 */ f32 unk1A4;
    /* 0x1A8 */ f32 unk1A8;
    /* 0x1AC */ u8 pad1AC[0x8];
    /* 0x1B4 */ s16 preset;                          /* row of D_800A34B0 (distance, height) */
    /* 0x1B6 */ s16 savedPreset[21];
    /* 0x1E0 */ s16 presetMask;                      /* presets reachable with the C buttons */
    /* 0x1E2 */ s16 savedPresetMask[21];
    /* 0x20C */ s16 slotUsed[21];
    /* 0x236 */ u8 pad236[0x2];
    /* 0x238 */ s32 unk238;
    /* 0x23C */ u8 snap;                             /* nonzero: jump to targets instead of easing (counts down) */
    /* 0x23D */ u8 playerIndex;                      /* this camera's slot in the per-camera tables */
    /* 0x23E */ u8 zone;                             /* copy of playerExt->cameraZone */
    /* 0x23F */ u8 unk23F;
    /* 0x240 */ s32 unk240;
    /* 0x244 */ s16 pairIndex;                       /* second object kept in frame (0 = none) */
    /* 0x246 */ u8 pad246[0x2];
    /* 0x248 */ Actor *pairObj;
    /* 0x24C */ f32 unk24C;
    /* 0x250 */ f32 unk250;
    /* 0x254 */ u8 pad254[0x4];
    /* 0x258 */ f32 distanceVel;
    /* 0x25C */ f32 heightVel;
    /* 0x260 */ u8 pad260[0x4];
    /* 0x264 */ f32 swayVel;
    /* 0x268 */ u8 pad268[0x4];
    /* 0x26C */ f32 unk26C;
    /* 0x270 */ u8 pad270[0x2];
    /* 0x272 */ s16 unk272;
    /* 0x274 */ s16 unk274;
    /* 0x276 */ u8 pad276[0x2];
    /* 0x278 */ f32 unk278;
    /* 0x27C */ Vec3f unk27C;
    /* 0x288 */ Vec3f unk288;
    /* 0x294 */ u8 pad294[0x4];
    /* 0x298 */ s16 playerMoved;
    /* 0x29A */ u8 pad29A[0x2];
    /* 0x29C */ f32 swayPhase;
    /* 0x2A0 */ f32 targetYVel;
    /* 0x2A4 */ Vec3f focus;                        /* point followed on the player */
    /* 0x2B0 */ Vec3f lastPlayerPos;
    /* 0x2BC */ Vec3f target;                       /* look-at point */
    /* 0x2C8 */ Vec3f prevTarget;
    /* 0x2D4 */ u8 pad2D4[0xC];
    /* 0x2E0 */ Vec3f savedTarget;
    /* 0x2EC */ Vec3f savedEye;
    /* 0x2F8 */ Vec3f eye;                          /* camera position */
    /* 0x304 */ Vec3f prevEye;
    /* 0x310 */ u8 pad310[0x34];
    /* 0x344 */ f32 unk344;
    /* 0x348 */ f32 height;
    /* 0x34C */ f32 unk34C;
    /* 0x350 */ u8 pad350[0x4];
    /* 0x354 */ f32 unk354;
    /* 0x358 */ f32 unk358;
    /* 0x35C */ f32 unk35C;
    /* 0x360 */ f32 unk360;
    /* 0x364 */ f32 unk364;
    /* 0x368 */ s16 controller;
    /* 0x36A */ u16 buttonsPressed;
    /* 0x36C */ OSContPad *contPad;
    /* 0x370 */ f32 distance;
    /* 0x374 */ f32 targetDistance;
    /* 0x378 */ u8 pad378[0x4];
    /* 0x37C */ f32 yaw;                             /* degrees */
    /* 0x380 */ f32 unk380;
    /* 0x384 */ f32 unk384;
    /* 0x388 */ f32 pitch;                           /* degrees */
    /* 0x38C */ f32 unk38C;
    /* 0x390 */ f32 unk390;
    /* 0x394 */ f32 unk394;
    /* 0x398 */ f32 pitchRad;
    /* 0x39C */ f32 yawRad;
    /* 0x3A0 */ f32 unk3A0;                          /* unk380 in radians */
    /* 0x3A4 */ f32 unk3A4;
    /* 0x3A8 */ f32 unk3A8;
    /* 0x3AC */ u8 pad3AC[0x8];
    /* 0x3B4 */ f32 distanceAccel;
    /* 0x3B8 */ f32 distanceMaxSpeed;
    /* 0x3BC */ u8 pad3BC[0x4];
    /* 0x3C0 */ Vec3f eyeVel;                       /* easing velocity of eye */
    /* 0x3CC */ s16 unk3CC;
    /* 0x3CE */ u8 pad3CE[0x2];
    /* 0x3D0 */ Actor *player;                   /* the object the camera follows */
    /* 0x3D4 */ ActorCtl *playerExt;                /* player->unk31C */
    /* 0x3D8 */ u8 pad3D8[0x4];
    /* 0x3DC */ f32 unk3DC;                          /* orbit yaw of the eye, radians */
    /* 0x3E0 */ f32 unk3E0;
    /* 0x3E4 */ u8 pad3E4[0x4];
    /* 0x3E8 */ u8 unk3E8;
    /* 0x3E9 */ u8 pad3E9[0x3];
    /* 0x3EC */ f32 unk3EC;
    /* 0x3F0 */ s16 unk3F0;
    /* 0x3F2 */ u8 pad3F2[0x2];
    /* 0x3F4 */ f32 unk3F4;
    /* 0x3F8 */ f32 unk3F8;
    /* 0x3FC */ Vec3f unk3FC;
    /* 0x408 */ u8 pad408[0x1C4];
    /* 0x5CC */ f32 unk5CC;
    /* 0x5D0 */ f32 unk5D0;
    /* 0x5D4 */ s32 unk5D4;
    /* 0x5D8 */ f32 unk5D8;
    /* 0x5DC */ f32 unk5DC;
    /* 0x5E0 */ u8 pad5E0[0x8];
    /* 0x5E8 */ f32 unk5E8;
    /* 0x5EC */ f32 swayAmount;                      /* roll, degrees */
    /* 0x5F0 */ s32 status;
    /* 0x5F4 */ u8 pad5F4[0x4];
    /* 0x5F8 */ s16 unk5F8;
    /* 0x5FA */ s16 unk5FA;
    /* 0x5FC */ s16 unk5FC;
    /* 0x5FE */ s16 unk5FE;
    /* 0x600 */ u8 blocked;                          /* wall between focus and eye this frame */
    /* 0x601 */ u8 pad601[0x3];
    /* 0x604 */ f32 blockedDistance;
    /* 0x608 */ u8 pad608[0x4];
    /* 0x60C */ f32 unk60C;
    /* 0x610 */ f32 unk610;
    /* 0x614 */ Vec3f *unk614;
    /* 0x618 */ Vec3f unk618;
    /* 0x624 */ u8 pad624[0x4];
    /* 0x628 */ s32 unk628;
    /* 0x62C */ f32 unk62C;
    /* 0x630 */ f32 unk630;                          /* yaw velocity while turning toward unk614 (func_15122170) */
    /* 0x634 */ u8 pad634[0xC];
    /* 0x640 */ s32 unk640;
    /* 0x644 */ s32 unk644;
    /* 0x648 */ s32 unk648;
    /* 0x64C */ u8 pad64C[0x10];
    /* 0x65C */ f32 unk65C;
    /* 0x660 */ f32 unk660;
    /* 0x664 */ Vec3f forward;
    /* 0x670 */ f32 followRate;
    /* 0x674 */ f32 followRateTarget;
    /* 0x678 */ f32 followRateVel;
    /* 0x67C */ f32 unk67C;
    /* 0x680 */ u8 pad680[0x18];
    /* 0x698 */ s32 unk698;
    /* 0x69C */ u8 pad69C[0x14];
    /* 0x6B0 */ s32 unk6B0;                          /* orbit direction */
    /* 0x6B4 */ u8 pad6B4[0xC];
    /* 0x6C0 */ s32 orbitCounter;
    /* 0x6C4 */ f32 orbitSpeed;                      /* degrees per frame, clamped to +-10 */
    /* 0x6C8 */ s32 cmdRunning;
    /* 0x6CC */ CamCmdState cmdState;
    /* 0x6FC */ CamCmd cmd;
    /* 0x734 */ s32 unk734;
    /* 0x738 */ s32 unk738;
    /* 0x73C */ s16 aimState;
    /* 0x73E */ u8 pad73E[0x2];
    /* 0x740 */ CamAim aim;
    /* 0x7A8 */ u8 pad7A8[0x8];
    /* 0x7B0 */ f32 wobblePhase;
    /* 0x7B4 */ f32 dt;                              /* frame time scale */
    /* 0x7B8 */ s32 presetCooldown;
    /* 0x7BC */ f32 unk7BC;
    /* 0x7C0 */ f32 unk7C0;
    /* 0x7C4 */ u8 pad7C4[0x4];
    /* 0x7C8 */ f32 yawEaseVel;                      /* velocity of yaw while func_15049688 eases it (game_14F130.c, game_123FB0.c) */
    /* 0x7CC */ s32 unk7CC;
    /* 0x7D0 */ f32 sideVel;
    /* 0x7D4 */ f32 liftVel;
    /* 0x7D8 */ f32 wobbleVel;
    /* 0x7DC */ f32 wobbleAmount;
    /* 0x7E0 */ f32 spinDir;
    /* 0x7E4 */ u8 unk7E4;
    /* 0x7E5 */ u8 pad7E5[0x1];
    /* 0x7E6 */ u16 unk7E6;
    /* 0x7E8 */ f32 unk7E8;
    /* 0x7EC */ u8 pad7EC[0x8];
    /* 0x7F4 */ CamTransition transition;
    /* 0x844 */ f32 yawVel;
    /* 0x848 */ u8 unk848;
    /* 0x849 */ u8 pad849[0x1];
    /* 0x84A */ s16 aimTimer;                        /* frames left before the aim swing gives up */
    /* 0x84C */ u8 zoneHookActive;                   /* func_151C9BA0 ran; func_151C9ED4 undoes it */
    /* 0x84D */ u8 shakeState;                       /* 0 idle, 1 start requested, 2 playing */
    /* 0x84E */ u8 shakeFrame;
    /* 0x84F */ u8 pad84F[0x1];
    /* 0x850 */ s32 shakeIndex;                      /* which D_800DC280 table */
    /* 0x854 */ Vec3f shakeRot;
    /* 0x860 */ Vec3f shakePos;
    /* 0x86C */ f32 pitchOverride;                   /* degrees, 999.0f = none */
    /* 0x870 */ CamRail rail;
    /* 0x8B4 */ f32 unk8B4;
    /* 0x8B8 */ u8 unk8B8;
    /* 0x8B9 */ u8 pad8B9[0x1];
    /* 0x8BA */ s16 unk8BA;
    /* 0x8BC */ u16 *unk8BC;
    /* 0x8C0 */ f32 yawSpringVel;                    /* yaw velocity for func_15121C00 */
    /* 0x8C4 */ f32 unk8C4;
    /* 0x8C8 */ f32 unk8C8;
    /* 0x8CC */ f32 unk8CC;
    /* 0x8D0 */ s32 unk8D0;
    /* 0x8D4 */ f32 yawFollowTarget;
    /* 0x8D8 */ f32 yawFollowVel;
    /* 0x8DC */ f32 yawFollow;                       /* how fast yaw turns toward the target */
    /* 0x8E0 */ f32 lookBehindYawVel;
    /* 0x8E4 */ u8 lookBehindTimer;
    /* 0x8E5 */ u8 pad8E5[0x3];
    /* 0x8E8 */ s32 lookBehindHold;
    /* 0x8EC */ u8 lookingBehind;
    /* 0x8ED */ u8 unk8ED;
    /* 0x8EE */ u8 pad8EE[0x2];
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
    /* 0x920 */ u16 pathFlags;                       /* type 7 command options (cmd.unk8) */
    /* 0x922 */ u8 pad922[0x2];
    /* 0x924 */ f32 unk924;
    /* 0x928 */ f32 unk928;
    /* 0x92C */ u8 unk92C;
    /* 0x92D */ u8 pad92D[0x3];
    /* 0x930 */ f32 unk930;
    /* 0x934 */ f32 unk934;
    /* 0x938 */ f32 unk938;
    /* 0x93C */ Vec3f unk93C;
    /* 0x948 */ u8 unk948;
    /* 0x949 */ u8 unk949;
    /* 0x94A */ u8 pad94A[0x2];
    /* 0x94C */ f32 unk94C;
    /* 0x950 */ f32 unk950;
    /* 0x954 */ f32 unk954;
    /* 0x958 */ f32 unk958;
    /* 0x95C */ f32 unk95C;
    /* 0x960 */ f32 unk960;
    /* 0x964 */ f32 unk964;
    /* 0x968 */ f32 unk968;
    /* 0x96C */ f32 speed;                           /* yaw easing speed and limit for func_15049688 (game_14F130.c) */
    /* 0x970 */ f32 limit;
    /* 0x974 */ f32 unk974;
    /* 0x978 */ f32 unk978;
    /* 0x97C */ f32 targetSpeed;
    /* 0x980 */ f32 targetLimit;
    /* 0x984 */ f32 unk984;
    /* 0x988 */ f32 unk988;
    /* 0x98C */ f32 speedVel;
    /* 0x990 */ f32 limitVel;
    /* 0x994 */ f32 sideOffset;
    /* 0x998 */ f32 sideOffsetVel;
    /* 0x99C */ f32 sideOffsetTarget;
}; // size 0x9A0



// func_80002E50
typedef struct {
    u16 unk0;
    u16 unk2;
    OSMesgQueue *unk4;
    OSMesgQueue *unk8;
    OSMesgQueue *unkC;
    OSMesgQueue *unk10;
    s32 (*unk14)(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
    s32 (*unk18)(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
} struct158;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u32 unk8;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} struct160; // size 0x14

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} struct161; // size 0x18

typedef struct {
    u8  pad0[0x8];
    s32 unk8;
    u8  padC[0x2];
    s8  unkE;
    s8  padF;
    u8  pad10[0x8];
    s32 unk18;
    u8  pad1C[0x2];
    s8  unk1E;
    s8  pad1F;
    u8  pad20[0x8];
    s32 unk28;
    u8  pad2C[0x2];
    s8  unk2E;
    s8  pad2F;
} struct162;

typedef struct {
    u16 unk0;
    u8  pad2[0xA];
    u8  unkC;
    u8  padD[0x2B];
} struct163; // size 0x38

typedef struct {
    f32 unk0;   // used
    f32 unk4;   // used
    f32 unk8;   // used
    f32 unkC;
    f32 unk10;  // used
    f32 unk14;  // used
    f32 unk18;  // used
    f32 unk1C;
    f32 unk20;  // used
    f32 unk24;  // used
    f32 unk28;  // used
    f32 unk2C;
} struct164;

typedef struct {
    s32 unk0;
    u8  pad[0x14];
    f32 unk18;
} struct165;

typedef struct {
    s32 unk0;  // used
    s32 unk4;  // used
    s32 unk8;  // used
} struct167;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4; // scene
    /* 0x08 */ f32 unk8;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ f32 unk14; // eye x, y, z
    /* 0x18 */ f32 unk18;
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24; // pitch
    /* 0x28 */ f32 unk28; // yaw
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ u8  pad38[0x24];
    /* 0x5C */ f32 unk5C;
} struct168; // a scripted camera shot (func_1512D560 message), D_800C3600

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    u8  pad12[0x3];
    u8  unk15;
} struct169;

typedef struct {
    s32 unk0; // sp60
    u8  pad4[0x10];
    f32 unk18;
    f32 unk1C;
    u8  pad20[0x4];
    f32 unk24;
    s8  unk28;
    s8  unk29;
    s8  unk2A;
    s8  unk2B;
    f32 unk2C; // sp8C
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8  pad3C[0x4];
    s32 unk40; // sp A0  // guess f32
    f32 unk44;           // guess f32
    s16 unk48;
    s16 unk4A;
    f32 unk4C;           // guess f32
    f32 unk50; // spB0   // guess f32
    s8  unk54;
    s8  unk55;
    s8  unk56;
    s8  unk57;
    s8  unk58;
    s8  unk59;
    s8  unk5A;
    s8  unk5B;
    s8  unk5C;
    s8  unk5D;
    s16 unk5E;
    s16 unk60; // spc0
    s16 unk62;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70; // spD0
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90; // spF0
    f32 unk94;
    s32 unk98;
    s32 unk9C;
    s8  unkA0; // sp100
    s8  unkA1;
    s8  unkA2;
    s8  unkA3;
    s8  unkA4;
    s8  unkA5;
} struct170;

typedef struct {
    f32 unk0;   // sp8C
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;  // sp9C
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;  // spAC
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;  // spBC
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40; // spCC
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50; // spDC
    f32 unk54;
    s32 unk58;
    s16 unk5C;
    s16 unk5E;
    s8  unk60; // spEC
    u8  pad61[0x3];
    s32 unk64;
    s8  unk68;
    s8  unk69;
    s8  unk6A;
    s8  unk6B;
    s8  unk6C;
    s8  unk6D;
    s8  unk6E;
    s8  unk6F;
    s8  unk70; // spFC
    s8  unk71;
    s8  unk72;
    u8  unk73;
    s32 unk74; // sp100
    s8  unk78;
    u8  pad79;
    s16 unk7A;
    s16 unk7C;
} struct171;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  pad5[0x3];
} struct172; // 0x8 bytes?

typedef struct {
    s32 unk0;
    f32 unk4;
    u8  unk8;
    u8  pad9[0x3];
} struct173;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
} struct174; // size 0x10

typedef struct {
    Actor* unk0;
    u8  unk4;
    u8  pad5;
    s16 unk6;
    s16 unk8;
    s16 unkA;
} struct175;

typedef struct {
    struct39* unk0;
    f32 unk4;
    s32 unk8;
    f32 unkC;
} struct176;

typedef struct {
    s16 unk0;
    s16 pad2;
    s32 unk4;
    s32 unk8;
} struct177;

/* A level placement record (trigger zones, gates, markers, light points): D_800D3098 points at the table. */
typedef struct TriggerZone {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 radius; // or the x extent
    /* 0x08 */ s16 height; // or the y extent
    /* 0x0A */ s16 depth;  // or the z extent
    /* 0x0C */ f32 pitch;  // degrees
    /* 0x10 */ f32 yaw;    // degrees
    /* 0x14 */ u8  disabled;
    /* 0x15 */ u8  kind;   // (type << 2) | shape
    /* 0x16 */ u8  flags;  // bit 0: only triggers near the ground
    /* 0x17 */ u8  subtype;
    /* 0x18 */ u32 unk18;  // per-type parameter (a colour for light points)
    /* 0x1C */ u32 unk1C;
    /* 0x20 */ u32 unk20;
    /* 0x24 */ f32 sinPitch;
    /* 0x28 */ f32 cosPitch;
    /* 0x2C */ f32 sinYaw;
    /* 0x30 */ f32 cosYaw;
} TriggerZone; // size 0x34

typedef struct {
    s32 unk0;
    f32 unk4;
    s32 unk8;
    s32 unkC;
    f32 unk10;
    s32 unk14;
} struct180; // size 0x18

typedef struct {
    s8  unk0;
    u8  pad[0x3];
    struct180 unk4;
    f32 unk1C;
    s8  unk20;
} struct179;

typedef struct {
    u8 pad[0x170];
    s32 unk170;
} struct181;

typedef struct {
    Actor *unk0;
    u8 unk4;
    u8 pad5[0x1];
    s16 unk6;
} struct182;

typedef struct {
    s8  unk0;
    u8  pad1[0x3];
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s8  unk24;
    s8  unk25;
    s8  unk26;
} struct183; // size 0x27

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} struct184; // size 0x20, 32 bytes

typedef struct {
    Actor* unk0;
    u8  unk4;
    u8  pad5;
    s16 unk6;
    s8  unk8;
    s8  unk9;
    s8  unkA;
} struct185;

typedef struct {
    u8 pad0[0x8];
    s32 unk8; // pointer
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u16 unk16;
    u8  pad18[0xC];
    u16 unk24;
    u8  pad26[0xA];
    u16 unk30;
    u16 unk32;
    u8  pad34[0x28];
    s32 unk5C;
    u8  pad60[0x14];
    s32 unk74; // pointer
    u8  pad78[0xC];
    s32 unk84;
    s32 unk88;
    u8  pad8C[0x8];
    s16 unk94;
    s16 unk96;
    u8  pad98[0x2];
    u8  unk9A;
    u8  pad9B[0x5];
} struct186; // size 0xA0, 160 bytes

/* A multiplayer spawn point (0x18 bytes) in the D_800D212C table, D_800D2128 entries long. */
typedef struct SpawnPoint {
    /* 0x00 */ s16 pos[3];
    /* 0x06 */ u8  unk6;   /* angle, 256 units per turn */
    /* 0x07 */ u8  facing; /* angle, 256 units per turn */
    /* 0x08 */ f32 unk8[3];
    /* 0x14 */ u8  unk14;
    /* 0x15 */ s8  unk15;
    /* 0x16 */ s8  link;   /* index of the spawn point to use instead; 0 = none, -1 = invalid */
    /* 0x17 */ u8  pad17;
} SpawnPoint; // size 0x18

typedef struct {
    s32 unk0;
    s32 unk4;
    struct158* unk8;
} struct188;

typedef struct struct189 struct189;

struct struct189 {
    s32 unk0;
    s32 unk4;
    struct189* unk8;
};

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    u8  padC[0x8];
    f32 unk14;
    u8  pad18[0x8];
    f32 unk20;
    u8  pad24[0xC];
} struct190;

typedef struct {
    f32 unk0;
    u8  pad4[0x3C];
    f32 unk40;
    u8  pad44[0x3C];
    f32 unk80;
    u8  pad84[0x2C];
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    u8  padB0[0x4];
    f32 unkB4;
    u8  padB8[0x3C];
    f32 unkF4;
    u8  padF8[0x3C];
    f32 unk134;
    u8  unk138[0x3C];
    f32 unk174;
    f32 unk178;
    f32 unk17C;
    f32 unk180;
    f32 unk184;
    f32 unk188;
    f32 unk18C;
    f32 unk190;
    f32 unk194;
} struct191; // size 0x194

typedef struct {
    f32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
} struct192; // size 0x14

typedef struct {
    Actor *unk0;
    Actor *unk4;
    u8 unk8;
    u8 unk9;
} struct193;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s16 unk14;
} struct194;

typedef struct {
    u16 unk0;
    u8  pad2[0x1E];
    u16 unk20;
} struct195;

typedef struct {
    s32 unk0;
    Actor *unk4;
    s32 unk8;
    s32 unkC;
} struct198;

typedef struct {
    u8  pad0[0x4];
    s32 unk4;
    u8  pad8[0x1C];
    f32 unk24;
    f32 unk28;
    f32 unk2C;
} struct199; // size 0x30

typedef struct {
    f32 unk0;
    f32 unk4;
} struct200;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
} struct201;

typedef struct {
    u8  pad0[0x1C];
    s16 unk1C;
    u8  pad1E[0xA];
    s8  unk28;
} struct202;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
} struct203;

typedef struct {
    u8 pad0[0x28];
    struct203 *unk28;
    u8  pad2C[0x8];
    u8  unk34;
} struct204;

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u8  padC[0x4];
    s32 unk10;
    u8  pad14;
    u8  unk15;
} struct208;

typedef struct {
    f32 unk0;
    f32 unk4;
    u8  pad8[0x3];
    u8  unkB;
    u8  padC[0x4];
    f32 unk10;
    f32 unk14;
    u8  pad18[0x3];
    u8  unk1B;
    u8  pad1C[0x4];
    f32 unk20;
    f32 unk24;
    f32 unk28;
    u8  pad2C[0x3];
    u8  unk2F;
} struct209;

typedef struct {
    s32 unk0;
    u8  pad4[0xC];
    s32 unk10;
    s32 unk14;
    union {
        struct UBYTES4 b;
        struct WORD    i;
    } unk18;
    u8  unk1C[0x2];
    u8  unk1E;
    u8  unk1F;
    u8  pad20[0xC];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    s32 unk3C;
    s16 unk40;
    s16 unk42;
    s16 unk44;
    u8  pad46[0xA];
    s16 unk50;
    s16 unk52;
    s16 unk54;
    s16 pad56;
    s32 unk58;
    u8  pad5C[0x4];
    s16 unk60;
    s16 unk62;
    s16 unk64;
    u8  pad66[0xA];
    s16 unk70;
    s16 unk72;
    s16 unk74;
    u8  pad76[0xA];
    u8  unk80;
    u8  unk81;
    u8  pad82[0x2];
    s32 unk84;
    u32 pad88;
    s32 unk8C[4];
    s32 unk9C;
    s32 unkA0;
    s32 unkA4[4];
    s32 unkB4;
    u8  padB8[0x48];
    s32 unk100;
    u8  pad104[0xC];
    s32 unk110;
    u8  pad114[0x8];
    s32 unk11C;
} struct210;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} struct211; //size 0x18

typedef struct {
    s32 unk0;
    union {
        struct UBYTES4 b;
        struct WORD   i;
    } unk4;
    u8  unk8;
    u8  unk9;
} struct212;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    u8  pad8[0x8];
    s16 unk10;
    s16 unk12;
    s16 unk14;
    s16 unk16;
    u8  pad18[0x8];
    s16 unk20;
    s16 unk22;
    s16 unk24;
    s16 unk26;
    u8  pad28[0x8];
    s16 unk30;
    s16 unk32;
    s16 unk34;
    s16 unk36;
    u8  pad38[0x8];
} struct213;

typedef struct {
    u8 pad0[0x7];
    u8 unk7;
    u8 pad8[0x3];
    u8 unkB;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
} struct214;

typedef struct {
    u8 pad0[0x8];
    s32 unk8;
    union {
        struct SHORTS s;
        struct WORD   i;
    } unkC;
    s32 unk10;
    s32 unk14;
} struct215;


struct struct216 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
};

typedef struct {
  f32 unk0;
  s32 unk4;
  u8  pad8[0x7];
  u8  unkF;
} struct217;

typedef struct {
    u16 unk0;
    u8  pad2[0x2];
    s32 unk4;
    s32 unk8;
    u8  padC[0x8];
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 pad17;
    u8 unk18;
    u8 unk19;
    u8 unk1A;
} struct218;

typedef struct {
    s32 unk0;
    s32 unk4;
    u8  unk8;
    u8  pad9[9];
    u8  unk12;
} struct219;

typedef struct {
    s8  unk0;
    s8  unk1;
    s16 unk2;
    u8  unk4;
} struct220;

typedef struct {
    s32 unk0; // struct pointer
    f32 unk4;
    f32 unk8;
} struct221; // size 0xC

typedef struct {
    u8 unk0[8];
} struct222; //size 0x08

typedef struct {
    s32 unk0;
    union {
        u8  ub;
        s32 w;
    } unk4;
    u8  unk8;
    u8  unk9;
} struct223;

typedef struct struct224 struct224;

struct struct224 {
    u8  pad0[0x10];
    s32 unk10;
    u8  unk14;
    u8  pad15[0xB];
    s16 unk20;
    u8  pad22[0x2];
    s32 unk24;
    s32 unk28;
    u8  unk2C[0x4];
    struct224 *unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    struct224 *unk44;
};

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    u8  pad12[0x12];
    u8  unk24;
} struct227;

typedef struct {
    u8  pad0[0x5];
    u8  unk5;
    u8  unk6;
    u8  unk7;
    u8  pad8;
    u8  unk9;
    u8  padA[0x4];
    s16 unkE;
    s16 unk10;
    s16 unk12;
    s16 unk14;
    u8  unk16[0x19];
    u8  unk2F;
} struct226;

typedef struct {
    Actor *unk0; // Actor pointer?
    u8  unk4;
    u8  pad5[0x9];
    u8  unkE; // 6 bytes?
    s8  unkF;
    s16 unk10;
    u8  unk12;
    u8  pad13;
    struct226 *unk14;
    struct227 *unk18; // struct227 pointer?
    f32 unk1C;
    s32 unk20; // Vec3f pointer?
    s8  unk24;
    u8  unk25;
    u8  pad26[0x2];
    s16 unk28;
    u8  unk2A;
    u8  pad2B[0x2];
    u8  unk2D;
    u8  pad2E[0x2];
    s32 unk30;
} struct225;

// only used by func_15163B98 ?
typedef struct {
    u8 pad0[0x28];
    WorldObject *unk28;
    u8  pad2C[0x14];
    f32 unk40;
} struct230;

typedef struct {
    s8  unk0;
    s8  unk1;
    s16 unk2;
    s8  unk4;
    u8  pad5;
} Header; // size 0x6

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    s16 unk10;
    u8  unk12;
    u8  pad13;
} struct232; // size 0x14

/* Per-track state of a running cutscene; D_800C3958[scene] points at one per track. */
typedef struct SeqTrackState {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 unk10;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 unk18; // light intensity (func_15161A68)
    /* 0x1C */ f32 unk1C;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24; // light colour r, g, b (func_15161A68)
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 unk2C;
    /* 0x30 */ f32 unk30;
    /* 0x34 */ f32 unk34;
    /* 0x38 */ f32 yOffset;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} SeqTrackState; // size 0x44

typedef struct {
    s8  unk0;
    u8  pad1[0x3];
    s32 unk4;
} struct234; // size 0x8;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
} struct235;

typedef struct {
    u8 pad0[0x28];
    struct235 unk28; // is this just 8 f32s ?
} struct236;

typedef struct {
    f32 unk0[5];
    struct226 *unk14;
    struct235 unk18; // is this just 8 f32s ?
} struct237;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    f32 unk20;
    s8  unk24;
    s8  unk25;
    s8  unk26;
    s8  unk27;
    s8  unk28;
    s8  pad29[0x7];
} struct238;

typedef struct {
    u8 pad0[0x12];
    u8 unk12;
    u8 pad13[0x35];
    u8 unk48;
} struct239;

typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    f32 unk20;
    f32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
} struct241; // size 0x34

typedef struct {
    u8  pad0[0x14];
    struct226 *unk14;
    s32 *unk18; // struct ptr?
    f32 unk1C;
    s32 unk20;
    s32 *unk24; // Vec3f pointer?
} struct242;

typedef struct {
    f32 unk0;
    u8  unk4;
    u8  pad5[0x3];
} struct243;

typedef struct {
    s32 unk0;
    u8  unk4;
    u8  unk5;
    u8  pad6[0x2];
} struct244;

typedef struct {
    s32 unk0; // ptr
    s32 unk4; // ptr
    s32 unk8; // ptr
    s8  unkC;
    u8  unkD;
    u8  padE[0x2];
} struct245; // size 0x10

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} Header2; // size 0xC

typedef struct {
    u8  pad0[0x8];
    u8 *unk8; // ptr?
    u8  unkC[0x4];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u8  unk2C[0x4];
    s32 unk30;
} struct246;

typedef struct {
    u8 pad0[0x760];
} struct247; // maybe naudio?

typedef struct {
    u16 unk0; // is this a pointer?
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
} struct248; // size 0xC;

typedef struct struct249 struct249;

struct struct249 {
    u16 unk0;
    u8  pad2[0x16];
    struct249 *next;
};

typedef struct {
    u16 length;
    u8  pad2[0x2];
    struct249 *unk4;
    s32 unk8;         // tbd
} struct250; // size 0xC;

typedef struct {
    u16 unk0;
    u8  pad2[0x16];
    union {
      s32 w;
      u16 h[2];
    } unk18;
    s32 unk1C;
} struct251;

typedef struct {
    u8 pad0[0x15];
    u8 unk15;
    u8 pad16[0x2];
} struct252; // size 0x18

typedef struct {
    u8  pad0[0x14];
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u8  pad20[0x8];
} struct253; // size 0x28

typedef struct {
    u8 pad0[0x1A];
    s16 unk1A;
} struct254;

struct struct255 {
    u8 pad0[0x240];
    struct254 *unk240;
};

struct struct256 {
    u8 pad0[0x80];
    struct256 *unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    f32 unkA0;
    f32 unkA4;
    f32 unkA8;
    f32 unkAC;
    f32 unkB0;
    u8  unkB4;
    u8  padB5[0x3];
}; // size 0xB8

typedef struct {
    u8 pad0[0xD16];
    s16 unkD16;
} struct257;

/*
 * Heap block header. Every block (free or allocated) starts with one; the
 * payload handed out by func_10003C6C starts right after it. The top byte of
 * taggedSize is the block's tag (0 = free), the low 24 bits its payload size.
 * A free block also links into the free list through its first payload words.
 */
typedef struct AllocatorBlock {
    /* 0x0 */ struct AllocatorBlock *next;
    /* 0x4 */ struct AllocatorBlock *prev;
    /* 0x8 */ u32 taggedSize;
} AllocatorBlock;

typedef struct AllocatorFreeBlock {
    /* 0x00 */ AllocatorBlock header;
    /* 0x0C */ struct AllocatorFreeBlock *nextFree;
    /* 0x10 */ struct AllocatorFreeBlock *prevFree;
} AllocatorFreeBlock;

/* A ground-probe candidate hit (func_150A3A70 / func_150A43E0 fill the D_800D3300 array with these). */
typedef struct GroundCandidate {
    /* 0x0 */ s32 height;  // 24.8 fixed point
    /* 0x4 */ s32 *tri;    // collision triangle (a D_800DBE3C entry): three vertex offsets from vtxBase
    /* 0x8 */ s32 vtxBase; // 0 for the static world mesh
    /* 0xC */ s32 unkC;    // group or object index (D_800DBEF4), depending on the probe type
} GroundCandidate;

/* One on-screen view (a player's region of the framebuffer). D_800BE628 points at an array of these,
 * one per player view (allocated in game_476D0.c). */
typedef struct RenderView {
    /* 0x000 */ u8  unk0;
    /* 0x001 */ u8  onScreen;
    /* 0x002 */ u8  pad2[2];
    /* 0x004 */ f32 width;
    /* 0x008 */ f32 height;
    /* 0x00C */ f32 halfWidth;  // also the screen centre x
    /* 0x010 */ f32 halfHeight; // also the screen centre y
    /* 0x014 */ f32 scaleX;     // sprite x scale (unscaled sprites)
    /* 0x018 */ f32 scaleY;
    /* 0x01C */ f32 unk1C;      // sprite x scale (perspective sprites)
    /* 0x020 */ f32 unk20;
    /* 0x024 */ f32 top;
    /* 0x028 */ f32 bottom;
    /* 0x02C */ f32 left;
    /* 0x030 */ f32 right;
    /* 0x034 */ f32 centerX;
    /* 0x038 */ f32 centerY;
    /* 0x03C */ u8  pad3C[4];
    /* 0x040 */ Vp  viewports[2]; // indexed by the framebuffer index D_800BE9C0
    /* 0x060 */ f32 aspect;
    /* 0x064 */ f32 unk64;
    /* 0x068 */ f32 unk68;
    /* 0x06C */ f32 fovX; // degrees
    /* 0x070 */ f32 fovY; // degrees
    /* 0x074 */ f32 frustumFovX;
    /* 0x078 */ f32 frustumFovY;
    /* 0x07C */ f32 pixelTanX; // tan(fovX / 2) / halfWidth
    /* 0x080 */ f32 pixelTanY; // tan(fovY / 2) / halfHeight
    /* 0x084 */ f32 perspScale; // guPerspectiveF scale (also scales camera shake)
    /* 0x088 */ f32 frustumPlanes[4][3]; // side-plane normals: [0..1] from frustumFovX, [2..3] from frustumFovY
    /* 0x0B8 */ u16 perspNorm;
    /* 0x0BA */ u8  padBA[2];
    /* 0x0BC */ MtxF projection;
    /* 0x0FC */ u8  padFC[4];
    /* 0x100 */ Mtx projectionMtx[2]; // indexed by the framebuffer index D_800BE9C0
} RenderView; // size 0x180

typedef struct {
    u8  pad0[0xD];
    u8  unkD;
    s16 unkE;
    s8  unk10;
    s8  unk11;
    s8  unk12;
    u8  unk13;
    s32 unk14; // union here?
    u8  pad18[0xC];
} struct260; // size at least 0x24

/* A thread that receives the scheduler's retrace message; func_100051C8 links it into D_8003B234. */
typedef struct SchedulerClient {
    /* 0x0 */ struct SchedulerClient *next;
    /* 0x4 */ OSMesgQueue *msgQ;
    /* 0x8 */ u32 flags; /* bit 0: don't send retrace messages */
} SchedulerClient;

#endif
