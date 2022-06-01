#include <Coffee/Transformable.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace cf
{
    void Transformable::position(glm::vec3 position)
    {
        m_position = position;
        m_updateNeeded = true;
    }

    glm::vec3 Transformable::position()
    {
        return m_position;
    }

    void Transformable::move(glm::vec3 translation)
    {
        m_position += translation;
        m_updateNeeded = true;
    }

    void Transformable::roll(float radians)
    {
        m_roll = radians;
        m_updateNeeded = true;
    }

    float Transformable::roll()
    {
        return m_roll;
    }

    void Transformable::rollBy(float radians)
    {
        m_roll += radians;
        m_updateNeeded = true;
    }

    void Transformable::yaw(float radians)
    {
        m_yaw = radians;
        m_updateNeeded = true;
    }

    float Transformable::yaw()
    {
        return m_yaw;
    }

    void Transformable::yawBy(float radians)
    {
        m_yaw += radians;
        m_updateNeeded = true;
    }

    void Transformable::pitch(float radians)
    {
        m_pitch = radians;
        m_updateNeeded = true;
    }

    float Transformable::pitch()
    {
        return m_pitch;
    }

    void Transformable::pitchBy(float radians)
    {
        m_pitch += radians;
        m_updateNeeded = true;
    }

    void Transformable::scale(float sx, float sy, float sz)
    {
        m_sx = sx;
        m_sy = sy;
        m_sz = sz;
        m_updateNeeded = true;
    }

    glm::vec3 Transformable::scale()
    {
        return glm::vec3(m_sx, m_sy, m_sz);
    }

    void Transformable::scaleBy(float sx, float sy, float sz)
    {
        m_sx *= sx;
        m_sy *= sy;
        m_sz *= sz;
        m_updateNeeded = true;
    }

    glm::mat4 Transformable::getMatrix() const
    {
        if (m_updateNeeded)
        {
            m_updateNeeded = false;

            m_transform =
                glm::scale(glm::vec3(m_sx, m_sy, m_sz)) *
                glm::rotate(m_roll, glm::vec3(0, 0, 1)) *
                glm::rotate(m_yaw, glm::vec3(0, 1, 0)) *
                glm::rotate(m_pitch, glm::vec3(1, 0, 0)) *
                glm::translate(m_position);
        }

        return m_transform;
    }
}