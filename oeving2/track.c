#include "track.h"
#include <stdlib.h>

// Initialize tracks
void tracks_init() {
    // Create tracks
    int i;
    for (i = 0; i < TRACKS_TOTAL; i++) {
        tracks[i] = track_new();
    }
}

// Creates and initializes a new track
track_t* track_new() {
    // Allocate memory
    track_t* track = (track_t*)malloc(sizeof(track_t));
    
    // Initialize with no sound
    track_play(track, NULL);
    
    return track;
}

// Starts a new sound on a track
void track_play(track_t *track, sound_t *sound) {
    // Set sound
    track->sound = sound;
    
    // Reset playback
    track->current_sample = 0;
    track->current_sample_iteration = 0;
    track->current_sample_point = 0;
}

// Gets the current sample point (100 times as loud, reduced in interrupt) and advances to the next
int track_advance(track_t *track) {
    // Default to silence
    int data = 0;
    
    // If it has a sound
    if (track->sound != NULL) {
        
        // Get current sample point (assume it exists)
        data = (int)track->sound->samples[track->current_sample]->points[track->current_sample_point];
        
        // Adjust volume
        data = data * track->sound->sample_vol[track->current_sample];
        
        // Go to next sample point
        track->current_sample_point++;
        
        // If this was last sample: Go to next iteration
        if (track->current_sample_point == track->sound->samples[track->current_sample]->n_points) {
            track->current_sample_point = 0;
            track->current_sample_iteration++;
            
            // If this was last iteration: Go to next sample
            if (track->current_sample_iteration == track->sound->sample_reps[track->current_sample]) {
                track->current_sample_iteration = 0;
                track->current_sample++;
                
                // If this was last sample: Stop
                if (track->current_sample == track->sound->n_samples) {
                    track->current_sample = 0;
                    track->sound = NULL;
                }
            } 
        }
        
    }
    
    return data;
}
