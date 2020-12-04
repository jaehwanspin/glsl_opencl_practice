#ifndef _GL_SHADER_HH_
#define _GL_SHADER_HH_
#include <GLES3/gl31.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>

#include <memory>
#include <type_traits>

#define GLSL_STRINGIZE_SOURCE(version_number, ...) \
"#version "#version_number"\n" \
#__VA_ARGS__

namespace gl {

template<typename _vec4f_type
    >
void clear_color(_vec4f_type& _colour)
{
    glClearColor(_colour[0], _colour[1],
                 _colour[2], _colour[3]);
}
void clear_color(GLfloat _red, GLfloat _green,
                 GLfloat _blue, GLfloat _alpha)
{
    glClearColor(_red, _green, _blue, _alpha);
}

class program;

template<GLenum __shader_type>
class _shader_base
{
    friend class program;
private:
    GLuint _shader;

public:
    _shader_base(std::string& source_code)
    {
        this->init(source_code.c_str());
    }
    _shader_base(const char* source_code)
    {
        this->init(source_code);
    }

    virtual ~_shader_base()
    {
        if (this->_shader)
            glDeleteShader(this->_shader);
    }

    bool compile()
    {
        GLint compiled;
        glCompileShader(this->_shader);
        glGetShaderiv(this->_shader, GL_COMPILE_STATUS, &compiled);
        return static_cast<bool>(compiled);
    }

    std::string info()
    {
        GLint len_info = 0;
        std::string info_log_str;
        glGetShaderiv(this->_shader, GL_INFO_LOG_LENGTH, &len_info);
        if (len_info > 1)
        {
            char *info_log = (char*)malloc(sizeof(char) * len_info);
            glGetShaderInfoLog(this->_shader, len_info, NULL, info_log);
            info_log_str = info_log;
            free(info_log);
        }

        return std::move(info_log_str);
    }

private:
    void init(const char* source_code)
    {
        this->_shader = glCreateShader(__shader_type);
        if (this->_shader == 0) return;
        glShaderSource(this->_shader, 1, &source_code, nullptr);
    }

};

using vertex_shader = _shader_base<GL_VERTEX_SHADER>;
using fragment_shader = _shader_base<GL_FRAGMENT_SHADER>;
using compute_shader = _shader_base<GL_COMPUTE_SHADER>;

class program
{
private:
    GLuint _program;
public:
    program()
    {
        this->_program = glCreateProgram();
    }

    virtual ~program()
    {
        if (this->_program != 0)
            glDeleteProgram(this->_program);
    }

    template <GLenum __shader_type>
    void attach(_shader_base<__shader_type>& _gl_shader)
    {
        glAttachShader(this->_program, _gl_shader._shader);
    }

    bool link()
    {
        int result = 0;
        glLinkProgram(this->_program);
        glGetProgramiv(this->_program, GL_LINK_STATUS, &result);
        return static_cast<bool>(result);
    }
    
    std::string info()
    {
        GLint len_info = 0;
        std::string info_log_str;
        glGetProgramiv(this->_program, GL_INFO_LOG_LENGTH, &len_info);
        if (len_info > 1)
        {
            char *info_log = (char*)malloc(sizeof(char) * len_info);
            glGetProgramInfoLog(this->_program, len_info, NULL, info_log);
            info_log_str = info_log;
            free(info_log);
        }

        return std::move(info_log_str);
    }
    

};

}

#endif