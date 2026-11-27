#include <ultra64.h>
#include "conker/particle.h"

/*
 * A data-only unit after the game/Particle/ActorEmitter/actoremitter..game/Effects/BurnDamage/burndamage unit in the link. Its .data (0x232150) starts on a 16-byte
 * boundary after that unit's handler tables and ends at 0x234300, where game/Effects/SurfaceHandlers/surfacehandlers's .data starts; no code
 * between them reads it. It holds the actor emitter data of game/Particle/ActorEmitter/actoremitter.c: the emission points, particle
 * templates and emitter infos, the per-LOD info rows, the animation id lists and the emitter tables.
 */

/* Emission points on the owner's bones (EmitterInfo.points), one per slot. */
static EmitterPoint gAllOverEmitPoints[] = {
    { { 26.0f, 0.0f, 0.0f }, 1 },
    { { 0.0f, 0.0f, -22.0f }, 1 },
    { { -29.0f, 0.0f, 9.0f }, 1 },
    { { 12.0f, 15.0f, -14.0f }, 1 },
    { { -17.0f, 9.0f, 21.0f }, 1 },
    { { -8.0f, 28.0f, -14.0f }, 1 },
    { { 18.0f, -1.0f, 19.0f }, 1 },
    { { 0.0f, -21.0f, 19.0f }, 1 },
    { { -6.0f, 24.0f, 13.0f }, 1 },
    { { 1.0f, 5.0f, 0.0f }, 2 },
    { { 10.0f, -4.0f, -3.0f }, 2 },
    { { 10.0f, -13.0f, -6.0f }, 2 },
    { { 6.0f, 0.0f, 0.0f }, 3 },
    { { 12.0f, -12.0f, 6.0f }, 3 },
    { { 15.0f, -18.0f, -8.0f }, 3 },
    { { 14.0f, -10.0f, 0.0f }, 4 },
    { { 17.0f, -19.0f, 12.0f }, 4 },
    { { -1.0f, -5.0f, 0.0f }, 7 },
    { { -10.0f, 4.0f, -4.0f }, 7 },
    { { -6.0f, 0.0f, 0.0f }, 8 },
    { { -12.0f, -15.0f, 0.0f }, 8 },
    { { 10.0f, -17.0f, 0.0f }, 10 },
    { { 10.0f, 8.0f, 23.0f }, 10 },
    { { 3.0f, 8.0f, -12.0f }, 10 },
    { { 0.0f, 0.0f, 14.0f }, 12 },
    { { 10.0f, -8.0f, 12.0f }, 15 },
    { { -14.0f, 0.0f, 21.0f }, 15 },
    { { 18.0f, 0.0f, 0.0f }, 16 },
    { { 18.0f, -14.0f, 28.0f }, 16 },
    { { 0.0f, -5.0f, -14.0f }, 17 },
    { { 8.0f, -5.0f, 5.0f }, 17 },
    { { 0.0f, -12.0f, -10.0f }, 18 },
    { { 12.0f, 3.0f, 17.0f }, 18 },
    { { 0.0f, 0.0f, 38.0f }, 19 },
    { { 0.0f, -10.0f, -14.0f }, 20 },
    { { 0.0f, -10.0f, -14.0f }, 21 },
    { { 0.0f, -10.0f, -5.0f }, 22 },
    { { 0.0f, 10.0f, 15.0f }, 23 },
    { { 0.0f, 9.0f, -21.0f }, 24 },
    { { -23.0f, 19.0f, -49.0f }, 24 },
    { { 13.0f, 13.0f, -24.0f }, 24 },
    { { 0.0f, 24.0f, -17.0f }, 25 },
    { { -42.0f, 18.0f, -46.0f }, 25 },
    { { 23.0f, 21.0f, -17.0f }, 25 },
    { { 40.0f, -1.0f, 0.0f }, 26 },
    { { -38.0f, -1.0f, -26.0f }, 26 },
    { { -8.0f, 35.0f, -35.0f }, 26 },
    { { 0.0f, 0.0f, -38.0f }, 27 },
    { { 0.0f, -32.0f, 12.0f }, 27 },
    { { 0.0f, -21.0f, 1.0f }, 27 },
    { { 42.0f, -14.0f, -6.0f }, 26 },
    { { -32.0f, -14.0f, -6.0f }, 25 },
};
static EmitterPoint gTailEmitPoints[] = {
    { { 19.0f, -26.0f, -41.0f }, 26 },
    { { -9.0f, -32.0f, -9.0f }, 26 },
    { { -42.0f, -13.0f, -28.0f }, 26 },
    { { 1.0f, -36.0f, -47.0f }, 25 },
    { { -40.0f, 1.0f, -47.0f }, 25 },
    { { 32.0f, 1.0f, -47.0f }, 25 },
    { { 27.0f, 10.0f, -23.0f }, 25 },
    { { 0.0f, -33.0f, -23.0f }, 25 },
    { { 31.0f, 0.0f, -23.0f }, 25 },
    { { 14.0f, -5.0f, -31.0f }, 24 },
    { { 21.0f, 0.0f, -40.0f }, 24 },
    { { -19.0f, -15.0f, 4.0f }, 24 },
    { { -13.0f, 0.0f, -24.0f }, 24 },
    { { 0.0f, 8.0f, -24.0f }, 24 },
    { { 0.0f, 0.0f, 0.0f }, 24 },
    { { 0.0f, 9.0f, -21.0f }, 24 },
    { { -23.0f, 19.0f, -49.0f }, 24 },
    { { 13.0f, 13.0f, -24.0f }, 24 },
    { { 0.0f, 24.0f, -17.0f }, 25 },
    { { -42.0f, 18.0f, -46.0f }, 25 },
    { { 23.0f, 21.0f, -17.0f }, 25 },
    { { 40.0f, -1.0f, 0.0f }, 26 },
    { { -38.0f, -1.0f, -26.0f }, 26 },
    { { -8.0f, 35.0f, -35.0f }, 26 },
    { { 0.0f, 0.0f, -38.0f }, 27 },
    { { 0.0f, -32.0f, 12.0f }, 27 },
    { { 0.0f, -21.0f, 1.0f }, 27 },
    { { 42.0f, -14.0f, -6.0f }, 26 },
    { { -32.0f, -14.0f, -6.0f }, 25 },
};
static EmitterPoint D_8008DBA0[] = {
    { { -27.0f, 0.0f, 0.0f }, 1 },
    { { -27.0f, 19.0f, -13.0f }, 1 },
    { { -5.0f, 19.0f, 22.0f }, 1 },
    { { 8.0f, 36.0f, 19.0f }, 1 },
    { { 17.0f, 6.0f, 12.0f }, 1 },
    { { 13.0f, 37.0f, -13.0f }, 1 },
    { { 0.0f, 38.0f, -19.0f }, 2 },
    { { 0.0f, 28.0f, 9.0f }, 2 },
    { { 7.0f, 12.0f, -1.0f }, 3 },
    { { -5.0f, 19.0f, -5.0f }, 3 },
    { { 0.0f, 60.0f, -28.0f }, 5 },
    { { 22.0f, 63.0f, -13.0f }, 5 },
    { { 14.0f, 74.0f, -10.0f }, 5 },
    { { -33.0f, 41.0f, -9.0f }, 5 },
    { { 13.0f, 19.0f, 21.0f }, 6 },
    { { 0.0f, 0.0f, 17.0f }, 7 },
    { { 0.0f, -19.0f, 77.0f }, 10 },
    { { 0.0f, 12.0f, 0.0f }, 11 },
    { { -3.0f, 17.0f, 0.0f }, 13 },
    { { -13.0f, -9.0f, 51.0f }, 15 },
    { { -18.0f, 0.0f, 0.0f }, 16 },
    { { -35.0f, 5.0f, 6.0f }, 16 },
    { { -7.0f, 7.0f, 0.0f }, 16 },
    { { -33.0f, 5.0f, 7.0f }, 16 },
    { { -31.0f, 8.0f, 0.0f }, 17 },
    { { -10.0f, 0.0f, 0.0f }, 17 },
    { { -24.0f, 5.0f, 0.0f }, 17 },
    { { -26.0f, 13.0f, -19.0f }, 18 },
    { { -58.0f, -5.0f, -19.0f }, 18 },
    { { -10.0f, -12.0f, -0.0f }, 18 },
    { { 12.0f, 0.0f, -4.0f }, 25 },
    { { 24.0f, 0.0f, 0.0f }, 25 },
    { { 40.0f, 7.0f, 0.0f }, 25 },
    { { 0.0f, 5.0f, 0.0f }, 26 },
    { { 29.0f, 5.0f, 0.0f }, 26 },
    { { 19.0f, -7.0f, 0.0f }, 26 },
    { { 26.0f, 10.0f, 0.0f }, 26 },
    { { 10.0f, 10.0f, 0.0f }, 27 },
    { { 14.0f, 5.0f, 0.0f }, 28 },
    { { 44.0f, -12.0f, 0.0f }, 30 },
    { { -9.0f, -8.0f, 0.0f }, 34 },
    { { 0.0f, 0.0f, 0.0f }, 34 },
    { { 0.0f, -12.0f, -18.0f }, 35 },
    { { 0.0f, 0.0f, 0.0f }, 35 },
    { { -12.0f, -0.0f, 22.0f }, 36 },
    { { -17.0f, -4.0f, 35.0f }, 36 },
    { { -16.0f, 6.0f, 30.0f }, 36 },
    { { 0.0f, 3.0f, 19.0f }, 37 },
    { { -14.0f, 4.0f, 0.0f }, 37 },
    { { 0.0f, 4.0f, 26.0f }, 37 },
    { { 0.0f, 0.0f, -0.0f }, 38 },
    { { 0.0f, -6.0f, 12.0f }, 38 },
    { { 0.0f, 0.0f, 0.0f }, 39 },
    { { 8.0f, -15.0f, -6.0f }, 39 },
    { { 0.0f, 0.0f, -10.0f }, 40 },
    { { 8.0f, 0.0f, 18.0f }, 40 },
    { { -6.0f, 0.0f, 38.0f }, 40 },
    { { 0.0f, 10.0f, 4.0f }, 41 },
    { { 13.0f, -6.0f, 4.0f }, 41 },
    { { 0.0f, 0.0f, 25.0f }, 41 },
    { { 0.0f, -8.0f, -16.0f }, 42 },
    { { 6.0f, -9.0f, -14.0f }, 42 },
    { { -5.0f, -24.0f, -29.0f }, 42 },
    { { 0.0f, -8.0f, -16.0f }, 43 },
    { { 0.0f, -5.0f, -24.0f }, 43 },
    { { 0.0f, 0.0f, -10.0f }, 43 },
    { { 0.0f, 0.0f, 0.0f }, 44 },
    { { 0.0f, 0.0f, -10.0f }, 44 },
    { { 0.0f, 0.0f, -18.0f }, 44 },
    { { 0.0f, 0.0f, -30.0f }, 44 },
    { { 0.0f, 0.0f, -20.0f }, 45 },
    { { 0.0f, 18.0f, -10.0f }, 45 },
};
static EmitterPoint D_8008E020[] = {
    { { 0.0f, 0.0f, -110.0f }, 1 },
    { { 52.0f, 0.0f, -82.0f }, 1 },
    { { 79.0f, 19.0f, -66.0f }, 1 },
    { { 111.0f, 19.0f, -34.0f }, 1 },
    { { 0.0f, 0.0f, 114.0f }, 1 },
    { { -44.0f, 8.0f, 114.0f }, 1 },
    { { -91.0f, 19.0f, 86.0f }, 1 },
    { { -52.0f, 0.0f, -82.0f }, 1 },
    { { -79.0f, 19.0f, -66.0f }, 1 },
    { { -111.0f, 19.0f, -34.0f }, 1 },
    { { 44.0f, 8.0f, 114.0f }, 1 },
    { { 91.0f, 19.0f, 86.0f }, 1 },
    { { -2.0f, 1.0f, -118.0f }, 2 },
    { { -67.0f, 24.0f, -78.0f }, 2 },
    { { -37.0f, 65.0f, -82.0f }, 2 },
    { { -82.0f, 101.0f, -35.0f }, 2 },
    { { -36.0f, 101.0f, -35.0f }, 2 },
    { { 67.0f, 24.0f, -78.0f }, 2 },
    { { 37.0f, 65.0f, -82.0f }, 2 },
    { { 82.0f, 101.0f, -35.0f }, 2 },
    { { 36.0f, 101.0f, -35.0f }, 2 },
    { { 0.0f, 74.0f, 1.0f }, 3 },
    { { -24.0f, 74.0f, 36.0f }, 3 },
    { { -51.0f, 58.0f, -3.0f }, 3 },
    { { 24.0f, 74.0f, 36.0f }, 3 },
    { { 51.0f, 58.0f, -3.0f }, 3 },
    { { -1.0f, 58.0f, 98.0f }, 11 },
    { { 62.0f, 19.0f, 62.0f }, 11 },
    { { 74.0f, 94.0f, 62.0f }, 11 },
    { { 112.0f, 44.0f, 24.0f }, 11 },
    { { 38.0f, 121.0f, 71.0f }, 11 },
    { { -62.0f, 19.0f, 62.0f }, 11 },
    { { -74.0f, 94.0f, 62.0f }, 11 },
    { { -112.0f, 44.0f, 24.0f }, 11 },
    { { -38.0f, 121.0f, 71.0f }, 11 },
    { { 0.0f, 31.0f, 0.0f }, 13 },
    { { 35.0f, 2.0f, 28.0f }, 13 },
    { { 17.0f, 2.0f, -31.0f }, 13 },
    { { 56.0f, 2.0f, -31.0f }, 13 },
    { { 56.0f, 2.0f, 38.0f }, 13 },
    { { 0.0f, 28.0f, 0.0f }, 14 },
    { { 17.0f, 28.0f, 27.0f }, 14 },
    { { 61.0f, 28.0f, 18.0f }, 14 },
    { { 61.0f, 28.0f, -39.0f }, 14 },
    { { 50.0f, 10.0f, 1.0f }, 15 },
    { { 34.0f, 21.0f, 3.0f }, 17 },
    { { 0.0f, 31.0f, 0.0f }, 22 },
    { { -35.0f, 2.0f, 28.0f }, 22 },
    { { -17.0f, 2.0f, -31.0f }, 22 },
    { { -56.0f, 2.0f, -31.0f }, 22 },
    { { -56.0f, 2.0f, 38.0f }, 22 },
    { { 0.0f, 28.0f, 0.0f }, 23 },
    { { -17.0f, 28.0f, 27.0f }, 23 },
    { { -61.0f, 28.0f, 18.0f }, 23 },
    { { -61.0f, 28.0f, -39.0f }, 23 },
    { { -50.0f, 10.0f, 1.0f }, 24 },
    { { 34.0f, 21.0f, 3.0f }, 26 },
};
static EmitterPoint D_8008E3B0[] = {
    { { 0.0f, 0.0f, 114.0f }, 1 },
    { { -44.0f, 8.0f, 114.0f }, 1 },
    { { -91.0f, 19.0f, 86.0f }, 1 },
    { { 44.0f, 8.0f, 114.0f }, 1 },
    { { 91.0f, 19.0f, 86.0f }, 1 },
    { { -1.0f, 58.0f, 98.0f }, 11 },
    { { 62.0f, 19.0f, 62.0f }, 11 },
    { { 74.0f, 94.0f, 62.0f }, 11 },
    { { 112.0f, 44.0f, 24.0f }, 11 },
    { { 38.0f, 121.0f, 71.0f }, 11 },
    { { -62.0f, 19.0f, 62.0f }, 11 },
    { { -74.0f, 94.0f, 62.0f }, 11 },
    { { -112.0f, 44.0f, 24.0f }, 11 },
    { { -38.0f, 121.0f, 71.0f }, 11 },
};
static EmitterPoint D_8008E490[] = {
    { { 0.0f, 31.0f, 0.0f }, 13 },
    { { 35.0f, 2.0f, 28.0f }, 13 },
    { { 17.0f, 2.0f, -31.0f }, 13 },
    { { 56.0f, 2.0f, -31.0f }, 13 },
    { { 56.0f, 2.0f, 38.0f }, 13 },
    { { 0.0f, 28.0f, 0.0f }, 14 },
    { { 17.0f, 28.0f, 27.0f }, 14 },
    { { 61.0f, 28.0f, 18.0f }, 14 },
    { { 61.0f, 28.0f, -39.0f }, 14 },
    { { 50.0f, 10.0f, 1.0f }, 15 },
    { { 34.0f, 21.0f, 3.0f }, 17 },
};
static EmitterPoint D_8008E540[] = {
    { { 0.0f, 31.0f, 0.0f }, 22 },
    { { -35.0f, 2.0f, 28.0f }, 22 },
    { { -17.0f, 2.0f, -31.0f }, 22 },
    { { -56.0f, 2.0f, -31.0f }, 22 },
    { { -56.0f, 2.0f, 38.0f }, 22 },
    { { 0.0f, 28.0f, 0.0f }, 23 },
    { { -17.0f, 28.0f, 27.0f }, 23 },
    { { -61.0f, 28.0f, 18.0f }, 23 },
    { { -61.0f, 28.0f, -39.0f }, 23 },
    { { -50.0f, 10.0f, 1.0f }, 24 },
    { { 34.0f, 21.0f, 3.0f }, 26 },
};
static EmitterPoint D_8008E5F0[] = {
    { { 26.0f, 0.0f, 0.0f }, 1 },
};
static EmitterPoint D_8008E600[] = {
    { { 26.0f, 0.0f, 0.0f }, 1 },
    { { 0.0f, 0.0f, -22.0f }, 1 },
    { { -29.0f, 0.0f, 9.0f }, 1 },
    { { 12.0f, 15.0f, -14.0f }, 1 },
    { { -17.0f, 9.0f, 21.0f }, 1 },
    { { -8.0f, 28.0f, -14.0f }, 1 },
    { { 18.0f, -1.0f, 19.0f }, 1 },
    { { 0.0f, -21.0f, 19.0f }, 1 },
    { { -6.0f, 24.0f, 13.0f }, 1 },
    { { 1.0f, 5.0f, 0.0f }, 2 },
    { { 10.0f, -4.0f, -3.0f }, 2 },
    { { 10.0f, -13.0f, -6.0f }, 2 },
    { { 6.0f, 0.0f, 0.0f }, 3 },
    { { 12.0f, -12.0f, 6.0f }, 3 },
    { { 15.0f, -18.0f, -8.0f }, 3 },
    { { 14.0f, -10.0f, 0.0f }, 4 },
    { { 17.0f, -19.0f, 12.0f }, 4 },
    { { -1.0f, -5.0f, 0.0f }, 7 },
    { { -10.0f, 4.0f, -4.0f }, 7 },
    { { -6.0f, 0.0f, 0.0f }, 8 },
    { { -12.0f, -15.0f, 0.0f }, 8 },
    { { 10.0f, -17.0f, 0.0f }, 10 },
    { { 10.0f, 8.0f, 23.0f }, 10 },
    { { 3.0f, 8.0f, -12.0f }, 10 },
    { { 0.0f, 0.0f, 14.0f }, 12 },
    { { 10.0f, -8.0f, 12.0f }, 15 },
    { { -14.0f, 0.0f, 21.0f }, 15 },
    { { 18.0f, 0.0f, 0.0f }, 16 },
    { { 18.0f, -14.0f, 28.0f }, 16 },
    { { 0.0f, -5.0f, -14.0f }, 17 },
    { { 8.0f, -5.0f, 5.0f }, 17 },
    { { 0.0f, -12.0f, -10.0f }, 18 },
    { { 12.0f, 3.0f, 17.0f }, 18 },
    { { 0.0f, 0.0f, 38.0f }, 19 },
    { { 0.0f, -10.0f, -14.0f }, 20 },
    { { 0.0f, -10.0f, -14.0f }, 21 },
    { { 0.0f, -10.0f, -5.0f }, 22 },
    { { 0.0f, 10.0f, 15.0f }, 23 },
    { { 0.0f, 9.0f, -21.0f }, 24 },
    { { -23.0f, 19.0f, -49.0f }, 24 },
    { { 13.0f, 13.0f, -24.0f }, 24 },
    { { 0.0f, 24.0f, -17.0f }, 25 },
    { { -42.0f, 18.0f, -46.0f }, 25 },
    { { 23.0f, 21.0f, -17.0f }, 25 },
    { { 40.0f, -1.0f, 0.0f }, 26 },
    { { -38.0f, -1.0f, -26.0f }, 26 },
    { { -8.0f, 35.0f, -35.0f }, 26 },
    { { 0.0f, 0.0f, -38.0f }, 27 },
    { { 0.0f, -32.0f, 12.0f }, 27 },
    { { 0.0f, -21.0f, 1.0f }, 27 },
    { { 42.0f, -14.0f, -6.0f }, 26 },
    { { -32.0f, -14.0f, -6.0f }, 25 },
};
/* Particle templates (EmitterInfo.tmpl). */
static ParticleTemplate D_8008E940 = {
    22.6f, 13.786f, 57.969f, 42.714f, 118.763f,
    25137.0f, 6733.0f, 0.5f, 0.455f, 0.05f,
    32, 8, 0.85f, 0.262f, 47, 0,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008E980 = {
    22.6f, 13.786f, 2.3883228f, 1.7598168f, 4.8930354f,
    25137.0f, 6733.0f, 0.0f, 0.815f, 0.034f,
    17, 15, 0.85f, 0.262f, 11, 17,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate D_8008E9C0 = {
    19.1196f, 11.662956f, 41.737682f, 30.754082f, 85.50936f,
    25137.0f, 6733.0f, 0.243f, 0.295f, 0.037f,
    37, 6, 0.575f, 0.332f, 42, 23,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EA00 = {
    28.656801f, 17.480648f, 65.56294f, 48.309536f, 134.32095f,
    25137.0f, 6733.0f, 0.03f, 0.735f, -0.02f,
    13, 19, 1.0f, 0.5f, 7, 14,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EA40 = {
    13.469601f, 8.216456f, 35.82484f, 26.397251f, 73.39553f,
    25137.0f, 6733.0f, 0.309f, 0.279f, -0.007f,
    32, 8, 0.5f, 0.209f, 40, 15,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EA80 = {
    14.464f, 8.82304f, 37.10016f, 27.33696f, 76.008316f,
    25137.0f, 6733.0f, 0.419f, 0.0f, 0.022f,
    25, 10, 0.853f, 0.099f, 30, 4,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EAC0 = {
    7.006f, 4.27366f, 20.521027f, 15.120756f, 42.042103f,
    25137.0f, 6733.0f, 0.154f, 0.5f, 0.015f,
    40, 6, 0.706f, 0.279f, 36, 8,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EB00 = {
    51.5506f, 31.445866f, 2.434698f, 1.793988f, 4.988046f,
    25137.0f, 6733.0f, 0.144f, 0.0f, -0.11f,
    24, 10, 1.0f, 0.5f, 25, 0,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate D_8008EB40 = {
    43.188602f, 26.345047f, 60.809483f, 44.806988f, 124.58239f,
    25137.0f, 6733.0f, 0.288f, 0.5f, -0.014f,
    18, 14, 0.863f, 0.5f, 35, 10,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008EB80 = {
    84.524f, 51.559643f, 5.008522f, 3.6904898f, 10.261124f,
    25137.0f, 6733.0f, 0.226f, 0.801f, 0.011f,
    29, 8, 0.85f, 0.262f, 23, 17,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate D_8008EBC0 = {
    32.5214f, 19.838055f, 3.3390145f, 2.4603264f, 6.840749f,
    25137.0f, 6733.0f, 0.0f, 0.137f, -0.011f,
    25, 10, 0.85f, 0.262f, 23, 10,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate D_8008EC00 = {
    23.2328f, 14.1720085f, 4.295503f, 3.1651075f, 8.800339f,
    25137.0f, 6733.0f, 0.0f, 0.0f, 0.011f,
    17, 15, 0.85f, 0.262f, 21, 14,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate D_8008EC40 = {
    24.1594f, 14.737234f, 3.5766876f, 2.635454f, 7.3276772f,
    25137.0f, 6733.0f, 0.0f, 0.0f, -0.0f,
    17, 15, 0.85f, 0.262f, 15, 7,
    0, 9, 6, 8, 7, 0, 6, 0,
};
static ParticleTemplate gCharacterSmokeTemplate = {
    8.3f, 17.9f, 25.4f, 29.5f, 57.969f,
    25137.0f, 6733.0f, 0.3f, 0.455f, -0.033f,
    22, 11, 0.85f, 0.308f, 36, 14,
    0, 12, 5, 7, 7, 0, 5, 1,
};
static ParticleTemplate D_8008ECC0 = {
    33.5836f, 20.485996f, 79.88129f, 58.859894f, 163.65541f,
    25137.0f, 6733.0f, 0.293f, 0.80100006f, -0.039f,
    15, 17, 0.14f, 0.091000006f, 20, 40,
    0, 9, 5, 7, 8, 0, 5, 0,
};
static ParticleTemplate D_8008ED00 = {
    22.6f, 13.786f, 57.969f, 42.714f, 118.763f,
    25137.0f, 6733.0f, 0.5f, 0.455f, 0.05f,
    32, 8, 0.85f, 0.262f, 47, 0,
    0, 9, 5, 7, 7, 0, 5, 0,
};
static ParticleTemplate D_8008ED40 = {
    28.656801f, 17.480648f, 65.56294f, 48.309536f, 134.32095f,
    25137.0f, 6733.0f, 0.03f, 0.735f, -0.02f,
    13, 19, 1.0f, 0.5f, 7, 14,
    0, 9, 5, 7, 7, 0, 5, 0,
};
/* Emitter infos. */
static EmitterInfo D_8008ED80 = {
    &D_8008E940, 72, D_8008DBA0, 4.0f, 72, 10, 0, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EDB8 = {
    &D_8008E980, 51, gAllOverEmitPoints, 5.0f, 13, 15, 10, 2, 2, { 0 },
    22.1f, 47.1f, 0.0f, 0.0f, -24.3f, -17.6f, 1,
};
static EmitterInfo D_8008EDF0 = {
    &D_8008E9C0, 51, gAllOverEmitPoints, 5.0f, 23, 17, 33, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EE28 = {
    &D_8008EA00, 51, gAllOverEmitPoints, 5.0f, 51, 12, 8, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EE60 = {
    &D_8008EA40, 72, D_8008DBA0, 0.919f, 2, 8, 2, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EE98 = {
    &D_8008EA80, 72, D_8008DBA0, 2.096f, 12, 22, 4, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EED0 = {
    &D_8008EAC0, 72, D_8008DBA0, 4.0f, 13, 13, 4, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EF08 = {
    &D_8008EB00, 72, D_8008DBA0, 5.0f, 72, 3, 0, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EF40 = {
    &D_8008EB40, 72, D_8008DBA0, 2.363f, 3, 10, 4, 2, 4, { 0 },
    28.8f, 65.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EF78 = {
    &D_8008EB80, 20, NULL, 5.5f, 7, 8, 8, 1, 2, { 0 },
    110.3f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EFB0 = {
    &D_8008EBC0, 20, NULL, 5.0f, 13, 10, 11, 1, 2, { 0 },
    30.8f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008EFE8 = {
    &D_8008EC00, 20, NULL, 5.0f, 14, 7, 14, 1, 2, { 0 },
    27.4f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4,
};
static EmitterInfo D_8008F020 = {
    &D_8008EC40, 20, NULL, 5.0f, 13, 12, 7, 1, 2, { 0 },
    37.7f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F058 = {
    &D_8008E980, 29, gTailEmitPoints, 5.0f, 13, 15, 10, 2, 1, { 0 },
    22.1f, 47.1f, 0.0f, 0.0f, -24.3f, -17.6f, 1,
};
static EmitterInfo D_8008F090 = {
    &D_8008E9C0, 29, gTailEmitPoints, 5.0f, 23, 17, 33, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F0C8 = {
    &D_8008EA00, 29, gTailEmitPoints, 5.0f, 51, 12, 8, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo gAllOverSmokeInfo = {
    &gCharacterSmokeTemplate, 51, gAllOverEmitPoints, 1.5f, 51, 18, 10, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo gTailSmokeInfo = {
    &gCharacterSmokeTemplate, 29, gTailEmitPoints, 1.5f, 29, 18, 10, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F170 = {
    &D_8008ECC0, 57, D_8008E020, 5.0f, 57, 3, 13, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F1A8 = {
    &D_8008ECC0, 14, D_8008E3B0, 5.0f, 14, 3, 13, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F1E0 = {
    &D_8008ECC0, 11, D_8008E490, 5.0f, 11, 3, 13, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F218 = {
    &D_8008ECC0, 11, D_8008E540, 5.0f, 11, 3, 13, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F250 = {
    &D_8008ED00, 72, D_8008E5F0, 4.0f, 72, 10, 0, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
static EmitterInfo D_8008F288 = {
    &D_8008ED40, 51, D_8008E600, 5.0f, 51, 12, 8, 1, 1, { 0 },
    1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0,
};
/* Info rows (EmitterTable.infos): n + 2 entries for each of the six owner LODs. */
static EmitterInfo *D_8008F2C0[] = {
    &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80,
    &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80, &D_8008ED80,
};
static EmitterInfo *gAllOverFireInfos[] = {
    &D_8008EDF0, &D_8008EE28, &D_8008EDB8, &D_8008EDF0, &D_8008EDF0, &D_8008EDF0,
    &D_8008EE28, &D_8008EDB8, &D_8008EDF0, &D_8008EDF0, &D_8008EDF0, &D_8008EE28,
    &D_8008EDB8, &D_8008EDF0, &D_8008EDF0, &D_8008EDF0, &D_8008EE28, &D_8008EDB8,
    &D_8008EDF0, &D_8008EDF0, &D_8008EDF0, &D_8008EE28, &D_8008EDB8, &D_8008EDF0,
    &D_8008EDF0, &D_8008EDF0, &D_8008EE28, &D_8008EDB8, &D_8008EDF0, &D_8008EDF0,
};
static EmitterInfo *gTailFireInfos[] = {
    &D_8008F090, &D_8008F0C8, &D_8008F058, &D_8008F090, &D_8008F090, &D_8008F090,
    &D_8008F0C8, &D_8008F058, &D_8008F090, &D_8008F090, &D_8008F090, &D_8008F0C8,
    &D_8008F058, &D_8008F090, &D_8008F090, &D_8008F090, &D_8008F0C8, &D_8008F058,
    &D_8008F090, &D_8008F090, &D_8008F090, &D_8008F0C8, &D_8008F058, &D_8008F090,
    &D_8008F090, &D_8008F090, &D_8008F0C8, &D_8008F058, &D_8008F090, &D_8008F090,
};
static EmitterInfo *D_8008F3E0[] = {
    &D_8008EE60, &D_8008EE98, &D_8008EF40, &D_8008EE60, &D_8008EE98, &D_8008EF40,
    &D_8008EE60, &D_8008EE98, &D_8008EF40, &D_8008EE60, &D_8008EE98, &D_8008EF40,
    &D_8008EE60, &D_8008EE98, &D_8008EF40, &D_8008EE60, &D_8008EE98, &D_8008EF40,
};
static EmitterInfo *D_8008F428[] = {
    &D_8008EF08, &D_8008EED0, &D_8008EED0, &D_8008EF08, &D_8008EED0, &D_8008EED0,
    &D_8008EF08, &D_8008EED0, &D_8008EED0, &D_8008EF08, &D_8008EED0, &D_8008EED0,
    &D_8008EF08, &D_8008EED0, &D_8008EED0, &D_8008EF08, &D_8008EED0, &D_8008EED0,
};
static EmitterInfo *D_8008F470[] = {
    &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0,
    &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0, &D_8008EFB0,
};
static EmitterInfo *D_8008F4A0[] = {
    &D_8008EFE8, &D_8008F020, &D_8008F020, &D_8008EFE8, &D_8008F020, &D_8008F020,
    &D_8008EFE8, &D_8008F020, &D_8008F020, &D_8008EFE8, &D_8008F020, &D_8008F020,
    &D_8008EFE8, &D_8008F020, &D_8008F020, &D_8008EFE8, &D_8008F020, &D_8008F020,
};
static EmitterInfo *D_8008F4E8[] = {
    &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78,
    &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78, &D_8008EF78,
};
static EmitterInfo *gAllOverSmokeInfos[] = {
    &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo,
    &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo,
    &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo,
    &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo,
    &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo, &gAllOverSmokeInfo,
};
static EmitterInfo *gTailSmokeInfos[] = {
    &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo,
    &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo,
    &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo,
    &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo,
    &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo, &gTailSmokeInfo,
};
static EmitterInfo *D_8008F608[] = {
    &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170,
    &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170, &D_8008F170,
};
static EmitterInfo *D_8008F638[] = {
    &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8,
    &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8, &D_8008F1A8,
};
static EmitterInfo *D_8008F668[] = {
    &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0,
    &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0, &D_8008F1E0,
};
static EmitterInfo *D_8008F698[] = {
    &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218,
    &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218, &D_8008F218,
};
static EmitterInfo *D_8008F6C8[] = {
    &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250,
    &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250, &D_8008F250,
};
static EmitterInfo *D_8008F6F8[] = {
    &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288,
    &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288, &D_8008F288,
};
/* Animation ids (EmitterIdList.ids) that select a row entry. */
static u8 D_8008F728[] = { 0x0F, 0x00, 0x01, 0x0B, 0xE9, 0x21, 0x36 };
static u8 D_8008F730[] = { 0x02 };
static u8 D_8008F734[] = { 0x32, 0x38, 0x39, 0x37, 0x69, 0xCF };
static u8 D_8008F73C[] = { 0x00, 0x04, 0x07, 0x09, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11 };
static u8 D_8008F748[] = { 0x02, 0x06 };
static u8 D_8008F74C[] = { 0x02 };
static EmitterIdList D_8008F750[] = { { 7, D_8008F728 }, { 1, D_8008F730 }, { 6, D_8008F734 } };
static EmitterIdList D_8008F768[] = { { 10, D_8008F73C } };
static EmitterIdList D_8008F770[] = { { 2, D_8008F748 } };
static EmitterIdList D_8008F778[] = { { 1, D_8008F74C } };
/* The emitter tables, by game/Particle/ActorEmitter/actoremitter.c's gActorEmitterTables. */
EmitterTable D_8008F780 = { D_8008F2C0, 0, NULL };
EmitterTable gAllOverFireEmitterTable = { gAllOverFireInfos, 3, D_8008F750 };
EmitterTable gTailFireEmitterTable = { gTailFireInfos, 3, D_8008F750 };
EmitterTable D_8008F7A4 = { D_8008F3E0, 1, D_8008F768 };
EmitterTable D_8008F7B0 = { D_8008F428, 1, D_8008F770 };
EmitterTable D_8008F7BC = { D_8008F470, 0, NULL };
EmitterTable D_8008F7C8 = { D_8008F4A0, 1, D_8008F778 };
EmitterTable D_8008F7D4 = { D_8008F4E8, 0, NULL };
EmitterTable gAllOverSmokeEmitterTable = { gAllOverSmokeInfos, 3, D_8008F750 };
EmitterTable gTailSmokeEmitterTable = { gTailSmokeInfos, 3, D_8008F750 };
EmitterTable gLavaBoulderImpactEmitterTable = { D_8008F608, 0, NULL };
EmitterTable D_8008F804 = { D_8008F638, 0, NULL };
EmitterTable gFireImpImpactEmitterTable = { D_8008F668, 0, NULL };
EmitterTable D_8008F81C = { D_8008F698, 0, NULL };
EmitterTable D_8008F828 = { D_8008F6C8, 0, NULL };
EmitterTable D_8008F834 = { D_8008F6F8, 0, NULL };
