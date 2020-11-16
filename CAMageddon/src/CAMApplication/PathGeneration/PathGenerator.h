#pragma once
#include "campch.h"
#include "CAD/SurfaceUV.h"
#include "CAMApplication/Simulation/Material/Material.h"

#include "CAMApplication/Simulation/Material/Material.h"

namespace CAMageddon
{
	class PathGenerator
	{
	public:
		PathGenerator(
			const std::string& folder,
			const MaterialSpecifiaction& materialSpecification,
			std::shared_ptr<Material> m_Material,
			std::vector<std::shared_ptr<SurfaceUV>> surfaces);

		void GenerateFirstPath(const std::string& fileName, int startIndex = 3);

	private:
		glm::ivec2 GetIndices(glm::vec2 position);
		glm::vec2 GetPosition(int row, int column);
		float GetZ(float cpx, float cpy, float toolRadius);
		void GetIndicesOfArea(glm::vec3 position, float range, int& left, int& right, int& top, int& bottom);

		void GenerateHeightMap();
		std::vector<glm::vec3> GenerateFirstPathLayer(float minZ);

	private:
		float minZ;
		float safeZ;

		std::shared_ptr<Material> m_Material; //TESTS;

		std::string m_Folder;
		MaterialSpecifiaction m_MaterialSpecification;
		std::vector<std::shared_ptr<SurfaceUV>> m_Surfaces;
		std::vector<std::vector<float>> m_HeightMap;
	};
}