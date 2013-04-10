#include<stdio.h>
#include<linux/soundcard.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<sys/mman.h>

#define SOUND_FILE_PATH "/dev/dsp"

void sound_init();

int dsp_rate = 44100;
int sound_file;

void sound_init() {
    sound_file = open(SOUND_FILE_PATH, O_WRONLY);
    if (sound_file == -1) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }
    ioctl(sound_file, SOUND_PCM_WRITE_RATE, &dsp_rate);
}

int main() {
    sound_init();
}
