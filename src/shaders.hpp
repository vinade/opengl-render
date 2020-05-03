#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include "data_types.hpp"

#define SHADERS_FOLDER std::string("./shaders/")
#define SHADERS_VERTEX_EXT std::string(".vertexshader")
#define SHADERS_FRAGMENT_EXT std::string(".fragmentshader")

#define check_opengl_errors() Shader::list_opengl_errors(__FILE__, __LINE__)

class BaseUniformItem
{
private:
    void *item;

public:
    int id;

    BaseUniformItem(){};
    virtual void set(int *item){};
    virtual void set(float *item){};
    virtual void set(glm::vec2 *item){};
    virtual void set(glm::vec3 *item){};
    virtual void set(glm::vec4 *item){};
    virtual void set(glm::mat4 *item){};
    virtual const std::type_info &getTID() { return typeid(void); };
};

template <typename T>
class UniformItem : public BaseUniformItem
{

private:
    T *item;

public:
    int id;

    UniformItem(std::string name, unsigned int program_id);
    // ~UniformItem();

    void set(T *item);
    T *get();
    const std::type_info &getTID();
};

class Shader
{

private:
    unsigned int loaded;
    unsigned int program_id;
    std::unordered_map<std::string, BaseUniformItem *> items;

    unsigned int compile(std::string file_path, unsigned int type);

public:
    Shader();
    Shader(std::string shader_name);
    ~Shader();

    void setup(std::string name, UniformType type);
    template <class T>
    void fill(std::string name, T *value);
    template <class T>
    void fill(std::string name, const T &value);

    void load(std::string shader_name);
    void exec();

    static void stop_all();
    static void list_opengl_errors(const char *file, int line);
};

#endif
