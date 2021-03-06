// ConsoleApplication1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <cmath>
#include <optional>
#include <vector>

struct Vector3d
{
	float x, y, z ;

	Vector3d() : x(0), y(0), z(0) {}
	Vector3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	
	float operator*(const Vector3d& right) const
	{
		return x * right.x + y * right.y + z * right.z;
	}


	Vector3d operator*(const float q) const
	{
		return Vector3d(x*q, y*q, z *q);
	}


	Vector3d operator-(const Vector3d& right) const
	{
		return Vector3d(x - right.x, y - right.y, z - right.z);
	}


	Vector3d operator+(const Vector3d& right) const
	{
		return Vector3d(x + right.x, y + right.y, z + right.z);
	}

	void Normalize()
	{
		float dist = sqrtf(x * x + y * y + z * z);
		x = x / dist;
		y = y / dist;
		z = z / dist;
	}
};

struct Plane
{
	Vector3d normal;
	Vector3d point;

	Plane() {}
	Plane(Vector3d norm, Vector3d p) : normal(norm), point(p) { normal.Normalize(); }
};

struct LineSegment
{
	Vector3d p1;
	Vector3d p2;

	LineSegment() {}
	LineSegment(Vector3d _p1, Vector3d _p2) : p1(_p1), p2(_p2) {}
};



std::optional<Vector3d> Intersect(const Plane plane,const LineSegment linesegment)
{
	auto p1Distance = (linesegment.p1 - plane.point)*plane.normal;
	auto p2Distance = (linesegment.p2 - plane.point)*plane.normal;

	if (p1Distance*p2Distance > 0) { return std::nullopt; }//No intersection

	auto lineVector = linesegment.p2 - linesegment.p1;
	lineVector.Normalize();

	auto Angle = lineVector * plane.normal;

	//If angle is zero, then line segments lies in plante.
	//In that case, we just return random point from line. The first one, for example

	if (Angle < 0.00001) { return std::make_optional(linesegment.p1); }

	return std::make_optional(linesegment.p2 - lineVector * (p2Distance / Angle));
}

struct Cuboid
{
	Vector3d position;
	Vector3d dimensions;

	Cuboid() {}
	Cuboid(Vector3d pos, Vector3d dims) : position(pos), dimensions(dims) {}

	std::vector<LineSegment> getEdges() const
	{
		std::vector<LineSegment> result;
		//Low
		result.emplace_back(Vector3d(position), Vector3d(position.x+ dimensions.x, position.y, position.z));
		result.emplace_back(Vector3d(position), Vector3d(position.x, position.y + dimensions.y, position.z));
		result.emplace_back(Vector3d(position), Vector3d(position.x, position.y, position.z + dimensions.z));

		//High
		result.emplace_back(Vector3d(position + dimensions), Vector3d(position.x , position.y + dimensions.y, position.z + dimensions.z));
		result.emplace_back(Vector3d(position + dimensions), Vector3d(position.x + dimensions.x, position.y, position.z + +dimensions.z));
		result.emplace_back(Vector3d(position + dimensions), Vector3d(position.x + dimensions.x, position.y + dimensions.y, position.z));

		//More low
		result.emplace_back(Vector3d(position.x, position.y, position.z + dimensions.z), Vector3d(position.x + dimensions.x, position.y, position.z + dimensions.z));
		result.emplace_back(Vector3d(position.x + dimensions.x, position.y, position.z), Vector3d(position.x + dimensions.x, position.y, position.z + dimensions.z));

		//mid
		result.emplace_back(Vector3d(position.x, position.y, position.z + dimensions.z), Vector3d(position.x, position.y + dimensions.y, position.z + dimensions.z));
		result.emplace_back(Vector3d(position.x + dimensions.x, position.y, position.z), Vector3d(position.x + dimensions.x, position.y + dimensions.y, position.z));

		//More High
		result.emplace_back(Vector3d(position.x, position.y + dimensions.y, position.z), Vector3d(position.x, position.y + dimensions.y, position.z + dimensions.z));
		result.emplace_back(Vector3d(position.x, position.y + dimensions.y, position.z), Vector3d(position.x + dimensions.x, position.y + dimensions.y, position.z));


			return result;
	}
};

std::vector<Vector3d> Intersect(const Plane plane, const Cuboid cuboid)
{
	std::vector<Vector3d> result;
	auto edges = cuboid.getEdges();

	for (auto& e : edges)
	{
		auto intersectResult = Intersect(plane, e);
		if (intersectResult)
		{
			result.push_back(*intersectResult);
		}
	}

	return result;
}

int main()
{
	Cuboid Qube{ {0.0f,0.0f,0.0f}, {1.0f,1.0f,1.0f} };
	Plane Plaine(Vector3d(0.5f, 0.5f, 0.5f), Vector3d(0.5f, 0.5f, 0.5f));

	auto Intersections = Intersect(Plaine, Qube);

    return 0;
}