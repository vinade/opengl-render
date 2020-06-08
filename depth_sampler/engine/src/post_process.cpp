#ifndef POST_PROCESS_CPP
#define POST_PROCESS_CPP

#include "post_process.hpp"
#include "tile_mesh.hpp"
#include <cstdlib>

PostProcess::PostProcess(const std::string shader_name)
{
    this->shader = Shader::get_shader(shader_name, TILE_MESH_SIMPLE_SHADER, SHADER_TYPE_POST_PROCESSING);
}

void PostProcess::setup()
{
    if (this->initialized)
    {
        return;
    }

    this->fbo = new FrameBuffer();
    this->fbo->set();

    this->shader->setup("u_Texture", DATA_TYPE_INT);
    this->shader->setup("u_Model", DATA_TYPE_MAT4);
    this->specific_setup();
    this->initialized = true;
}

void PostProcess::fill()
{
    this->shader->fill("u_Texture", 1);
    this->shader->fill("u_Model", this->model_matrix);
    this->specific_fill();
}

/*********************************

    Ruído Gaussiano

**********************************/

void GaussianNoise::specific_setup()
{
    this->shader->setup("u_Seed", DATA_TYPE_FLOAT);
    this->shader->setup("u_Level", DATA_TYPE_FLOAT);
}

void GaussianNoise::specific_fill()
{
    float seed = 0.0;
    if (this->every_frame)
    {
        seed = float(std::rand() % 1000);
    }

    this->shader->fill("u_Seed", seed);
    this->shader->fill("u_Level", this->level);
}

/*********************************

    Blur Gaussiano

**********************************/
void GaussianBlur::specific_setup()
{
    this->shader->setup("u_Range", DATA_TYPE_INT);
}

void GaussianBlur::specific_fill()
{
    this->shader->fill("u_Range", this->range);
}

#endif