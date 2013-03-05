#include "midi.h"

#include <stdlib.h>
#include <stdint.h>

#include "tones.h"

// Playback data
midi_channel_t *midi_channels;
const midi_soundtrack_t *midi_soundtrack;
int32_t midi_current_event;
int32_t midi_time_passed;

// Initializes the midi player
void midi_init() {
    // Allocate memory to channels
    midi_channels = (midi_channel_t*)malloc(sizeof(midi_channel_t) * MIDI_CHANNELS);
    
    // Create channels
    int i;
    for (i = 0; i < MIDI_CHANNELS; i++) {
        midi_channels[i] = (midi_channel_t){silence, 0, 0, 0, 0};
    }
}

// Sets the soundtrack to play
void midi_play(const midi_soundtrack_t *soundtrack) {
    midi_soundtrack = soundtrack;
    midi_current_event = 0;
    midi_time_passed = 0;
    
    // Clear channels
    int i;
    for (i = 0; i < MIDI_CHANNELS; i++) {
        midi_channels[i].sample = silence;
    }
    
    midi_channels[0].sample = tones[4];
    midi_channels[0].volume = 100;
}

// Advances the midi player one tick and gets output
int16_t midi_tick() {
    // Process event(s)
    if (midi_time_passed >= midi_soundtrack->events[midi_current_event].delta_time) {
        // Get event
        midi_event_t event = midi_soundtrack->events[midi_current_event];
        
        // Update channel sample
        midi_channels[event.channel].sample = tones[event.tone % 12];
        midi_channels[event.channel].sample_point = 0;
        
        // Update channel pitch (samples are pitch 5)
        int32_t pitch = event.tone / 12 - 5;
        midi_channels[event.channel].pitch_up = (pitch > 0) ? pitch : 0;
        midi_channels[event.channel].pitch_down = (pitch < 0) ? -pitch : 0;
        
        // Update channel volume
        midi_channels[event.channel].volume = event.volume;
        
        // Next event (loop)
        midi_current_event = (midi_current_event + 1) % midi_soundtrack->num_events;
        midi_time_passed = 0;
    }
    
    // Update time
    midi_time_passed++;
    
    // Default output
    int32_t data = 0;
    
    // Advance channels
    int i;
    for (i = 0; i < MIDI_CHANNELS; i++) {
        data += midi_channel_advance(&midi_channels[i]);
    }
    
    // Normalize volume (divide by 128 which is max vol and number of channels)
    data = ((data >> 7) / MIDI_CHANNELS);
    
    return (int16_t)data;
}

// Advances a midi channel one tick and gets output
int32_t midi_channel_advance(midi_channel_t *channel) {
    sample_t *sample = channel->sample;
    
    // Get sample point
    int32_t data = (int32_t)sample->points[channel->sample_point >> MIDI_PITCH_SHIFT];
    
    // Adjust for volume
    data = data * channel->volume;
    
    // Adjust advancement for pitch
    int32_t advance = 1 << MIDI_PITCH_SHIFT;
    advance = (advance << channel->pitch_up) >> channel->pitch_down;
    
    // Advance to next sample point (loop)
    channel->sample_point = (channel->sample_point + advance) % (sample->n_points << MIDI_PITCH_SHIFT);
    
    return data;
}

