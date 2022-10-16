#include <Shared.h>


namespace Engine {
	/* maybe in the future needs to change and have a map with the tmin and gos 
		in order to return more than 1 object if they are collapsed in the scene(cliping)*/
	bool RayCast::trace(Ray _ray, GameObject* _go) {
		auto all_objects = GWorld->GetScene()->get_all_objects();
		for (auto it = all_objects.begin(); it != all_objects.end(); it++) {
			//get iteration go's collider

			Collider* col = (*it)->GetComponentByType<Collider>();
			if (!col)
				continue;
			if (col->mColliderShape == ECollisionShape::CSHAPE_AABB) {
				float t_min = intersection_ray_aabb(_ray, AABB(*it));
				if (t_min != -1.f) {
					_go = *it;
					return true;
				}
			}
			//else if (col && col->mColliderShape == ECollisionShape::CSHAPE_OBB) {
			//	return intersection_ray_obb(_ray, OBB(*it));
			//}
			else if (col->mColliderShape == ECollisionShape::CSHAPE_CIRCLE) {
				float t_min = intersection_ray_sphere(_ray, Sphere(*it));
				if (t_min != -1.f) {
					_go = *it;
					return true;
				}
			}
		}

		return false;
	}

	float RayCast::intersection_ray_aabb(Ray _ray, AABB _aabb) {
		float tmin = 0.f;
		//give to the tmax the maximun value possible
		float tmax = FLT_MAX;
		float t1, t2;

		for (int i = 0; i < 3; i++)
		{
			if (glm::abs(_ray.dir[i]) < cEpsilon) { // Ray is parallel
				if (_ray.start[i] < _aabb.min[i] || _ray.start[i] > _aabb.max[i]) return -1.f;
			}
			else {
				// Compute intersection t value of ray with near and far plane
				float ood = 1.f / _ray.dir[i];
				t1 = (_aabb.min[i] - _ray.start[i]) * ood;
				t2 = (_aabb.max[i] - _ray.start[i]) * ood;
				//Intersect t1 with near plane and t2 with far plane
				if (t1 > t2) std::swap(t1, t2);

				// Compute the interrsection
				if (t1 > tmin) tmin = t1;
				if (t2 < tmax) tmax = t2;

				// no collision
				if (tmin > tmax) return -1.f;
			}
		}
		// check if the values are negative
		if (t1 < 0.f && t2 < 0.f)
			return -1.f;

		return tmin;
	}

	//float RayCast::intersection_ray_obb(Ray _ray, OBB _obb) {
	//	//set some paramters
	//	float t_max = 1000000.f; //very big number
	//	float t_min = 0.0f;
	//	//get world position
	//	glm::vec3 OBBworld_pos = glm::vec3(_obb.model_mtx[3].x, _obb.model_mtx[3].y, _obb.model_mtx[3].z);

	//	//compute aabb min and max
	//	glm::vec3 min = OBBworld_pos - _obb.scale /= 2;
	//	glm::vec3 max = OBBworld_pos + _obb.scale /= 2;

	//	glm::vec3 ray_to_obb = OBBworld_pos - _ray.start;

	//	// get axis
	//	std::vector<glm::vec3> axis;
	//	axis.push_back(glm::vec3(_obb.model_mtx[0].x, _obb.model_mtx[0].y, _obb.model_mtx[0].z)); //x
	//	axis.push_back(glm::vec3(_obb.model_mtx[1].x, _obb.model_mtx[1].y, _obb.model_mtx[1].z)); //y
	//	axis.push_back(glm::vec3(_obb.model_mtx[2].x, _obb.model_mtx[2].y, _obb.model_mtx[2].z)); //z

	//	//iteerate throguh the axises
	//	for (int i = 0; i < axis.size(); i++) {
	//		float e = glm::dot(axis[i], ray_to_obb);
	//		float f = glm::dot(_ray.dir, axis[i]);

	//		//sannity check
	//		if (fabs(f) < 0.001f)
	//			continue;

	//		float t1 = (e + min[i]) / f;
	//		float t2 = (e + max[i]) / f;

	//		if (t1 > t2) {
	//			float k = t1; t1 = t2; t2 = k;
	//		}

	//		if (t2 < t_max)
	//			t_max = t2;
	//		if (t1 > t_min)
	//			t_min = t1;

	//		// no collision
	//		if (t_min > t_max)
	//			return false;
	//	}

	//	//greatest t_min must be < smallest t_max
	//	// t1 is the collision distance
	//	return true;
	//}

	float RayCast::intersection_ray_sphere(Ray _ray, Sphere _sphere) {
		// a'2 t + bt + c = 0
		float a = glm::length(_ray.dir) * glm::length(_ray.dir);
		float b = 2 * glm::dot(_ray.dir, (_ray.start - _sphere.c));
		float c = (glm::length(_ray.start - _sphere.c)* glm::length(_ray.start - _sphere.c)) - _sphere.r * _sphere.r;

		//compute the top part of the second grade equation
		float topPart = b * b - (4.f * a * c);
		//check for imposible cases
		if (topPart < 0.f || a == 0.f) return -1.f;

		//compute second grade equation
		float t1 = (-b + glm::sqrt(topPart)) / (2.f * a);
		float t2 = (-b - glm::sqrt(topPart)) / (2.f * a);

		if (t1 > 0.f && t2 > 0.f)
			return glm::min(t1, t2);
		else if (t1 < 0.f && t2 < 0.f)
			return -1.f;

		if (t1 > 0.f)
			return t1;

		return t2;
	}
}