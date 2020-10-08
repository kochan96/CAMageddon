#pragma once
#include "Core/Base.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/VertexArray.h"

#include "Simulation/Material/Material.h"
#include "Simulation/Cutter/Cutter.h"


namespace CAMageddon
{

	struct RenderOptions
	{
		bool RenderPlane = true;
		bool RenderMaterial = true;
		bool RenderLight = true;
		bool RenderCutter = true;
	};

	class Scene
	{
	public:
		Scene(FPSCamera& camera);
		void Init();
		void Update(Timestep ts);
		void Render();

		RenderOptions& GetRenderOptions() { return m_RenderOptions; }

	private:
		void LoadShaders();
		void LoadTextures();

		void InitPlane();
		void InitMaterial();
		void InitLight();
		void InitCutter();

		void RenderPlane();
		void RenderMaterial();
		void RenderLight();
		void RenderCutter();

	private:
		Scope<Material> m_Material;
		Scope<Cutter> m_Cutter;

		RenderOptions m_RenderOptions;
		FPSCamera& m_Camera;

		Ref<OpenGLVertexArray> m_PlaneVertexArray;
		Ref<OpenGLVertexArray> m_LightVertexArray;
		Ref<OpenGLVertexArray> m_CutterVertexArray;
	};
}