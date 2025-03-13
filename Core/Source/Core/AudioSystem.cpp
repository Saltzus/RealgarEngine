#include "AudioSystem.h"

namespace Realgar
{
    ma_result AudioSystem::result;
    ma_engine AudioSystem::engine;

    void AudioSystem::Init()
    {
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            printf("Could not init engine! \n");
        }
    }

    void AudioSystem::Close()
    {
        ma_engine_uninit(&engine);
    }
}