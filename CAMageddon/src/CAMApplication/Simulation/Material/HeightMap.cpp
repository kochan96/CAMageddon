#include "HeightMap.h"
#include <CAMApplication/Simulation/MilimetersGLConverter.h>
#include <glad/glad.h>

namespace CAMageddon
{
	HeightMap::HeightMap(HeightMapSpecification specification)
	{
		m_Width = MilimetersGLConverter::MilimetersToGL(specification.Width);
		m_Height = MilimetersGLConverter::MilimetersToGL(specification.Height);
		m_TextureWidth = specification.WidthCount;
		m_TextureHeight = specification.HeightCount;

		int count = m_TextureWidth * m_TextureHeight;
		m_HeightValues.resize(count);
		std::fill(m_HeightValues.begin(), m_HeightValues.end(), MilimetersGLConverter::MilimetersToGL(specification.Value));

		InitTexture();
	}

	HeightMap::~HeightMap()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void HeightMap::InitTexture()
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_TextureWidth, m_TextureHeight, 0, GL_RED, GL_FLOAT, m_HeightValues.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	int GetIndex(int row, int column, int stride)
	{
		return row * stride + column;
	}

	float HeightMap::GetHeightValue(int row, int column) const
	{
		return MilimetersGLConverter::GLToMilimeters(m_HeightValues[GetIndex(row, column, m_TextureWidth)]);
	}

	void HeightMap::SetHeightValue(int row, int column, float value)
	{
		m_HeightValues[GetIndex(row, column, m_TextureWidth)] = MilimetersGLConverter::MilimetersToGL(value);
	}

	void HeightMap::Bind(uint32_t slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void HeightMap::Update()
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_TextureWidth, m_TextureHeight, GL_RED, GL_FLOAT, m_HeightValues.data());
	}
}
