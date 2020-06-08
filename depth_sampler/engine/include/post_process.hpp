#ifndef POST_PROCESS_HPP
#define POST_PROCESS_HPP

#include "shader.hpp"
#include "frame_buffer.hpp"
#include <glm/glm.hpp>

class PostProcess
{
public:
    Shader *shader;
    FrameBuffer *fbo;
    glm::mat4 model_matrix;
    bool initialized = false;

    PostProcess(const std::string shader_name);
    void exec() { this->shader->exec(); };

    void setup();
    void fill();

    virtual void specific_setup() = 0;
    virtual void specific_fill() = 0;
};

/*********************************

    Ruído Gaussiano

**********************************/

class GaussianNoise : public PostProcess
{
private:
    float level = 0.5;

public:
    bool every_frame = true;
    GaussianNoise(const std::string shader_name) : PostProcess(shader_name){};

    void specific_setup();
    void specific_fill();

    void set_level(float level) { this->level = level; };
};

/*********************************

    Blur Gaussiano

**********************************/

class GaussianBlur : public PostProcess
{
private:
    int range = 1;

public:
    bool every_frame = true;
    GaussianBlur(const std::string shader_name) : PostProcess(shader_name){};

    void specific_setup();
    void specific_fill();

    void set_range(int range) { this->range = range; };
};

#endif