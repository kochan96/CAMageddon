#include "Simulation.h"
#include "CAMApplication/Converter/MilimetersGLConverter.h"

#include "Intersections.h"


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
		m_Error = CuttingError::NONE;

		auto startPositon = MilimetersGLConverter::MilimetersToGL(m_Instructions[0].GetPosition());
		m_Cutter->SetPosition(startPositon);

		m_CurrentInstruction = 0;
		m_CutterMovementEquation.ActualTime = 0.0f;
		m_CutterMovementEquation.CurrentPosition = m_Instructions[m_CurrentInstruction].GetPosition();
		int nextInstruction = m_CurrentInstruction + 1;
		m_CutterMovementEquation.NextPosition = m_Instructions[nextInstruction].GetPosition();
		m_CutterMovementEquation.Distance = glm::distance(m_CutterMovementEquation.CurrentPosition, m_CutterMovementEquation.NextPosition);
		m_CutterMovementEquation.VelocityValue = 250.0f; //250mm/s
		m_CutterMovementEquation.Velocity = m_CutterMovementEquation.VelocityValue * glm::normalize(m_CutterMovementEquation.NextPosition - m_CutterMovementEquation.CurrentPosition);
		m_CutterMovementEquation.TotalTime = m_CutterMovementEquation.Distance / m_CutterMovementEquation.VelocityValue;
	}

	void CuttingSimulation::Pause()
	{
		m_State = SimulationState::PAUSED;
	}

	void CuttingSimulation::Resume()
	{
		if (!IsPaused())
		{
			LOG_ERROR("Try to resume not paused");
			return;
		}

		m_State = SimulationState::RUNNING;
	}

	void CuttingSimulation::Update(Timestep ts)
	{
		if (!IsRunning())
		{
			return;
		}

		glm::vec3 previousPosition = MilimetersGLConverter::GLToMilimeters(m_Cutter->GetPosition());
		glm::vec3 nextPosition = previousPosition;

		auto dt = m_DeltaTime;
		auto frameTime = ts * m_SimulationSpeed;
		while (frameTime > 0.0f && !HasError())
		{
			float deltaTime = std::min(frameTime, dt);
			nextPosition = GetNextCutterPosition(deltaTime);
			if (CheckForErrors(previousPosition, nextPosition))
			{
				break;
			}

			CutMaterial(previousPosition, nextPosition);
			frameTime = frameTime - deltaTime;
			previousPosition = nextPosition;
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

	void CuttingSimulation::CutMaterial(glm::vec3 previousPosition, glm::vec3 cutterNextPosition)
	{
		if (m_Cutter->GetType() == CutterType::FLAT)
			CutFlat(previousPosition, cutterNextPosition);
		else
			CutSphere(previousPosition, cutterNextPosition);
	}

	void CuttingSimulation::CutFlat(glm::vec3 previousPosition, glm::vec3 cutterNextPosition)
	{
		bool wentDown = cutterNextPosition.z < previousPosition.z;

		auto cutterRadius = m_Cutter->GetRadius();
		auto cutterHeight = cutterNextPosition.z;
		auto cutterTestPosition = glm::vec2(cutterNextPosition.x, cutterNextPosition.y);

		glm::vec2 leftBottom = cutterTestPosition - glm::vec2(cutterRadius);
		glm::vec2 topBottom = cutterTestPosition + glm::vec2(cutterRadius);

		auto boundingIndices = m_Material->GetIndices(leftBottom, topBottom);

		for (int row = boundingIndices.StartRow; row < boundingIndices.EndRow; row++)
		{
			for (int column = boundingIndices.StartColumn; column < boundingIndices.EndColumn; column++)
			{
				auto position = m_Material->GetPosition(row, column);
				auto position2 = position - cutterTestPosition;
				if (glm::dot(position2, position2) <= (cutterRadius * cutterRadius))
				{
					auto materialHeight = m_Material->GetHeigth(row, column);
					if (wentDown && materialHeight > cutterHeight)
					{
						m_State = SimulationState::SIMULATION_ERROR;
						m_Error = CuttingError::FLAT_CUTTER_DOWN;
						LOG_ERROR("Flat cutter down");
						return;
					}

					m_Material->SetHeight(row, column, std::min(materialHeight, cutterHeight));
				}
			}
		}
	}

	void CuttingSimulation::CutSphere(glm::vec3 previousPosition, glm::vec3 cutterNextPosition)
	{

		auto cutterRadius = m_Cutter->GetRadius();
		auto cutterHeight = cutterNextPosition.z;
		auto cutterTestPosition = glm::vec2(cutterNextPosition.x, cutterNextPosition.y);

		glm::vec2 leftBottom = cutterTestPosition - glm::vec2(cutterRadius);
		glm::vec2 topBottom = cutterTestPosition + glm::vec2(cutterRadius);

		Sphere sphere;
		sphere.Center = glm::vec3(cutterNextPosition.x, cutterNextPosition.y, cutterNextPosition.z + cutterRadius);
		sphere.Radius = cutterRadius;

		auto boundingIndices = m_Material->GetIndices(leftBottom, topBottom);

		for (int row = boundingIndices.StartRow; row < boundingIndices.EndRow; row++)
		{
			for (int column = boundingIndices.StartColumn; column < boundingIndices.EndColumn; column++)
			{
				auto position = m_Material->GetPosition(row, column);
				auto position2 = position - cutterTestPosition;
				if (glm::dot(position2, position2) <= (cutterRadius * cutterRadius))
				{
					auto materialHeight = m_Material->GetHeigth(row, column);
					Line line;
					line.Origin = glm::vec3(position.x, position.y, 0.0f);
					line.Direction = glm::vec3(0.0f, 0.0f, 1.0f);

					auto intersection = Intersections::Intersect(line, sphere);
					if (intersection.Type == IntersectionType::NoIntersection)
						continue;

					m_Material->SetHeight(row, column, std::min(intersection.d1, materialHeight));
				}
			}
		}
	}

	glm::vec3 CuttingSimulation::GetNextCutterPosition(float dt)
	{
		m_CutterMovementEquation.ActualTime += dt;

		while (m_CutterMovementEquation.ActualTime >= m_CutterMovementEquation.TotalTime)
		{
			int tmp = m_CurrentInstruction + 2;
			if (tmp >= static_cast<int>(m_Instructions.size()))
			{
				m_CurrentInstruction = m_Instructions.size() - 1;
				m_State = SimulationState::FINSHED;
				return m_CutterMovementEquation.NextPosition;
			}

			m_CurrentInstruction++;
			m_CutterMovementEquation.ActualTime = m_CutterMovementEquation.ActualTime - m_CutterMovementEquation.TotalTime;
			m_CutterMovementEquation.CurrentPosition = m_Instructions[m_CurrentInstruction].GetPosition();

			int nextInstruction = m_CurrentInstruction + 1;
			m_CutterMovementEquation.NextPosition = m_Instructions[nextInstruction].GetPosition();
			m_CutterMovementEquation.Distance = glm::distance(m_CutterMovementEquation.CurrentPosition, m_CutterMovementEquation.NextPosition);
			m_CutterMovementEquation.Velocity = m_CutterMovementEquation.VelocityValue * glm::normalize(m_CutterMovementEquation.NextPosition - m_CutterMovementEquation.CurrentPosition);
			m_CutterMovementEquation.TotalTime = m_CutterMovementEquation.Distance / m_CutterMovementEquation.VelocityValue;
		}

		return m_CutterMovementEquation.CurrentPosition + m_CutterMovementEquation.Velocity * m_CutterMovementEquation.ActualTime;
	}

	bool CuttingSimulation::CheckForErrors(glm::vec3 previousPosition, glm::vec3 nextPosition)
	{
		auto max = m_Material->GetMaxHeight() - m_Material->GetMaxDepth();
		if (nextPosition.z < max)
		{
			//Max Depth reached
			LOG_ERROR("Max Depth reached");
			m_State = SimulationState::SIMULATION_ERROR;
			m_Error = CuttingError::MAX_DEPTH_REACHED;
			return true;
		}

		return false;
	}

	void CuttingSimulation::FastForward()
	{

		glm::vec3 previousPosition = MilimetersGLConverter::GLToMilimeters(m_Cutter->GetPosition());
		glm::vec3 nextPosition = previousPosition;

		while (IsRunning() || IsPaused())
		{
			nextPosition = GetNextCutterPosition(m_DeltaTime);
			if (!CheckForErrors(previousPosition, nextPosition))
			{
				CutMaterial(previousPosition, nextPosition);
				previousPosition = nextPosition;
			}

		}

		m_Material->Update();
		m_Cutter->SetPosition(MilimetersGLConverter::MilimetersToGL(nextPosition));
	}
}

