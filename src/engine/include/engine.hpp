#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "render_window.hpp"

class Engine
{
private:
    const std::vector<int> multiple_keys_allowed = {
        GLFW_KEY_A,
        GLFW_KEY_D,
        GLFW_KEY_S,
        GLFW_KEY_W,
        GLFW_KEY_Q,
        GLFW_KEY_E,
    };
    std::unordered_map<int, bool> multiple_keys_state;
    void (*keyboard_handler)(int key, int scancode, int action, int mods);
    void (*mouse_handler)(int button);

public:
    bool cursor_enabled = true;
    RenderWindow render;
    static glm::vec2 mouse;
    static glm::vec2 delta_mouse;
    static Engine *context;

    Engine();

    void start();
    void set_preload(void (*fn)());
    void set_render_handler(void (*fn)());
    void set_cursor_mode(bool enabled);
    void switch_cursor_mode();
    void set_keyboard_handler(void (*handler)(int key, int scancode, int action, int mods));
    void set_mouse_handler(void (*handler)(int button));
    void init_multiple_keys_state();

    static bool check_key(const int const_key, int key, int action);
    static void cursor_position_callback(GLFWwindow *window, double x, double y);
    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif