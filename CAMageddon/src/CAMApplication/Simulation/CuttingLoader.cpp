#include "CuttingLoader.h"
#include <fstream>

namespace CAMageddon
{
	struct Coordinate
	{
		size_t endIndex;
		char type = '\0';
		float value = 0;
	};

	Scope<Cutter> CuttingLoader::LoadCutter(std::filesystem::path filePath)
	{
		auto cutterType = GetCutterType(filePath);
		auto diameter = GetDiameter(filePath);
		return CreateScope<Cutter>(cutterType, diameter);
	}

	std::vector<Instruction> CuttingLoader::LoadInstructions(std::filesystem::path filePath)
	{
		return GetInstructions(filePath);
	}

	float CuttingLoader::GetDiameter(std::filesystem::path filePath)
	{
		auto diameter = filePath.extension().string().substr(2);
		return atof(diameter.c_str());
	}

	CutterType CuttingLoader::GetCutterType(std::filesystem::path filePath)
	{
		auto extension = filePath.extension().string();
		return extension[1] == 'k' ? CutterType::SPHERICAL : CutterType::FLAT;
	}

	std::vector<Instruction> CuttingLoader::GetInstructions(std::filesystem::path filePath)
	{
		glm::vec3 firstPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		std::ifstream file;
		file.open(filePath);
		std::string rawInstruction;
		std::vector<Instruction> instructions;
		while (std::getline(file, rawInstruction))
		{
			instructions.push_back(GetInstruction(rawInstruction, firstPosition));
		}

		return instructions;
	}

	Coordinate GetNextCoordinate(int startIndex, std::string rawInstruction)
	{
		auto coordinateStart = rawInstruction.find_first_of("XYZ", startIndex + 1);
		if (coordinateStart == std::string::npos)
			return { std::string::npos };

		auto coordinateEnd = rawInstruction.find_first_of("XYZ", coordinateStart + 1);
		if (coordinateEnd == std::string::npos)
			coordinateEnd = rawInstruction.length();

		Coordinate coordinate;
		coordinate.endIndex = coordinateEnd;
		coordinate.type = rawInstruction[coordinateStart];
		std::string value = "";
		for (int i = coordinateStart + 1; i < coordinateEnd; i++)
			value += rawInstruction[i];

		coordinate.value = std::atof(value.c_str());

		return coordinate;
	}

	Instruction CuttingLoader::GetInstruction(std::string rawInstruction, glm::vec3& lastPosition)
	{
		int id;
		InstructionSpeedMode mode;
		const std::string fastMode = "G00";
		const std::string normalMode = "G01";

		auto indexStart = rawInstruction.find("N");
		auto indexEnd = rawInstruction.find("G", indexStart + 1);
		auto gEnd = rawInstruction.find_first_of("XYZ", indexEnd + 1);

		id = std::atoi(rawInstruction.substr(indexStart + 1, indexEnd - indexStart - 1).c_str());
		if (rawInstruction.substr(indexEnd, gEnd - indexEnd) == fastMode)
			mode = InstructionSpeedMode::FAST;
		else
			mode = InstructionSpeedMode::NORMAL;

		Coordinate coord;
		coord.endIndex = gEnd;
		while (coord.endIndex != std::string::npos)
		{
			coord = GetNextCoordinate(coord.endIndex - 1, rawInstruction);
			switch (coord.type)
			{
				case 'X':
					lastPosition.x = coord.value;
					break;
				case 'Y':
					lastPosition.y = coord.value;
					break;
				case 'Z':
					lastPosition.z = coord.value;
					break;
			}
		}

		return Instruction(id, lastPosition, mode);
	}
}
