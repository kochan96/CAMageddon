#pragma once
#include <vector>
#include "Instruction.h"

namespace CAMageddon
{
	enum class CutterType
	{
		FLAT = 1, SPHERICAL = 2,
	};

	class Cutter
	{
	public:
		Cutter(const std::vector<Instruction>& instructions);

	private:
		std::vector<Instruction> m_Instructions;
	};
}