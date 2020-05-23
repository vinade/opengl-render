#ifndef POST_PROCESS_HPP
#define POST_PROCESS_HPP

#include "shader.hpp"
#include <glm/glm.hpp>

class PostProcess
{
public:
    Shader *shader;
    glm::mat4 model_matrix;
    bool initialized = false;

    PostProcess(const std::string shader_name);
    void exec() { this->shader->exec(); };

    virtual void setup() = 0;
    virtual void fill() = 0;
};

class GaussianNoise : public PostProcess
{
private:
    float noise_level = 0.5;

public:
    bool every_frame = true;
    GaussianNoise(const std::string shader_name) : PostProcess(shader_name){};

    void setup();
    void fill();

    void set_noise_level(float noise_level);
};

#endif