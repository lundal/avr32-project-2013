#ifndef TRACK_H
#define TRACK_H

#include <inttypes.h>

#include "sound.h"

#define TRACKS_TOTAL 8

// Structure for a sound and playback information
typedef struct {
    sound_t *sound;
    int16_t current_sample;
    int16_t current_sample_iteration;
    int16_t current_sample_point;
} track_t;

track_t **tracks;

// Prototypes
void tracks_init();
track_t* track_new();
void track_play(track_t *track, sound_t *sound);
int32_t track_advance(track_t* track);

#endif
