#include "Simulation.h"
#include "CAMApplication/Converter/MilimetersGLConverter.h"


namespace CAMageddon
{
	CuttingSimulation::CuttingSimulation()
	{
		m_CurrentInstruction = 0;
	}

	void CuttingSimulation::SetCutter(Ref<Cutter> cutter)
	{
		if (IsRunning())
		{
			LOG_ERROR("Cutter changed during simulation");
			return;
		}

		m_Cutter = cutter;
		if (!m_Material)
			m_State = SimulationState::MATERIAL_NOT_ASSIGNED;
		else if (m_Material)
			m_Cutter->SetPosition(glm::vec3(0.0f, 0.0f, MilimetersGLConverter::MilimetersToGL(m_Material->GetMaxHeight() + 100.0f)));
		else if (m_Instructions.empty())
			m_State = SimulationState::INSTRUCTIONS_NOT_ASSINGED;
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
			m_Cutter->SetPosition(glm::vec3(0.0f, 0.0f, MilimetersGLConverter::MilimetersToGL(m_Material->GetMaxHeight() + 100.0f)));
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
			return;
		}

		m_State = SimulationState::RUNNING;

		auto startPositon = MilimetersGLConverter::MilimetersToGL(m_Instructions[0].GetPosition());
		m_Cutter->SetPosition(startPositon);

		m_CurrentInstruction = 0;
		m_CutterMovementEquation.ActualTime = 0.0f;
		m_CutterMovementEquation.CurrentPosition = m_Instructions[m_CurrentInstruction].GetPosition();
		m_CutterMovementEquation.NextPosition = m_Instructions[m_CurrentInstruction + 1].GetPosition();
		m_CutterMovementEquation.Distance = glm::distance(m_CutterMovementEquation.CurrentPosition, m_CutterMovementEquation.NextPosition);
		m_CutterMovementEquation.VelocityValue = 250.0f;//250.0f; //250mm/s
		m_CutterMovementEquation.Velocity = m_CutterMovementEquation.VelocityValue * glm::normalize(m_CutterMovementEquation.NextPosition - m_CutterMovementEquation.CurrentPosition);
		m_CutterMovementEquation.TotalTime = m_CutterMovementEquation.Distance / m_CutterMovementEquation.VelocityValue;
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

		glm::vec3 previousPosition = MilimetersGLConverter::GLToMilimeters(m_Cutter->GetPosition());
		glm::vec3 nextPosition = previousPosition;

		auto dt = m_DeltaTime * m_SimulationSpeed * 0.5f;
		auto frameTime = ts;
		while (frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime.getSeconds(), dt);
			nextPosition = GetNextCutterPosition(deltaTime);
			CutMaterial(previousPosition, nextPosition);
			frameTime = frameTime.getSeconds() - deltaTime;
		}

		m_Material->Update();
		m_Cutter->SetPosition(MilimetersGLConverter::MilimetersToGL(nextPosition));
	}

	float CuttingSimulation::GetProgress() const
	{
		if (m_Instructions.empty())
			return 0;

		return (float)m_CurrentInstruction / (m_Instructions.size() - 1);
	}

	void CuttingSimulation::CutMaterial(glm::vec3 cutterPreviousPosition, glm::vec3 cutterNextPosition)
	{
		int rowCount = m_Material->GetRowCount();
		int columnCount = m_Material->GetColumnCount();

		auto cutterTestPosition = glm::vec2(cutterNextPosition.x, cutterNextPosition.y);
		auto cutterHeight = cutterNextPosition.z;
		auto cutterRadius = m_Cutter->GetRadius();

		for (int row = 0; row < rowCount; row++)
		{
			for (int column = 0; column < columnCount; column++)
			{
				auto position = m_Material->GetPosition(row, column);
				auto position2 = position - cutterTestPosition;
				if (glm::dot(position2, position2) <= (cutterRadius * cutterRadius))
				{
					auto materialHeight = m_Material->GetHeigth(row, column);
					m_Material->SetHeight(row, column, std::min(materialHeight, cutterHeight));
				}
			}
		}
	}

	glm::vec3 CuttingSimulation::GetNextCutterPosition(float dt)
	{
		m_CutterMovementEquation.ActualTime += dt;

		while (m_CutterMovementEquation.ActualTime >= m_CutterMovementEquation.TotalTime)
		{
			if (m_CurrentInstruction + 2 >= m_Instructions.size())
			{
				m_CurrentInstruction = m_Instructions.size() - 1;
				m_State = SimulationState::FINSHED;
				return m_CutterMovementEquation.NextPosition;
			}

			m_CurrentInstruction++;
			m_CutterMovementEquation.ActualTime = m_CutterMovementEquation.ActualTime - m_CutterMovementEquation.TotalTime;
			m_CutterMovementEquation.CurrentPosition = m_Instructions[m_CurrentInstruction].GetPosition();
			m_CutterMovementEquation.NextPosition = m_Instructions[m_CurrentInstruction + 1].GetPosition();
			m_CutterMovementEquation.Distance = glm::distance(m_CutterMovementEquation.CurrentPosition, m_CutterMovementEquation.NextPosition);
			m_CutterMovementEquation.Velocity = m_CutterMovementEquation.VelocityValue * glm::normalize(m_CutterMovementEquation.NextPosition - m_CutterMovementEquation.CurrentPosition);
			m_CutterMovementEquation.TotalTime = m_CutterMovementEquation.Distance / m_CutterMovementEquation.VelocityValue;
		}

		return m_CutterMovementEquation.CurrentPosition + m_CutterMovementEquation.Velocity * m_CutterMovementEquation.ActualTime;
	}

	void CuttingSimulation::FastForward()
	{
		//TODO
	}


}

