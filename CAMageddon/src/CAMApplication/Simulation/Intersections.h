#pragma once
#include <glm/glm.hpp>


namespace CAMageddon
{
	struct Sphere
	{
		glm::vec3 Center;
		float Radius;
	};

	struct  Line
	{
		glm::vec3 Origin;
		glm::vec3 Direction;
	};

	enum class IntersectionType
	{
		NoIntersection,
		Single,
		Double,
	};

	struct IntersectionLineSphere
	{
		IntersectionType Type;
		float d1;
		float d2;
	};

	class Intersections
	{
	public:
		static IntersectionLineSphere Intersect(Line line, Sphere sphere);
	};
}