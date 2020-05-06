#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <GL/glew.h>
#include "debug_flags.hpp"
#include "data_types.hpp"

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "./"
#endif

#define SHADERS_FOLDER "./shaders/"
#define SHADERS_VERTEX_EXT std::string(".vertexshader")
#define SHADERS_FRAGMENT_EXT std::string(".fragmentshader")
#define SHADERS_DEFAULT_SHADER std::string("std")

#define check_opengl_errors() Shader::list_opengl_errors(__FILE__, __LINE__)

enum ShaderType
{
    SHADER_TYPE_SCENE,
    SHADER_TYPE_POST_PROCESSING,
    SHADER_TYPE_OTHER
};

class BaseUniformItem
{
public:
    int id;
    UniformType type;

    BaseUniformItem(){};
    virtual void set(){};
    virtual const std::type_info &getTID() { return typeid(void); };
};

template <typename T>
class UniformItem : public BaseUniformItem
{

private:
    T *item_ref;
    T item;

public:
    UniformItem(std::string name, unsigned int program_id);
    // ~UniformItem();

    void set(T *item);
    void set(const T &item);
    T *get();
    const std::type_info &getTID();
};

class Shader
{

private:
    unsigned int loaded;
    unsigned int program_id;
    std::unordered_map<std::string, BaseUniformItem *> items;

    static std::unordered_map<std::string, Shader *> loaded_shaders;
    static const std::string shaders_folder;

    unsigned int compile(std::string file_path, unsigned int type);
    void init(ShaderType shader_type);
    void init(std::string shader_name, ShaderType shader_type);
    std::string name;

public:
    bool use_ligths = true;
    bool use_mvp = true;
    bool use_materials = true;

    Shader();
    Shader(ShaderType shader_type);
    Shader(std::string shader_name);
    Shader(std::string shader_name, ShaderType shader_type);
    ~Shader();

    void setup(std::string name, UniformType type);
    template <class T>
    void fill(std::string name, T *value);
    template <class T>
    void fill(std::string name, const T &value);

    void load(std::string shader_name);
    void exec();
    void set_shader_type(ShaderType shader_type);

    static Shader *get_shader(std::string shader_name);
    static Shader *get_shader(std::string shader_name, ShaderType shader_type);
    static void stop_all();
    static void list_opengl_errors(const char *file, int line);
};

template <typename T>
void Shader::fill(std::string name, T *value)
{
#ifdef DEBUG_MODE_COMPILE
    if (this->items.find(name) == this->items.end())
    {
        std::cerr << "[Shader] Fill do Uniform " << name.c_str() << " chamado antes do setup." << std::endl;
        std::cerr << "\tshader: " << this->name << std::endl;
        exit(1);
    }
#endif

    UniformItem<T> *item = (UniformItem<T> *)this->items[name];
    item->set(value);
}

template <typename T>
void Shader::fill(std::string name, const T &value)
{
    this->fill(name, (T *)&value);
}

template <typename T>
UniformItem<T>::UniformItem(std::string name, unsigned int program_id)
{
    this->id = glGetUniformLocation(program_id, name.c_str());

    if (this->id == -1)
    {
        std::cerr << "[Shader] Uniform " << name.c_str() << " não utilizado pelo shader" << std::endl;
    }
}

template <typename T>
void UniformItem<T>::set(T *_item)
{
    this->item = *_item;
    this->item_ref = &this->item;
}

template <typename T>
T *UniformItem<T>::get()
{
    return this->item_ref;
}

template <typename T>
const std::type_info &UniformItem<T>::getTID()
{
    return typeid(T);
}

#endif
