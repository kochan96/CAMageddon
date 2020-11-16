#include "BSplinePatch.h"

namespace CAMageddon
{
	BSplinePatch::BSplinePatch(std::string name, int patchCountX, int patchCountY, int uDivisionCount, int vDivisionCount)
		: m_UDivisionCount(uDivisionCount), m_VDivisionCount(vDivisionCount), m_PatchCountX(patchCountX), m_PatchCountY(patchCountY)
	{
	}

	std::vector<glm::vec3> BSplinePatch::GetRenderingVertices() const
	{
		int rowCount = m_PatchCountY + 3;
		int verticesColumnCount = m_IsCylinder ? m_PatchCountX : m_PatchCountX + 3;
		int columnCount = m_PatchCountX + 3;
		std::vector<glm::vec3> vertices(rowCount * columnCount);

		for (int i = 0; i < rowCount; i++)
		{
			int controlPointsRow = i * verticesColumnCount;
			int row = i * columnCount;
			for (int j = 0; j < columnCount; j++)
			{
				int controlPointsIndex = controlPointsRow + j;
				int index = row + j;
				if (j >= verticesColumnCount)
				{
					controlPointsIndex -= verticesColumnCount;
				}

				vertices[index] = m_ControlPoints[controlPointsIndex];
			}
		}

		return vertices;
	}

	std::shared_ptr<BSplinePatch> BSplinePatch::CreateBSplinePatch(
		std::string name,
		std::vector<glm::vec3> controlPoints,
		int rowCount,
		int columnCount,
		int uDivisionCount,
		int vDivisionCount,
		bool isCylinder,
		bool showPolygon)
	{
		int PatchCountx = isCylinder ? columnCount : columnCount - 3;
		int PatchCounty = rowCount - 3;

		auto bSplinePatch = std::make_shared<BSplinePatch>(name, PatchCountx, PatchCounty, uDivisionCount, vDivisionCount);
		bSplinePatch->m_ControlPoints = controlPoints;
		bSplinePatch->m_IsCylinder = isCylinder;
		bSplinePatch->m_ShowPolygon = showPolygon;

		bSplinePatch->GenerateIndices(PatchCountx, PatchCounty);
		bSplinePatch->GenerateTextureCoordinates(PatchCounty + 3, PatchCountx + 3);
		bSplinePatch->GenerateGridIndices(rowCount, columnCount);

		return bSplinePatch;
	}



	std::shared_ptr<BSplinePatch> BSplinePatch::CreateRectPatch(
		std::string name,
		glm::vec3 startPosition,
		int PatchCountx,
		int PatchCounty,
		float width,
		float height,
		int uDivisionCount,
		int vDivisionCount)
	{
		auto bSplinePatch = std::make_shared<BSplinePatch>(name, PatchCountx, PatchCounty, uDivisionCount, vDivisionCount);
		bSplinePatch->m_IsCylinder = false;
		bSplinePatch->GenerateRectControlPoints(startPosition, PatchCountx, PatchCounty, width, height);
		bSplinePatch->GenerateIndices(PatchCountx, PatchCounty);
		bSplinePatch->GenerateTextureCoordinates(PatchCounty + 3, PatchCountx + 3);
		bSplinePatch->GenerateGridIndices(PatchCounty + 3, PatchCountx + 3);
		return bSplinePatch;
	}

	void BSplinePatch::GenerateRectControlPoints(glm::vec3 startPosition, int PatchCountx, int PatchCounty, float width, float height)
	{
		int verticesCountX = PatchCountx + 3;
		int verticesCountY = PatchCounty + 3;

		float deltaWidth = width / (verticesCountX - 1);
		float deltaHeight = height / (verticesCountY - 1);
		int pointCount = 0;

		for (int i = 0; i < verticesCountY; i++)
		{
			for (int j = 0; j < verticesCountX; j++)
			{
				float u = j * deltaWidth;
				float v = i * deltaHeight;

				glm::vec3 position = startPosition + glm::vec3(u, 0.0f, 0.0f) + glm::vec3(0.0f, v, 0.0f);
				m_ControlPoints.push_back(position);
			}
		}
	}

