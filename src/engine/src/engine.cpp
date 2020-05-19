#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <stdio.h>
#include <cstdlib>
#include <ctime>

#include "engine.hpp"

Engine::Engine()
{
    std::srand(std::time(nullptr));
}

#endif