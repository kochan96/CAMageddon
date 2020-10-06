#pragma once

#include "Core/Base.h"
#include <glm/glm.hpp>

namespace CAMageddon
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;
        glm::vec4 ClearColor = glm::vec4(0.0f,0.0f,0.0f,1.0f);
        bool SwapChainTarget = false;
    };


    class OpenGLFramebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        ~OpenGLFramebuffer();

        void Invalidate();

        void BindRead();
        void BindDraw();
        void Bind();
        void UnBind();

        void Resize(uint32_t width, uint32_t height);

        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
        uint32_t GetDepthAttachmentID() const { return m_DepthAttachment; }

        const FramebufferSpecification& GetSpecification() const { return m_Specification; }
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
        FramebufferSpecification m_Specification;

    };
}