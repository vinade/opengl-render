#ifndef POST_PROCESS_CPP
#define POST_PROCESS_CPP

#include "post_process.hpp"
#include <cstdlib>

PostProcess::PostProcess(const std::string shader_name)
{
    this->shader = Shader::get_shader(shader_name, "fbo_color", SHADER_TYPE_POST_PROCESSING);
}

void GaussianNoise::set_noise_level(float noise_level)
{
    this->noise_level = noise_level;
}

void GaussianNoise::setup()
{
    this->shader->setup("u_Texture", DATA_TYPE_INT);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);

    this->shader->setup("u_Seed", DATA_TYPE_FLOAT);
    this->shader->setup("u_Level", DATA_TYPE_FLOAT);
}

void GaussianNoise::fill()
{
    /* padrão */
    this->shader->fill("u_Texture", 1);
    this->shader->fill("u_Model", this->model_matrix);

    /* próprio */
    float seed = 0.0;
    if (this->every_frame)
    {
        seed = float(std::rand() % 1000);
    }

    this->shader->fill("u_Seed", seed);
    this->shader->fill("u_Level", this->noise_level);
}

#endif