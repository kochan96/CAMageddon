#include "PathGenerator.h"
#include <fstream>
#include <iomanip>

#include <glm/gtc/matrix_transform.hpp>

namespace CAMageddon
{
	const std::string CombinePath(const std::string folder, const std::string path)
	{
		return folder + path;
	}

	void AddCommand(std::ofstream& ofstream, int index, float x, float y, float z)
	{
		ofstream << "N" << index;
		ofstream << "G01";
		ofstream << std::fixed << std::setprecision(3);
		ofstream << "X" << x;
		ofstream << "Y" << y;
		ofstream << "Z" << z;
		ofstream << std::endl;
	}

	void SaveToFile(
		std::vector<glm::vec3> moves,
		const std::string& filePath,
		int startIndex)
	{
		int index = startIndex;
		std::ofstream ofstream(filePath, std::fstream::out | std::fstream::trunc);
		if (ofstream.is_open())
		{
			for (auto move : moves)
			{
				AddCommand(ofstream, index++, move.x, move.y, move.z);
			}

			ofstream.close();
		}
		else
		{
			LOG_ERROR("Error generating first path {}", filePath);
		}
	}

	PathGenerator::PathGenerator(
		const std::string& folder,
		const MaterialSpecifiaction& materialSpecification,
		std::shared_ptr<Material> material,
		std::vector<std::shared_ptr<SurfaceUV>> surfaces)
		:m_Folder(folder), m_MaterialSpecification(materialSpecification), m_Surfaces(surfaces)
	{
		safeZ = m_MaterialSpecification.SizeZ + 20.0f;
		minZ = m_MaterialSpecification.SizeZ - m_MaterialSpecification.MaxDepth;

		m_Material = material;
	}

	void PathGenerator::GenerateFirstPath(
		const std::string& filePath,
		int startIndex)
	{
		auto path = CombinePath(m_Folder, filePath);
		GenerateHeightMap();
		std::vector<glm::vec3> moves = GenerateFirstPathLayer((m_MaterialSpecification.SizeZ + minZ) / 2);
		std::vector<glm::vec3> moves2 = GenerateFirstPathLayer(minZ + 1);
		moves.insert(moves.end(), moves2.begin(), moves2.end());
		SaveToFile(moves, path, startIndex);
	}

	glm::ivec2 PathGenerator::GetIndices(glm::vec2 position)
	{
		float x = position.x;
		float y = position.y;

		float u = x / m_MaterialSpecification.SizeX + 0.5f;
		float v = y / m_MaterialSpecification.SizeY + 0.5f;

		int column = u * (m_MaterialSpecification.PrecisionX - 1);
		int row = v * (m_MaterialSpecification.PrecisionY - 1);

		return { column,row };
	}

	glm::vec2 PathGenerator::GetPosition(int row, int column)
	{
		float deltaWidth = 1.0f / (m_MaterialSpecification.PrecisionX - 1);
		float deltaHeight = 1.0f / (m_MaterialSpecification.PrecisionY - 1);

		float u = column * deltaWidth;
		float v = row * deltaHeight;

		float x = (2 * u - 1) * m_MaterialSpecification.SizeX / 2.0f;
		float y = (2 * v - 1) * m_MaterialSpecification.SizeY / 2.0f;

		return { x,y };
	}

	float PathGenerator::GetZ(float cpx, float cpy, float toolRadius)
	{
		float rangeSq = toolRadius * toolRadius;
		glm::vec3 currentPosition{ cpx, cpy, 0 };

		int left, right, top, bottom;
		GetIndicesOfArea(currentPosition, toolRadius, left, right, top, bottom);

		float result = minZ;

		for (int i = bottom; i < top; i++)
		{
			for (int j = left; j < right; j++)
			{
				glm::vec2 pos = GetPosition(i, j);
				float x = currentPosition.x - pos.x;
				float y = currentPosition.y - pos.y;

				if (x * x + y * y > rangeSq)
					continue;

				result = std::max(result, m_HeightMap[i][j]);
			}
		}

		return result;
	}

