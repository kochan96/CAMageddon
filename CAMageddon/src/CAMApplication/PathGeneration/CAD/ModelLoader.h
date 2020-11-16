#pragma once
#include "campch.h"
#include "SurfaceUV.h"

namespace CAMageddon
{
	class ModelLoader
	{
	public:
		static std::vector<std::shared_ptr<SurfaceUV>> LoadModel(const std::string& filePath);
	};
}