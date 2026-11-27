#include <ultra64.h>

#include "functions.h"
#include "variables.h"

void music_init(void) {
    OSMesgQueue *msgQueue;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        msgQueue = &gMarkerQueues[i];
        osCreateMesgQueue(msgQueue, gMarkerQueueBufs[i], 8);
        audio_setSeqMsgQueue(i, msgQueue);
    }

    func_100085A4(0, 6, 1); // stub function
    music_reset();
}

void func_15000090(void) {
    music_freeStoppedTunes();
}

void music_startLevelMusic(void) {
    LevelMusic q;
    s32 i;
    s32 j;
    s32 arr[3];
    s32 n;
    s32 cnt;
    s32 a2;

    cnt = 0;
    a2 = 0;
    gMusicPaused = 0;
    if (gLevelHeader) {
        q = gLevelHeader->music;
    } else {
        q.mode = 1;
        q.volume = 0;
        q.volume2 = 0;
        q.unkD = 2;
        q.song = 0;
        q.song2 = 0;
    }
    gMusicEventFlags = 0;
    D_80041F08 = 0;
    D_80041F0C = 0;
    if (q.song == 1 && (gTaskDoneBits[0x4] & 0x80)) {
        q.song = 0x46;
        q.mutedChannels = 0x10;
    }
    switch (q.mode) {
    case 2:
        a2 = 1;
    case 4:
    case 0:
        music_freeStoppedTunes();
        break;
    case 3:
        a2 = 1;
    case 1:
        if (gLevelLoadState == 2) {
            music_freeStoppedTunes();
        } else if (gPrevLevel != 0x22) {
            a2 = 0;
            q.song = -1;
            q.song2 = -1;
        }
        break;
    }
    i = music_getPlayingIds(arr);
    n = i;
    for (; i < 3; i++) {
        arr[i] = -1;
    }
    if (q.mode == 4 && q.song2 >= 0) {
        for (i = 0; i < 3; i++) {
            if (q.song2 == arr[i]) {
                if (q.song >= 0) {
                    for (j = 0; j < 3; j++) {
                        if (q.song == arr[j]) {
                            q.song2 = 0;
                        }
                    }
                }
                if (q.song2 >= 0) {
                    q.song = q.song2;
                    q.mutedChannels = q.mutedChannels2;
                    q.volume = q.volume2;
                }
            }
        }
        if (q.song2 >= 0) {
            q.song2 = 0;
        }
    }
    music_pauseAll(0);
    if (a2 == 0) {
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                if (q.song == arr[i]) {
                    music_fadeChannelVolume(q.song, 0, q.mutedChannels, 0);
                    music_fadeChannelVolume(q.song, 0x64, (q.mutedChannels ^ 0xFFFF), 0);
                    music_setVolume(q.song, q.volume);
                    music_setFade(q.song, 0x7FFF, 0);
                    arr[i] = -1;
                    q.song = -1;
                    n--;
                } else if (q.song2 == arr[i]) {
                    music_fadeChannelVolume(q.song2, 0, q.mutedChannels2, 0);
                    music_fadeChannelVolume(q.song2, 0x64, (q.mutedChannels2 ^ 0xFFFF), 0);
                    music_setVolume(q.song2, q.volume2);
                    music_setFade(q.song2, 0x7FFF, 0);
                    arr[i] = -1;
                    q.song2 = -1;
                    n--;
                }
            }
        }
    }
    if (q.song >= 0) {
        cnt = 1;
    }
    if (q.song2 >= 0) {
        cnt++;
    }
    if (n != 0 && cnt < n) {
        j = n - cnt;
        for (i = 2; i >= 0 && j != 0; i--) {
            if (arr[i] != -1) {
                music_stopTune(arr[i], 4);
                arr[i] = -1;
                j--;
            }
        }
    }
    if (q.song >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.song > 0) {
                music_playTune(q.song, 0, 0);
            }
        } else {
            if (q.song > 0) {
                music_playTune(q.song, arr[j], 5);
            } else {
                music_stopTune(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.song > 0) {
            music_fadeChannelVolume(q.song, 0, q.mutedChannels, 0);
            music_setVolume(q.song, q.volume);
        }
    }
    if (q.song2 >= 0) {
        j = -1;
        for (i = 0; i < 3; i++) {
            if (arr[i] != -1) {
                j = i;
            }
        }
        if (j == -1) {
            if (q.song2 > 0) {
                music_playTune(q.song2, 0, 0);
            }
        } else {
            if (q.song2 > 0) {
                music_playTune(q.song2, arr[j], 5);
            } else {
                music_stopTune(arr[j], 4);
            }
            arr[j] = -1;
        }
        if (q.song2 > 0) {
            music_fadeChannelVolume(q.song2, 0, q.mutedChannels2, 0);
            music_setVolume(q.song2, q.volume2);
        }
    }
    func_10017944(1, q.unkD);
}

