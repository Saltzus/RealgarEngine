#include "Core/Core.h"

#include <glm/common.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;





int main(int argc, char** argv)
{
    RED::Window window("RED - Test", 800, 600);
    RED::Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.f,0.f,-4.f));
    RED::Shader shader("Game/Resources/Shaders/default.vert", "Game/Resources/Shaders/default.frag");

    RED::Audio audio;
    audio.PlayAudio("Game/Resources/Audio/BIG-Trouble.ogg");

    int sus = 0;
    RED::Sprite sprite1;

    while (!glfwWindowShouldClose(window))
    {
        sus ++;


        camera.Inputs(window, 1);
        camera.updateMatrix(45.0f, 0.1f, 10000.0f, false);

        sprite1.Render(&shader, &camera);
        sprite1.scale = glm::vec3(4,4,4);
        sprite1.rotation.y += 0.1f;
        sprite1.scale = glm::vec3(4,4 + glm::sin(sus/4) / 4,4);
        sprite1.translation.y = glm::sin(sus/4) / 3;



        RED::Input::processInput(window);
        RED::Display(window);
    }

    glfwTerminate();
    return 0;
}