	void PathGenerator::GetIndicesOfArea(glm::vec3 position, float range, int& left, int& right, int& top, int& bottom)
	{
		left = 0;
		right = 0;
		top = 0;
		bottom = 0;

		glm::vec2 leftBottom = { position.x - range, position.y - range };
		glm::vec2 rightTop = { position.x + range,position.y + range };

		int maxRow = m_MaterialSpecification.PrecisionY - 1;
		int maxColumn = m_MaterialSpecification.PrecisionX - 1;

		float halfWidth = m_MaterialSpecification.SizeX / 2.0f;
		float halfHeight = m_MaterialSpecification.SizeY / 2.0f;

		int startRow = (leftBottom.y / m_MaterialSpecification.SizeY + 0.5f) * maxRow - 0.5f;
		int endRow = (rightTop.y / m_MaterialSpecification.SizeY + 0.5f) * maxRow + 0.5f;

		if (endRow < 0 || startRow >= m_MaterialSpecification.PrecisionY)
			return;

		int startColumn = (leftBottom.x / m_MaterialSpecification.SizeX + 0.5f) * maxColumn - 0.5f;
		int endColumn = (rightTop.x / m_MaterialSpecification.SizeX + 0.5f) * maxColumn + 0.5f;

		if (endColumn < 0 || startColumn >= m_MaterialSpecification.PrecisionX)
			return;


		startColumn = std::max(startColumn, 0);
		endColumn = std::min(endColumn, maxColumn + 1);

		startRow = std::max(startRow, 0);
		endRow = std::min(endRow, maxRow + 1);

		left = startColumn;
		right = endColumn;
		top = endRow;
		bottom = startRow;
	}

	void PathGenerator::GenerateHeightMap()
	{
		m_HeightMap.resize(m_MaterialSpecification.PrecisionY);
		for (int i = 0; i < m_MaterialSpecification.PrecisionY; i++)
			m_HeightMap[i] = std::vector<float>(m_MaterialSpecification.PrecisionX, minZ);

		float deltaV = 1.0f / (m_MaterialSpecification.PrecisionY - 1);
		float deltaU = 1.0f / (m_MaterialSpecification.PrecisionX - 1);

		float halfWidth = m_MaterialSpecification.SizeX / 2.0f;
		float halfHeight = m_MaterialSpecification.SizeY / 2.0f;


		glm::vec3 translate = glm::vec3(m_MaterialSpecification.SizeX / 2.0f, m_MaterialSpecification.SizeY / 2.0f, 0.0f);

		for (int vCount = 0; vCount < m_MaterialSpecification.PrecisionY; vCount++)
		{
			float currentV = vCount * deltaV;
			for (int uCount = 0; uCount < m_MaterialSpecification.PrecisionX; uCount++)
			{
				for (auto& surface : m_Surfaces)
				{
					float currentU = uCount * deltaU;
					auto point = surface->GetPointAt(currentU, currentV);

					auto indices = GetIndices(point);

					int x = indices.x;
					int y = indices.y;

					if (x < m_MaterialSpecification.PrecisionX
						&& y < m_MaterialSpecification.PrecisionY
						&& x >= 0 && y >= 0)
					{
						m_HeightMap[x][y] = std::max(m_HeightMap[x][y], point.z);
					}
				}
			}
		}

		for (int vCount = 0; vCount < m_MaterialSpecification.PrecisionY; vCount++)
		{
			for (int uCount = 0; uCount < m_MaterialSpecification.PrecisionX; uCount++)
			{
				m_Material->SetHeight(vCount, uCount, m_HeightMap[vCount][uCount]);
			}
		}

		m_Material->Update();
	}

	std::vector<glm::vec3> PathGenerator::GenerateFirstPathLayer(float minZ)
	{
		std::vector<glm::vec3> path;

		//k16
		float xoff = 10;
		float yoff = 3;

		glm::vec2 bound = { m_MaterialSpecification.SizeX / 2.0f, m_MaterialSpecification.SizeY / 2.0f };

		path.push_back({ -bound.x, -bound.y - yoff, safeZ });

		float x, y;
		bool reversed = false;
		for (x = -bound.x; x < bound.x + xoff; x += xoff)
		{
			std::vector<glm::vec3> subPath;

			float prevZ = minZ;
			subPath.push_back({ x, -bound.y - yoff, minZ });

			for (y = -bound.y; y < bound.y + yoff; y += yoff)
			{
				float z = std::max(minZ, GetZ(x, y, 8));

				if (prevZ == z)
					continue;

				if (z < prevZ) {
					subPath.push_back({ x, y , prevZ });
					subPath.push_back({ x, y , z });
				}
				if (z > prevZ) {
					subPath.push_back({ x, y - yoff , prevZ });
					subPath.push_back({ x, y - yoff , z });
				}

				prevZ = z;
			}

			subPath.push_back({ x, y, minZ });

			if (reversed)
				path.insert(path.end(), subPath.rbegin(), subPath.rend());
			else
				path.insert(path.end(), subPath.begin(), subPath.end());

			reversed = !reversed;
		}

		path.push_back({ x, y, safeZ });


		return path;
	}
}

