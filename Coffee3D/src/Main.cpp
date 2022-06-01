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
#include <glm/gtx/rotate_vector.hpp>

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

int main(int argc, char** argv)
{
    cf::Window window = cf::Window("Hello world!", 800, 600);

    cf::Texture tex = cf::Texture("wallRGBA.png");

    cf::Shader shader = cf::Shader();
    shader.load(cf::ShaderType::Vertex, vs_code);
    shader.load(cf::ShaderType::Fragment, fs_code);

    std::cout << "Shader load status: " << shader.ready() << "\n";
    std::cout << "Shader logs: " << shader.getCombinedLogs() << "\n";

    cf::Model box = cf::ModelGenerator::box();
    cf::Model sphere = cf::ModelGenerator::sphere();

    cf::ModelDrawable obj = cf::ModelDrawable(box);
    cf::ModelDrawable obj2 = cf::ModelDrawable(sphere);
    cf::ModelDrawable obj3 = cf::ModelDrawable(sphere);
    cf::ModelDrawable wall = cf::ModelDrawable(box);

    //wall.move({10, 0, 10});
    wall.scaleBy(5.0, 5.0, 1.0);

    cf::Scene scene;

    scene.createRootNode("First");
    scene.createNode("Second");
    scene.createNode("Third");
    scene.createRootNode("XAxis");

    scene.createRootNode("Wall1");
    scene.getNode("Wall1")->drawable = &wall;
    scene.getNode("Wall1")->move({ 10, 10, 0 });

    scene.createRootNode("Wall2");
    scene.getNode("Wall2")->drawable = &wall;
    scene.getNode("Wall2")->move({ -10, 10, 0 });

    scene.createRootNode("Wall3");
    scene.getNode("Wall3")->drawable = &wall;
    scene.getNode("Wall3")->move({ -10, -10, 0 });

    scene.createRootNode("Wall4");
    scene.getNode("Wall4")->drawable = &wall;
    scene.getNode("Wall4")->move({ 10, -10, 0 });

    scene.connect("First", "Second");
    scene.connect("Second", "Third");

    scene.getNode("First")->drawable = &obj;
    scene.getNode("First")->move({ 0.f, 0.f, 0.f });

    scene.getNode("Second")->drawable = &obj2;

    scene.getNode("Third")->drawable = &obj3;
    scene.getNode("Wall1")->drawable = &wall;

    window.viewport(0, 0, 800, 600);

    cf::Scene scene2;

    scene2.createRootNode("Ligma");
    scene2.getNode("Ligma")->move({ 0.0f, 0.0f, -40.0f });
    scene2.createChildNode("Ligma", "Ligma2");
    scene2.getNode("Ligma2")->yaw(glm::pi<float>() / 4);
    scene2.getNode("Ligma2")->drawable = &scene;

    cf::Camera camera(glm::pi<float>() / 4.f, glm::pi<float>() / 4.f, 100.0f);

    camera.position(glm::vec3(0, 0, 10));

    float angle = 0.0f;
    float angle2 = 0.0f;

    bool pressingW = false;
    bool pressingS = false;
    bool pressingA = false;
    bool pressingD = false;
    bool pressingShift = false;

    window.setKeyCallback([&](int key, int scancode, int action, int mods)
        {
            pressingW |= key == GLFW_KEY_W && action == GLFW_PRESS;
            pressingW &= !(key == GLFW_KEY_W && action == GLFW_RELEASE);

            pressingS |= key == GLFW_KEY_S && action == GLFW_PRESS;
            pressingS &= !(key == GLFW_KEY_S && action == GLFW_RELEASE);

            pressingA |= key == GLFW_KEY_A && action == GLFW_PRESS;
            pressingA &= !(key == GLFW_KEY_A && action == GLFW_RELEASE);

            pressingD |= key == GLFW_KEY_D && action == GLFW_PRESS;
            pressingD &= !(key == GLFW_KEY_D && action == GLFW_RELEASE);

            pressingShift = (mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT;
        }
    );

    window.setMouseCallback([&](int button, int action, int mods)
        {
        }
    );


    bool first = true;
    double last_xpos = 0;
    double last_ypos = 0;

    window.setMousePosCallback([&](double xpos, double ypos)
        {
            if (!first)
            {
                double deltaX = xpos - last_xpos;
                double deltaY = ypos - last_ypos;

                camera.yawBy((float)deltaX / -300.0f);
                camera.pitchBy((float)deltaY / -300.f);
            }

            first = false;

            last_xpos = xpos;
            last_ypos = ypos;
        }
    );

    float deltaTime = 0.16f;

    /* Loop until the user closes the window */
    while (!window.closeEventReceived())
    {
        auto frameStart = std::chrono::system_clock::now();

        /* Poll for and process events */
        glfwPollEvents();

        window.clear(cf::Color::White);

        scene.getNode("First")->roll(angle2);

        scene.getNode("Second")->scale(0.6f, 0.6f, 0.6f);
        scene.getNode("Second")->yaw(angle);
        scene.getNode("Second")->position({ 3.0f, 0.0f, 0.0f });

        scene.getNode("Third")->scale(0.6f, 0.6f, 0.6f);
        scene.getNode("Third")->roll(angle / 2);
        scene.getNode("Third")->position({ 3.0f, 0.0f, 0.0f });

        float speed = 3.5f;

        if (pressingShift)
        {
            speed *= 0.4f;
        }

        if (pressingW)
        {
            camera.move(camera.getViewDirection() * speed * deltaTime);
        }
        if (pressingS)
        {
            camera.move(camera.getViewDirection() * -speed * deltaTime);
        }
        if (pressingA)
        {
            camera.move(glm::rotateY(glm::vec3(-speed * deltaTime, 0.f, 0.f), camera.yaw()));
        }
        if (pressingD)
        {
            camera.move(glm::rotateY(glm::vec3(speed * deltaTime, 0.f, 0.f), camera.yaw()));
        }

        auto cameraPos = camera.position();

        //std::cout << "Camera position: " << cameraPos.x << ' ' << cameraPos.y << ' ' << cameraPos.z << '\n';

        angle += glm::pi<float>() / 60.f;
        angle2 += glm::pi<float>() / 180.f;

        cf::RenderState state;
        state.texture = &tex;
        state.shader = &shader;
        state.projectionMatrix = camera.getProjectionMatrix();
        state.viewMatrix = camera.getViewMatrix();

        scene2.draw(state);

        window.display();

        auto frameEnd = std::chrono::system_clock::now();

        if ((frameEnd - frameStart) <= std::chrono::milliseconds(16))
        {
            std::this_thread::sleep_for(frameEnd - frameStart);
        }
    }

    glfwTerminate();
    return 0;
}
