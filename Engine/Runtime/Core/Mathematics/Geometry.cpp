//
//	Geometry.cpp
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

using namespace glm;

namespace Engine
{
	Ray::Ray()
		: start(vec3(0))
		, end(vec3(0))
		, dir(vec3(0))
	{}

	Ray::Ray(const vec3& _s, const vec3& _e)
		: start(_s)
		, end(_e)
		, dir(normalize(_e - _s))
	{}

	Ray::Ray(const Ray& copy)
		: start(copy.start)
		, end(copy.end)
		, dir(copy.dir)
	{}

	void Ray::set_start(const vec3& _s)
	{
		start = _s;
		dir = normalize(end - start);
	}

	void Ray::set_end(const vec3& _e)
	{
		end = _e;
		dir = normalize(end - start);
	}

	void Ray::set_dir(const vec3& _d)
	{
		dir = _d;
		end = start + dir;
	}

	AABB::AABB()
		: min(vec3(0))
		, max(vec3(0))
	{}

	AABB::AABB(GameObject* obj)
	{
		auto pos = obj->GetWorldPosition();

		auto col = obj->GetComponentByType<Collider>();
		if (col) {
			glm::vec3 half_sca = glm::vec3((obj->GetWorldScale().x * col->mTransform.mScale.x)/2.f, (obj->GetWorldScale().y * col->mTransform.mScale.y) / 2.f,
				(obj->GetWorldScale().z * col->mTransform.mScale.z) / 2.f);

			min = (pos+col->mTransform.mPos) - half_sca;
			max = (pos+col->mTransform.mPos) + half_sca;
			return;
		}

		glm::vec3 half_scale = obj->GetWorldScale() / 2.0f;
		min = pos - half_scale;
		max = pos + half_scale;
	}

	AABB::AABB(const vec3& _min, const vec3& _max)
		: min(_min)
		, max(_max)
	{}

	AABB::AABB(const AABB& copy)
		: min(copy.min)
		, max(copy.max)
	{}

	Triangle::Triangle()
		: a(vec3(0))
		, b(vec3(0))
		, c(vec3(0))
	{}
	Triangle::Triangle(const glm::vec3& _a, const glm::vec3& _b, const glm::vec3& _c)
		: a(_a)
		, b(_b)
		, c(_c)
	{}

	Triangle::Triangle(const Triangle& copy)
		: a(copy.a)
		, b(copy.b)
		, c(copy.c)
	{}

	glm::vec3 Triangle::operator[](unsigned idx)const
	{
		switch (idx)
		{
		case 0:
			return a;
		case 1:
			return b;
		case 2:
			return c;
		default:
			throw std::runtime_error("Bad triangle index");
		}
	}
	glm::vec3& Triangle::operator[](unsigned idx)
	{
		switch (idx)
		{
		case 0:
			return a;
		case 1:
			return b;
		case 2:
			return c;
		default:
			throw std::runtime_error("Bad triangle index");
		}
	}

	Sphere::Sphere()
		: c(vec3(0))
		, r(0)
	{}

	Sphere::Sphere(const vec3& _c, float _r)
		: c(_c)
		, r(_r)
	{}

	Sphere::Sphere(const Sphere& copy)
		: c(copy.c)
		, r(copy.r)
	{}

	Plane::Plane()
		: d(0)
		, n(vec3(0))
		, p(vec3(0))
	{}

	Plane::Plane(const vec4& plane)
		: d(-plane.w)
		, n(plane)
		, p(n * d)
	{}

	Plane::Plane(const vec3& _p, const vec3& _n)
		: d(dot(_p, _n))
		, n(_n)
		, p(_p)
	{}

	Plane::Plane(const Plane& copy)
		: d(copy.d)
		, n(copy.n)
		, p(copy.p)
	{}

	Plane& Plane::operator=(const Plane& copy)
	{
		d = copy.d;
		n = copy.n;
		p = copy.p;
		return *this;
	}


	Frustum::Frustum()
		: vp(mat4())
	{}

	Frustum::Frustum(const mat4& mtx)
		: vp(mtx)
	{
		Compute();
	}

	Frustum::Frustum(const Frustum& copy)
		: vp(copy.vp)
	{
		for (unsigned i = 0; i < 6; ++i)
			planes[i] = copy.planes[i];
	}

