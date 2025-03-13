#pragma once
#include "component.h"
#include "../AudioSystem.h"
#include <miniaudio/miniaudio.h>

namespace Realgar::Components
{
    class AudioListenerComponent : public Component
    {
    public:
        AudioListenerComponent();
        ~AudioListenerComponent();

        static void registerAudioListenerComponent(lua_State* L);
        virtual void update(float deltaTime) override;

        glm::vec3* parentTranslation;
        glm::vec3* parentRotation;

        glm::vec3 translation = glm::vec3(0.f);
        glm::vec3 rotation = glm::vec3(0.f);

    private:
        ma_engine* engine = Realgar::AudioSystem::getEngine();
    };
    
} 
