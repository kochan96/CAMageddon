#include "MilimetersGLConverter.h"

namespace CAMageddon
{
	float MillimeterToGLScale = 0.009f;

	glm::vec2 MilimetersGLConverter::MilimetersToGL(const glm::vec2& mm_vec)
	{
		return glm::vec2(
			MilimetersToGL(mm_vec.x),
			MilimetersToGL(mm_vec.y));
	}

	glm::vec3 MilimetersGLConverter::MilimetersToGL(const glm::vec3& mm_vec)
	{
		return glm::vec3(
			MilimetersToGL(mm_vec.x),
			MilimetersToGL(mm_vec.y),
			MilimetersToGL(mm_vec.z));
	}

	float MilimetersGLConverter::MilimetersToGL(const float& mm)
	{
		return mm * MillimeterToGLScale;
	}

	glm::vec2 MilimetersGLConverter::GLToMilimeters(const glm::vec2& gl_vec)
	{
		return glm::vec2(
			GLToMilimeters(gl_vec.x),
			GLToMilimeters(gl_vec.y));
	}

	glm::vec3 MilimetersGLConverter::GLToMilimeters(const glm::vec3& gl_vec)
	{
		return glm::vec3(
			GLToMilimeters(gl_vec.x),
			GLToMilimeters(gl_vec.y),
			GLToMilimeters(gl_vec.z));
	}

	float MilimetersGLConverter::GLToMilimeters(const float& gl)
	{
		return gl / MillimeterToGLScale;
	}
}