	void BSplinePatch::GenerateIndices(int PatchCountx, int PatchCounty)
	{
		int verticesCountX = PatchCountx + 3;

		for (int j = 0; j < PatchCounty; j++)
		{
			int startRow = j * verticesCountX;
			for (int i = 0; i < PatchCountx; i++)
			{
				int start = startRow + i;
				for (int k = 0; k < 4; k++)
				{
					for (int l = 0; l < 4; l++)
					{
						int index = start + k * verticesCountX + l;
						m_Indices.push_back(index);
					}
				}

				for (int k = 3; k >= 0; k--)
				{
					for (int l = 0; l < 4; l++)
					{
						int index = start + k * verticesCountX + l;
						m_Indices.push_back(index);
					}
				}

				for (int k = 0; k < 4; k++)
				{
					for (int l = 0; l < 4; l++)
					{
						int index = start + l * verticesCountX + k;
						m_Indices.push_back(index);
					}
				}

				for (int k = 3; k >= 0; k--)
				{
					for (int l = 0; l < 4; l++)
					{
						int index = start + l * verticesCountX + k;
						m_Indices.push_back(index);
					}
				}
			}
		}
	}

	void BSplinePatch::GenerateTextureCoordinates(int rowCount, int columnCount)
	{
		for (int i = 0; i < rowCount; i++)
		{
			float v = float(i) / (rowCount - 1);
			for (int j = 0; j < columnCount; j++)
			{
				float u = float(j) / (columnCount - 1);
				m_TextureCooridnates.push_back(glm::vec2(u, v));
			}
		}
	}

	std::shared_ptr<BSplinePatch> BSplinePatch::CreateCyliderPatch(std::string name, glm::vec3 center, int PatchCountx, int PatchCounty, float radius, float height, int uDivisionCount, int vDivisionCount)
	{
		auto bSplinePatch = std::make_shared<BSplinePatch>(name, PatchCountx, PatchCounty, uDivisionCount, vDivisionCount);
		bSplinePatch->m_IsCylinder = true;
		bSplinePatch->GenerateCylinderControlPoints(center, PatchCountx, PatchCounty, radius, height);
		bSplinePatch->GenerateTextureCoordinates(PatchCounty + 3, PatchCountx + 3);
		bSplinePatch->GenerateIndices(PatchCountx, PatchCounty);
		bSplinePatch->GenerateGridIndices(PatchCounty + 3, PatchCountx);
		return bSplinePatch;
	}

	void BSplinePatch::GenerateCylinderControlPoints(glm::vec3 center, int PatchCountx, int PatchCounty, float radius, float height)
	{
		int verticesCountX = PatchCountx;
		int verticesCountY = PatchCounty + 3;

		float deltaAngle = glm::two_pi<float>() / verticesCountX;

		float deltaHeight = height / (verticesCountY - 1);

		int pointCount = 0;

		for (int i = 0; i < verticesCountY; i++)
		{
			for (int j = 0; j < verticesCountX; j++)
			{
				float u = j * deltaAngle;
				float v = i * deltaHeight;

				glm::vec3 position = center;
				position.x += radius * cos(u);
				position.z += radius * sin(u);
				position.y += v;
				m_ControlPoints.push_back(position);
			}
		}
	}

	void BSplinePatch::GenerateGridIndices(int rowCount, int columnCount)
	{
		for (int i = 0; i < rowCount; i++)
		{
			for (int j = 0; j < columnCount; j++)
			{
				int row = i * columnCount;
				int index = row + j;
				if (index < row + columnCount - 1)
				{
					m_GridIndices.push_back(index);
					m_GridIndices.push_back(index + 1);
				}
				else if (m_IsCylinder)
				{
					m_GridIndices.push_back(index);
					m_GridIndices.push_back(row);
				}

				if (i < rowCount - 1)
				{
					m_GridIndices.push_back(index);
					m_GridIndices.push_back(index + columnCount);
				}
			}
		}
	}

