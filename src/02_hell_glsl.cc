#include <iostream>
#include <string>
#include <array>

#include <boost/qvm/all.hpp>
namespace bq = boost::qvm;

#define GLFW_INCLUDE_ES31
#include <GLFW/glfw3.h>
#include <GL/gl.hh>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

GLFWwindow *window;

std::string vertex_shader_src = GLSL_STRINGIZE_SOURCE(
310 es,

layout(location = 0) in vec3 vertexPosition_modelspace;

void main()
{
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
}

);

std::string fragment_shader_src = GLSL_STRINGIZE_SOURCE(
310 es,

out vec3 color;

void main()
{
    color = vec3(1, 0, 0);
}

);

int main(int argc, char **argv)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    
    bq::vec<float, 4> colour = { 0, 0, 0, 0 };
    glm::vec4 colour_ = { 0, 0, 0, 0 };
    std::array<GLfloat, 4> colour__ = { 0.0f, 0.0f, 0.0f, 0.0f };
    gl::clear_color(colour__);
    
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    gl::vertex_shader vertex_shader(vertex_shader_src);
    vertex_shader.compile();
    gl::fragment_shader fragment_shader(fragment_shader_src);
    fragment_shader.compile();

    gl::program glsl_program;
    glsl_program.attach(vertex_shader);
    glsl_program.attach(fragment_shader);
    // std::swap()

    do
    {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw nothing, see you in tutorial 2 !

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        // Initialize GLEW
        //
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    
    return 0;
}