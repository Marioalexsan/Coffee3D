#ifndef COFFEE_WINDOW_HPP
#define COFFEE_WINDOW_HPP

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace coffee
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

        bool useContext();

        GLFWwindow* getGlfw();

        void close();

    private:
        GLFWwindow* m_window = nullptr;
        unsigned int m_width = 0;
        unsigned int m_height = 0;
    };
}

#endif