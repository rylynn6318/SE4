#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "core/graphics/GL.cpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

auto framebuffer_size_callback(GLFWwindow *window, int width, int height) -> void
{
    glViewport(0, 0, width, height);
}

auto error_callback(int code, const char *description) -> void
{
    std::cerr << description << std::endl;
}

// 콜백 스타일
//void processInput_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_A && action == GLFW_PRESS)
//    {
//        tmp_x -= 0.1f;
//        glUniform3f(glGetUniformLocation(shaderProgram, "vPos"), tmp_x, 0.0, 0.0);
//    }
//}
// 이건 매프레임 직접 호출
auto processInput(GLFWwindow *window, GLuint shader_program, float tmp) -> void
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        tmp -= 0.01f;
        glUniform3f(glGetUniformLocation(shader_program, "vPos"), tmp, 0.0, 0.0);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        tmp += 0.01f;
        glUniform3f(glGetUniformLocation(shader_program, "vPos"), tmp, 0.0, 0.0);
    }
}

auto render(GLuint VAO) -> void
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

auto main(int argc, char *argv[]) -> int
{
    glfwSetErrorCallback(error_callback);
    GLFWwindow *window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(800, 600, "test project", nullptr, nullptr);

    if (!window)
    {
        std::cout << "window not found. terminate\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetKeyCallback(window, processInput_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    GLuint shader_program = se4::compile_shaders();

    GLuint vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glUseProgram(shader_program);

    float tmp_x = 0;
    while (!glfwWindowShouldClose(window))
    {
        render(vao);

        glfwSwapBuffers(window);

        processInput(window, shader_program, tmp_x);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}