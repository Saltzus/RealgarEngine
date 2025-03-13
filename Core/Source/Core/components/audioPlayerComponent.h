#pragma once
#include "component.h"
#include "../AudioSystem.h"
#include <miniaudio/miniaudio.h>


namespace Realgar::Components
{
    class AudioPlayerComponent : public Component
    {
    public:
        AudioPlayerComponent();
        ~AudioPlayerComponent();

        static void registerAudioPlayerComponent(lua_State* L);
        virtual void update(float deltaTime) override;
        virtual void init() override;

        glm::vec3* parentTranslation;
        glm::vec3 translation = glm::vec3(0.f);
        ma_sound* sound;
    private:
        ma_engine* engine = Realgar::AudioSystem::getEngine();

        ma_result result;
    };
    
} 
