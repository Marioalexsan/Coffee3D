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

#include <Coffee/Coffee.hpp>
#include <glm/gtx/rotate_vector.hpp>



int main(int argc, char** argv)
{
    cf::Window window = cf::Window("Hello world!", 800, 600);

    cf::Texture tex = cf::Texture("assets/wallRGBA.png");

    cf::Shader shader = cf::Shader::defaultShader();

    std::cout << "Shader load status: " << (shader.ready() ? "Ready" : "Not ready") << "\n";
    std::cout << "Shader logs: " << shader.getCombinedLogs() << "\n";

    cf::Model box = cf::ModelGenerator::box();
    cf::Model sphere = cf::ModelGenerator::sphere();

    cf::Object3D obj = cf::Object3D(box);
    cf::Object3D obj2 = cf::Object3D(sphere);
    cf::Object3D obj3 = cf::Object3D(sphere);
    cf::Object3D wall = cf::Object3D(box);

    //wall.move({10, 0, 10});
    wall.scaleBy(5.0, 5.0, 1.0);

    cf::Scene scene;

    scene.createRootNode("First");
    scene.createNode("Second");
    scene.createNode("Third");
    scene.createRootNode("WallFixture");

    scene.createChildNode("WallFixture", "Wall1");
    scene.getNode("Wall1")->drawable = &wall;
    scene.getNode("Wall1")->move({ 10, 10, 0 });

    scene.createChildNode("WallFixture", "Wall2");
    scene.getNode("Wall2")->drawable = &wall;
    scene.getNode("Wall2")->move({ -10, 10, 0 });

    scene.createChildNode("WallFixture", "Wall3");
    scene.getNode("Wall3")->drawable = &wall;
    scene.getNode("Wall3")->move({ -10, -10, 0 });

    scene.createChildNode("WallFixture", "Wall4");
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

    // Scenes are also drawables
    cf::Scene scene2;

    scene2.createRootNode("BaseScene");
    scene2.getNode("BaseScene")->move({ 0.0f, 0.0f, -40.0f });
    scene2.createChildNode("BaseScene", "BaseSceneNode");
    scene2.getNode("BaseSceneNode")->yaw(glm::pi<float>() / 4);
    scene2.getNode("BaseSceneNode")->drawable = &scene;

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
            // Add
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

    float deltaTime = 0.016f;

    /* Loop until the user closes the window */
    while (!window.closeEventReceived())
    {
        /* Mark the start of the frame */
        auto frameStart = std::chrono::system_clock::now();

        /* Poll for and process events */
        cf::Window::pollEvents();

        window.clear(cf::Color::White);

        float yawBy = ((std::rand() % 10) + 50) / 100.f * glm::pi<float>() * deltaTime;
        float rollBy = ((std::rand() % 10) + 50) / 100.f * glm::pi<float>() * deltaTime;
        float pitchBy = ((std::rand() % 10) + 50) / 100.f * glm::pi<float>() * deltaTime;

        scene.getNode("WallFixture")->yawBy(yawBy);
        scene.getNode("WallFixture")->rollBy(rollBy);
        scene.getNode("WallFixture")->pitchBy(pitchBy);

        scene.getNode("First")->rollBy(glm::pi<float>() * deltaTime);

        scene.getNode("Second")->scale(0.6f, 0.6f, 0.6f);
        scene.getNode("Second")->yawBy(glm::pi<float>() * deltaTime);
        scene.getNode("Second")->position({ 3.0f, 0.0f, 0.0f });

        scene.getNode("Third")->scale(0.6f, 0.6f, 0.6f);
        scene.getNode("Third")->rollBy(glm::pi<float>() * deltaTime);
        scene.getNode("Third")->position({ 3.0f, 0.0f, 0.0f });

        float speed = pressingShift ? 4.f : 35.f;

        if (pressingW)
            camera.move(camera.getViewDirection() * speed * deltaTime);

        if (pressingS)
            camera.move(camera.getViewDirection() * -speed * deltaTime);

        if (pressingA)
            camera.move(glm::rotateY(glm::vec3(-speed * deltaTime, 0.f, 0.f), camera.yaw()));

        if (pressingD)
            camera.move(glm::rotateY(glm::vec3(speed * deltaTime, 0.f, 0.f), camera.yaw()));

        cf::RenderState state;
        state.texture = &tex;
        state.shader = &shader;
        state.projectionMatrix = camera.getProjectionMatrix();
        state.viewMatrix = camera.getViewMatrix();

        scene2.draw(state);

        window.display();

        /* Mark end of frame */
        auto frameEnd = std::chrono::system_clock::now();

        if ((frameEnd - frameStart) <= std::chrono::milliseconds(16))
        {
            std::this_thread::sleep_for(frameEnd - frameStart);
        }
    }
    return 0;
}
