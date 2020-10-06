#pragma once

struct GLFWwindow;

namespace CAMageddon
{
    class OpenGLContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        void Init();
        void SwapBuffers();

    private:
        GLFWwindow* m_windowHandle;
    };
}