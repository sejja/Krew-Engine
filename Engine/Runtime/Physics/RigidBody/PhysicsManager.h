#ifndef _PHY_MGR__H_
#define _PHY_MGR__H_

//
//  PhysicsManager.h
//  Astro Engine
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine {

	//Solves the collision passed by CollisionSystem.cpp
	struct PhysicsManager : public Singleton<PhysicsManager>
	{
		//Constructor
		PhysicsManager() : mGravity(0, -9.8, 0), mRestitution(0.908f) {}

		void ResolveCollision(Collider* c0, Collider* c1, Contact* contact);
		void ResolveForPosition(Collider* c0, Collider* c1, Contact* contact, GameObject* g0, GameObject* g1);
		void ResolveForVelocity(Collider* c0, Collider* c1, Contact* contact);

		glm::vec3 mGravity;
		float mRestitution;
	};
}
#define GPhysics (&Engine::PhysicsManager::Instance())

#endif
