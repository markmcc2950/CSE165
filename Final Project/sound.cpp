#include "sound.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <MMSystem.h>

sound::sound(const char* sound_filename) {
	filename = sound_filename;
}

void sound::play() {
	PlaySound(TEXT(filename), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);				// SND_ASYNC plays while the game is running
}
void sound::stop() {
	PlaySound(NULL, 0, 0);
}

void sound::playOnce() {
	PlaySound(TEXT(filename), NULL, SND_FILENAME | SND_SYNC | 0);						// SND_SYNC plays and pauses the game while the sound is playing
}

sound::~sound() {
	stop();
}
