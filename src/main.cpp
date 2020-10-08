#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "core/graphics/GL.cpp"

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};

GLuint shaderProgram;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void error_callback(int code, const char *description)
{
    std::cerr << description << std::endl;
}

float tmp_x = 0;
void processInput_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    //     glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        tmp_x -= 0.1;
        glUniform3f(glGetUniformLocation(shaderProgram, "vPos"), tmp_x, 0.0, 0.0);
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        glTranslatef(0.1, 0, 0);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void render(GLuint shaderProgram, int VAO)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[])
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
    glfwSetKeyCallback(window, processInput_callback);

    shaderProgram = se4::compile_shaders();

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glUseProgram(shaderProgram);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    while (!glfwWindowShouldClose(window))
    {
        render(shaderProgram, VAO);

        glfwSwapBuffers(window);

        processInput(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}