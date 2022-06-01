#ifndef COFFEE_CAMERA_HPP
#define COFFEE_CAMERA_HPP

#include <glm/glm.hpp>
#include <Coffee/Transformable.hpp>
#include <glm/gtc/constants.hpp>

namespace cf
{
    class Camera
    {
    public:
        Camera(float fovx, float fovy, float far, float near = 0.1f)
        {
            set3D(fovx, fovy, far, near);
        }

        Camera(float width, float height)
        {
            set2D(width, height);
        }

        void set3D(float fovx = glm::pi<float>() / 6.f, float fovy = glm::pi<float>() / 6.f, float far = 100.0f, float near = 0.1f);

        void set2D(float width, float height);

        void position(glm::vec3 position);
        glm::vec3 position();
        void move(glm::vec3 distance);

        void yaw(float radians);
        void yawBy(float radians);
        float yaw();

        void pitch(float radians);
        void pitchBy(float radians);
        float pitch();

        glm::vec3 getViewDirection();

        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix();

    private:
        glm::vec3   m_position = glm::vec3(0.0f, 0.0f, 0.0f);
        float       m_pitch = 0.0f;
        float       m_yaw = 0.0f;

        glm::mat4   m_viewMatrix = glm::mat4(1.0f);
        glm::mat4   m_projectionMatrix = glm::mat4(1.0f);
        bool        m_updateNeeded = true;
    };
}

#endif