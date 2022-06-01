#ifndef COFFEE_WINDOW_HPP
#define COFFEE_WINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Coffee/Color.hpp>
#include <functional>

namespace cf
{
    class Window
    {
    public:

        Window() = default;
        Window(const std::string& title, unsigned int width, unsigned int height);
        Window(const Window&) = delete;
        Window(Window&&) noexcept;

        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) noexcept;

        bool open(const std::string& title, unsigned int width, unsigned int height);

        void clear(Color color);

        void display();

        bool useContext();

        GLFWwindow* getGlfw();

        void close();

        bool closeEventReceived();

        bool viewport(int x, int y, int width, int height);

        void setKeyCallback(const std::function<void(int key, int scancode, int action, int mods)>& callback)
        {
            m_keyCallback = callback;
        }
        
        void setMouseCallback(const std::function<void(int button, int action, int mods)>& callback)
        {
            m_buttonCallback = callback;
        }

        void setMousePosCallback(const std::function<void(double xpos, double ypos)>& callback)
        {
            m_buttonPosCallback = callback;
        }

    private:
        static bool s_glfwInited;
        static GLFWwindow* s_lastContext;

        GLFWwindow* m_window = nullptr;

        std::function<void(int key, int scancode, int action, int mods)> m_keyCallback = nullptr;
        std::function<void(int button, int action, int mods)> m_buttonCallback = nullptr;
        std::function<void(double xpos, double ypos)> m_buttonPosCallback = nullptr;

        unsigned int m_width = 0;
        unsigned int m_height = 0;

        double m_mousex = 0;
        double m_mousey = 0;

        static void static_key_callback(GLFWwindow* glfw, int key, int scancode, int action, int mods);
        static void static_mouse_callback(GLFWwindow* glfw, int button, int action, int mods);
        static void static_mouse_pos_callback(GLFWwindow* glfw, double xpos, double ypos);
    };
}

#endif