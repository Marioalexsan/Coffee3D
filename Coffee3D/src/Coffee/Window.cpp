#include <Coffee/Window.hpp>
#include <Coffee/Color.hpp>
#include <GLFW/glfw3.h>

namespace cf
{
    GLFWwindow* Window::s_lastContext = nullptr;
    std::vector<Window*> Window::s_windows = std::vector<Window*>();

    Window::Window(const std::string& title, unsigned int width, unsigned int height)
    {
        if (s_windows.size() == 0)
        {
            if (!glfwInit())
                throw std::exception("GLFW failed to init.");
        }

        s_windows.push_back(this);

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

    Window::~Window()
    {
        std::erase(s_windows, this);

        if (s_windows.size() == 0)
        {
            glfwTerminate();
        }
    }

    bool Window::open(const std::string& title, unsigned int width, unsigned int height)
    {
        if (m_window)
            return false;

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

        if (!m_window)
            return false;

        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetKeyCallback(m_window, static_key_callback);
        glfwSetMouseButtonCallback(m_window, static_mouse_callback);
        glfwSetCursorPosCallback(m_window, static_mouse_pos_callback);

        m_width = width;
        m_height = height;

        return true;
    }

    void Window::clear(Color color)
    {
        useContext();

        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::display()
    {
        glfwSwapBuffers(m_window);
    }

    bool Window::useContext()
    {
        if (!m_window)
            return false;

        // Check if there's a context change to begin with
        if (this->m_window == s_lastContext)
            return true;

        s_lastContext = this->m_window;

        glfwMakeContextCurrent(m_window); 

        // https://stackoverflow.com/questions/35683334/call-glewinit-once-for-each-rendering-context-or-exactly-once-for-the-whole-app
        GLenum glewStatus = glewInit();

        if (glewStatus != GLEW_OK)
            return false;

        //glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

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

    bool Window::closeEventReceived()
    {
        if (!m_window)
            return false;

        return glfwWindowShouldClose(m_window);
    }

    bool Window::viewport(int x, int y, int width, int height)
    {
        if (!m_window)
            return false;

        useContext();
        glViewport(x, y, width, height);

        return true;
    }

    void Window::static_key_callback(GLFWwindow* glfw, int key, int scancode, int action, int mods)
    {
        cf::Window* window = (cf::Window*)glfwGetWindowUserPointer(glfw);

        if (!window || !window->m_keyCallback)
            return;

        window->m_keyCallback(key, scancode, action, mods);
    }

    void Window::static_mouse_callback(GLFWwindow* glfw, int button, int action, int mods)
    {
        cf::Window* window = (cf::Window*)glfwGetWindowUserPointer(glfw);

        if (!window || !window->m_buttonCallback)
            return;

        window->m_buttonCallback(button, action, mods);
    }

    void Window::static_mouse_pos_callback(GLFWwindow* glfw, double xpos, double ypos)
    {
        cf::Window* window = (cf::Window*)glfwGetWindowUserPointer(glfw);

        if (!window || !window->m_buttonCallback)
            return;

        window->m_buttonPosCallback(xpos, ypos);
    }

    void Window::pollEvents()
    {
        glfwPollEvents();
    }
}