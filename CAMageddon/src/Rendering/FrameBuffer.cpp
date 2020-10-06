#include "campch.h"
#include "FrameBuffer.h"

#include <glad/glad.h>

namespace CAMageddon
{
	static const uint32_t s_MaxFramebufferSize = 8192;

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		bool multisample = m_Specification.Samples > 1;

		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			if (multisample)
				glDeleteRenderbuffers(1, &m_DepthAttachment);
			else
				glDeleteTextures(1, &m_DepthAttachment);
		}

		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		//Color attachment
		glGenTextures(1, &m_ColorAttachment);
		if (multisample)
		{
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment);
			glTexImage2DMultisample(
				GL_TEXTURE_2D_MULTISAMPLE,
				m_Specification.Samples,
				GL_RGBA,
				m_Specification.Width,
				m_Specification.Height,
				GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				m_Specification.Width,
				m_Specification.Height,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
		}

		glGenTextures(1, &m_DepthAttachment);


		//Depth attachment
		if (multisample)
		{
			glGenRenderbuffers(1, &m_DepthAttachment);
			glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
			glRenderbufferStorageMultisample(
				GL_RENDERBUFFER,
				m_Specification.Samples,
				GL_DEPTH24_STENCIL8,
				m_Specification.Width,
				m_Specification.Height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
		}

		if (multisample)
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_ColorAttachment, 0);
		else
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

		if (!multisample)
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);

		LOG_ASSERT(
			glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::BindRead()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_RendererID);
		//glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::BindDraw()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_RendererID);
		//glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);
	}

	void OpenGLFramebuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			LOG_WARNING("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}
}
