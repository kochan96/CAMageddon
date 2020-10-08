#pragma once
#include "HeightMap.h"
#include "Rendering/VertexArray.h"
#include <glm/glm.hpp>

namespace CAMageddon
{
	struct MaterialSpecifiaction
	{
		float SizeX; //width
		float SizeZ; //height
		float SizeY; // depth

		float MaxDepth; //cutter cannot go lower

		int PrecisionX; //textureHeight;
		int PrecisionZ; //textureWidth
	};

	class FPSCamera;

	class Material
	{
	public:
		Material();
		Material(const MaterialSpecifiaction& specification);

		const MaterialSpecifiaction& GetSpecification() { return m_Specification; }
		void SetSpecification(const MaterialSpecifiaction& specification) { m_Specification = specification; Init(); }

		void Render(const FPSCamera& camera, const glm::vec3& lightPos);

	private:
		void Init();

	private:
		Scope<OpenGLVertexArray> m_VertexArray;

		Scope<HeightMap> m_HeightMap;
		MaterialSpecifiaction m_Specification;
	};
}