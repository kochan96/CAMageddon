#include "Simulation.h"
#include "CAMApplication/Converter/MilimetersGLConverter.h"


namespace CAMageddon
{
	void CuttingSimulation::SetCutter(Ref<Cutter> cutter)
	{
		if (IsRunning())
		{
			LOG_ERROR("Cutter changed during simulation");
			return;
		}

		m_Cutter = cutter;
		if (!m_Material)
			m_State == SimulationState::MATERIAL_NOT_ASSIGNED;
		else if (m_Material)
			m_Cutter->SetPosition(glm::vec3(0.0f, 0.0f, MilimetersGLConverter::MilimetersToGL(m_Material->GetHeight() + 100.0f)));
		else if (m_Instructions.empty())
			m_State == SimulationState::INSTRUCTIONS_NOT_ASSINGED;
		else
			m_State = SimulationState::READY;
	}

	void CuttingSimulation::SetMaterial(Ref<Material> material)
	{
		if (IsRunning())
		{
			LOG_ERROR("Material changed during simulation");
			return;
		}

		m_Material = material;
		if (!m_Cutter)
			m_State = SimulationState::CUTTER_NOT_ASSIGNED;
		else if (m_Cutter)
			m_Cutter->SetPosition(glm::vec3(0.0f, 0.0f, MilimetersGLConverter::MilimetersToGL(m_Material->GetHeight() + 100.0f)));
		else if (m_Instructions.empty())
			m_State = SimulationState::INSTRUCTIONS_NOT_ASSINGED;
		else
			m_State = SimulationState::READY;
	}

	void CuttingSimulation::SetInstructions(const std::vector<Instruction>& instructions)
	{
		if (IsRunning())
		{
			LOG_ERROR("Instructions changed during simulation");
			return;
		}

		m_Instructions = instructions;
		if (!m_Cutter)
			m_State = SimulationState::CUTTER_NOT_ASSIGNED;
		else if (!m_Material)
			m_State = SimulationState::MATERIAL_NOT_ASSIGNED;
		else
			m_State = SimulationState::READY;
	}

	void CuttingSimulation::Start()
	{
		if (!IsReady())
		{
			LOG_ERROR("Simulation not ready");
		}

		m_State = SimulationState::RUNNING;

		auto startPositon = MilimetersGLConverter::MilimetersToGL(m_Instructions[0].GetPosition());
		m_Cutter->SetPosition(startPositon);
	}

	void CuttingSimulation::Pause()
	{
		m_State = SimulationState::PAUSED;
	}

	void CuttingSimulation::Update(Timestep ts)
	{
		if (!IsRunning())
		{
			return;
		}

		/*const float distance = MilimetersGLConverter::MilimetersToGL(25.0f) * ts;
		auto cutterPositon = m_Cutter->GetPosition();
		cutterPositon.x += distance;

		m_Cutter->SetPosition(cutterPositon);*/
	}

	void CuttingSimulation::FastForward()
	{
		//TODO
	}
}

