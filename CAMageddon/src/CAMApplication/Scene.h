#pragma once
#include "Core/Base.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/VertexArray.h"

namespace CAMageddon
{

	class Scene
	{
	public:
		Scene(FPSCamera& camera);
		void Init();
		void Update(Timestep ts);
		void Render();

	private:
		void LoadShaders();
		void InitPlane();
		void InitMaterial();
		void InitLight();

		void RenderPlane();
		void RenderMaterial();
		void RenderLight();

	private:
		FPSCamera& m_Camera;

		Ref<OpenGLVertexArray> m_PlaneVertexArray;
		Ref<OpenGLShader> m_PlaneShader;

		Ref<OpenGLVertexArray> m_MaterialVertexArray;
		Ref<OpenGLShader> m_MaterialShader;

		Ref<OpenGLVertexArray> m_LightVertexArray;
		Ref<OpenGLShader> m_LightShader;
	};
}