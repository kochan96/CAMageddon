#include "Instruction.h"
#include <iomanip>

namespace CAMageddon
{
	Instruction::Instruction(int id, const glm::vec3& position, InstructionSpeedMode mode)
		: m_Id(id), m_Position(position), m_SpeedMode(mode)
	{
		m_RawInstructionString = ConstructRaw(m_Id, m_Position, m_SpeedMode);
	}

	Instruction::~Instruction()
	{
	}

	std::string Instruction::ToString()
	{
		std::stringstream ss;
		ss << "ID: " << GetId() << std::endl;
		const glm::vec3& pos = GetPosition();
		ss << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
		InstructionSpeedMode speed_modee = GetSpeedMode();
		if (speed_modee == InstructionSpeedMode::FAST)
			ss << "Speed Mode: " << "Fast" << std::endl;
		else
			ss << "Speed Mode: " << "Normal" << std::endl;
		return ss.str();
	}


	std::string Instruction::ConstructRaw(int id, const glm::vec3& position, InstructionSpeedMode mode)
	{
		std::stringstream ss;
		std::string speed = mode == InstructionSpeedMode::NORMAL ? "G01" : "G00";
		ss << "N" << id
			<< speed
			<< "X" << ToStringWithPrecision(position.x)
			<< "Y" << ToStringWithPrecision(position.y)
			<< "Z" << ToStringWithPrecision(position.z);
		
		return ss.str();
	}

	std::string Instruction::ToStringWithPrecision(float d)
	{
		std::stringstream ss;
		const int precision = 3;
		ss << std::fixed << std::setprecision(3) << d;

		return ss.str();
	}
}
