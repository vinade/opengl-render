#ifndef SHADER_CPP
#define SHADER_CPP

#include <algorithm>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cmake_params.hpp"
#include "shader.hpp"
#include "render_window.hpp"

Shader::Shader()
{
    this->init(SHADER_TYPE_DEFAULT);
}

Shader::Shader(ShaderType shader_type)
{
    this->init(shader_type);
}

Shader::Shader(std::string shader_name)
{
    this->init(shader_name, SHADER_TYPE_DEFAULT);
}

Shader::Shader(std::string shader_name, ShaderType shader_type)
{
    this->init(shader_name, shader_type);
}

Shader::Shader(std::string fragment_shader_name, std::string vertex_shader_name)
{
    this->init(fragment_shader_name, vertex_shader_name, SHADER_TYPE_DEFAULT);
}

Shader::Shader(std::string fragment_shader_name, std::string vertex_shader_name, ShaderType shader_type)
{
    this->init(fragment_shader_name, vertex_shader_name, shader_type);
}

void Shader::init(ShaderType shader_type)
{
    this->loaded = false;

    if (shader_type == SHADER_TYPE_DEFAULT)
    {
        shader_type = SHADER_TYPE_SCENE;
    }

    this->set_shader_type(shader_type);
}

void Shader::init(std::string shader_name, ShaderType shader_type)
{
    this->init(shader_name, shader_name, SHADER_TYPE_DEFAULT);
}

void Shader::init(std::string fragment_shader_name, std::string vertex_shader_name, ShaderType shader_type)
{
    this->init(shader_type);
    this->name = fragment_shader_name + ", " + vertex_shader_name;
    this->frag_name = fragment_shader_name;
    this->vert_name = vertex_shader_name;

    if (!RenderWindow::is_render_thread())
    {
        RenderWindow::context->to_setup(this);
        return;
    }

    this->load();
}

Shader::~Shader()
{
    if (this->loaded)
    {
        glDeleteProgram(this->program_id);
    }
}

Shader *Shader::get_shader(std::string shader_name)
{
    return Shader::get_shader(shader_name, shader_name, SHADER_TYPE_DEFAULT);
}

Shader *Shader::get_shader(std::string shader_name, ShaderType shader_type)
{
    return Shader::get_shader(shader_name, shader_name, shader_type);
}

Shader *Shader::get_shader(std::string fragment_shader_name, std::string vertex_shader_name)
{
    return Shader::get_shader(fragment_shader_name, vertex_shader_name, SHADER_TYPE_DEFAULT);
}

Shader *Shader::get_shader(std::string fragment_shader_name, std::string vertex_shader_name, ShaderType shader_type)
{
    std::string id = Shader::get_id(fragment_shader_name, vertex_shader_name);
    if (Shader::loaded_shaders.find(id) == Shader::loaded_shaders.end())
    {
        /*
            Issue:
            pode gerar dois ou mais shaders que contenham a combinacão frag_name, vert_name diferentes
            mas usem os mesmos program shaders (por fallback)
        */
        return new Shader(fragment_shader_name, vertex_shader_name, shader_type);
    }

    std::cerr << "[Shader] load de shader cacheado: " << id.c_str() << std::endl;

    return Shader::loaded_shaders[id]; // Ignora shader_type.
}

void Shader::set_shader_type(ShaderType shader_type)
{
    switch (shader_type)
    {
    case SHADER_TYPE_OTHER:
        this->use_lights = false;
        this->use_mvp = false;
        this->use_materials = false;
        break;
    case SHADER_TYPE_POST_PROCESSING:
        this->use_lights = false;
        this->use_mvp = false;
        this->use_materials = false;
        break;
    case SHADER_TYPE_SCENE:
    default:
        this->use_lights = true;
        this->use_mvp = true;
        this->use_materials = true;
        break;
    }
}

void Shader::setup(std::string name, UniformType type)
{
    BaseUniformItem *selected_type;

    if (this->items.find(name) != this->items.end())
    {
        if (this->items[name]->type != type)
        {
            std::cerr << "[Shader] Uniform definido duas vezes, com tipos diferentes." << std::endl;
            std::cerr << "\tuniform: " << name.c_str() << std::endl;
            std::cerr << "\tLEMBRETE: talvez o BaseUniformItem->type não esteja guardando o dado." << std::endl;
            exit(1);
        }
        return; // Ignora, caso o uniform já tenha sido definido
    }

    switch (type)
    {
    case DATA_TYPE_FLOAT:
        selected_type = new UniformItem<float>(name, this->program_id);
        break;
    case DATA_TYPE_INT:
        selected_type = new UniformItem<int>(name, this->program_id);
        break;
    case DATA_TYPE_VEC2:
        selected_type = new UniformItem<glm::vec2>(name, this->program_id);
        break;
    case DATA_TYPE_VEC3:
        selected_type = new UniformItem<glm::vec3>(name, this->program_id);
        break;
    case DATA_TYPE_VEC4:
        selected_type = new UniformItem<glm::vec4>(name, this->program_id);
        break;
    case DATA_TYPE_MAT4:
        selected_type = new UniformItem<glm::mat4>(name, this->program_id);
        break;
    default:
        std::cerr << "[Shader] Tipo inválido de dado no setup";
        exit(1);
    }

    selected_type->type = type;
    this->items[name] = selected_type;
}

