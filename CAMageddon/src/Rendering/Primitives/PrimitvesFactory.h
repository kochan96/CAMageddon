#pragma once
#pragma once
#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace CAMageddon
{
	struct Vertex
	{
		glm::vec3 position;
	};

	struct VertexN
	{
		glm::vec3 position;
		glm::vec3 normal;
	};

	struct VertexT
	{
		glm::vec3 position;
		glm::vec2 textureCoord;
	};

	struct VertexNT
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoord;
	};

	template<class T>
	struct VerticesBufferData
	{
		std::vector<T> vertexBufferData;
	};

	template<class T>
	struct VerticesIndicesBufferData
	{
		std::vector<T> vertexBufferData;
		std::vector<uint32_t> indexBufferData;
	};

	class PrimitiveFactory
	{
	public:
		static VerticesBufferData<Vertex> CreateCubeVertices(float size = 2.0f);

		static VerticesBufferData<VertexNT> CreateCubeVerticesNormalsTexture(float size = 2.0f);

		static VerticesBufferData<Vertex> CreateFlatPatchVertices(float width = 2.0f, float height = 2.0f);

		static VerticesIndicesBufferData<VertexNT> CreateFlatPlaneVerticesNormalsTexture(
			float width = 2.0f,
			float height = 2.0f,
			int widthCount = 5,
			int heightCount = 5);

		static VerticesIndicesBufferData<VertexNT> CreateOpenCylinderVerticesNormalsTexture(
			float radius = 1.0f,
			float height = 5.0f,
			int radiusCount = 10);

		static VerticesIndicesBufferData<VertexNT> CreateSphereVerticesNormalsTexture(
			float radius = 1.0f,
			int uDivisionCount = 10,
			int vDivisionCount = 10);
	};
}