#pragma once
#include <glm/glm.hpp>

namespace CAMageddon
{
	enum class LightType
	{
		Point,
		Directional,
		Spot
	};

	struct Light
	{
		Light(glm::vec3 position, glm::vec3 direction, const std::string& name = "Light")
			:Position(position), Direction(direction), Name(name) {}

		std::string Name;

		LightType LightType = LightType::Point;

		glm::vec3 Position;

		//Common
		glm::vec3 Direction;

		glm::vec3 Ambient = glm::vec3(1.0f);
		glm::vec3 Diffuse = glm::vec3(1.0f);
		glm::vec3 Specular = glm::vec3(1.0f);

		//Attenuation
		bool AttenuationEnabled = true;
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.0032f;

		//SpotLight
		float CutOff = glm::cos(glm::radians(12.5f));
		float OuterCutOff = glm::cos(glm::radians(15.0f));
	};
}