#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <ultra64.h>

#include "structs.h"

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

/* Positional sound records (init_EB00.c). A record callback runs each update with the values about to be
 * applied and may change them; returning nonzero stops the record. */
typedef struct SoundRecord SoundRecord;
typedef s32 (*SoundRecordCallback)(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix,
                                   u16 *sound);

/* A positional sound being tracked by func_10011624 (dynamicSounds, up to 32).
 * original: struct AlSSound (rom_debug_struct.c); field names are Rare's labels with spaces as underscores,
 * except mix and the members of the two callback parameter unions. */
struct SoundRecord {
    /* 0x00 */ u16 sample;
    /* 0x02 */ s16 x;
    /* 0x04 */ s16 y;
    /* 0x06 */ s16 z;
    /* 0x08 */ u16 distance_max;
    /* 0x0A */ s16 distance_loudest;
    /* 0x0C */ s32 max_volume;
    /* 0x10 */ u32 flags; /* see init_EB00.c */
    /* 0x14 */ SoundRecordCallback callback_ptr;
    /* 0x18 */ union {
        void *ptr;
        Actor *owner;   /* the object the sound follows (func_1000EE70, func_1000EFB4) */
        s32 timer;      /* countdown (func_1000EB00, func_1000EBC4) */
        s32 delay;      /* low half: countdown; high half: the sound held back meanwhile */
        u16 half[2];
    } callback_param1;
    /* 0x1C */ union {
        s32 val;        /* func_1000EE70: the owner's unique_id in the low byte */
        Actor *obj;     /* the object the delayed sound plays on (func_1000ECCC, game_130CB0.c) */
        u16 *ids;       /* func_1000EFB4: 0-terminated list of the owner's animation ids (Actor.unk84) */
    } callback_param2;
    /* 0x20 */ s16 pitch_shift_cent;
    /* 0x22 */ u8 pan;     /* last pan sent to the voice; bit 7 is the fx-mix bit */
    /* 0x23 */ u8 mix;
    /* 0x24 */ u16 sound_id; /* voice handle */
    /* 0x26 */ s16 current_volume;
    /* 0x28 */ s16 distance_current;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 pitch;
}; /* size 0x30 */

/* Sound bank loader (init_8F90.c): a bank fetch proc takes an owner slot (a ROM reference, or a record
 * once loading has started) and returns the bank's data, loading it on demand. */
typedef struct BankRecord BankRecord;
typedef void *(*BankFetchProc)(u32 *slot, s32 mode);

/* A playing music sequence (init_B1B0.c). */
typedef struct MusicTrack MusicTrack;