	std::vector<uint32_t> BSplinePatch::GetPatchIndices(float u, float v)
	{
		//handle multipatch and cylinder
		const int verticesCountX = m_IsCylinder ? m_PatchCountX : m_PatchCountX + 3;
		unsigned int startRow = std::min(int(v), m_PatchCountY - 1) * verticesCountX;
		unsigned int startColumn = std::min(int(u), m_PatchCountX - 1);
		std::vector<uint32_t> indices;
		for (int k = 0; k < 4; k++)
		{
			for (int l = 0; l < 4; l++)
			{
				int index = startRow + k * verticesCountX + (l + startColumn) % verticesCountX;
				indices.push_back(index);
			}
		}

		return indices;
	}

	float BSplinePatch::Spline(float t, float ti, int n)
	{
		static const float intval = 1.0f;
		if (n == 0) {
			if (ti > t && ti - intval <= t)
				return 1;
			else
				return 0;
		}
		float val1 = Spline(t, ti, n - 1) * (t - ti + intval);
		float val2 = Spline(t, ti + intval, n - 1) * (ti + n * intval - t);
		return (val1 + val2) / (n * intval);
	}

	glm::vec4 BSplinePatch::SplineBasis(float t)
	{
		return glm::vec4(
			Spline(t, -2, 3),
			Spline(t, -1, 3),
			Spline(t, 0, 3),
			Spline(t, 1, 3));
	}

	glm::vec4 BSplinePatch::dSplineBasis(float t)
	{
		//TODO optimize

		return glm::vec4(
			Spline(t, -2, 2) - Spline(t, -1, 2),
			Spline(t, -1, 2) - Spline(t, 0, 2),
			Spline(t, 0, 2) - Spline(t, 1, 2),
			Spline(t, 1, 2) - Spline(t, 2, 2));
	}

	glm::vec3 BSplinePatch::GetPointAt(float u, float v)
	{
		v = std::clamp(v, 0.0f, 1.0f);
		u = m_IsCylinder ? u - std::floorf(u) : glm::clamp(u, 0.0f, 1.0f);

		u = u * m_PatchCountX;
		v = v * m_PatchCountY;

		auto patchIndices = GetPatchIndices(u, v);

		v -= std::min(int(v), m_PatchCountY - 1);
		u -= std::min(int(u), m_PatchCountX - 1);

		glm::vec3 point;
		auto basisU = SplineBasis(u);
		auto basisV = SplineBasis(v);


		point = basisU.x * (basisV.x * m_ControlPoints[patchIndices[0]]
			+ basisV.y * m_ControlPoints[patchIndices[4]]
			+ basisV.z * m_ControlPoints[patchIndices[8]]
			+ basisV.w * m_ControlPoints[patchIndices[12]]);

		point += basisU.y * (basisV.x * m_ControlPoints[patchIndices[1]]
			+ basisV.y * m_ControlPoints[patchIndices[5]]
			+ basisV.z * m_ControlPoints[patchIndices[9]]
			+ basisV.w * m_ControlPoints[patchIndices[13]]);

		point += basisU.z * (basisV.x * m_ControlPoints[patchIndices[2]]
			+ basisV.y * m_ControlPoints[patchIndices[6]]
			+ basisV.z * m_ControlPoints[patchIndices[10]]
			+ basisV.w * m_ControlPoints[patchIndices[14]]);

		point += basisU.w * (basisV.x * m_ControlPoints[patchIndices[3]]
			+ basisV.y * m_ControlPoints[patchIndices[7]]
			+ basisV.z * m_ControlPoints[patchIndices[11]]
			+ basisV.w * m_ControlPoints[patchIndices[15]]);

		return point;
	}

