#ifndef TRACK_H
#define TRACK_H

#include "sound.h"

// Structure for a sound and playback information
typedef struct {
    sound_t *sound;
    int current_sample;
    int current_sample_iteration;
    int current_sample_point;
} track_t;

// Prototypes
track_t* track_new();
void track_play(track_t *track, sound_t *sound);
short track_advance(track_t* track);

#endif
