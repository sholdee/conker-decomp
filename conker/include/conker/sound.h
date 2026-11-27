#ifndef CONKER_SOUND_H
#define CONKER_SOUND_H

#include <ultra64.h>
#include "structs.h"

/*
 * Subsystem "sound": prototypes of the non-static functions and declarations of the globals
 * that these files define, grouped by file in address order:
 *   init/Audio/SoundPan/soundpan.c
 *   init/Audio/DynamicSound/dynamicsound.c
 *   game/Audio/ObjectSound/objectsound.c
 */

/* init/Audio/SoundPan/soundpan.c */
s32 sound_computePanVolume(s32 dx, s32 dy, s32 dz, f32 angle, s32 distX, s32 distY, s32 distZ, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut);
s32 sound_computePathPanVolume(s32 path, s32 panX, s32 panY, s32 panZ, f32 angle, s32 x, s32 y, s32 z, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volume, s32 *distanceOut);
s32 sound_computePan(f32 dx, f32 dz, s32 angle);

/* init/Audio/DynamicSound/dynamicsound.c */
s32 sound_chirpCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_fadeOutCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan);
s32 sound_delayedPlayCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_actorSound1DelayCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_actorUntrackedDelayCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_followOwnerCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_stopWhenSilentCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_followOwnerAnimsCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void sound_resetVoices(void);
void sound_initLevel(s32 mode);
s32 sound_isPlaying(u16 handle);
s32 sound_isLooping(u16 handle);
s32 sound_isSoundPlaying(u16 sound);
s32 sound_pickVariant(s32 base, s32 count);
s32 sound_getNearestListenerVolume(s32 arg0, s16 x, s16 y, s16 z, s32 *pan, s32 nearDistance, s32 farDistance);
void sound_setVoiceParam(u16 handle, s16 param, s32 value);
void sound_setVoiceSpatial(u16 handle, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance);
void sound_setVoicePanAt(u16 handle, s16 x, s16 y, s16 z);
u16 sound_addDynamicSound(u16 sound, s16 x, s16 y, s16 z, s32 volume, u16 farDistance, s16 nearDistance, SoundRecordCallback callback, void *owner, s32 key, s32 flags, s32 cents);
void sound_stopDynamicSoundAt(u16 sound, s16 x, s16 y, s16 z, u16 farDistance); /* stops a positional sound */
void sound_stopByCallback(SoundRecordCallback callback, void *owner, s32 key);
void sound_stopByHandle(u16 handle);
s32 sound_stopRecord(SoundRecord *records, s32 index, s32 *count);
s32 sound_findByHandle(u16 handle, void *owner, s32 key);
s32 sound_findByCallback(SoundRecordCallback callback, void *owner, s32 key);
void sound_setVolumeByCallback(SoundRecordCallback callback, void *owner, s32 key, s32 volume, s32 cents);
void sound_replaceCallback(SoundRecordCallback callback, void *owner, s32 key, SoundRecordCallback newCallback, void *newOwner, s32 newKey);
s32 sound_playActorSound2(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance);
s32 sound_playActorSound1(u16 sound, void *owner, u32 volume, s16 nearDistance, u16 farDistance);
void sound_playActorSound1Delayed(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance, s32 delay);
void sound_playActorUntracked(u16 sound, Actor *obj, u32 volume, s16 nearDistance, u16 farDistance);
void sound_playActorUntrackedDelayed(u16 sound, Actor *obj, s32 volume, s16 nearDistance, u16 farDistance, s32 delay);
s32 sound_isActorSound2Playing(Actor *obj);
s32 sound_isActorSound1Playing(Actor *obj);
void sound_setActorSound2Volume(Actor *obj, u32 volume);
s32 sound_stopActorSound2(Actor *obj);
void sound_stopActorSound1(Actor *obj);
void sound_stopActorSounds(Actor *obj);
u16 sound_startVoice(u16 handle, s32 sound, u16 volume, u8 pan, s16 cents, u8 mix, u8 bus);
u16 sound_startVoiceAt(u16 handle, s32 sound, u16 volume, s16 cents, s32 mix, s32 arg5, s32 x, s32 y, s32 z, s32 nearDistance, s32 farDistance);
/* old-style definition in init/Audio/DynamicSound/dynamicsound.c; this is the prototype its callers use */
u16 sound_play(s32 sound, u16 volume, u8 pan, s16 cents, u8 mix);
u16 sound_playAt(s32 sound, u16 volume, s16 cents, u8 mix, s32 arg4, s16 x, s16 y, s16 z, s16 nearDistance, s16 farDistance);
s32 sound_playOnActor(u16 handle, s32 sound, u16 volume, s16 cents, u8 mix, Actor *obj);
void sound_stop(u16 handle);
void sound_stopNextUpdate(u16 handle);
s32 sound_queueStop(s32 handle, s32 delay);
void sound_runStopQueue(void);
s32 sound_getPlayingSound(u16 handle);
s32 sound_getPanVolumeAt(s32 x, s32 y, s32 z, s32 volume, s32 farDistance, s32 nearDistance, s32 *pan, s32 *volumeOut, s32 *distance);
void sound_updateRecords(SoundRecord *records, s32 *count, s32 start, s32 end);
void sound_update(void);
void sound_emptyInitLevel(s32 mode);
void sound_setMuted(s32 mute);
u16 sound_pickEffectVariant(s32 effect, s16 *volume, s32 variant);
extern s8 gSoundFxMixBoost;
extern u8 D_80041FD9;
extern s32 gSoundTargetVolume;

/* game/Audio/ObjectSound/objectsound.c */
void sound_clearCooldowns(void);
void sound_updateCooldowns(void);
s32 sound_followDelayedCameraCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
s32 sound_followDelayedCallback(SoundRecord *record, s32 *active, u32 *volume, s32 *pan, s32 *cents, s32 *mix, u16 *sound);
void sound_playWithCooldown(s32 sound, s32 objId, u32 variantCount, s32 slot, s32 pairVariants);

#endif
