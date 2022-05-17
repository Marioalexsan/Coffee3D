#include <stack>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Coffee/Coffee.hpp>

const std::string fs_code =
"#version 400\n"

"in vec3 Position;"
"in vec2 TexCoord;"
"in vec4 Color;"

"out vec4 FragColor;"

"uniform sampler2D tex;"

"void main()"
"{"
"FragColor = Color * texture(tex, TexCoord);"
"}";

const std::string vs_code = 
"#version 400\n"

"layout(location = 0) in vec3 aPos;"
"layout(location = 1) in vec2 aTexCoord;"
"layout(location = 2) in vec4 aColor;"

"out vec3 Position;"
"out vec2 TexCoord;"
"out vec4 Color;"

"uniform sampler2D tex;"
"uniform mat4x4 mvpMatrix;"

"void main()"
"{"
"gl_Position = mvpMatrix * vec4(aPos, 1.0);"
"Color = aColor;"
"TexCoord = aTexCoord;"
"Position = aPos;"
"}";

void printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten = 0;
    char* infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char*)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n", infoLog);
        free(infoLog);
    }
}

void printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten = 0;
    char* infoLog;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char*)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n", infoLog);
        free(infoLog);
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
        return -1;

    coffee::Window window = coffee::Window("Hello world!", 800, 600);

    coffee::Texture tex = coffee::Texture("wallRGBA.png");

    coffee::Shader shader = coffee::Shader();
    shader.load(coffee::ShaderType::Vertex, vs_code);
    shader.load(coffee::ShaderType::Fragment, fs_code);

    std::cout << "Shader load status: " << shader.ready() << "\n";
    std::cout << "Shader logs: " << shader.getCombinedLogs() << "\n";

    coffee::Model model = coffee::ModelGenerator::sphere();

    coffee::ModelDrawable obj = coffee::ModelDrawable(model);
    coffee::ModelDrawable obj2 = coffee::ModelDrawable(model);
    coffee::ModelDrawable obj3 = coffee::ModelDrawable(model);

    glClearColor(1, 1, 1, 1);

    glm::vec3 viewPos = glm::vec3(10.0, 0.0, 0.0);

    glViewport(0, 0, 800, 600);

    coffee::SceneNode render;

    render.m_children.push_back(coffee::SceneNode(&obj));
    render.m_children[0].m_transform *= glm::translate(glm::vec3(0.0, 0.0, -1.0));
    render.m_children[0].m_children.push_back(coffee::SceneNode(&obj2));
    render.m_children[0].m_children[0].m_children.push_back(coffee::SceneNode(&obj3));

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    float angle = 0.0f;
    float angle2 = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window.getGlfw()))
    {
        /* Poll for and process events */
        glfwPollEvents();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render.m_children[0].m_children[0].m_transform = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * glm::rotate(angle, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.6, 0.6, 0.6));
        render.m_children[0].m_children[0].m_children[0].m_transform = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) * glm::rotate(angle / 2, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(0.6, 0.6, 0.6));
        render.m_children[0].m_transform = glm::rotate(angle2, glm::vec3(0, 1, 0));

        angle += glm::pi<float>() / 60.f;
        angle2 += glm::pi<float>() / 180.f;

        coffee::RenderState state;
        state.texture = &tex;
        state.shader = &shader;
        state.projectionMatrix = glm::perspective(glm::pi<float>() / 6, (float)800 / 600, 0.1f, 100.0f);
        state.viewMatrix = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        render.renderScene(state);


        /* Swap front and back buffers */
        glfwSwapBuffers(window.getGlfw());

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    glfwTerminate();
    return 0;
}
