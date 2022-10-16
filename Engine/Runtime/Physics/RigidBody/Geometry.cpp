#include "Shared.h"

namespace Engine {
	Ray::Ray(glm::vec3 _s, glm::vec3 _e) : start(_s), end(_e), dir(end - start) {
		dir = glm::normalize(dir);
	}

	void Ray::set_start(const glm::vec3& s)
	{
		start = s;
		dir = end - start;
		dir = glm::normalize(dir);
	}

	void Ray::set_end(const glm::vec3& e)
	{
		end = e;
		dir = end - start;
		dir = glm::normalize(dir);
	}

	AABB::AABB(GameObject* _go) {
		pos = _go->GetWorldPosition();
		sca = _go->GetWorldScale();

		glm::vec3 half_scale = { sca.x / 2, sca.y / 2, sca.z / 2 };
		min = pos - half_scale;
		max = pos + half_scale;
	}

	AABB::AABB(glm::vec3 _min, glm::vec3 _max) : min(_min), max(_max){
		pos = glm::vec3((_max.x + _min.x) / 2, (_max.y + _min.y) / 2, (_max.z + _min.z) / 2);
		sca = glm::vec3(glm::abs(_max.x - _min.x), glm::abs(_max.y - _min.y), glm::abs(_max.z - _min.z));
	}

	OBB::OBB(GameObject* _go) :
		// ignore the scale, only T*R 
		model_mtx(_go->GetTransform().GetModelMtx_noScale()),
		scale(_go->GetScale()) {
	}

	Sphere::Sphere(GameObject* _go) : pos(_go->GetWorldPosition()), rad(_go->GetScale().x / 2) {
	}

	Sphere::Sphere(glm::vec3 _pos, float _rad) : pos(_pos), rad(_rad) {
	
	}

	glm::vec3 Plane::closest_point_plane(glm::vec3 point) {
		//compute the distance
		float distance = glm::dot(normal, point) - d;
		return point - distance * normal;
	}

	classification_t Plane::classify_vs_aabb(const AABB& aabb) {
		// Compute AABB positive extents
		glm::vec3 extent = aabb.max - aabb.pos;
		//introduce in the plane equation the aabb position
		float r = extent.x * glm::abs(normal.x) + extent.y * glm::abs(normal.y) +
			extent.z * glm::abs(normal.z);
		//Compute distance from plane to the position of the box
		float distance = glm::dot(normal, aabb.pos) - d;

		if (glm::abs(distance) <= r)
			return classification_t::overlapping;
		//get closest point from the plane to the point
		glm::vec3 closestPoint = closest_point_plane(aabb.pos);
		float pos = glm::dot(normal, aabb.pos - closestPoint);
		//check in which side of the plane the point is
		if (pos > 0)
			return classification_t::inside;
		else
			return classification_t::outside;
	}

	classification_t Frustrum::classify_vs_aabb(const AABB& aabb) {
		classification_t cPlanes[6];
		bool equalClassification = true;

		// perform plane-aabb test with all the frustum planes
		for (int i = 0; i < 6; i++)
		{
			cPlanes[i] = planes[i].classify_vs_aabb(aabb);

			// check if the classification is different
			if (cPlanes[0] != cPlanes[i])
				equalClassification = false;
		}

		// if all the planes share the same classifcation respect to the abb
		if (equalClassification) return cPlanes[0];

		int count = 0;

		// compute the number of planes that are outside
		for (int i = 0; i < 6; i++) {
			if (cPlanes[i] == classification_t::outside)
				count++;
		}

		//if there is not any plane outside
		if (count == 0)
			return classification_t::overlapping;

		return classification_t::outside;
	}
}