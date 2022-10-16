#ifndef _RAY_CAST__H_
#define _RAY_CAST__H_

//
//  RayCasting.h
//  Astro Engine
//
//  Created by Unai Morentin on 24/9/21.
//  Copyright 2021 Digipen. All rights reserved.
//

namespace Engine {
	class GameObject; //fwd declaration
	namespace RayCast {
		bool trace(Ray _ray, GameObject* _go);

		float intersection_ray_aabb(Ray _ray, AABB _aabb);
		//float intersection_ray_obb(Ray _ray, OBB _obb);
		float intersection_ray_sphere(Ray _ray, Sphere _sphere);

		constexpr float cEpsilon = 1e-6f;
	}
}
#endif