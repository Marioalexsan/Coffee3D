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
    /* Setup window */
    cf::Window window = cf::Window("Hello world!", 1600, 900);
    window.viewport(0, 0, 1600, 900);

    /* Setup shader */
    cf::Shader shader = cf::Shader::defaultShader();

    std::cout << "Shader load status: " << (shader.ready() ? "Ready" : "Not ready") << "\n";
    std::cout << "Shader logs: " << shader.getCombinedLogs() << "\n";

    /* Setup textures */
    cf::Texture earth_texture = cf::Texture("assets/earth_texture.png");
    cf::Texture sun_texture = cf::Texture("assets/sun_texture.png");
    cf::Texture moon_texture = cf::Texture("assets/moon_texture.png");

    /* Setup model - in this case, a shared sphere model*/
    cf::Model sphere_model = cf::ModelGenerator::sphere();

    /* Setup 3D objects - scales are obviously not accurate to reality */
    cf::Object3D earth = cf::Object3D(sphere_model);
    earth.scale(1, 1, 1);
    earth.texture(&earth_texture);

    cf::Object3D sun = cf::Object3D(sphere_model);
    sun.scale(4, 4, 4);
    sun.texture(&sun_texture);

    cf::Object3D moon = cf::Object3D(sphere_model);
    moon.scale(0.2f, 0.2f, 0.2f);
    moon.texture(&moon_texture);

    /* Setup the skybox */
    std::array<std::string, 6> faces = {
        "assets/skytex_positiveX.png",
        "assets/skytex_negativeX.png",
        "assets/skytex_positiveY.png",
        "assets/skytex_negativeY.png",
        "assets/skytex_positiveZ.png",
        "assets/skytex_negativeZ.png"
    };

    cf::CubemapTexture cubemap;
    cubemap.load(faces);

    cf::Skybox skybox(&cubemap);

    /* Setup scene */
    cf::Scene solarSystem;

    solarSystem.createRootNode("skybox");
    solarSystem.createRootNode("Sun_Center");
    solarSystem.createChildNode("Sun_Center", "Sun");
    solarSystem.createChildNode("Sun_Center", "Sun_AttachEarth");
    solarSystem.createChildNode("Sun_AttachEarth", "Earth_Center");
    solarSystem.createChildNode("Earth_Center", "Earth");
    solarSystem.createChildNode("Earth_Center", "Earth_AttachMoon");
    solarSystem.createChildNode("Earth_AttachMoon", "Moon_Center");
    solarSystem.createChildNode("Moon_Center", "Moon");

    solarSystem.getNode("Earth_Center")->move(glm::vec3(7.0f, 0.0f, 0.0f));
    solarSystem.getNode("Moon_Center")->move(glm::vec3(2.0f, 0.0f, 0.0f));

    solarSystem.getNode("skybox")->drawable = &skybox;
    solarSystem.getNode("Sun")->drawable = &sun;
    solarSystem.getNode("Earth")->drawable = &earth;
    solarSystem.getNode("Moon")->drawable = &moon;

    /* Setup camera */
    cf::Camera camera(glm::pi<float>() / 4.f, glm::pi<float>() / 6.f, 100.0f);
    camera.position(glm::vec3(0, 0, 10));

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
            // Add any mouse actions here
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

    /* Numbers are somewhat made up */
    float sunRotationFactor = 0.08f;
    float earthRotationFactor = 0.3f;
    float moonRotationFactor = -0.2f;

    float earthRevolutionFactor = 0.018f;
    float moonRevolutionFactor = 0.030f;

    float deltaTime = 0.016f;

    /* Loop until the user closes the window */
    while (!window.closeEventReceived())
    {
        /* Mark the start of the frame */
        auto frameStart = std::chrono::system_clock::now();

        /* Poll for and process events */
        cf::Window::pollEvents();

        window.clear(cf::Color::White);

        solarSystem.getNode("Sun")->yawBy(sunRotationFactor * glm::pi<float>() * deltaTime);
        solarSystem.getNode("Earth")->yawBy(earthRotationFactor * glm::pi<float>() * deltaTime);
        solarSystem.getNode("Moon")->yawBy(moonRotationFactor * glm::pi<float>() * deltaTime);

        solarSystem.getNode("Sun_AttachEarth")->yawBy(earthRevolutionFactor * glm::pi<float>() * deltaTime);
        solarSystem.getNode("Earth_AttachMoon")->yawBy(moonRevolutionFactor * glm::pi<float>() * deltaTime);

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
        state.shader = &shader;
        state.projectionMatrix = camera.getProjectionMatrix();
        state.viewMatrix = camera.getViewMatrix();

        solarSystem.draw(state);

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
