#include <Coffee/Camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <Coffee/Utility.hpp>

namespace cf
{
    void Camera::set3D(float fovx, float fovy, float far, float near)
    {
        m_projectionMatrix = glm::perspective(fovy, fovx / fovy, near, far);
    }

    void Camera::set2D(float width, float height)
    {
        m_projectionMatrix = glm::ortho(0.f, width, 0.f, height);
    }

    void Camera::position(glm::vec3 position)
    {
        m_position = position;
        m_updateNeeded = true;
    }

    glm::vec3 Camera::position()
    {
        return m_position;
    }

    void Camera::move(glm::vec3 distance)
    {
        m_position += distance;
        m_updateNeeded = true;
    }
    
    void Camera::yaw(float radians)
    {
        m_yaw = radians;
        m_updateNeeded = true;
    }

    void Camera::yawBy(float radians)
    {
        m_yaw += radians;
        m_updateNeeded = true;
    }

    float Camera::yaw()
    {
        return m_yaw;
    }

    void Camera::pitch(float radians)
    {
        m_pitch = cf::clamp(radians, -glm::pi<float>() / 2.f + 0.001f, glm::pi<float>() / 2.f - 0.001f);
        m_updateNeeded = true;
    }

    void Camera::pitchBy(float radians)
    {
        m_pitch = cf::clamp(m_pitch + radians, -glm::pi<float>() / 2.f + 0.001f, glm::pi<float>() / 2.f - 0.001f);
        m_updateNeeded = true;
    }

    float Camera::pitch()
    {
        return m_pitch;
    }

    glm::vec3 Camera::getViewDirection()
    {
        glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);

        forward = glm::rotateX(forward, m_pitch);
        forward = glm::rotateY(forward, m_yaw);

        return forward;
    }

    glm::mat4 Camera::getViewMatrix()
    {
        if (m_updateNeeded)
        {
            m_updateNeeded = false;

            glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);

            forward = glm::rotateX(forward, m_pitch);
            forward = glm::rotateY(forward, m_yaw);

            m_viewMatrix = glm::lookAt(m_position, m_position + forward, glm::vec3(0.f, 1.f, 0.f));
        }

        return m_viewMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix()
    {
        return m_projectionMatrix;
    }
}