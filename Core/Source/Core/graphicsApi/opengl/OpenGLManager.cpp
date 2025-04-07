#include "OpenGlManager.h"
#include "../../Camera.h"

namespace Realgar::Opengl
{
    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
        alignas(16) float time;
    };


    OpenglTexture::OpenglTexture(const char* filePath)
    {
        // generate textures
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    
        unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
        int format = nrChannels == 4 ? GL_RGBA : GL_RGB;

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else std::cout << "Failed to load texture" << std::endl;
    
        stbi_image_free(data);
    }
    OpenglTexture::OpenglTexture(unsigned char* data, int format, int width, int height)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

        switch (format)
        {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
             break;       
        default:
            format = GL_RGBA;
            break;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    OpenglTexture::~OpenglTexture()
    {
        glDeleteTextures(1, &this->texture);
    }
    void OpenglTexture::Bind()
    {
        // Bind the texture to the specified texture unit
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    

    GLuint Opengl::UBO = 0;
    GLuint Opengl::FBO = 0;
    GLuint Opengl::RBO = 0;
    GLuint Opengl::framebufferTexture = 0;

    GLFWwindow* Opengl::window = nullptr;

    Opengl::Opengl(GLFWwindow* window)
    {
        this->window = window;

        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glGenTextures(1, &framebufferTexture);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        //

        glGenBuffers(1, &UBO);
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(UniformBufferObject), NULL, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, sizeof(UniformBufferObject));
    }
    Opengl::~Opengl()
    {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &framebufferTexture);
        glDeleteRenderbuffers(1, &RBO);

        //

        glDeleteBuffers(1, &UBO);
        
    }

    void Opengl::RescaleFramebuffer()
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    }

    int d = 0;
    OpenglRenderer::OpenglRenderer(std::vector<GLuint>& indices, std::vector<GLfloat>& vertices) : opengl(opengl)
    {
        glGenVertexArrays(1, &VAO);
        
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        // adds vertices and indices inside buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex color
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); 

        // Texture position attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    OpenglRenderer::~OpenglRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void OpenglRenderer::Render(Shader* shader, Camera* camera, glm::mat4 model) 
    {
        UniformBufferObject ubo;
        ubo.model = model;
        ubo.view = camera->view;
        ubo.proj = camera->projection;
        ubo.time = glfwGetTime();

        GLuint blockIndex = glGetUniformBlockIndex(shader->ID(), "UniformBufferObject");
        glUniformBlockBinding(shader->ID(), blockIndex, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, Opengl::UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(UniformBufferObject), &ubo);

        glUniform1i(glGetUniformLocation(shader->ID(), "texSampler"), 0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6 * 2, GL_UNSIGNED_INT, 0);
    }
}