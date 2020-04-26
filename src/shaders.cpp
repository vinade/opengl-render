#ifndef SHADERS_CPP
#define SHADERS_CPP

#include "shaders.hpp"

unsigned int deb = 0;

template<typename T>
UniformItem<T>::UniformItem(std::string name, unsigned int program_id)
{
    T _item;
    this->id = glGetUniformLocation(program_id, name.c_str());
    this->debug = deb++;
}

template<typename T>
void UniformItem<T>::set(T *_item)
{
    this->item = _item;
}

template<typename T>
T *UniformItem<T>::get()
{
    return this->item;
}

template<typename T>
const std::type_info &UniformItem<T>::getTID()
{
    return typeid(T);
}

Shader::Shader()
{
    this->loaded = GL_FALSE;
}

Shader::Shader(std::string shader_name)
{
    this->loaded = GL_FALSE;
    this->load(shader_name);
}

Shader::~Shader()
{
    if (this->loaded){
        glDeleteProgram(this->program_id);
    }
}

void Shader::setup(std::string name, UniformType type)
{
    BaseUniformItem *selected_type;

    switch(type)
    {
    case SHADER_TYPE_FLOAT:
        selected_type = new UniformItem<float>(name, this->program_id);
        break;
    case SHADER_TYPE_INT:
        selected_type = new UniformItem<int>(name, this->program_id);
        break;
    case SHADER_TYPE_VEC2:
        selected_type = new UniformItem<glm::vec2>(name, this->program_id);
        break;
    case SHADER_TYPE_VEC3:
        selected_type = new UniformItem<glm::vec3>(name, this->program_id);
        break;
    case SHADER_TYPE_MAT4:
        selected_type = new UniformItem<glm::mat4>(name, this->program_id);
        break;
    default:
        std::cerr << "[Shader] Tipo inválido de dado no setup";
        exit(1);
    }

    this->items[name] = selected_type;
}

template <typename T>
void Shader::fill(std::string name, T *value)
{
    BaseUniformItem *item = this->items[name];
    item->set(value);
}

template <typename T>
void Shader::fill(std::string name, const T &value)
{
    BaseUniformItem *item = this->items[name];
    item->set((T *)&value);
}

unsigned int Shader::compile(std::string file_path, unsigned int type)
{
    unsigned int shader_id = glCreateShader(type);

    std::string shader_code;
    std::ifstream shader_stream(file_path.c_str(), std::ios::in);
    if(shader_stream.is_open())
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

        std::cerr << "[shader] Failed to compile "
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

void Shader::load(std::string shader_name)
{

    int status;
    std::string vertex_file_path = SHADERS_FOLDER + shader_name + SHADERS_VERTEX_EXT;
    std::string fragment_file_path = SHADERS_FOLDER + shader_name + SHADERS_FRAGMENT_EXT;

    unsigned int vertex_shader_id = this->compile(vertex_file_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader_id = this->compile(fragment_file_path, GL_FRAGMENT_SHADER);

    unsigned int program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE){
        int log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, NULL, &log_message[0]);

        std::cerr << "[shader] Failed to link shader program" << std::endl;
        std::cerr << "\t" << &log_message[0] << std::endl;
        glDeleteProgram(program_id);
        return;
    }


    glValidateProgram(program_id);
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE){
        int log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        std::vector<char> log_message(log_length + 1);
        glGetProgramInfoLog(program_id, log_length, NULL, &log_message[0]);

        std::cerr << "[shader] Failed to validate shader program" << std::endl;
        std::cerr << "\t" << &log_message[0] << std::endl;
        glDeleteProgram(program_id);
        return;
    }

    // glDetachShader(program_id, vertex_shader_id);
    // glDetachShader(program_id, fragment_shader_id);

    this->loaded = GL_TRUE;
    this->program_id = program_id;
}


void Shader::exec()
{

    if (!this->loaded)
    {
        std::cerr << "[shader] Shader executado sem ser carregado\n";
        exit(1);
    }

    glUseProgram(this->program_id);

    for(auto item_map : this->items)
    {
        BaseUniformItem *item = item_map.second;
        const std::type_info &tid = item->getTID();

        if (tid == typeid(int))
        {
            UniformItem<int> *item_int = (UniformItem<int> *)item;
            glUniform1i(item_int->id, *(item_int->get()));
        }
        else if (tid == typeid(float))
        {
            UniformItem<float> *item_float = (UniformItem<float> *)item;
            glUniform1f(item_float->id, *(item_float->get()));
        }
        else if (tid == typeid(glm::vec2))
        {
            UniformItem<glm::vec2> *item_vec2 = (UniformItem<glm::vec2> *)item;
            glm::vec2 value = *(item_vec2->get());
            glUniform2fv(item_vec2->id, 1, glm::value_ptr(value));
        }
        else if (tid == typeid(glm::vec3))
        {
            UniformItem<glm::vec3> *item_vec3 = (UniformItem<glm::vec3> *)item;
            glm::vec3 value = *(item_vec3->get());
            glUniform3fv(item_vec3->id, 1, glm::value_ptr(value));
        }
        else if (tid == typeid(glm::mat4))
        {
            UniformItem<glm::mat4> *item_mat4 = (UniformItem<glm::mat4> *)item;
            glm::mat4 value = *(item_mat4->get());
            glUniformMatrix4fv(item_mat4->id, 1, GL_FALSE, glm::value_ptr(value));
        }
        else
        {
            std::cerr << "[shader] Tipo de dado inválido na execução\n";
            exit(1);
        }
    }
}

void Shader::list_opengl_errors(const char *file, int line){

    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cerr << "[opengl] Error "
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


#endif