#pragma once
#include "campch.h"
#include <glm/glm.hpp>

namespace CAMageddon
{
	enum class InstructionSpeedMode
	{
		NORMAL = 1, FAST = 2
	};

	class Instruction
	{
	public:
		Instruction(int id, const glm::vec3& position, InstructionSpeedMode mode = InstructionSpeedMode::NORMAL);
		~Instruction();

		int GetId() const { return m_Id; }
		InstructionSpeedMode GetSpeedMode() const { return m_SpeedMode; }
		const glm::vec3& GetPosition() const { return m_Position; }
		std::string GetRawInstruction() const { return m_RawInstructionString; }

		std::string ToString();
	private:
		std::string ConstructRaw(int id, const glm::vec3& position, InstructionSpeedMode mode);
		std::string ToStringWithPrecision(float d);

	private:
		int m_Id;
		glm::vec3 m_Position;
		InstructionSpeedMode m_SpeedMode;

		std::string m_RawInstructionString;
	};
}