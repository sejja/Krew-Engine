#include <Shared.h>
//
//  PhysicsManager.cpp
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine {

	// ------------------------------------------------------------------------
	/*! ResolveCollsion
	*
	*   Resolves the collision by using the contact data filled previously
	*/ //----------------------------------------------------------------------
	void PhysicsManager::ResolveCollision(Collider* c0, Collider* c1, Contact* contact) 
	{
		//Get their owners
		GameObject* go0 = c0->GetOwner();
		GameObject* go1 = c1->GetOwner();

		//If both objects have RigidBody and at least one of them is Dynamic
		if (c0->mRigidBody && c1->mRigidBody && (c0->mRigidBody->m_inv_mass || c1->mRigidBody->m_inv_mass))
		{
			//Resolve the position and velocity of each object
			ResolveForPosition(c0, c1, contact, go0, go1);
			//ResolveForVelocity(c0, c1, contact);
		}
	}

	// ------------------------------------------------------------------------
	/*! ResolveForPosition
	*
	*   Resolves the position of each object
	*/ //----------------------------------------------------------------------
	void PhysicsManager::ResolveForPosition(Collider* c0, Collider* c1, Contact* contact, GameObject* g0, GameObject* g1) 
	{
		//Get previous positions
		glm::vec3 obj0_pos = g0->GetPosition();
		float Y0 = g0->GetPosition().y;

		glm::vec3 obj1_pos = g1->GetPosition();
		float Y1 = g1->GetPosition().y;

		//Get each inverse mass and sum them
		float total_mass = c0->mRigidBody->m_inv_mass + c1->mRigidBody->m_inv_mass;
		float obj0_mass = c0->mRigidBody->m_inv_mass / total_mass;
		float obj1_mass = c1->mRigidBody->m_inv_mass / total_mass;

		//Compute new position using the following formula
		obj0_pos -= (contact->mNormal * contact->mPenetration * obj0_mass);
		obj1_pos += (contact->mNormal * contact->mPenetration * obj1_mass);

		//Set new position
		g0->SetPosition({ obj0_pos.x, Y0, obj0_pos.z });
		g1->SetPosition({ obj1_pos.x, Y1, obj1_pos.z });
	}

	// ------------------------------------------------------------------------
	/*! ResolveForVelocity
	*
	*   Resolves the velocity of each object
	*/ //----------------------------------------------------------------------
	void PhysicsManager::ResolveForVelocity(Collider* c0, Collider* c1, Contact* contact) 
	{
		//Get the RigidBodies of each object
		RigidBody* r0 = c0->mRigidBody; 
		RigidBody* r1 = c1->mRigidBody;

		//Compute the separation velocity
		glm::vec3 relative_vel = r1->m_vel - r0->m_vel;
		float sep = glm::dot(relative_vel, contact->mNormal);
		float Nsep = -sep * mRestitution;
		float Tsep = Nsep - sep;

		//Conpute total inverse mass
		float total_mass = r0->m_inv_mass + r1->m_inv_mass;
		float obj0_mass = r0->m_inv_mass / total_mass;
		float obj1_mass = r1->m_inv_mass / total_mass;

		//Compute final velocities using the following formula
		r0->m_vel -= contact->mNormal * Tsep * (obj0_mass * obj0_mass);
		r1->m_vel += contact->mNormal * Tsep * (obj1_mass * obj1_mass);
	}
}