/* matching */
void func_10001444(void);
void func_100014A0(void);
s32  func_10002070(s32 arg0, s32 arg1, s32 arg2);
void func_1000349C(s32 arg0);
void func_10003930(void);
void func_10004250(void);
void func_10004308(void);
void func_1000440C(void);
void func_100084D8(u8 arg0);
s32  func_1000853C(u8 arg0);
void func_10008570(u8 player, OSMesgQueue *queue);
void func_100085A4(s32 arg0, s32 arg1, s32 arg2);
void func_100085B8(u8 arg0, s32 arg1, u8 arg2);
void func_100085F8(u8 arg0, s32 arg1);
void func_1000862C(u8 arg0, s32 arg1);
void func_10008660(u8 arg0, u8 arg1, u8 arg2, s32 arg3);
void func_100086FC(u8 arg0, u8 arg1, u8 arg2);
void func_10008744(u8 arg0, u8 arg1, u8 arg2);
void func_10008790(u8 arg0, s32 arg1, u8 arg2, s32 arg3);
void func_10008824(u8 arg0, u8 arg1, u8 arg2);
void func_1000886C(u8 arg0, s32 arg1, u8 arg2);
void func_100088F0(u8 arg0, s32 arg1, s32 arg2);
void func_10008988(u8 idx, s32 mask, s32 enable);
u8   func_10008A4C(u8 idx, u8 chan);
void func_10008A94(u8 arg0, s32 arg1, s32 arg2);
void func_10008B2C(u8 arg0);
void func_10008B60(u8 arg0, u8 arg1, u8 arg2, u8 arg3, s32 arg4);
void func_10008BC0(u8 arg0, f32 arg1, f32 arg2);
void func_10008EE0(u8 arg0, s32 arg1);
void func_10008F24(u8 arg0);
void func_10008F58(u8 arg0);
void func_100093CC(void);
ALDMAproc func_10009980(void *state);
void func_10009B2C(BankRecord *record);
void func_10009B4C(BankRecord *record);
void func_10009B90(BankRecord *record);
s32  func_1000BA18(u32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4);
MusicTrack *func_1000B1B0(s32 id);
s32  func_1000B830(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_1000B8B8(s32 arg0, u8 arg1, f32 arg2, f32 arg3, f32 arg4);
s32  func_1000BBE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_1000BC28(s32 arg0, u8 arg1, s32 arg2, s32 arg3);
void func_1000CBA8(s32 arg0);
s32  func_1000CD40(s32 arg0, s32 arg1, s32 arg2);
void func_1000E054(s32 arg0, s32 arg1);
s32  func_1000E0F8(s32 arg0);
void func_1000E40C(s32 arg0, s32 arg1);
s32  func_1000E654(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  func_1000E704(s32 arg0, s32 arg1, s32 arg2);
void func_1000E75C(s32 arg0);
s32  func_1000E770(s32 *arg0, s32 *arg1);
u16  func_1000EA94(s32 arg0);
s32 func_1000EB00(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EBC4(SoundRecord *record, s32 *active, u32 *volume, s32 *pan);
s32 func_1000EF40(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void func_1000F248(s32 arg0);
s32  func_1000F3D0(u16 arg0);
s32  func_1000F44C(u16 arg0);
void func_1000F91C(u16 arg0, u16 arg1, s16 arg2, u8 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7, s16 arg8, s16 arg9);
void func_1000F9D4(u16 arg0, s16 arg1, s16 arg2, s16 arg3);
void func_10010720(u16 arg0, Actor *arg1, s32 arg2, s16 arg3, u16 arg4, s32 arg5) ;
s32  func_100107F8(Actor *arg0);
s32  func_10010894(Actor *arg0);
void func_100109D0(Actor *arg0);
void func_10010A3C(Actor *arg0);
void func_10010AA8(Actor *arg0);
u16 func_10010F30(s32 sound, u16 volume, u8 pan, s16 cents, u8 mix);
void func_100111C8(u16 arg0);
void func_1001123C(u16 arg0);
s32  func_100112BC(s32 arg0, s32 arg1);
s32  func_1001147C(u16 arg0);
void func_10011E88(s32 arg0);
void func_10011E94(s32 arg0);
void func_10011FA0(s32 *arg0);
void func_10011FB0(s32 arg0);
void func_10011FDC(s32 arg0);
void func_10011FEC(void);
void func_10012560(s32 arg0);
void func_10012588(s32 arg0);
void func_100125CC(s32 arg0);
void func_1001263C(s32 arg0, s32 arg1, s32 arg2);
s32  func_100126E8(u8 *dst, s32 dstSize);
s32  func_10012718(u16 arg0, Actor *arg1, s32 arg2, s16 arg3, u16 arg4);
s32  func_100127D0(void);
void func_10012A28(ALBank *bank, s32 offset, s32 table, s32 arg3);
void func_10012B84(ALInstrument *inst, s32 arg1);
void func_10012BD0(ALInstrument *inst, s32 arg1);
void func_10012C5C(ALSound *sound, s32 offset, s32 table);
void func_10012CFC(WaveTable *wave, s32 offset, s32 table);
f32  func_10012D80(u8 arg0);
s32  func_10012E04(s32 *arg0, f32 *arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5);
s32  func_10012F94(struct OscState *osc, f32 *value);
void func_100131D8(s32 *arg0);
void func_100131FC(ALSeqpConfig *config, s32 arg1);
void func_10016E90(SndState *arg0);
void func_10016F00(SndState *state);
SndState *func_10017100(s32 arg0, s16 arg1);
void func_10017298(SndState *arg0);
s32  func_100173C4(SndState **arg0);
void func_10017594(SndState *arg0);
void func_100176C4(void);
void func_100176EC(void);
void func_10017714(s32 arg0, s16 arg1, s32 arg2);
void func_10017870(u8 arg0);
void func_10017944(s32 arg0, u32 arg1);


void n_alInit(void *globals, void *config);
void n_alClose(s32 arg0);
s32  _n_timeToSamplesNoRound(s32 micros);
s32  _n_timeToSamples(s32 micros);
f32  alCents2Ratio(s32 arg0);
void func_1001CBF0(f32 arg0, f32 arg1, f32 arg2, f32 arg3[3], f32 arg4[3]);
f32  func_1001CEA4(s32 arg0);
s32  func_1001D9B0(s16 arg0);
s32  func_1001DA28(s16 arg0);
void func_1001DAA0(s32 arg0, s16 arg1, s32 arg2);
void func_1001DAE4(ALVoiceConfig *arg0, s16 arg1, s32 *arg2);
s32  func_1001E4A0(s16 arg0, s32 arg1, s32 arg2);
f32  func_1001FA78(MtxF arg0, s32 arg1);
/* chunk0 */
void func_15000000(void);
void func_15000090(void);
void func_15000AC0(void);
void func_15001B08(void);
u16* func_15001B10(void);
void func_15001BC8(s32 arg0, s16 *arg1, s16 *arg2);
void func_15001970(void);
void func_150026C4(s32 arg0);
void func_150026E8(s32 arg0);
void func_15002724(s32 arg0);
void func_15002F40(s32 arg0);
s32  func_15002FA0(s32 arg0);
void func_15002FB4(s32 arg0);
void func_1500310C(void);
s32  func_150034B4(void);
void func_150038A0(void);
s32  func_1500390C(s32 arg0);
void func_150039B0(s32 arg0);
void func_150039BC(s32 arg0);
void func_150045BC(void);
void func_15004E00(void);
void func_15004E80(void);
void func_15004F00(void);
void func_15004F10(void);
void func_15004F30(void);
void func_15005270(void);
void func_15005A60(Camera *arg0, s32 arg1);
void func_15005AB0(Camera *arg0);
void func_15005AF0(s32 arg0);
void func_15005B00(s32 arg0);
void func_15005B10(Camera *arg0);
void func_15005B50(s32 arg0);
void func_15005B60(s32 arg0);
void func_15005B70(s32 arg0);
void func_15005BD0(Camera *arg0);
void func_15005C30(Camera *arg0);
void func_15005C80(Camera *arg0);
void func_15005CF0(s32 arg0);
void func_15005D00(Camera *arg0);
void func_15005D60(Camera *arg0);
void func_15005DB0(Camera *arg0);
void func_15005E30(Camera *arg0);
void func_15005E70(Camera *arg0);
void func_15005EA0(Camera *arg0);
void func_15005EE0(Camera *arg0);
void func_15005F20(Camera *arg0);
void func_15005F60(Camera *arg0);
void func_15005FA0(s32 arg0);
void func_15005FB0(Camera *arg0);
void func_15006010(Camera *arg0);
void func_15006140(Camera *arg0);
void func_15006170(Camera *arg0);
void func_150061B0(void);
void func_150064E0(void);
void func_15007644(void);
void func_1500764C(void);
void func_15007668(void);
void func_15007684(void);
void func_150076A0(); /* defined with no parameters, but game_20BDA0.c passes one */
void func_150076BC(s32 arg0);
void func_15007718(s32 arg0);
void func_15007750(s32 arg0);
void func_15007A20(void);
void func_15007A70(s16 arg0, s16 arg1, s16 arg2);
void func_150081E4(void);
void func_15008230(void);
void func_15008248(s32 arg0);
void func_150082CC(void);
void func_15008840(void);
void func_15008A10(void);
void func_15008B90(void);
void func_15008BB0(void);
void func_15008BE0(void);
void func_15008DD0(void);
void func_15008E00(void);
void func_15009150(s32 arg0);
void func_150095D8(s32 arg0, s32 arg1);
void func_15009600(s32 arg0, s32 arg1);
void func_1500969C(s32 arg0, s32 arg1);
void func_15009740(s32 arg0, s32 arg1);
void func_15009768(s32 arg0, s32 arg1);
void func_150097A4(s32 arg0, s32 arg1);
void func_150097CC(s32 arg0, s32 arg1);
void func_15009818(s32 arg0, s32 arg1);
void func_15009844(s32 arg0, s32 arg1);
void func_15009870(s32 arg0, s32 arg1);
void func_15009894(s32 arg0, s32 arg1);
void func_150098D0(s32 arg0, s32 arg1);
void func_150098F8(s32 arg0, s32 arg1);
void func_15009944(s32 arg0, s32 arg1);
void func_15009AA0(s32 arg0, s32 arg1);
void func_15009AEC(s32 arg0, s32 arg1);
void func_15009B38(s32 arg0, s32 arg1);
void func_15009B84(s32 arg0, s32 arg1);
void func_15009D28(s32 arg0, s32 arg1);
void func_15009D6C(s32 arg0, s32 arg1);
void func_15009DB0(s32 arg0, s32 arg1);
void func_15009DFC(s32 arg0, s32 arg1);
void func_15009E48(s32 arg0, s32 arg1);
void func_15009E84(s32 arg0, s32 arg1);
void func_15009EC8(s32 arg0, s32 arg1);
void func_15009EF4(s32 arg0, s32 arg1);
void func_15009F30(s32 arg0, s32 arg1);
void func_1500A028(s32 arg0, s32 arg1);
void func_1500A06C(s32 arg0, s32 arg1);
void func_1500A0B0(s32 arg0, s32 arg1);
void func_1500A0FC(s32 arg0, s32 arg1);
void func_1500A148(s32 arg0, s32 arg1);
void func_1500A194(s32 arg0, s32 arg1);
void func_1500A1E0(s32 arg0, s32 arg1);
void func_1500A21C(s32 arg0, s32 arg1);
void func_1500A260(s32 arg0, s32 arg1);
void func_1500A33C(s32 arg0, s32 arg1);
void func_1500A380(s32 arg0, s32 arg1);
void func_1500A3C4(s32 arg0, s32 arg1);
void func_1500A410(s32 arg0, s32 arg1);
void func_1500A454(s32 arg0, s32 arg1);
void func_1500A490(s32 arg0, s32 arg1);
void func_1500A4D4(s32 arg0, s32 arg1);
void func_1500A518(s32 arg0, s32 arg1);
void func_1500A55C(s32 arg0, s32 arg1);
void func_1500A5A8(s32 arg0, s32 arg1);
void func_1500A5F4(s32 arg0, s32 arg1);
void func_1500A640(s32 arg0, s32 arg1);
void func_1500A68C(s32 arg0, s32 arg1);
void func_1500A79C(s32 arg0, s32 arg1);
void func_1500A8C8(s32 arg0, s32 arg1);
void func_1500A904(s32 arg0, s32 arg1);
void func_1500A94C(s32 arg0, s32 arg1);
void func_1500A990(s32 arg0);
void func_1500AB5C(s32 arg0);
void func_1500ABA0(s32 arg0);
void func_1500BE40(s32 arg0);
void func_1500BE68(void);

void func_1500EB20(void);
void func_1500EB30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_1500EBC4(void);
void func_1500ED80(void);
void func_1500EE18(Actor *arg0, s32 arg1, s32 arg2);
void func_1500EE94(TriggerZone *zone);
void func_1500EF20(void);

void func_15010240(void);
void func_150102C0(void);
void func_150102D0(void);
void func_150103E0(void);
void func_15010538(Actor *arg0);
void func_15010680(void);
void func_150106A0(void);
void func_150106B0(void);
s32  func_150106D0(TriggerZone *zone);
void func_15010780(void);
void func_15010FB0(void);
void func_150110F0(void);
void func_15011170(void);
void func_15011330(void);
void func_15011360(void);
void func_15011A78(Actor *arg0, u8 arg1, s32 arg2);
void func_15011B00(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_15011B94(void);
void func_15011C40(void);
void func_15011C70(void);
void func_15011CA0(void);
void func_15011CC0(void);
void func_15011F20(s32 arg0);
void func_15011FA0(s32 arg0);
void func_15012020(s32 arg0, s32 arg1);
u8   func_15012720(void);
void func_15012770(void);
void func_15012780(void);
s32  func_150150A4(void);
s32  func_15015300(TriggerZone *obj);
void func_15016370(void);
void func_15016500(void);
void func_15017300(s16 arg0, s16 arg1);
void func_150175E0(void);
void func_15017790(void);
void func_150177F8(void);
void func_1501C860(void);
void func_1501C870(void);

void func_15042D50(void);
void func_15042D78(u8 arg0);
void func_150432BC(f32 arg0);
void func_150432CC(s32 arg0, s32 arg1);
void func_150432FC(s16 arg0, s16 arg1);
void func_1504332C(u8 arg0, u8 arg1, u8 arg2, u8 arg3);
void func_15043A00(MsgRing *ring, u8 *buffer, s32 capacity);
void func_15043D90(Mtx *m, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);
void func_15043E68(Mtx *m, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void func_15043EC8(MtxF mtx, f32 x, f32 y, f32 z, f32 arg4, f32 arg5, f32 arg6);
void math_mtxGetTranslation(Vec3f *out, Mtx *mtx);
void func_150442C0(MtxF arg0, f32 x, f32 y, f32 z);
void func_15047F00(MtxF mtx, u16 *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void func_15048134(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
f32  func_150484A0(f32 arg0, f32 arg1);
u16  func_15048664(s16 arg0);
s16  func_150486B8(s16 arg0);
f32  func_15048360(f32 arg0);
f32  func_15048408(f32 arg0);
f32  func_15048720(f32 arg0, f32 arg1, f32 arg2);
void func_15048758(f32 *arg0);
f32  func_150487E0(f32 arg0);
f32  func_15048864(f32 arg0);
s32  func_15043CA4(MsgRing *ring, u8 *dst, s32 dstSize);
f32  func_15048A40(u8 arg0);
void func_15142314(void *bones, s32 bone, Vec3f *out);
f32  func_15048A70(f32 arg0, f32 arg1);
s32  func_15048AD0(s32 arg0, s32 arg1);
void func_15048B10(s32 arg0, s32 arg1);
void func_15048F20(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
void func_15048F58(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
void func_15048F90(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
void func_15049148(Vec3f *arg0, f32 arg1, Vec3f *arg2);
void func_1504917C(Vec3f *arg0, Vec3f *arg1);
void func_150491EC(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
void func_150492CC(f32 arg0, f32 arg1, f32 arg2);
s32 func_15049440(MtxF arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9);
f32  func_1504A5E0(f32 arg0);
s32  func_1504AEF4(s32 arg0, s32 arg1);
s32  func_1504C078(void);
s32  func_1504C0B8(void);
void func_1504C854(Actor *arg0);

void func_15052408(Actor *arg0);
void func_15052458(s32 arg0);
void func_15052464(Actor *arg0);
void func_15052490(Actor *arg0, u16 arg1, f32 arg2, f32 arg3);
void func_15052EF0(Actor *arg0);
Actor *func_15052F58(s32 arg0, s32 arg1);
void func_15053694(Actor *arg0);
void func_150536D0(Actor *arg0);
void func_150536E8(Actor *arg0);
void func_1505371C(Actor *arg0);
void func_15055D48(Actor *arg0);
void func_15056150(Actor *arg0);
void func_15056258(Actor *arg0);
s32  func_1505693C(Actor *arg0, s32 arg1);
void func_15058EA4(Actor *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);
void func_15059140(Actor *arg0);
void func_150593C4(Actor *arg0, u16 arg1, f32 arg2, f32 arg3);
void func_15059444(Actor *arg0);
void func_1505959C(Actor *arg0, s32 arg1);
f32  func_1505A5CC(OSContPad *arg0);
u16  func_1505A630(f32 arg0, f32 arg1, s32 arg2);
f32  func_1505A6F8(Actor *arg0, Actor *arg1);
s16  func_1505C140(Actor *arg0, Actor *arg1);
void func_1505D2B8(Actor *arg0, u8 arg1);
f32  func_1505D34C(f32 arg0, f32 arg1, f32 arg2, f32 arg3, f32 *arg4);

void func_15060B70(s32 arg0, void *arg1);
void func_150615DC(Actor *arg0);
void func_150627D4(Actor *arg0);
void func_15062AC4(Actor *arg0);
void func_15062B1C(Actor *arg0, f32 arg1);
void func_15062B50(Actor *arg0, f32 arg1);
void func_15062B84(Actor *arg0);
void func_15062BDC(Actor *arg0, f32 arg1, f32 arg2);

void func_15075498(void);
void func_15075A50(void);
void func_15075B60(void) ;

void func_1508F9C4(void);

void func_15093818(s32 arg0);
void func_15093878(void);
void func_1509B4A0(s32 arg0, s32 arg1);
void func_1509BFB0(s32 num, s32, s32, ...);
void func_150AECCC(struct42 *arg0);
void func_150AED4C(struct BillboardParticle *p);
s32  func_150C3D48(s32 arg0);
void func_150CEF10(Camera *arg0);

void func_150DE310(s32 arg0);
void func_150DE32C(s32 arg0, s32 arg1, s32 arg2);
void func_150DEC90(s32 arg0);
void func_150EB430(Vec3f *start, Vec3f *delta, Vec3f *hit);
void func_150EC4B0(Camera *arg0);
void func_150F51A0(s32 arg0, s32 arg1);
s32  func_150F51BC(void);
void func_150F51E8(s32 arg0);
void func_150FCA00(Camera *arg0);

void func_15100330(s32 arg0);
void func_15103800(void);
void func_15103828(void);
void func_1510550C(EffectHeader *arg0, s32 arg1, u8 arg2);
typedef struct BoltArcObj BoltArcObj; /* game_131F30.c */
typedef struct SparkObj SparkObj;     /* game_131F30.c */
void func_15105548(BoltArcObj *obj, s32 *arg1, u8 msg);
void func_15105848(SparkObj *obj, s32 arg1, u8 msg);
void func_1511FC20(s32 arg0);
Gfx *func_1511FC2C(Gfx *gfx, f32 x, f32 y, f32 z, f32 rot, s32 view);
Gfx *func_1511FC44(Gfx *gfx, f32 x, f32 y, f32 z, s32 view);
void func_15122AE0(void);
void func_15124B18(Camera *arg0);
void func_15123508(Camera *arg0);
void func_15124770(Camera *arg0, s32 arg1);
void func_15124AB4(Camera *arg0);
void func_1512523C(Camera *arg0);
void func_15125330(Camera *arg0);
void func_15125394(Camera *arg0);
s32  func_151253CC(Camera *arg0);
void func_15125594(Camera *arg0);
void func_15125608(Camera *arg0);
void func_15125690(Camera *arg0, s32 arg1);
void func_15126138(Camera *arg0);
void func_15127FEC(Camera *arg0, s32 arg1, s32 arg2);
void func_15128680(Camera *arg0);
void func_1512868C(Camera *arg0);
void func_15128774(Camera *arg0, Actor *arg1);
void func_1512D380(s32 arg0);
struct DefParticleHead;
s32  func_1513416C(struct DefParticleHead *particle);

struct QuadSprite *func_1513C4EC(void *def, void *colors, u8 arg2, u8 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9, u8 argA, s32 argB, s32 argC, s32 argD, u8 argE, s32 argF);
struct QuadSprite *func_1513C5B0(void *arg0, void *arg1, u8 arg2, u8 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9, u8 argA, s32 argB, u8 argC, s32 argD);
struct QuadSprite *func_1513C650(void *def, u8 arg1, u8 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9, u8 argA, s32 argB, s32 argC, s32 argD, u8 argE, s32 argF);
struct QuadSprite *func_1513C73C(void *def, u8 arg1, u8 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9, u8 argA, s32 argB, u8 argC, s32 argD);
struct QuadSprite *func_1513C804(void *def, s32 arg1, u8 arg2, u8 arg3, s32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, u8 argA, u8 argB, s32 argC, u8 argD, s32 argE);
void func_1513C8D4(struct QuadSprite *sprite);
void func_1513C900(struct QuadSprite *sprite);
void func_1513C92C(struct QuadSprite *sprite);
void func_1513C9B0(struct QuadSprite *sprite);
void func_1513C9FC(struct QuadSprite *sprite);
void func_1513CA48(struct DefParticleHead *particle);
void func_1513CA6C(struct DefParticleHead *particle);
void func_1513CAA0(struct DefParticleHead *particle);
void func_1513CAD4(struct DefParticleHead *particle);
void func_1513CB58(struct DefParticleHead *particle);
void func_1513CBA4(struct DefParticleHead *particle);
void *func_1513D4B8(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 arg8, s32 arg9);
void *func_1513D524(void *def, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 motionSize, u8 arg7, s32 arg8);
void *func_1513D594(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width, f32 height, s32 arg9, s32 argA, s32 model, s32 argC, u8 argD, s32 motionSize, u8 argF, s32 arg10);
void *func_1513D668(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 flip, s16 angle, f32 width, f32 height, s32 arg9, s32 argA, u8 argB, s32 motionSize, u8 argD, s32 argE);
void func_1513E070(s32 arg0, s32 arg1, s32 arg2);
void func_1513E084(struct DefParticleHead *particle, union ActorMsg *msg, u8 eventId);
void func_1513E134(void);
void func_1513E2A4(void);
Vtx *func_1513EDB4(void *particle, s16 viewport);
Vtx *func_1513EDE4(void *particle, s16 viewport);
void func_1513F4B0(struct DefParticleHead *particle, s16 arg1);
void func_151403A8(s32 arg0, u8 arg1);
void func_151403DC(s32 arg0, u8 arg1);
void func_151411A4(struct DefParticleHead *particle);
void func_151411C4(struct DefParticleHead *particle);
s32  func_15141818(s32 arg0, s32 arg1);
void func_15141990(void *arg0);
void func_151419B0(void *arg0);
void func_15141DA4(void *arg0, s32 arg1, s32 arg2);
s32  func_151422C0(s32 id, const char *name, s32 min, s32 max, const char *file, s32 line);
s32  func_151422DC(s32 id, const char *name, s32 min, s32 max, s32 value, const char *file, s32 line);
s32  func_151422F8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32  func_15143E08(Actor *arg0);
s32  func_15144C2C(s16 arg0);
f32  func_15144C8C(f32 arg0, f32 arg1);
void func_151450B4(Vec3f *arg0, Vec3f *arg1, Vec3f *arg2);
s32  func_151454BC(u8 arg0, f32 arg1, Vec3f *arg2);
f32  func_15145A0C(f32 arg0, f32 arg1, f32 arg2);
void func_15145A50(Actor *arg0);
void func_15146508(Actor *arg0, Actor *arg1);
void func_151467A4(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 *arg7);
TimedEffect *func_15149130(s16 timer, s8 expireCallback, s8 tickCallback, s8 drawCallback, u8 flags, u8 callbackSet, s32 dataSize, u8 owner, s32 list);
TimedEffect *func_151491F4(s16 timer, s8 expireCallback, s8 tickCallback, u8 flags, u8 callbackSet, s32 dataSize, u8 owner, s32 list);
void func_15149318(TimedEffect *arg0);
void func_1514933C(TimedEffect *arg0);
void func_15149368(TimedEffect *arg0);
void func_15149394(TimedEffect *arg0);
void func_151493E4(TimedEffect *arg0);
void func_15149434(TimedEffect *arg0, s32 arg1, u8 arg2);
s32  func_15149490(s32 arg0, TimedEffect *arg1, s16 arg2);
void func_151494E0(s32 arg0, u8 arg1);
void func_15149514(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);
/* Actor attachment lists (Actor.attachments, game_179F30): `handle` is the attached effect or object. */
Attachment *func_1514EC1C(void *handle, Actor *owner, s16 type);
s32  func_1514ECE0(Attachment *list, s16 type, Attachment **out);
s32  func_1514ED3C(Attachment *list, void *handle, Attachment **out);
void func_1514EDF0(void *handle, Actor *owner);
void func_15165F70(void);
void *func_15167A68(s32 type, s32 list, s32 size, s32 arg3, s32 owner, s32 arg5);
void func_1516972C(EffectHeader *obj);
void func_1516979C(EffectHeader *obj);
void func_15169804(EffectHeader *obj);
void func_15169824(EffectHeader *obj);
void func_151346EC(EffectHeader *obj); /* func_15169804 wrapper (BloodDrip teardown) */
void func_1513470C(EffectHeader *obj); /* func_15169824 wrapper */
void func_1519EF70(Actor *arg0, s32 arg1, s32 arg2);

void func_151DB004(struct ParticleDesc *desc);
void func_151DB068(struct ParticleDesc *desc);
void func_151DB0CC(struct ParticleDesc *desc);
void func_151DB15C(struct ParticleDesc *desc);
void func_151DB1EC(struct ParticleDesc *desc);
void func_151DB27C(struct ParticleDesc *desc);
void func_151DB2A8(struct ParticleDesc *desc);
void func_151DB2CC(struct ParticleDesc *desc);
void func_151DB330(struct ParticleDesc *desc);
void func_151DB3D8(struct ParticleDesc *desc);
void func_151DB43C(struct ParticleDesc *desc);
void func_151DBBD4(Vec3f *arg0, s32 arg1, u8 *arg2, u8 arg3, s32 arg4);
void func_151F0080(MtxF arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_151F00E0(MtxF arg0, MtxF arg1, MtxF arg2);

void func_16000000(void);
s32  func_16000028(void);
void func_16000058(void);
s32  func_16000224(void);
void func_16000304(void);
void func_1600030C(void);
void func_16000314(void);
s32  func_16000384(void);
void func_16000424(OSThread *faulted);
void func_160012B0(s32 arg0, u8 *arg1);
void func_16001338(u8 arg0, u8 arg1, u8 arg2);
s32  func_160016F4(s32 arg0);
s32  func_16001984();
s32  func_160019A8(s32 direction, void *dramAddr);
void func_16001A64(void);
s32  func_16001A6C(f32 arg0);
void func_16001AB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u8*  func_16001AD0(u8 *arg0, u8 *arg1, u32 arg2);
s32  func_16001B8C(u8 *arg0, u8 *arg1, u32 arg2);


/* non-matching */

void func_10001420(void);
void func_100014C4(s32 arg0);
void func_10001550(void *arg0, u8 arg1);
//func_10001AA8
// s32  func_100020D0(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3);
//func_10002718
// s32  func_10002DB0(s32 arg0, s32 arg1);
//func_10002E50
void func_100030A0(OSPri priority, OSMesgQueue *mesgQueue, OSMesg *arg2, s32 arg3);
//func_100034E0
//func_10003658
void func_10003920(void);
// void func_10003BD0(void);
s32 func_10003C6C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
//func_10004074
void func_100043B4(s32 *arg0, u32 arg1);
//func_10005570
//func_100056A0
//func_100057E0
//func_10005948
//func_10005AB0
void func_10005B04(s32 arg0);
//func_10005BE0
//func_10005C2C
void func_1000709C(void);
//func_10007C74
//func_10007CC4
//func_10007D28
//func_10007DA0
//func_10007DAC
//func_10008120
//func_10008180
//func_10008B60
//func_10008BC0
//func_10008C04
void func_10008C6C(u8 arg0, u8 arg1);
//func_10008CE8
//func_10008F90
void func_10009400(void *arg);
//func_100095A0
s32 func_100097CC(s32 addr, s32 len, void *state);
//func_100099BC
void func_10009BE4(BankRecord *record);
void *func_10009CBC(u32 *slot, s32 mode);
BankFetchProc func_10009FFC(void);
//func_1000A03C
//func_1000A348
//func_1000A420
//func_1000A750
//func_1000B060
MusicTrack *func_1000B1FC(s32 id);
//func_1000B294
//func_1000B2F4
//func_1000B3D4
//func_1000B548
//func_1000B638
//func_1000B8B8
//func_1000BAFC
//func_1000BCBC
//func_1000BF60
//func_1000C350
s32 func_1000C530(s32, u8, f32, f32, f32);
//func_1000C7E8
//func_1000C934
//func_1000CA18
//func_1000CAE4
//func_1000CBF0
//func_1000CC54
//func_1000CDA0
//func_1000CEAC
//func_1000D2F8
void func_1000D758(f32 x, f32 y, f32 z);
//func_1000D96C
//func_1000DE1C
//func_1000DEC4
void func_1000DF68(s32 id, s32 volume, s32 frames);
//func_1000E134
//func_1000E17C
//func_1000E2F4
s32 func_1000E46C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_1000E588(s32 arg0, s32 arg1, s32 arg2);
//func_1000E654
//func_1000E7A0
//func_1000E8C4
//func_1000E8F0
//func_1000E934
//func_1000EC24
//func_1000ECCC
s32 func_1000EDA0(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 func_1000EE70(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
//func_1000EFB4
//func_1000F1A8
//func_1000F44C
//func_1000F4D8
//func_1000F568
//func_1000F6B8
void func_1000F85C(u16 handle, s16 param, s32 value);
u16 func_1000FA64(u16 sound, s16 x, s16 y, s16 z, s32 volume, u16 farDistance, s16 nearDistance, SoundRecordCallback callback, void *owner, s32 key, s32 flags, s32 cents);
//func_1000FC18
//func_1000FD38
//func_1000FDF4
//func_1000FE88
//func_1000FEF0
s32 func_1000FF90(SoundRecordCallback callback, void *owner, s32 key);
//func_1001001C
//func_100100E0
s32 func_10010154(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance);
s32 func_10010344(u16 sound, void *owner, u32 volume, s16 nearDistance, u16 farDistance);
//func_10010558
void func_10010630(u16 arg0, Actor *arg1, s32 arg2, s16 arg3, u16 arg4);
//func_1001091C
u16 func_10010BE8(u16 handle, s32 sound, u16 volume, u8 pan, s16 cents, u8 mix, u8 bus);
//func_10010E78
u16 func_10010F88(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance);
//func_10010FFC
//func_10011310
s32  func_100114D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 *arg6, s32 *arg7, s32 *arg8);
//func_10011624
//func_10011BB8
//func_10011EB8
//func_10012020
//func_1001263C
// void func_10017870(u8 arg0);
void func_100186DC(ALCSeq *seq, ALCSeqMarker *marker);
void func_10018790(ALCSeq *seq, ALCSeqMarker *markers, u32 count, u32 base);
//func_1001AAE0
//func_1001ADA4
//func_1001AFEC
//func_1001B07C
//func_1001B200
// s32  func_1001B310(void *arg0, void *arg1);
// u8   func_1001B450(void *arg0, void *arg1);
Acmd *func_1001E530(s16 *outp, Acmd *p, s32 bus);
//func_1019EA88
//func_1019EAB0
//func_1019EAE0
//func_1019ECAC
//func_1019ED8C
//func_1019ED94
//func_1019ED9C
//func_1019EE0C
//func_1019EEAC
//func_1019F018
//func_1019F154
//func_1019F214
//func_1019F4E4
//func_1019F59C
//func_1019FA14
//func_1019FACC
//func_1019FD38
//func_1019FDC0
//func_1019FE18
//func_1019FF78
//func_101A0094
//func_101A0100
//func_101A017C
//func_101A0188
//func_101A02B8
//func_101A0344
//func_101A040C
//func_101A0430
//func_101A04EC
//func_101A04F4
//func_101A0538
void func_1001C224(void *arg0, s32 arg1, s32 arg2, s32 arg3);
void func_1001CF38(void *lowpass, f32 rate);

u16 *func_15001DE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_1501748C(s16 arg0);

Gfx *func_1501A490(Gfx *dl, s16 viewIndex, s32 arg2, s32 arg3, s32 maxBottom, s32 mode);
ModelAttachment *func_1503195C(Actor *, s32, s32);
s32 func_15043BB8(MsgRing *ring, u8 *message, s32 size);
f32  func_15047C00(f32 arg0);
f32  func_15047D60(f32 arg0);
f32  func_150488C8(f32 arg0);
f32  func_15048C30(f32 arg0);
f32  func_15048FC8(Vec3f *arg0);
void func_15049688(f32 *arg0, f32 arg1, f32 *arg2, f32 arg3, f32 arg4, f32 arg5);
void func_1505841C(Actor *arg0, f32 arg1);
void func_15058898(Actor *, f32);
void func_1505A184(u16 arg0, f32 arg1, f32 arg2, f32 *arg3, f32 *arg4, f32 *arg5);
f32  func_1505A72C(Actor *arg0, Actor *arg1);
void func_1505B5F8(Actor *arg0, f32 arg1);
void func_1505E650(Actor *arg0, u16 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6);
u32  func_1505E7CC(s32, Actor*);
void func_1505E874(s32 sourceIdx, Actor *actor);
Actor* func_1505F0AC(s32);
void func_15060778(s32 arg0, Actor *arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_15060F28(Actor *arg0, s32 arg1);
void func_1506160C(Actor *actor, u8 cmd, u8 value, u8 step, s32 player);
void func_15062B1C(Actor *arg0, f32 arg1);
void func_15062B50(Actor *arg0, f32 arg1);
void func_1507BAD0(void);
s32  func_15081574(s32, f32, f32, s32, s32, s32);
void *func_15083568(Actor *parent, s32 setId, f32 arg2, s32 arg3);
s32  func_15083E0C(u8);
Actor *func_15083E90(u8 id);
void func_15085710(s16, s16, s32);
s32  func_150859AC(s16 arg0, s16 arg1);
void func_1508B20C(f32, f32, f32, f32);
s32 func_150A29C8(u32 actor, u32 id);
void func_150A7CB0(MtxF mtx, f32 x, f32 y, f32 z);
void func_150A8050(MtxF mtx, f32 arg1, f32 arg2, f32 arg3);
void func_150A9B0C(MtxF mtx, f32, f32, f32, f32, f32, f32);
void func_150AD770(void);
f32  func_150AD780(f32);
f32  func_150AD78C(f32);
f32  func_150AD900(f32 *arg0, f32 *arg1);
f32  func_150AD930(f32 *arg0);
u32  func_150ADA20(void);
f32  func_150ADA68(void);
void func_150E2EA4(Actor*, s32, s32, s32, f32, f32, f32, f32, f32, f32, s32, s32, s32, f32, f32, s32, f32);
void func_150EA904(s32 arg0, s32 arg1);
void func_1510B32C(s32 arg0, f32 arg1, f32 arg2, f32 arg3);
void func_1510B958(s32 arg0);
Gfx *func_1510B9D0(Gfx *gfx, s16 view);
s32  func_1510FD20(s32 arg0, s32 arg1);
s32  func_15123934(Camera *camera, s32 newMode, s32 newMotion, s32 newTightness, CameraMode slot);
s32  func_151239CC(Camera *camera, CameraMode slot);
void func_15127EB8(Camera *arg0);
void func_1512D748(Camera*, s32, s32);
struct QuadSprite *func_1513C350(void *def, void *colors, u8 arg2, u8 arg3, u8 arg4, s32 arg5, s32 arg6, Vec3f *arg7, s32 arg8, u8 arg9, s32 argA);
void *func_1513D2F0(void *def, void *colors, u8 updateFn1, u8 updateFn2, u8 expireFn, u8 vertexFn, u8 flip, s32 model, s32 arg8, s32 motionSize, u8 argA, s32 argB);
s32  func_1513D6FC(ParticleAxes *axes, s16 angle, f32 width, f32 height, s32 arg4, s32 arg5, u8 onSlope);
void  func_1513E13C(struct QuadSprite *sprite, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, u8 arg6);
s32  func_1513E2AC(s32, s32, s32, f32, f32, f32, f32, f32, u8);
f32  func_151423D8(u8);
f32  func_15144598(TriggerZone *zone);
f32  func_1514462C(s32 arg0);
f32  func_15144B68(f32 arg0);
s32 func_1514D3B0(Actor *owner, s16 type, s32 msg, s32 arg);
Gfx *func_1515D6D0(Gfx *gfx, s32 player);
void func_1515D4D4(s32 r, s32 g, s32 b, u8 priority);
void func_15169260(void *types, s32 count, s32 event, u8 eventType);
void func_1516944C(s32, s32, u8);
void func_15169850(s32 event, u8 eventType, Actor **owner, u8 *ownerId, void *obj);
void func_15174690(s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_15177410(u8 arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4, f32 arg5, u16 arg6, f32 arg7, s8 arg8, s8 arg9, u8 argA, u8 argB, u8 argC, u8 argD, u8 argE, u8 argF);
struct LensFlare *func_1517E080(s32 type, s32 alpha);
s32  func_1517EFAC(s32 arg0);
void func_15178E14(u8 tag);
s32  func_15187EC0(s32 arg0, f32 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, u8 arg6, u8 arg7);
void func_151B8DB0(s32 arg0, u8 arg1, u8 arg2, s32 arg3);
void func_151D5404(Vec3f*, f32, f32, f32, s32, s32, s32, s32);
void func_151D5714(Actor *obj, Vec3f *from, Vec3f *to, u8 bone, f32 scale, u8 arg5, s32 arg6);
void func_151D9B8C(u8 arg0, f32 arg1, u8 arg2, s32 arg3, Vec3f *arg4, s16 arg5, u8 arg6, u8 arg7, u8 arg8, u8 arg9, s32 argA);
s32 func_151DA08C(u8 kind, f32 size, f32 shrink, u8 arg3, s16 lifetime, s32 arg5, Vec3f *pos, s32 arg7, s32 arg8);
void func_151DBCBC(u8 arg0, f32 arg1, s16 arg2, s32 arg3, void *arg4, u8 arg5, s32 arg6);
void func_151EF954(Mtx *m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far, f32 scale); // guOrtho: builds the float matrix (func_151EF800) and converts it with guMtxF2L2
s32 func_151F3C4C(s32 arg0, void *dst, s32 len, s32 offset);
void func_151FA130(void);
void func_1019EA88(void);
void func_15168B10(EffectHeader *obj, s32 type);
#include "subsystems.h"

#endif
