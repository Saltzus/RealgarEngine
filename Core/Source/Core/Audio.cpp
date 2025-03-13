#include "Audio.h"

namespace Realgar
{
    Audio::Audio(std::string path, bool spatialized) : path(path), spatialized(spatialized)
    {
        ma_uint32 flags = !spatialized ? MA_SOUND_FLAG_NO_SPATIALIZATION : 0;

        result = ma_sound_init_from_file(engine, path.c_str(), flags, NULL, NULL, &sound);
        if (result != MA_SUCCESS) {
            printf("Failed to load sound file.\n");
            ma_engine_uninit(engine);
        }
    }

    Audio::~Audio()
    {
        ma_sound_uninit(&sound);
    }
}