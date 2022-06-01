#ifndef COFFEE_TRANSFORM_HPP
#define COFFEE_TRANSFORM_HPP

#include <glm/glm.hpp>

namespace cf
{
    class Transformable
    {
    public:

        void position(glm::vec3 position);
        glm::vec3 position();
        void move(glm::vec3 translation);

        void roll(float radians);
        float roll();
        void rollBy(float radians);

        void pitch(float radians);
        float pitch();
        void pitchBy(float radians);

        void yaw(float radians);
        float yaw();
        void yawBy(float radians);

        void scale(float sx, float sy, float sz);
        glm::vec3 scale();
        void scaleBy(float sx, float sy, float sz);

        glm::mat4 getMatrix() const;

    protected:

        glm::vec3 m_position = glm::vec3(0.0f);

        float m_roll = 0.0f;
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;

        float m_sx = 1.0f;
        float m_sy = 1.0f;
        float m_sz = 1.0f;

        glm::mat4 mutable   m_transform = glm::mat4(1.0f);
        bool mutable        m_updateNeeded = true;
    };
}

#endif // COFFEE_TRANSFORM_HPP