#ifndef _GEOMTRY__H_
#define _GEOMTRY__H_

//
//  Geometry.h
//  Astro Engine
//
//  Copyright 2021 Digipen. All rights reserved.
//

namespace Engine {
	enum class classification_t {
		inside, outside, overlapping
	};

	struct Ray {
		Ray(){}
		Ray(glm::vec3 _s, glm::vec3 _e);
		void set_start(const glm::vec3& s);
		void set_end(const glm::vec3& e);
		glm::vec3 start;
		glm::vec3 end;
		glm::vec3 dir;
	};

	struct AABB {
		glm::vec3 min;
		glm::vec3 max;
		glm::vec3 pos;
		glm::vec3 sca;

		AABB(GameObject* _go);
		AABB() : min{ 0.f,0.f,0.f }, max{ 0.f, 0.f, 0.f }, pos{ 0.f, 0.f, 0.f }, sca{ 0.f, 0.f, 0.f } {}
		AABB(glm::vec3 _min, glm::vec3 _max);
	};

	struct OBB {
		OBB(GameObject* _go);
		//OBB(glm::vec3 _pos, glm::vec3 _sca, glm::vec4 _rot);
		//AABB aabb;
		
		glm::mat4 model_mtx; // no scale
		glm::vec3 scale;
	};

	struct Sphere {
		Sphere(GameObject* _go);
		Sphere(glm::vec3 _pos, float _rad);

		glm::vec3 pos;
		float rad;
	};

	struct Plane {
		glm::vec3 pos;
		glm::vec3 normal;
		float a, b, c, d; // ax + by+ cz = d
		Plane() : normal({ 0.f,0.f,0.f }), a(0.f), b(0.f), c(0.f), d(0.f){}


		classification_t classify_vs_aabb(const AABB& aabb);
		glm::vec3 closest_point_plane(glm::vec3 point);
	};

	struct Frustrum {
		Plane planes[6];
		Frustrum() {}
		Plane& operator[] (int i) {
			return planes[i];
		}

		classification_t classify_vs_aabb(const AABB& aabb);
	};

}


#endif