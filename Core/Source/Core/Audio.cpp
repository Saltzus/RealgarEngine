#include "Audio.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

namespace RED
{
    Audio::Audio()
    {
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            printf("Could not init engine! \n");
        }
    }
    
    Audio::~Audio()
    {
        ma_engine_uninit(&engine);
    }  

    void Audio::PlayAudio(const char* audioFile)
    {
        ma_result resultt;
        ma_sound sound;

        resultt = ma_sound_init_from_file(&engine, audioFile, 0, NULL, NULL, &sound);
        if (resultt != MA_SUCCESS) {
            printf("Could not init audio file\n");
        }

        ma_sound_start(&sound);
    }


} 

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    (void)pInput;
}