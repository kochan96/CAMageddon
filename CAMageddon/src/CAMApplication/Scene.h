#pragma once
#include "Core/Base.h"
#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Rendering/VertexArray.h"

#include "Simulation/Material/Material.h"
#include "Simulation/Cutter/Cutter.h"
#include "Simulation/Simulation.h"


namespace CAMageddon
{

	struct RenderOptions
	{
		bool RenderPlane = true;
		bool RenderMaterial = true;
		bool RenderLight = true;
		bool RenderCutter = true;
		bool RenderTrajectory = true;
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
		bool IsSimulationRunning() const {return  m_Simulation->IsRunning(); }

		void StartSimulation() const { m_Simulation->Start(); }
		void PauseSimulation() const { m_Simulation->Pause(); }
		void FastForwardSimulation() const { m_Simulation->FastForward(); }

	private:
		void LoadShaders();
		void LoadTextures();

		void InitSimulation();

		void InitPlane();
		void InitMaterial();
		void InitLight();
		void InitCutter();
		void InitTrajectory(const std::vector<Instruction>& instructions);

		void RenderPlane();
		void RenderMaterial();
		void RenderLight();
		void RenderCutter();
		void RenderTrajectory();

	private:
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
	};
}