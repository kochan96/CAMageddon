#pragma once
#include <vector>
#include <Rendering/VertexArray.h>
#include <Rendering/Shader.h>
#include <CAMApplication/Light.h>

namespace CAMageddon
{
	enum class CutterType
	{
		FLAT = 1, SPHERICAL = 2,
	};

	class FPSCamera;

	class Cutter
	{
	public:
		Cutter(CutterType type, float diameter);
		glm::vec3 GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }

		float GetDiameter() const { return m_Diameter; }
		float GetRadius() const { return m_Diameter / 2.0f; }

		void SetDiameter(float diameter) { m_Diameter = diameter; Init(); }

		void Render(const FPSCamera& camera, std::vector<Light> lights);

		CutterType GetType() const { return m_Type; }
		void SetType(const CutterType& type) { m_Type = type; Init(); }

	private:
		void Init();
		void InitFlat();
		void InitSphere();

	private:
		float m_Diameter;
		CutterType m_Type;
		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };

		Ref<OpenGLVertexArray> m_CutterHandleVertexArray;
		Ref<OpenGLVertexArray> m_CutterEndVertexArray;
	};
}