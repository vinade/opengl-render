#ifndef ENGINE_CPP
#define ENGINE_CPP

#include <stdio.h>
#include <cstdlib>
#include <ctime>

#include "engine.hpp"

Engine::Engine()
{
    if (Engine::context == nullptr)
    {
        std::srand(std::time(nullptr));
        Engine::context = this;
    }
}

void Engine::start()
{
    Engine::context = this;
    this->render.start();
}

void Engine::set_preload(void (*fn)())
{
    this->render.set_preload(fn);
}

void Engine::set_render_handler(void (*fn)())
{
    this->render.set_render_handler(fn);
}

void Engine::set_cursor_mode(bool enabled)
{
    int value = enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
    glfwSetInputMode(RenderWindow::context->window, GLFW_CURSOR, value);
    this->cursor_enabled = enabled;
}

void Engine::switch_cursor_mode()
{
    this->set_cursor_mode(!this->cursor_enabled);
}

void Engine::set_keyboard_handler(void (*handler)(int key, int scancode, int action, int mods))
{
    this->keyboard_handler = handler;
}

void Engine::set_mouse_handler(void (*handler)(int button))
{
    this->mouse_handler = handler;
}

void Engine::init_multiple_keys_state()
{
    for (auto item : this->multiple_keys_allowed)
    {
        this->multiple_keys_state[item] = false;
    }
}

void Engine::cursor_position_callback(GLFWwindow *window, double x, double y)
{
    if (Engine::context->mouse_handler == nullptr)
    {
        return;
    }

    Engine::delta_mouse.x = x - Engine::mouse.x;
    Engine::delta_mouse.y = y - Engine::mouse.y;

    Engine::mouse.x = x;
    Engine::mouse.y = y;

    if (Engine::delta_mouse.x || Engine::delta_mouse.y)
    {
        (*Engine::context->mouse_handler)(0);
    }
}

void Engine::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (Engine::context->keyboard_handler != nullptr)
    {
        if ((action == GLFW_RELEASE) || (action == GLFW_PRESS))
        {
            if (AppUtils::has_item(Engine::context->multiple_keys_allowed, key))
            {
                Engine::context->multiple_keys_state[key] = (action == GLFW_PRESS);
            }
        }

        (*Engine::context->keyboard_handler)(key, scancode, action, mods);
    }
}

bool Engine::check_key(const int const_key, int key, int action)
{
    if (action == GLFW_RELEASE)
    {
        return false;
    }

    if (const_key == key)
    {
        return true;
    }

    if (!AppUtils::has_item(Engine::context->multiple_keys_allowed, const_key))
    {
        return false;
    }

    return Engine::context->multiple_keys_state[const_key];
}

glm::vec2 Engine::mouse;
glm::vec2 Engine::delta_mouse;
Engine *Engine::context = nullptr;

#endif