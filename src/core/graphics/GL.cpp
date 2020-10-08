#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace se4
{
    auto compile_shaders() -> GLuint
    {
        static const GLchar *vertex_shader_source[] = {
            "#version 330 core                                                              \n"
            "layout (location = 0) in vec3 aPos;                                            \n"
            "uniform vec3 vPos;                                                             \n"
            "void main()                                                                    \n"
            "{                                                                              \n"
            "   gl_Position = vec4(aPos.x + vPos.x, aPos.y + vPos.y, aPos.z + vPos.z, 1.0); \n"
            "}                                                                              \n"};

        static const GLchar *fragment_shader_source[] = {
            "#version 330 core                              \n"
            "out vec4 FragColor;                            \n"
            "void main()                                    \n"
            "{                                              \n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);   \n"
            "}                                              \n"};

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vertex_shader_source, nullptr);
        glCompileShader(vertexShader);

        // todo : 로깅 분리
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fragment_shader_source, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    }
} // namespace se4