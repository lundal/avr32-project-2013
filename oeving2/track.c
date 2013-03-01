#include "track.h"

#include <stdlib.h>

// Initialize tracks
void tracks_init() {
    // Create tracks
    int16_t i;
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
int32_t track_advance(track_t *track) {
    // Default to silence
    int32_t data = 0;
    
    // Dereference some vars
    sound_t *sound = track->sound;
    int16_t current_sample = track->current_sample;
    
    // If it has a sound
    if (sound != NULL) {
        
        // Get current sample point (assume it exists)
        data = (int32_t)sound->samples[current_sample]->points[track->current_sample_point >> 5];
        
        // Adjust volume
        data = data * sound->sample_vol[current_sample];
        
        // Calculate jump
        int16_t pitch = sound->sample_pitch[current_sample];
        int16_t jump = (pitch > 0) ? 32 << pitch : 32 >> -pitch;
        //int16_t jump = (32 << pitch) >> -pitch;
        
        // Go to next sample point
        track->current_sample_point += jump;
        
        // If this was last sample: Go to next iteration
        if (track->current_sample_point >= 32 * sound->samples[current_sample]->n_points) {
            track->current_sample_point = 0;
            track->current_sample_iteration++;
            
            // If this was last iteration: Go to next sample
            if (track->current_sample_iteration >= sound->sample_reps[current_sample]) {
                track->current_sample_iteration = 0;
                track->current_sample++;
                
                // If this was last sample: Stop
                if (track->current_sample >= sound->n_samples) {
                    track->current_sample = 0;
                    track->sound = NULL;
                }
            } 
        }
        
    }
    
    return data;
}
/*
// Gets the current sample point (100 times as loud, reduced in interrupt) and advances to the next
int32_t track_advance(track_t *track) {
    // Default to silence
    int32_t data = 0;
    
    // If it has a sound
    if (track->sound != NULL) {
        
        // Get current sample point (assume it exists)
        data = (int32_t)track->sound->samples[track->current_sample]->points[track->current_sample_point / 32];
        
        // Adjust volume
        data = data * track->sound->sample_vol[track->current_sample];
        
        // Calculate jump
        int16_t pitch = track->sound->sample_pitch[track->current_sample];
        int16_t jump = (pitch > 0) ? 32 << pitch : 32 >> -pitch;
        
        // Go to next sample point
        track->current_sample_point += jump;
        
        // If this was last sample: Go to next iteration
        if (track->current_sample_point >= 32 * track->sound->samples[track->current_sample]->n_points) {
            track->current_sample_point = 0;
            track->current_sample_iteration++;
            
            // If this was last iteration: Go to next sample
            if (track->current_sample_iteration >= track->sound->sample_reps[track->current_sample]) {
                track->current_sample_iteration = 0;
                track->current_sample++;
                
                // If this was last sample: Stop
                if (track->current_sample >= track->sound->n_samples) {
                    track->current_sample = 0;
                    track->sound = NULL;
                }
            } 
        }
        
    }
    
    return data;
}
*/
