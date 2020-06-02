#ifndef PUPPETEER_CPP
#define PUPPETEER_CPP

#include "puppeteer.hpp"

void Puppeteer::draw(Shader *shader)
{
    if (this->obj == nullptr)
    {
        return;
    }

    this->obj->update_model_matrix(this->position, this->rotation, this->scale);
    this->obj->draw(shader);
}

#endif