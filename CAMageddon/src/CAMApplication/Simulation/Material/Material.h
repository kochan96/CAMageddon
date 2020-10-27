#pragma once
#include "HeightMap.h"
#include "Rendering/VertexArray.h"
#include <glm/glm.hpp>
#include <CAMApplication/Light.h>


namespace CAMageddon
{
	struct MaterialSpecifiaction
	{
		float SizeX; //width
		float SizeZ; // depth
		float SizeY; //height

		float MaxDepth; //cutter cannot go lower

		int PrecisionX; //textureWidth;
		int PrecisionY; //textureHeight
	};

	struct BoundingIndices
	{
		int StartRow;
		int EndRow;

		int StartColumn;
		int EndColumn;
	};

	class FPSCamera;

	class Material
	{
	public:
		Material();
		Material(const MaterialSpecifiaction& specification);

		MaterialSpecifiaction& GetSpecification() { return m_Specification; }
		void SetSpecification(const MaterialSpecifiaction& specification) { m_Specification = specification; Init(); }

		void Render(const FPSCamera& camera, std::vector<Light> lights);

		int GetRowCount() const { return m_Specification.PrecisionY; }
		int GetColumnCount() const { return m_Specification.PrecisionX; }

		float GetMaxDepth() { return m_Specification.MaxDepth; }
		float GetMaxHeight() { return m_Specification.SizeZ; }

		bool GetIsWood() const { return m_IsWood; }
		void SetIsWood(bool isWood) { m_IsWood = isWood; }

		float GetHeigth(int row, int column);
		void SetHeight(int row, int column, float value);
		glm::vec2 GetPosition(int row, int column);

		BoundingIndices GetIndices(glm::vec2 leftBottom, glm::vec2 rightTop);

		void Update();

	private:
		void Init();

	private:
		bool m_IsWood = false;

		Scope<OpenGLVertexArray> m_VertexArray;

		Scope<HeightMap> m_HeightMap;
		MaterialSpecifiaction m_Specification;
	};
}