	void Frustum::Compute()
	{
		const mat4 tm = transpose(vp);

		const vec4& x = tm[0];
		const vec4& y = tm[1];
		const vec4& z = tm[2];
		const vec4& w = tm[3];

		vec4 p[6] =
		{
			-(w + x),	//left
			-(w - x),	//right
			-(w + y),	//bottom
			-(w - y),	//top
			-(w + z),	//near
			-(w - z),	//far
		};

		for (unsigned i = 0; i < 6; i++)
		{
			float l = length(vec3(p[i]));
			p[i] /= l;
		}

		for (unsigned i = 0; i < 6; ++i)
			planes[i] = p[i];
	}

	vec3 closest_point_plane(const vec3& p, const vec3& n, float d)
	{
		float t = (dot(p, n) - d) / length2(n); // Get time by formula

		return p - t * n;
	}
	glm::vec3 closest_point_plane(const glm::vec3& p, const Plane& plane)
	{
		return closest_point_plane(p, plane.n, plane.d);
	}

	void closest_segment_segment(const vec3& sA, const vec3& eA, const vec3& sB, const vec3& eB, vec3* rA, vec3* rB)
	{
		vec3 v = eA - sA; // Segment A dir
		vec3 w = eB - sB; // Segment B dir
		vec3 k = sA - sB;

		float a = dot(v, v);
		float b = dot(v, w);
		float c = dot(w, w);
		float d = dot(v, k);
		float e = dot(w, k);

		float s = 0.0f; // tA
		float t = 0.0f; // tB

		float div = (b * b - a * c);

		// Until here, treated as a line

		if (div != 0.0f) // Check if parallel
			t = clamp((b * d - a * e) / div, 0.0f, 1.0f);

		if (a != 0) // Avoid nan
			s = clamp((-d + b * t) / a, 0.0f, 1.0f);
		else
			s = clamp((-e + c * t) / b, 0.0f, 1.0f);

		if (c != 0) // Avoid nan
			t = clamp((e + s * b) / c, 0.0f, 1.0f);
		else
			t = clamp((d + a * b) / b, 0.0f, 1.0f);

		*rA = sA + v * s; // Segment A closest point
		*rB = sB + w * t; // Segment B closest point
	}

	bool overlap_point_aabb(const vec3& p, const vec3& min, const vec3& max)
	{
		return (min.x <= p.x && p.x <= max.x)
			&& (min.y <= p.y && p.y <= max.y)
			&& (min.z <= p.z && p.z <= max.z);
	}
	bool overlap_point_aabb(const glm::vec3& p, const AABB& aabb)
	{
		return overlap_point_aabb(p, aabb.min, aabb.max);
	}

	bool overlap_point_sphere(const vec3& p, const vec3& c, float r)
	{
		return r >= length(p - c);
	}
	bool overlap_point_sphere(const glm::vec3& p, const Sphere& sphere)
	{
		return overlap_point_sphere(p, sphere.c, sphere.r);
	}

	int classify_plane_point(const vec3& n, float d, const vec3& p)
	{
		float t = dot(p, n) - d; // Get time by formula

		return t * t < EPSILON ? eOVERLAPPING : t < 0 ? eINSIDE : eOUTSIDE;
	}
	int classify_plane_point(const Plane& plane, const glm::vec3& p)
	{
		return classify_plane_point(plane.n, plane.d, p);
	}

	int classify_plane_triangle(const vec3& n, float d, const vec3& a, const vec3& b, const vec3& c)
	{
		int first = classify_plane_point(n, d, a);
		if (first == eOVERLAPPING) // If first == overlap, done, so skip others
			return eOVERLAPPING;

		// first != overlap, so any diferent result will be an overlap
		if (first != classify_plane_point(n, d, b)) return eOVERLAPPING;
		if (first != classify_plane_point(n, d, c)) return eOVERLAPPING;

		return first;
	}
	int classify_plane_triangle(const Plane& plane, const Triangle& triangle)
	{
		return classify_plane_triangle(plane.n, plane.d, triangle.a, triangle.b, triangle.c);
	}

