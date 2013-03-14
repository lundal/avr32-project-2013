#include "wav2c.h"

#include "buffer.h"

#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
    return 0;
}

void print_wave_as_byte_array(wave_t *wav, char *name, int target_rate) {
    // Simplify access
    int *points = wav->sample_points;
    
    printf("{\n");
    printf(".points = {");
    
    // Repeat until there are no more sample points (Might loose 1 sample point)
    int num_points = 0;
    double progress = 0;
    while (progress < (double)wav->num_points - 1) {
        // Calculate whole and float parts
        int whole = (int)progress
        double part = progress - (double)whole;
        
        // Interpolate between sample points
        int value = (int)((double)points[whole] * (1 - part) + (double)points[whole + 1] * part);
        
        // Reduce to one byte
        value = value >> (8 * (wav->bytes_per_point - 1));
        
        // Print value
        printf("%d", value);
        
        // Add separator
        if (progress > 0) printf(",");
        
        // Advance
        progress += (double)wav->sample_rate / (double)target_rate;
        
        // Count points
        num_points++;
    }
    
    printf("},\n");
    printf(".n_points = %d\n", num_points);
    printf("};");
}

wave_t* next_wave(buffer_t *buffer) {
    // Skip to format chunk;
    buffer->position = 16;
    
    // Get some data!
    int format_size = buffer_next_int_le(buffer, 4);
    int audio_format = buffer_next_int_le(buffer, 2); // PCM = 1
    int num_channels = buffer_next_int_le(buffer, 2); // Mono = 1, Stereo = 2
    int sample_rate = buffer_next_int_le(buffer, 4);
    int byte_rate = buffer_next_int_le(buffer, 4); // sample_rate * num_channels * bits_per_sample / 8 
    int block_align = buffer_next_int_le(buffer, 2); // num_channels * bits_per_sample / 8
    int bits_per_sample = buffer_next_int_le(buffer, 2);
    
    // Calculate bytes per sample
    int bytes_per_sample = bits_per_sample / 8;
    
    // Skip to next chunk
    buffer->position += format_size - 16;
    
    // Skip chunks until data (0x64617461 = "data")
    while (buffer_next_int(buffer, 4) != 0x64617461) {
        int chunk_size = buffer_next_int_le(buffer, 4);
        buffer->position += chunk_size;
    }
    
    // Read data size
    int data_size = buffer_next_int_le(buffer, 4);
    
    // We are now at the sample points!
    
    // Allocate memory and fill info
    wave_t *wav = (wave_t*)malloc(sizeof(wave_t));
    wav->num_points = data_size / (num_channels * bytes_per_sample);
    wav->bytes_per_point = bytes_per_sample;
    wav->sample_rate = sample_rate;
    wav->sample_points = (int*)malloc(sizeof(int) * wav->num_points);
    
    // Used to track progress
    int current_point = 0;
    
    // Iterate through the sample points
    while (buffer->position < buffer->length) {
        // Store sample point from first channel
        wav->sample_points[current_point++] = buffer_next_int_le(buffer, bytes_per_sample);
        
        // Skip other channels
        buffer->position += 2 * (num_channels - 1);
    }
    
    return wav;
}

