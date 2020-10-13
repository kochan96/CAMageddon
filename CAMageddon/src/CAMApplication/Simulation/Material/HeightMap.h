#pragma once
#include "campch.h"
#include "Core/Base.h"

namespace CAMageddon
{
	struct HeightMapSpecification
	{
		int WidthCount;
		int HeightCount;
		float Value;
	};

	class HeightMap
	{
	public:
		HeightMap(HeightMapSpecification specification);
		~HeightMap();

		int GetTextureWidth() const { return m_TextureWidth; }
		int GetTextureHeight() const { return m_TextureHeight; }

		float GetHeightValue(int row, int column) const;
		void SetHeightValue(int row, int column, float value);

		void Bind(uint32_t slot = 0);

		void Update();

	private:
		void InitTexture();

	private:
		unsigned int m_TextureID;
		int m_TextureWidth, m_TextureHeight;
		
		std::vector<float> m_HeightValues;
	};
}