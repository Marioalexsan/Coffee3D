#include <Coffee/Window.hpp>

namespace coffee
{
    Window::Window(const std::string& title, unsigned int width, unsigned int height)
    {
        if (open(title, width, height))
            useContext();
    }

    Window::Window(Window&& other) noexcept
    {
        *this = std::move(other);
    }

    Window& Window::operator=(Window&& other) noexcept
    {
        m_window = other.m_window;
        other.m_window = nullptr;

        return *this;
    }

    bool Window::open(const std::string& title, unsigned int width, unsigned int height)
    {
        if (m_window)
            return false;

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!m_window)
            return false;

        m_width = width;
        m_height = height;

        return true;
    }

    bool Window::useContext()
    {
        if (!m_window)
            return false;

        glfwMakeContextCurrent(m_window);

        // https://stackoverflow.com/questions/35683334/call-glewinit-once-for-each-rendering-context-or-exactly-once-for-the-whole-app
        GLenum glewStatus = glewInit();

        if (glewStatus != GLEW_OK)
            return false;

        return true;
    }

    GLFWwindow* Window::getGlfw()
    {
        return m_window;
    }

    void Window::close()
    {
        if (!m_window)
            return;

        glfwDestroyWindow(m_window);

        m_window = nullptr;
        m_width = 0;
        m_height = 0;
    }
}