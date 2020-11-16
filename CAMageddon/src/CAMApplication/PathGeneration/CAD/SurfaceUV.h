#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace CAMageddon
{
	class SurfaceUV
	{
	public:
		virtual glm::vec3 GetPointAt(float u, float v) = 0;
		virtual glm::vec3 GetTangentUAt(float u, float v) = 0;
		virtual glm::vec3 GetTangentVAt(float u, float v) = 0;

		virtual float GetMinU() const = 0;
		virtual float GetMaxU() const = 0;
		virtual float GetMinV() const = 0;
		virtual float GetMaxV() const = 0;

		virtual int GetUDivision() const = 0;
		virtual int GetVDivision() const = 0;

		virtual bool GetRollU() const { return false; }
		virtual bool GetRollV() const { return false; }

		std::vector<glm::vec3>& GetControlPoints() { return m_ControlPoints; }

	protected:
		std::vector<glm::vec3> m_ControlPoints;
	};
}