#pragma once
#include "Core/Base.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/VertexArray.h"

#include "Simulation/Material/Material.h"
#include "Simulation/Cutter/Cutter.h"
#include "Simulation/Simulation.h"

#include "Light.h"


namespace CAMageddon
{
	struct RenderOptions
	{
		bool RenderPlane = true;
		bool RenderMaterial = true;
		bool RenderLight = true;
		bool RenderCutter = true;
		bool RenderTrajectory = false;
	};

	class Scene
	{
	public:
		Scene(FPSCamera& camera);
		void Init();
		void Update(Timestep ts);
		void Render();

		RenderOptions& GetRenderOptions() { return m_RenderOptions; }

		Ref<Material> GetMaterial() const { return m_Material; }
		void SetMaterial(Ref<Material> material);

		Ref<Cutter> GetCutter() const { return m_Cutter; }
		void SetCutter(Ref<Cutter> cutter);
		void SetCutterInstructions(const std::vector<Instruction> instruction);

		bool IsSimulationReady() const { return m_Simulation->IsReady(); }
		bool IsSimulationPaused() const { return m_Simulation->IsPaused(); }
		bool IsSimulationRunning() const { return  m_Simulation->IsRunning(); }

		void StartSimulation() const { m_Simulation->Start(); }
		void PauseSimulation() const { m_Simulation->Pause(); }
		void ResumeSimulation() const { m_Simulation->Resume(); }
		void FastForwardSimulation() const { m_Simulation->FastForward(); }

		std::vector<Light>& GetLights() { return m_Lights; }

		float GetSimulationProgress() const { return m_Simulation->GetProgress(); }
		float GetSimulationSpeed() const { return m_Simulation->GetSimulationSpeed(); }
		void SetSimulationSpeed(float simulationSpeed) { m_Simulation->SetSimulationSpeed(simulationSpeed); }

		void SetPlaneHeight(float height) { m_PlaneHeight = height; }

	private:
		void LoadShaders();
		void LoadTextures();

		void InitSimulation();

		void InitPlane();
		void InitMaterial();
		void InitLight();
		void InitTrajectory(const std::vector<Instruction>& instructions);

		void RenderPlane();
		void RenderLight();
		void RenderTrajectory();

	private:
		float m_PlaneHeight = 10.0f;

		Scope<CuttingSimulation> m_Simulation;
		Ref<Material> m_Material;
		Ref<Cutter> m_Cutter;

		RenderOptions m_RenderOptions;
		FPSCamera& m_Camera;

		Ref<OpenGLVertexArray> m_PlaneVertexArray;
		Ref<OpenGLVertexArray> m_LightVertexArray;
		Ref<OpenGLVertexArray> m_CutterVertexArray;
		Ref<OpenGLVertexArray> m_TrajectoryVertexArray;
		int trajectoryCount = 0;

		std::vector<Light> m_Lights;
	};
}