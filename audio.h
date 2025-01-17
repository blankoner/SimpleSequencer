#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_stdinc.h>

struct Audio
{
public:
    int frequency;
    Uint16 format;
    int channels;
    int buffer;
};

#endif // AUDIO_H