void music_initLevelViewState() {
    s32 i;
    s32 j;

    gDefaultNear = gLevelHeader->nearPlane;
    gDefaultFar = gLevelHeader->farPlane;
    gCullFarZ = 100.0f - gDefaultFar;

    level_initTintWaves();

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++) {
            gTintPrimColors[j][i] = 0;
            gTintEnvColors[j][i] = 0;
        }
    }

    for (i = 0; i < 3; i++) {
        D_800D9B84[i] = 0;
        D_800D9B88[i] = 0;
    }

    D_800D9B8B = 0xFF;
    D_800D9B8C = 0xFF;
    gViewGfxLightMax = 0xB;

    for (i = 0; i <= gScreenLayout; i++) {
        gViewGfxLights[i] = light_allocGfxLights(gViewGfxLightMax);
    }

    for (j = 0; j <= gScreenLayout; j++) {
        for (i = 0; i < 2; i++) {
            gViewAmbients[j][i].l.col[0] = gViewAmbients[j][i].l.colc[0] = 0xFF;
            gViewAmbients[j][i].l.col[1] = gViewAmbients[j][i].l.colc[1] = 0xFF;
            gViewAmbients[j][i].l.col[2] = gViewAmbients[j][i].l.colc[2] = 0xFF;
        }
    }

    for (i = 0; i <= gScreenLayout; i++) {
        gFovChanges[i].x = -1.0f;
    }

    gFovChangePending = 0;
    for (i = 0; i <= gScreenLayout; i++) {
        gViewClips[i].near = -1.0f;
    }
    gClipChangePending = 0;
}
// NON-MATCHING: almost only reg remaining
// void music_initLevelViewState(void) {
//     s32 i;
//     s32 j;
//
//     gDefaultNear = gLevelHeader->nearPlane;
//     gDefaultFar = gLevelHeader->farPlane;
//     gCullFarZ = 100.0f - gDefaultFar;
//
//     level_initTintWaves();
//
//     // downhill from here!
//     for (i = 0; i < 4; i++) {
//         for (j = 0; j < 3; j++) {
//             gTintPrimColors[i][j] = 0;
//             gTintEnvColors[i][j] = 0;
//         }
//     }
//
//     for (i = 0; i < 3; i++) {
//         D_800D9B84[i] = 0;
//         D_800D9B88[i] = 0;
//     }
//
//     D_800D9B8B = 0xFF;
//     D_800D9B8C = 0xFF;
//     gViewGfxLightMax = 0xBU;
//
//     for (i = 0; i <= gScreenLayout; i++) {
//         gViewGfxLights[i] = light_allocGfxLights(gViewGfxLightMax);
//     }
//
//     for (i = 0; i <= gScreenLayout; i++) {
//         for (j = 0; j < 2; j++) {
//             gViewAmbients[i][j][4] = 0xff;
//             gViewAmbients[i][j][0] = 0xff;
//             gViewAmbients[i][j][5] = 0xff;
//             gViewAmbients[i][j][1] = 0xff;
//             gViewAmbients[i][j][6] = 0xff;
//             gViewAmbients[i][j][2] = 0xff;
//         }
//     }
//
//     for (i = 0; i <= gScreenLayout; i++) {
//         gFovChanges[i].unk0 = -1.0f;
//     }
//
//     gFovChangePending = 0;
//     for (i = 0; i <= gScreenLayout; i++) {
//         gViewClips[i].near = -1.0f;
//     }
//     gClipChangePending = 0;
// }
