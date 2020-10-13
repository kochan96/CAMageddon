#pragma once
#include "Core/Base.h"
#include "Core/Timestep.h"
#include "Cutter/Cutter.h"
#include "Material/Material.h"
#include "Instruction/Instruction.h"

namespace CAMageddon
{
	enum class SimulationState
	{
		READY = 0, RUNNING = 1, PAUSED = 3, FINSHED = 2, SIMULATION_ERROR = 3, CUTTER_NOT_ASSIGNED = 4, MATERIAL_NOT_ASSIGNED = 5, INSTRUCTIONS_NOT_ASSINGED = 6,
	};

	enum class CuttingError
	{
		NONE = 0, MAX_DEPTH_REACHED = 1, FLAT_CUTTER_DOWN = 2
	};


	struct CutterMovementEquation
	{
		glm::vec3 CurrentPosition = { 0.0f,0.0f,0.0f };
		glm::vec3 NextPosition = { 0.0f,0.0f,0.0f };
		glm::vec3 Velocity = { 0.0f,0.0f,0.0f }; //next-curent normalize;

		float VelocityValue = 250;  //250mm/s
		float Distance = 0.0f;
		float TotalTime = 0.0f; // distance/velocity
		float ActualTime = 0.0f;
	};

	class CuttingSimulation
	{
	public:
		CuttingSimulation();

		void SetCutter(Ref<Cutter> cutter);
		void SetMaterial(Ref<Material> material);
		void SetInstructions(const std::vector<Instruction>& instructions);

		void Start();
		void Pause();
		void Update(Timestep ts);
		void FastForward();

		float GetSimulationSpeed() const { return m_SimulationSpeed; }
		void SetSimulationSpeed(float simulationSpeed) { m_SimulationSpeed = simulationSpeed; }

		bool IsReady() const { return m_State == SimulationState::READY; }
		bool IsRunning() const { return m_State == SimulationState::RUNNING; }
		bool IsPaused() const { return m_State == SimulationState::PAUSED; }

		bool HasError() const { return m_State == SimulationState::SIMULATION_ERROR; }
		CuttingError GetError() const { return m_Error; }

		float GetProgress() const;

	private:
		void CutMaterial(glm::vec3 cutterPreviousPosition, glm::vec3 cutterNextPosition);
		glm::vec3 GetNextCutterPosition(float dt); //milimeters

	private:
		Ref<Cutter> m_Cutter;
		Ref<Material> m_Material;

		int m_CurrentInstruction = 0;
		std::vector<Instruction> m_Instructions;
		CutterMovementEquation m_CutterMovementEquation;

		CuttingError m_Error = CuttingError::NONE;
		SimulationState m_State = SimulationState::MATERIAL_NOT_ASSIGNED;

		float m_Time = 0.0f;
		float m_DeltaTime = 0.1f;
		float m_SimulationSpeed = 1.0f;
	};
}