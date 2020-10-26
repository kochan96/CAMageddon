#include "PrimitvesFactory.h"
#include <glm/gtc/constants.inl>

namespace CAMageddon
{
	VerticesBufferData<Vertex> PrimitiveFactory::CreateCubeVertices(float size)
	{
		std::vector<Vertex> vertices;
		vertices.reserve(36);
		float halfSize = size / 2.0f;

		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, -halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, -halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, halfSize) });
		vertices.push_back({ glm::vec3(-halfSize, halfSize, -halfSize) });

		return { vertices };
	}

	VerticesBufferData<Vertex> PrimitiveFactory::CreateFlatPatchVertices(float width, float height)
	{
		std::vector<Vertex> vertices;
		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;

		vertices.reserve(4);
		vertices.push_back({ glm::vec3(-halfWidth,-halfHeight,0.0f) });
		vertices.push_back({ glm::vec3(halfWidth, -halfHeight,0.0f) });
		vertices.push_back({ glm::vec3(halfWidth, halfHeight, 0.0f) });
		vertices.push_back({ glm::vec3(-halfWidth,halfHeight, 0.0f) });

		/*vertices.push_back({ glm::vec3(-halfWidth,0.0f,	halfHeight) });
		vertices.push_back({ glm::vec3(halfWidth, 0.0f,	halfHeight) });
		vertices.push_back({ glm::vec3(halfWidth, 0.0f, -halfHeight) });
		vertices.push_back({ glm::vec3(-halfWidth,0.0f, -halfHeight) });*/

		return { vertices };
	}

	VerticesIndicesBufferData<VertexNT> PrimitiveFactory::CreateFlatPlaneVerticesNormalsTexture(float width, float height, int widthCount, int heightCount)
	{
		//TODO use vertices and indexes
		const glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
		std::vector<VertexNT> vertices;
		std::vector<uint32_t> indices;

		float uDelta = 1.0f / (widthCount - 1);
		float vDelta = 1.0f / (heightCount - 1);

		float halfWidth = width / 2.0f;
		float halfHeight = height / 2.0f;

		for (int v = 0; v < heightCount - 1; v++)
		{
			float currentV = v * vDelta;
			float nextV = (v + 1) * vDelta;

			for (int u = 0; u < widthCount - 1; u++)
			{
				float currentU = u * uDelta;
				float nextU = (u + 1) * uDelta;

				float currentX = (2 * currentU - 1) * halfWidth;
				float nextX = (2 * nextU - 1) * halfWidth;
				float currentY = (2 * currentV - 1) * halfHeight;
				float nextY = (2 * nextV - 1) * halfHeight;

				int count = vertices.size();
				vertices.push_back({ glm::vec3(currentX, currentY, 0.0f), normal, glm::vec2(currentU, currentV) });
				vertices.push_back({ glm::vec3(nextX,	 currentY,  0.0f), normal, glm::vec2(nextU, currentV) });
				vertices.push_back({ glm::vec3(nextX,	 nextY,  0.0f),	 normal, glm::vec2(nextU, nextV) });
				vertices.push_back({ glm::vec3(currentX, nextY,  0.0f),	 normal, glm::vec2(currentU, nextV) });

				indices.push_back(count);
				indices.push_back(count + 1);
				indices.push_back(count + 2);
				indices.push_back(count + 2);
				indices.push_back(count + 3);
				indices.push_back(count);
			}
		}

		return { vertices,indices };
	}

	VerticesIndicesBufferData<VertexN> PrimitiveFactory::CreateOpenCylinderVerticesNormals(float radius, float height, int radiusCount, float startHeight)
	{
		std::vector<VertexN> vertices;
		std::vector<uint32_t> indices;

		float angleDelta = glm::two_pi<float>() / radiusCount;
		float texDelta = 1.0f / radiusCount;

		for (int i = 0; i < radiusCount; i++)
		{
			float currentAngle = i * angleDelta;
			float nextAngle = (i + 1) * angleDelta;

			float currentTex = i * texDelta;
			float nextTex = (i + 1) * texDelta;

			glm::vec3 currentNormal = glm::vec3(glm::cos(currentAngle), glm::sin(currentAngle), 0.0f);
			glm::vec3 nextNormal = glm::vec3(glm::cos(nextAngle), glm::sin(nextAngle), 0.0f);

			int count = vertices.size();
			glm::vec3 bottomCurrentAngle = radius * currentNormal + glm::vec3(0.0f, 0.0f, startHeight);
			glm::vec3 bottomNextAngle = radius * nextNormal + glm::vec3(0.0f, 0.0f, startHeight);;
			glm::vec3 topCurrentAngle = radius * currentNormal + glm::vec3(0.0f, 0.0f, height);
			glm::vec3 topNextAngle = radius * nextNormal + glm::vec3(0.0f, 0.0f, height);

			vertices.push_back({ bottomCurrentAngle, currentNormal, });
			vertices.push_back({ bottomNextAngle, nextNormal, });
			vertices.push_back({ topNextAngle, nextNormal, });
			vertices.push_back({ topCurrentAngle, currentNormal, });

			indices.push_back(count);
			indices.push_back(count + 1);
			indices.push_back(count + 2);
			indices.push_back(count + 2);
			indices.push_back(count + 3);
			indices.push_back(count);
		}

		return { vertices,indices };
	}

	VerticesIndicesBufferData<VertexN> PrimitiveFactory::CreateFlatDiscNormals(float radius, int uDivisionCount, float height)
	{
		std::vector<VertexN> vertices;
		std::vector<uint32_t> indices;

		float startHeight = 0.0f;

		float uAngleDelta = glm::two_pi<float>() / uDivisionCount;
		float uTexDelta = 1.0f / uDivisionCount;

		glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 center = glm::vec3(0.0f);


		vertices.push_back({ center,normal });

		for (int u = 0; u < uDivisionCount; u++)
		{
			float currentU = u * uAngleDelta;
			float nextU = (u + 1) * uAngleDelta;

			glm::vec3 current = radius * glm::vec3(
				glm::cos(currentU),
				glm::sin(currentU),
				height);

			glm::vec3 next = radius * glm::vec3(
				glm::cos(nextU),
				glm::sin(nextU),
				height);

			int size = vertices.size();
			vertices.push_back({ current, normal });
			vertices.push_back({ next, normal });

			indices.push_back(0);
			indices.push_back(size);
			indices.push_back(size + 1);
		}

		return { vertices,indices };
	}


	VerticesIndicesBufferData<VertexN> PrimitiveFactory::CreateSphereVerticesNormals(float radius, int uDivisionCount, int vDivisionCount, float height)
	{
		std::vector<VertexN> vertices;
		std::vector<uint32_t> indices;

		float startHeight = 0.0f;

		float uAngleDelta = glm::two_pi<float>() / uDivisionCount;
		float uTexDelta = 1.0f / uDivisionCount;

		float vAngleDelta = glm::two_pi<float>() / vDivisionCount;
		float vTexDelta = 1.0f / vDivisionCount;

		for (int v = 0; v < vDivisionCount; v++)
		{
			float currentV = v * vAngleDelta;
			float nextV = (v + 1) * vAngleDelta;

			float currentTexV = v * vTexDelta;
			float nextTexV = (v + 1) * vTexDelta;

			for (int u = 0; u < uDivisionCount; u++)
			{
				float currentU = u * uAngleDelta;
				float nextU = (u + 1) * uAngleDelta;

				float currentTexU = u * uTexDelta;
				float nextTexU = (u + 1) * uTexDelta;

				glm::vec3 currentUVNormal = glm::vec3(
					glm::sin(currentV) * glm::cos(currentU),
					glm::sin(currentV) * glm::sin(currentU),
					glm::cos(currentV));

				glm::vec3 currentVnextUNormal = glm::vec3(
					glm::sin(currentV) * glm::cos(nextU),
					glm::sin(currentV) * glm::sin(nextU),
					glm::cos(currentV));

				glm::vec3 nextUVNormal = glm::vec3(
					glm::sin(nextV) * glm::cos(nextU),
					glm::sin(nextV) * glm::sin(nextU),
					glm::cos(nextV));

				glm::vec3 currentUnextVNormal = glm::vec3(
					glm::sin(nextV) * glm::cos(currentU),
					glm::sin(nextV) * glm::sin(currentU),
					glm::cos(nextV));

				const glm::vec3 offset = glm::vec3(0.0f, 0.0f, height);

				int count = vertices.size();
				glm::vec3 currentUV = radius * currentUVNormal + offset;
				glm::vec3 currentVnextU = radius * currentVnextUNormal + offset;
				glm::vec3 nextUV = radius * nextUVNormal + offset;
				glm::vec3 currentUnextV = radius * currentUnextVNormal + offset;

				vertices.push_back({ currentUV, currentUVNormal });
				vertices.push_back({ currentVnextU, currentVnextUNormal });
				vertices.push_back({ nextUV, nextUVNormal });
				vertices.push_back({ currentUnextV, currentUnextVNormal });

				indices.push_back(count);
				indices.push_back(count + 1);
				indices.push_back(count + 2);
				indices.push_back(count + 2);
				indices.push_back(count + 3);
				indices.push_back(count);
			}
		}

		return { vertices,indices };
	}

	VerticesIndicesBufferData<VertexN> PrimitiveFactory::CreateHalfSphereVerticesNormals(float radius, int uDivisionCount, int vDivisionCount, float height)
	{
		std::vector<VertexN> vertices;
		std::vector<uint32_t> indices;

		float startHeight = 0.0f;

		float uAngleDelta = glm::two_pi<float>() / uDivisionCount * 0.5f;
		float uTexDelta = 1.0f / uDivisionCount;

		float vAngleDelta = glm::two_pi<float>() / vDivisionCount;
		float vTexDelta = 1.0f / vDivisionCount;

		for (int v = 0; v < vDivisionCount; v++)
		{
			float currentV = v * vAngleDelta;
			float nextV = (v + 1) * vAngleDelta;

			float currentTexV = v * vTexDelta;
			float nextTexV = (v + 1) * vTexDelta;

			for (int u = 0; u < uDivisionCount; u++)
			{
				float currentU = u * uAngleDelta;
				float nextU = (u + 1) * uAngleDelta;

				float currentTexU = u * uTexDelta;
				float nextTexU = (u + 1) * uTexDelta;

				glm::vec3 currentUVNormal = glm::vec3(
					glm::sin(currentV) * glm::cos(currentU),
					glm::sin(currentV) * glm::sin(currentU),
					glm::cos(currentV));

				glm::vec3 currentVnextUNormal = glm::vec3(
					glm::sin(currentV) * glm::cos(nextU),
					glm::sin(currentV) * glm::sin(nextU),
					glm::cos(currentV));

				glm::vec3 nextUVNormal = glm::vec3(
					glm::sin(nextV) * glm::cos(nextU),
					glm::sin(nextV) * glm::sin(nextU),
					glm::cos(nextV));

				glm::vec3 currentUnextVNormal = glm::vec3(
					glm::sin(nextV) * glm::cos(currentU),
					glm::sin(nextV) * glm::sin(currentU),
					glm::cos(nextV));


				const glm::vec3 offset = glm::vec3(0.0f, 0.0f, height);
				int count = vertices.size();
				glm::vec3 currentUV = radius * currentUVNormal + offset;
				glm::vec3 currentVnextU = radius * currentVnextUNormal + offset;
				glm::vec3 nextUV = radius * nextUVNormal + offset;
				glm::vec3 currentUnextV = radius * currentUnextVNormal + offset;

				vertices.push_back({ currentUV, currentUVNormal });
				vertices.push_back({ currentVnextU, currentVnextUNormal });
				vertices.push_back({ nextUV, nextUVNormal });
				vertices.push_back({ currentUnextV, currentUnextVNormal });

				indices.push_back(count);
				indices.push_back(count + 1);
				indices.push_back(count + 2);
				indices.push_back(count + 2);
				indices.push_back(count + 3);
				indices.push_back(count);
			}
		}

		return { vertices,indices };
	}
}