unsigned int Shader::compile(std::string file_path, unsigned int type)
{
    unsigned int shader_id = glCreateShader(type);

    std::string shader_code;
    std::ifstream shader_stream(file_path.c_str(), std::ios::in);
    if (shader_stream.is_open())
    {
        std::stringstream sstr;
        sstr << shader_stream.rdbuf();
        shader_code = sstr.str();
        shader_stream.close();
    }
    else
    {
        std::cerr << "Impossible to open " << file_path << ". Are you in the right directory?" << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }

    char const *source_pointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &source_pointer, NULL);
    glCompileShader(shader_id);

    int compile_status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE)
    {
        int log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log_message(log_length + 1);
        glGetShaderInfoLog(shader_id, log_length, NULL, &log_message[0]);

        std::cerr << "[Shader] Failed to compile "
                  << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment")
                  << " shader ("
                  << file_path
                  << ")."
                  << std::endl;
        std::cerr << "\t" << &log_message[0] << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

void Shader::load()
{

    int status;
    std::string vertex_file_path = Shader::shaders_folder + this->vert_name + SHADERS_VERTEX_EXT;
    std::string fragment_file_path = Shader::shaders_folder + this->frag_name + SHADERS_FRAGMENT_EXT;

    std::cerr << "[Shader] carregamento de shader: " << this->name.c_str() << std::endl;

    {
        bool vert_found;
        bool frag_found;

        std::ifstream vertex_shader_file(vertex_file_path.c_str());
        std::ifstream fragment_shader_file(fragment_file_path.c_str());

        vert_found = vertex_shader_file.good();
        frag_found = fragment_shader_file.good();

        vertex_shader_file.close(); // não é necessário, pois fecha ao sair do escopo
        fragment_shader_file.close();

        if (!vert_found && !frag_found)
        {
            std::cerr << "[Shader] Shader não encontrado: " << this->name.c_str() << std::endl;
            std::cerr << "\tpath: " << vertex_file_path.c_str() << std::endl;
            exit(1);
        }

        if (!vert_found)
        {
            vertex_file_path = Shader::shaders_folder + SHADERS_DEFAULT_SHADER + SHADERS_VERTEX_EXT;
        }

        if (!frag_found)
        {
            fragment_file_path = Shader::shaders_folder + SHADERS_DEFAULT_SHADER + SHADERS_FRAGMENT_EXT;
        }
    }

    unsigned int vertex_shader_id = this->compile(vertex_file_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader_id = this->compile(fragment_file_path, GL_FRAGMENT_SHADER);

    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, NULL, &log_message[0]);

        std::cerr << "[Shader] Failed to link shader program" << std::endl;
        std::cerr << "\t" << &log_message[0] << std::endl;
        glDeleteProgram(program_id);
        return;
    }

    glValidateProgram(program_id);
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, NULL, &log_message[0]);

        std::cerr << "[Shader] Failed to validate shader program" << std::endl;
        std::cerr << "\t" << &log_message[0] << std::endl;
        glDeleteProgram(program_id);
        return;
    }

    // glDetachShader(program_id, vertex_shader_id);
    // glDetachShader(program_id, fragment_shader_id);

    this->loaded = true;
    this->program_id = program_id;

    Shader::loaded_shaders[this->name] = this;
}

void Shader::exec()
{

    if (!this->loaded)
    {
        std::cerr << "[Shader] Shader executado sem ser carregado\n";
        exit(1);
    }

    glUseProgram(this->program_id);
    for (auto item_map : this->items)
    {
        BaseUniformItem *base_item = item_map.second;
        const std::type_info &tid = base_item->getTID();

        if (tid == typeid(int))
        {
            UniformItem<int> *item = (UniformItem<int> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glUniform1i(item->id, *(item->get()));
        }
        else if (tid == typeid(float))
        {
            UniformItem<float> *item = (UniformItem<float> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glUniform1f(item->id, *(item->get()));
        }
        else if (tid == typeid(glm::vec2))
        {
            UniformItem<glm::vec2> *item = (UniformItem<glm::vec2> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glm::vec2 value = *(item->get());
            glUniform2fv(item->id, 1, glm::value_ptr(value));
        }
        else if (tid == typeid(glm::vec3))
        {
            UniformItem<glm::vec3> *item = (UniformItem<glm::vec3> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glm::vec3 value = *(item->get());
            glUniform3fv(item->id, 1, glm::value_ptr(value));
        }
        else if (tid == typeid(glm::vec4))
        {
            UniformItem<glm::vec4> *item = (UniformItem<glm::vec4> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glm::vec4 value = *(item->get());
            glUniform4fv(item->id, 1, glm::value_ptr(value));
        }
        else if (tid == typeid(glm::mat4))
        {
            UniformItem<glm::mat4> *item = (UniformItem<glm::mat4> *)base_item;

            if (item->id == -1)
            {
                continue;
            }

            glm::mat4 value = *(item->get());
            glUniformMatrix4fv(item->id, 1, GL_FALSE, glm::value_ptr(value));
        }
        else
        {
            std::cerr << "[Shader] Tipo de dado inválido na execução\n";
            exit(1);
        }
    }
}

void Shader::list_opengl_errors(const char *file, int line)
{

    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cerr << "[Opengl] Error "
                  << file
                  << " (" << line << ")"
                  << std::endl;
        std::cout << "\t" << error << std::endl;
    }
}

void Shader::stop_all()
{
    glUseProgram(0);
}

void Shader::setup()
{
    this->load();
}

std::string Shader::get_id(std::string shader_name)
{
    return Shader::get_id(shader_name, shader_name);
}

std::string Shader::get_id(std::string fragment_shader_name, std::string vertex_shader_name)
{
    return fragment_shader_name + ", " + vertex_shader_name;
}

std::unordered_map<std::string, Shader *> Shader::loaded_shaders;
const std::string Shader::shaders_folder = std::string(CMAKE_ROOT_DIR SHADERS_FOLDER);

#endif