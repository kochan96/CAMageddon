#pragma once
#pragma once
#include "campch.h"
#include <glad/glad.h>

namespace CAMageddon
{
	class OpenGLTexture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() const { return m_RendererID; }

		void Bind(uint32_t slot = 0) const;

		bool operator==(const OpenGLTexture2D& other) const
		{
			return m_RendererID == other.m_RendererID;
		}

		void SetData(unsigned char* data);

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}