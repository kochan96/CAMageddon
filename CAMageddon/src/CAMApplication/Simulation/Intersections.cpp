#include "Intersections.h"

namespace CAMageddon
{
	IntersectionLineSphere Intersections::Intersect(Line line, Sphere sphere)
	{
		//https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
		line.Direction = glm::normalize(line.Direction);

		auto originCenter = line.Origin - sphere.Center;
		auto b = glm::dot(line.Direction, originCenter);

		float delta = b * b - (glm::dot(originCenter, originCenter) - (sphere.Radius * sphere.Radius));

		if (delta < 0)
			return { IntersectionType::NoIntersection,-1,-1 };

		IntersectionLineSphere intersection;
		if (delta == 0)
			intersection.Type = IntersectionType::Single;
		else
			intersection.Type = IntersectionType::Double;

		float delta_sqrt = std::sqrt(delta);

		float d1 = -b - delta_sqrt;
		float d2 = -b + delta_sqrt;

		if (d1 < d2)
		{
			intersection.d1 = d1;
			intersection.d2 = d2;
		}
		else
		{
			intersection.d1 = d2;
			intersection.d2 = d1;
		}

		return intersection;
	}
}