	glm::vec3 BSplinePatch::GetTangentUAt(float u, float v)
	{
		v = std::clamp(v, 0.0f, 1.0f);
		u = m_IsCylinder ? u - std::floorf(u) : glm::clamp(u, 0.0f, 1.0f);

		u = u * m_PatchCountX;
		v = v * m_PatchCountY;

		auto patchIndices = GetPatchIndices(u, v);

		v -= std::min(int(v), m_PatchCountY - 1);
		u -= std::min(int(u), m_PatchCountX - 1);

		glm::vec3 point;
		auto basisU = dSplineBasis(u);
		auto basisV = SplineBasis(v);


		point = basisU.x * (basisV.x * m_ControlPoints[patchIndices[0]]
			+ basisV.y * m_ControlPoints[patchIndices[4]]
			+ basisV.z * m_ControlPoints[patchIndices[8]]
			+ basisV.w * m_ControlPoints[patchIndices[12]]);

		point += basisU.y * (basisV.x * m_ControlPoints[patchIndices[1]]
			+ basisV.y * m_ControlPoints[patchIndices[5]]
			+ basisV.z * m_ControlPoints[patchIndices[9]]
			+ basisV.w * m_ControlPoints[patchIndices[13]]);

		point += basisU.z * (basisV.x * m_ControlPoints[patchIndices[2]]
			+ basisV.y * m_ControlPoints[patchIndices[6]]
			+ basisV.z * m_ControlPoints[patchIndices[10]]
			+ basisV.w * m_ControlPoints[patchIndices[14]]);

		point += basisU.w * (basisV.x * m_ControlPoints[patchIndices[3]]
			+ basisV.y * m_ControlPoints[patchIndices[7]]
			+ basisV.z * m_ControlPoints[patchIndices[11]]
			+ basisV.w * m_ControlPoints[patchIndices[15]]);

		return point * float(m_PatchCountX);
	}

	glm::vec3 BSplinePatch::GetTangentVAt(float u, float v)
	{
		v = std::clamp(v, 0.0f, 1.0f);
		u = m_IsCylinder ? u - std::floorf(u) : glm::clamp(u, 0.0f, 1.0f);

		u = u * m_PatchCountX;
		v = v * m_PatchCountY;

		auto patchIndices = GetPatchIndices(u, v);

		v -= std::min(int(v), m_PatchCountY - 1);
		u -= std::min(int(u), m_PatchCountX - 1);

		glm::vec3 point;
		auto basisU = SplineBasis(u);
		auto basisV = dSplineBasis(v);

		point = basisU.x * (basisV.x * m_ControlPoints[patchIndices[0]]
			+ basisV.y * m_ControlPoints[patchIndices[4]]
			+ basisV.z * m_ControlPoints[patchIndices[8]]
			+ basisV.w * m_ControlPoints[patchIndices[12]]);

		point += basisU.y * (basisV.x * m_ControlPoints[patchIndices[1]]
			+ basisV.y * m_ControlPoints[patchIndices[5]]
			+ basisV.z * m_ControlPoints[patchIndices[9]]
			+ basisV.w * m_ControlPoints[patchIndices[13]]);

		point += basisU.z * (basisV.x * m_ControlPoints[patchIndices[2]]
			+ basisV.y * m_ControlPoints[patchIndices[6]]
			+ basisV.z * m_ControlPoints[patchIndices[10]]
			+ basisV.w * m_ControlPoints[patchIndices[14]]);

		point += basisU.w * (basisV.x * m_ControlPoints[patchIndices[3]]
			+ basisV.y * m_ControlPoints[patchIndices[7]]
			+ basisV.z * m_ControlPoints[patchIndices[11]]
			+ basisV.w * m_ControlPoints[patchIndices[15]]);

		return point * float(m_PatchCountY);
	}

	float BSplinePatch::GetMinU() const
	{
		return 0.0f;
	}

	float BSplinePatch::GetMaxU() const
	{
		return 1.0f;
	}

	float BSplinePatch::GetMinV() const
	{
		return 0.0f;
	}

	float BSplinePatch::GetMaxV() const
	{
		return 1.0f;
	}
}
