#pragma once

#include <miniaudio/miniaudio.h>
#include <glm/common.hpp>
#include <iostream>

namespace Realgar
{
    class AudioSystem
    {
    public:
        static void Init();
        static void Close();

        static ma_engine* getEngine() { return &engine; };
    private:
        static ma_result result;
        static ma_engine engine;
    };
}