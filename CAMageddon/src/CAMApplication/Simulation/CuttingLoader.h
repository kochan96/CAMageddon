#pragma once
#include <filesystem>
#include "Cutter/Cutter.h"
#include "Instruction/Instruction.h"
#include <Core/Base.h>

namespace CAMageddon
{
	class CuttingLoader
	{
	public:
		static Scope<Cutter> LoadCutter(std::filesystem::path filePath);
		static std::vector<Instruction> LoadInstructions(std::filesystem::path filePath);
	private:
		static float GetDiameter(std::filesystem::path filePath);
		static CutterType GetCutterType(std::filesystem::path filePath);
		static std::vector<Instruction> GetInstructions(std::filesystem::path filePath);
		static Instruction GetInstruction(std::string rawInstruction, glm::vec3& lastPosition);
	};
}