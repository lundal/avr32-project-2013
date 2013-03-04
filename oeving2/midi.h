#ifndef MIDI_H
#define MIDI_H

#define MIDI_PITCH_SHIFT 5 // Shift playback numbers up to allow pitch reduction (by up to this amount)
#define MIDI_CHANNELS 8

#include "sample.h"

typedef struct {
    sample_t *sample;
    int32_t sample_point;
    int8_t pitch_up;
    int8_t pitch_down;
    int8_t volume;
} midi_channel_t;

typedef struct {
    int32_t delta_time;
    int8_t channel;
    int8_t tone;
    int8_t volume;
} midi_event_t;

typedef struct {
    int32_t num_events;
    midi_event_t events[];
} midi_soundtrack_t;

// Prototypes
void midi_init();
void midi_play(midi_soundtrack_t *soundtrack);
int16_t midi_tick();
int32_t midi_channel_advance(midi_channel_t *channel);

#endif
