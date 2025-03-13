#pragma once

#include "AudioSystem.h"
#include <string>

namespace Realgar
{
    class Audio
    {
    public:
        Audio(std::string path, bool spatialized);
        ~Audio();

        std::string path;
        bool spatialized;

        ma_sound sound;

    private:
        ma_engine* engine = Realgar::AudioSystem::getEngine();
        ma_result result;
    };
}