	int classify_plane_aabb(const vec3& n, float d, const vec3& min, const vec3& max)
	{
		vec3 c = (max + min) * 0.5f; // center
		vec3 h = max - c; // half size

		float s = dot(n, c) - d; // distance between aabb center and plane
		int noOverlap = s < 0 ? eINSIDE : eOUTSIDE;
		float s2 = s * s;

		if (s2 > length2(h))
			return noOverlap;

		vec3 nAbs = abs(n);
		float r = 0; // AABB projection radius
		r += nAbs.x * h.x;
		r += nAbs.y * h.y;
		r += nAbs.z * h.z;
		float r2 = r * r;

		return s2 - r2 < EPSILON ? eOVERLAPPING : noOverlap;
	}
	int classify_plane_aabb(const Plane& plane, const AABB& aabb)
	{
		return classify_plane_aabb(plane.n, plane.d, aabb.min, aabb.max);
	}

	int classify_plane_sphere(const vec3& n, float d, const vec3& c, float r)
	{
		float t = (dot(c, n) - d) / length2(n); // Get time by formula

		return (t * t - r * r) < EPSILON ? eOVERLAPPING : t < 0 ? eINSIDE : eOUTSIDE;
	}
	int classify_plane_sphere(const Plane& plane, const Sphere& sphere)
	{
		return classify_plane_sphere(plane.n, plane.d, sphere.c, sphere.r);
	}

	bool overlap_aabb_aabb(const vec3& minA, const vec3& maxA, const vec3& minB, const vec3& maxB)
	{
		return	minA.x <= maxB.x && minA.y <= maxB.y && minA.z <= maxB.z &&
				maxA.x >= minB.x && maxA.y >= minB.y && maxA.z >= minB.z;
	}
	bool overlap_aabb_aabb(const AABB& aabb1, const AABB& aabb2)
	{
		return overlap_aabb_aabb(aabb1.min, aabb1.max, aabb2.min, aabb2.max);
	}

	bool overlap_sphere_sphere(const vec3& c1, float r1, const vec3& c2, float r2)
	{
		return distance(c1, c2) <= (r1 + r2);
	}
	bool overlap_sphere_sphere(const Sphere& sphere1, const Sphere& sphere2)
	{
		return overlap_sphere_sphere(sphere1.c, sphere1.r, sphere2.c, sphere2.r);
	}

	float intersection_time_ray_plane(const vec3& s, const vec3& v, const vec3& n, float d)
	{
		float dotNV = dot(n, v);
		if (dotNV * dotNV < EPSILON) // Parallel check
			return -1.0f;

		float t = (d - dot(n, s)) / dotNV; // Get time by formula

		return t < 0 ? -1.0f : t;
	}
	float intersection_time_ray_plane(const Ray& ray, const Plane& plane)
	{
		return intersection_time_ray_plane(ray.start, ray.dir, plane.n, plane.d);
	}

	float intersection_time_ray_aabb(const vec3& s, const vec3& v, const vec3& minP, const vec3& maxP)
	{
		if (overlap_point_aabb(s, minP, maxP)) // Ray is inside
			return 0.0f;

		vec3 tMin3(0);
		vec3 tMax3(0);

		for (unsigned i = 0; i < 3; ++i)
		{
			if (v[i] == 0) // Axis parallel
			{
				// Check parallel is out
				if (s[i] < minP[i] || maxP[i] < s[i])
					return -1.0f;

				// Set to a value that is gonna be omitted
				tMin3[i] = -INFINITY;
				tMax3[i] = INFINITY;
			}
			else if (v[i] < 0) // Negative ray dir, so swap min max values
			{
				tMin3[i] = ((maxP[i] - s[i]) / v[i]);
				tMax3[i] = ((minP[i] - s[i]) / v[i]);
			}
			else // Just normal computation
			{
				tMin3[i] = ((minP[i] - s[i]) / v[i]);
				tMax3[i] = ((maxP[i] - s[i]) / v[i]);
			}
		}
		// Biggest minimum value of the time intervals
		float tMin = max(-1.0f, max(tMin3.x, max(tMin3.y, tMin3.z)));
		if (tMin < 0)
			return -1.0f;

		// Smallest maximum value of the time intervals
		float tMax = min(INFINITY, min(tMax3.x, min(tMax3.y, tMax3.z)));
		if (tMax < 0)
			return -1.0f;

		return tMin <= tMax ? tMin : -1.0f; // Check time interval correctness
	}
	float intersection_time_ray_aabb(const Ray& ray, const AABB& aabb)
	{
		return intersection_time_ray_aabb(ray.start, ray.dir, aabb.min, aabb.max);
	}

