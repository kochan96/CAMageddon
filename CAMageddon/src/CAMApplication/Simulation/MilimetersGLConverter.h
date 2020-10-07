#pragma once
#include <glm/glm.hpp>

namespace CAMageddon
{
	class MilimetersGLConverter
	{
	public:
		static glm::vec2 MilimetersToGL(const glm::vec2& mm_vec);
		static glm::vec3 MilimetersToGL(const glm::vec3& mm_vec);
		static float MilimetersToGL(const float& mm);

		static glm::vec2 GLToMilimeters(const glm::vec2& gl_vec);
		static glm::vec3 GLToMilimeters(const glm::vec3& gl_vec);
		static float GLToMilimeters(const float& gl);
	};
}