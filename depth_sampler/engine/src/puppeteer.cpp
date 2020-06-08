#ifndef PUPPETEER_CPP
#define PUPPETEER_CPP

#include "puppeteer.hpp"

void Puppeteer::draw(Shader *shader)
{
    if (this->obj == nullptr)
    {
        return;
    }

    this->obj->draw(shader, this->position, this->rotation, this->scale);
}

#endif