	float intersection_time_ray_sphere(const vec3& s, const vec3& v, const vec3& center, float r)
	{
		if (length(s - center) <= r) // Check inside
			return 0.0f;

		// By using a quadratic equation [Ax^2 + Bx + C = 0]

		float a = length2(v); // Wrong input
		if (a == 0)
			return -1.0f;

		vec3 cs = s - center;
		float b = 2.0f * (dot(v, cs));
		float c = length2(cs) - r * r;

		float inSqrt = b * b - 4 * a * c;
		if (inSqrt < 0)
			return -1.0f;

		float div = 2 * a;
		b *= -1;
		if (inSqrt == 0) // One result == tangent
		{
			float t = b / div;
			return t < 0 ? -1.0f : t;
		}

		inSqrt = sqrt(inSqrt);
		float t0 = (b + inSqrt) / div;
		float t1 = (b - inSqrt) / div;

		// Avoid negative time
		bool avoid0 = t0 < 0;
		bool avoid1 = t1 < 0;
		if (avoid0)
		{
			if (avoid1)
				return -1.0f;
			return t1;
		}
		if (avoid1)
			return t0;
		return min(t0, t1);
	}
	float intersection_time_ray_sphere(const Ray& ray, const Sphere& sphere)
	{
		return intersection_time_ray_sphere(ray.start, ray.dir, sphere.c, sphere.r);
	}

	float intersection_time_ray_triangle(const vec3& s, const vec3& v, const vec3& a, const vec3& b, const vec3& c)
	{
		vec3 ab(b - a);
		vec3 bc(c - b);
		vec3 n = normalize(cross(ab, bc));

		float d = dot(n, a);
		float t = (d - dot(n, s)) / dot(n, v);

		if (t < 0) // Behind ray
			return -1.0f;

		vec3 point = s + v * t;

		vec3 ap(point - a);
		if (dot(n, cross(ab, ap)) < 0) // Outside ab segment
			return -1.0f;

		vec3 bp(point - b);
		if (dot(n, cross(bc, bp)) < 0) // Outside bc segment
			return -1.0f;

		vec3 ca(a - c);
		vec3 cp(point - c);
		if (dot(n, cross(ca, cp)) < 0) // Outside ca segment
			return -1.0f;

		return t;
	}
	float intersection_time_ray_triangle(const Ray& ray, const Triangle& triangle)
	{
		return intersection_time_ray_triangle(ray.start, ray.dir, triangle.a, triangle.b, triangle.c);
	}

	int classify_frustum_sphere_naive(const vec3* n, float* d, const vec3& c, float r)
	{
		bool overlapped = false; // One or more overlaps && no out == body overlaps
		for (unsigned i = 0; i < 6; ++i)
		{
			int result = classify_plane_sphere(n[i], d[i], c, r);
			if (result == eOUTSIDE) // One out == body out
				return eOUTSIDE;
			else if (result == eOVERLAPPING)
				overlapped = true;
		}

		return overlapped ? eOVERLAPPING : eINSIDE;
	}
	int classify_frustum_aabb_naive(const Frustum& f, const Sphere& sphere)
	{
		bool overlapped = false; // One or more overlaps && no out == body overlaps
		for (unsigned i = 0; i < 6; ++i)
		{
			int result = classify_plane_sphere(f.planes[i], sphere);
			if (result == eOUTSIDE) // One out == body out
				return eOUTSIDE;
			else if (result == eOVERLAPPING)
				overlapped = true;
		}

		return overlapped ? eOVERLAPPING : eINSIDE;
	}

	int classify_frustum_aabb_naive(const vec3* n, float* d, const vec3& min, const vec3& max)
	{
		bool overlapped = false; // One or more overlaps && no out == body overlaps
		for (unsigned i = 0; i < 6; ++i)
		{
			int result = classify_plane_aabb(n[i], d[i], min, max);
			if (result == eINSIDE)
				continue;
			if (result == eOUTSIDE) // One out == body out
				return eOUTSIDE;
			overlapped = true;
		}

		return overlapped ? eOVERLAPPING : eINSIDE;
	}

	int classify_frustum_aabb_naive(const Frustum& f, const AABB& bv)
	{
		bool overlapped = false; // One or more overlaps && no out == body overlaps
		for (unsigned i = 0; i < 6; ++i)
		{
			int result = classify_plane_aabb(f.planes[i], bv);
			if (result == eINSIDE)
				continue;
			if (result == eOUTSIDE) // One out == body out
				return eOUTSIDE;
			overlapped = true;
		}

		return overlapped ? eOVERLAPPING : eINSIDE;
	}
}
		