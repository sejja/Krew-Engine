#include <Shared.h>

//
//  CollisionSystem.h
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine
{
	CollisionSystem* gCollisionSystem = new CollisionSystem;

	// ------------------------------------------------------------------------
	/*! Default Destructor
	*
	*   Clears all bodies from lists
	*/ //----------------------------------------------------------------------
	CollisionSystem::~CollisionSystem()
	{
		Shutdown();
	}


	// ------------------------------------------------------------------------
	/*! Init
	*
	*   Initializes the collision group table
	*/ //----------------------------------------------------------------------
	void CollisionSystem::InitCollisionGroup()
	{
		//Environment vs Environment
		mColliderGroup[ENVIRONMENT | ENVIRONMENT] = false;
		//Environment vs Zone
		mColliderGroup[ENVIRONMENT | ZONE] = false;
		//Environment vs Player
		mColliderGroup[ENVIRONMENT | PLAYER] = true;
		//Environment vs Enemy
		mColliderGroup[ENVIRONMENT | ENEMY] = true;
		//Environment vs Player Bullet
		mColliderGroup[ENVIRONMENT | PLAYER_BULLET] = true;
		//Environment vs Enemy Bullet
		mColliderGroup[ENVIRONMENT | ENEMY_BULLET] = true;


		//Player vs Player
		mColliderGroup[PLAYER | PLAYER] = true;
		//Player vs Zone
		mColliderGroup[PLAYER | ZONE] = true;
		//Player vs Enemy
		mColliderGroup[PLAYER | ENEMY] = true;
		//Player vs Player Bullet
		mColliderGroup[PLAYER | PLAYER_BULLET] = false;
		//Player vs Enemy Bullet
		mColliderGroup[PLAYER | ENEMY_BULLET] = true;

		//Enemy vs Zone
		mColliderGroup[ENEMY | ZONE] = true;
		//Enemy vs Enemy
		mColliderGroup[ENEMY | ENEMY] = true;
		//Enemy vs Player Bullet
		mColliderGroup[ENEMY | PLAYER_BULLET] = true;
		//Enemy vs Enemy Bullet
		mColliderGroup[ENEMY | ENEMY_BULLET] = false;

		//Player Bullet vs Zone
		mColliderGroup[PLAYER_BULLET | ZONE] = false;
		//Player Bullet vs Player Bullet
		mColliderGroup[PLAYER_BULLET | PLAYER_BULLET] = false;
		//Player Bullet vs Enemy Bullet
		mColliderGroup[PLAYER_BULLET | ENEMY_BULLET] = false;

		//Enemy Bullet vs Zone
		mColliderGroup[ENEMY_BULLET | ZONE] = false;
		//Enemy Bullet vs Enemy Bullet
		mColliderGroup[ENEMY_BULLET | ENEMY_BULLET] = false;

		//Zone vs Zone
		mColliderGroup[ZONE | ZONE] = false;
	}

	// ------------------------------------------------------------------------
	/*! Init
	*
	*   Initializes the function pointers to handle each collision case
	*/ //----------------------------------------------------------------------
	void CollisionSystem::Init()
	{
		for (unsigned u = 0; u < CSHAPE_INDEX_MAX; ++u)
			mCollisionTests[u] = NULL;

		//Assign each function pointer to their respective case
		mCollisionTests[CSHAPE_POINT | CSHAPE_POINT] = CollideCircles;
		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_CIRCLE] = CollideCircles;
		mCollisionTests[CSHAPE_AABB | CSHAPE_AABB] = CollideAABBs;
		mCollisionTests[CSHAPE_OBB | CSHAPE_OBB] = CollideOBBs;
		mCollisionTests[CSHAPE_POINT | CSHAPE_AABB] = CollideAABBToCircle;
		mCollisionTests[CSHAPE_POINT | CSHAPE_OBB] = CollideOBBToCircle;
		mCollisionTests[CSHAPE_POINT | CSHAPE_CIRCLE] = CollideCircles;
		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_AABB] = CollideAABBToCircle;
		mCollisionTests[CSHAPE_CIRCLE | CSHAPE_OBB] = CollideOBBToCircle;
		mCollisionTests[CSHAPE_AABB | CSHAPE_OBB] = CollideOBBs;
		
		//Initialize collision group table
		InitCollisionGroup();

		//Space Partitioning enabled
		if (mUsingSpacePart)
		{
			mPhySpacePart = new PhysicsPartitioning;
			mPhySpacePart->Init(this);
		}
	}

	// ------------------------------------------------------------------------
	/*! Update
	*
	*   Updates the collision systeam each frame
	*/ //----------------------------------------------------------------------
	void CollisionSystem::Update()
	{
		//Use space partitioning if enabled
		if (mUsingSpacePart)
			mPhySpacePart->Update();
		else //Use default mode
			CollideAllBodies();
	}

	// ------------------------------------------------------------------------
	/*! Shutdown
	*
	*   Clears the bodies from the lists
	*/ //----------------------------------------------------------------------
	void CollisionSystem::Shutdown()
	{
		ClearBodies();
		if (mUsingSpacePart)
		{
			mPhySpacePart->ShutDown();
			delete mPhySpacePart;
		}
	}

	// ------------------------------------------------------------------------
	/*! AddCollider
	*
	*   Adds a collider to the collision lists.
	*/ //----------------------------------------------------------------------
	void CollisionSystem::AddCollider(Collider* obj, bool is_dynamic)
	{
		//Inverse Mass > 0 -> Dynamic object
		if (is_dynamic || obj->mRigidBody->m_inv_mass > 0)
			mBodies.push_back(obj);
		//Inverse Mass == 0 -> Static object
		else if(!is_dynamic || obj->mRigidBody->m_inv_mass == 0)
			mStaticBodies.push_back(obj);
	}

	// ------------------------------------------------------------------------
	/*! RemoveCollider
	*
	*   Removes a collider from the collision lists.
	*/ //----------------------------------------------------------------------
	void CollisionSystem::RemoveCollider(Collider* obj)
	{
		mBodies.remove(obj);
		mStaticBodies.remove(obj);
	}

	// ------------------------------------------------------------------------
	/*! ClearBodies
	*
	*   Clears the collision lists.
	*/ //----------------------------------------------------------------------
	void CollisionSystem::ClearBodies()
	{
		mBodies.clear();
		mStaticBodies.clear();
	}

	// ------------------------------------------------------------------------
	/*! GetCollisionFn
	*
	*   Gets the index of the function pointer depending of their shapes
	*/ //----------------------------------------------------------------------
	CollisionFn CollisionSystem::GetCollisionFn(Collider* b1, Collider* b2)
	{
		unsigned collision_index = b1->mColliderShape | b2->mColliderShape;
		return mCollisionTests[collision_index];
	}

	// ------------------------------------------------------------------------
	/*! GetColliderGroup
	*
	*   Gets the Collider Group to ckeck
	*/ //----------------------------------------------------------------------
	bool CollisionSystem::GetColliderGroup(Collider* b1, Collider* b2)
	{
		unsigned groupIndex = b1->mColliderGroup | b2->mColliderGroup;
		return mColliderGroup[groupIndex];
	}

	// ------------------------------------------------------------------------
	/*! CollideCircles
	*
	*   Case CirclevsCircle.
	*/ //----------------------------------------------------------------------
	bool CollideCircles(Collider* body1, Collider* body2, Contact * c)
	{
		//Center of first circle
		glm::vec3 pos1 = body1->GetOwner()->GetWorldPosition() + body1->mTransform.mPos;
		//Center of second circle
		glm::vec3 pos2 = body2->GetOwner()->GetWorldPosition() + body2->mTransform.mPos;

		//Perform a collision check and fill the data
		return StaticCircleToStaticCircleEx(pos1, (body1->GetOwner()->GetScale().x * body1->mTransform.mScale.x) / 2, pos2, (body2->GetOwner()->GetScale().x * body2->mTransform.mScale.x) / 2, c);
	}

	// ------------------------------------------------------------------------
	/*! CollideAABBs
	*
	*   Case AABBvsAABB.
	*/ //----------------------------------------------------------------------
	bool CollideAABBs(Collider* body1, Collider* body2, Contact * c)
	{
		//Get center of first AABB
		glm::vec3 p1 = body1->GetOwner()->GetWorldPosition() + body1->mTransform.mPos;
		//Get the bounding volume of first AABB
		glm::vec3 s1(body1->GetOwner()->GetScale().x * body1->mTransform.mScale.x, body1->GetOwner()->GetScale().y * body1->mTransform.mScale.y, body1->GetOwner()->GetScale().z * body1->mTransform.mScale.z);

		//Get center of second AABB
		glm::vec3 p2 = body2->GetOwner()->GetWorldPosition() + body2->mTransform.mPos;
		//Get the bounding volume of second AABB
		glm::vec3 s2(body2->GetOwner()->GetScale().x * body2->mTransform.mScale.x, body2->GetOwner()->GetScale().y * body2->mTransform.mScale.y, body2->GetOwner()->GetScale().z * body2->mTransform.mScale.z);

		//Perform a collision check and fill the data
		return StaticRectToStaticRectEx(p1, s1, p2, s2, c);
	}

	// ------------------------------------------------------------------------
	/*! CollideOBBs
	*
	*   Case OBBvsOBB. NOT WORKING, DON'T USE IT!!!
	*/ //----------------------------------------------------------------------
	bool CollideOBBs(Collider* body1, Collider* body2, Contact* c)
	{
		Math::Transform obb1(body1->GetOwner()->GetPosition() + body1->mTransform.mPos, body1->GetOwner()->GetRotation() + body1->mTransform.mRot, body1->GetOwner()->GetScale() + body1->mTransform.mPos);
		Math::Transform obb2(body2->GetOwner()->GetPosition() + body2->mTransform.mPos, body2->GetOwner()->GetRotation() + body2->mTransform.mRot, body2->GetOwner()->GetScale() + body2->mTransform.mPos);

		return OrientedRectToOrientedRectEx(obb1, obb2, c);
	}

	// ------------------------------------------------------------------------
	/*! CollideAABBToCircle
	*
	*   Case AABBvsCircle.
	*/ //----------------------------------------------------------------------
	bool CollideAABBToCircle(Collider* body1, Collider* body2, Contact* c)
	{
		//Determine which is the AABB and which the Circle
		Collider* rect = body1->mColliderShape == CSHAPE_AABB ? body1 : body2;
		Collider* circle = body1->mColliderShape == CSHAPE_CIRCLE ? body1 : body2;

		//Get the position and the bounding volume of AABB
		glm::vec3 rPos = rect->GetOwner()->GetWorldPosition();
		glm::vec3 rScal = rect->GetOwner()->GetScale();
		glm::vec3 temp = glm::vec3(rPos + (glm::vec3)rect->mTransform.mPos);

		//Get the center of the Circle
		glm::vec3 temp2 = glm::vec3(circle->GetOwner()->GetWorldPosition() + (glm::vec3)circle->mTransform.mPos);

		//Perform a collision check and fill the data
		if (StaticRectToStaticCircleEx(temp, rScal.x * rect->mTransform.mScale.x, rScal.y * rect->mTransform.mScale.y, rScal.z * rect->mTransform.mScale.z,
									   temp2, (circle->GetOwner()->GetScale().x * circle->mTransform.mScale.x) / 2, c))
		{
			if (circle == body1) //In this case, flip the normal to get the correct output
				c->mNormal = -c->mNormal;
			return true;
		}
		return false;
	}

	// ------------------------------------------------------------------------
	/*! CollideOBBToCircle
	*
	*   Case OBBvsCircle. NOT WORKING, DON'T USE IT!!!!!
	*/ //----------------------------------------------------------------------
	bool CollideOBBToCircle(Collider* body1, Collider* body2, Contact* c)
	{
		// which is which
		Math::Transform obb = body1->mColliderShape == CSHAPE_OBB
			? Math::Transform(body1->GetOwner()->GetPosition() + body1->mTransform.mPos, body1->GetOwner()->GetRotation() + body1->mTransform.mRot, body1->GetOwner()->GetScale() * body1->mTransform.mScale)
			: Math::Transform(body2->GetOwner()->GetPosition() + body2->mTransform.mPos, body2->GetOwner()->GetRotation() + body2->mTransform.mRot, body2->GetOwner()->GetScale() * body2->mTransform.mScale);

		Collider* circle = body1->mColliderShape == CSHAPE_CIRCLE ? body1 : body2;

		glm::vec3 temp_ = glm::vec3(circle->GetOwner()->GetPosition() + glm::vec3(circle->mTransform.mPos));

		if (StaticOBBToStaticCircleEx(obb, temp_, (circle->GetOwner()->GetScale().x * circle->mTransform.mScale.x) / 2, c))
		{
			if (circle == body1) // flip normal to match our convention
				c->mNormal = -c->mNormal;
			return true;
		}
		return false;
	}

	// ------------------------------------------------------------------------
	/*! CollideAllBodies
	*
	*   Performs a collision check against all objects
	*/ //----------------------------------------------------------------------
	void CollisionSystem::CollideAllBodies()
	{
		//Store contacts from previous frames
		mPrevContacts = mNewContacts;
		mNewContacts.clear();

		//If you increment the limit of the loop, the collision will be mre precise
		//However, the performance will get worse
		for (unsigned i = 0; i < mCollisionPrecision; i++)
		{
			//Store the iteration just to avoid duplicated
			mCurrentCollisionIteration = i;

			//Get all the contacts done in this frame
			for (auto it0 = mBodies.begin(); it0 != mBodies.end(); it0++) 
			{
				//Case Dyn. vs Dyn.
				for (auto it1 = std::next(it0); it1 != mBodies.end(); it1++)
					if(GetColliderGroup(*it0,*it1) == true)
						CollideAndResolve(*it0, *it1);

				//Case Dyn. vs Sta.
				for (auto it1 = mStaticBodies.begin(); it1 != mStaticBodies.end(); it1++)
					if(GetColliderGroup(*it0,*it1) == true)
						CollideAndResolve(*it0, *it1);
			}
		}

		//After we got our contacts, trigger collision events
		BroadcastCollisions();
		//Check if objects are in the correct list
		CheckLists();
	}

	// ------------------------------------------------------------------------
	/*! CollideAndResolve
	*
	*   Check if two objects are colliding. In case they do, resolve it
	*/ //----------------------------------------------------------------------
	void CollisionSystem::CollideAndResolve(Collider* obj0, Collider* obj1)
	{
		//Get their respective owners
		Object* go0 = obj0->GetOwner();
		Object* go1 = obj1->GetOwner();

		//Check they aren't duplicates
		if (obj0 == obj1)
			return;

		//Get the function pointer to check collision
		CollisionFn fn = GetCollisionFn(obj0, obj1);
		Contact result;

		if (!fn)
			return;
		
		//Use the function pointer to check if they are in contact
		if (fn(obj0, obj1, &result))
		{
			//If they are, just fill and add the data once
			if (mCurrentCollisionIteration == 0)
			{
				BodyContact contact;
				contact.objects[0] = obj0;
				contact.objects[1] = obj1;
				contact.contactData = result;
				mNewContacts.push_back(contact);
			}

			//Send the data to resolve the collision (in PhysicsManager.cpp)
			GPhysics->ResolveCollision(obj0, obj1, &result);
		}
	}

	// ------------------------------------------------------------------------
	/*! BroadcastCollisions
	*
	*   Trigger collision events depending of their collision state
	*/ //----------------------------------------------------------------------
	void CollisionSystem::BroadcastCollisions()
	{
		//Traverse through recently added contacts
		for (auto& nc : mNewContacts) {

			//Check in previous contacts if there is the exact same one
			auto it = std::find(mPrevContacts.begin(), mPrevContacts.end(), nc);

			//Not found in previous contacts -> CollisionStarted
			if (it == mPrevContacts.end())
				EventDisp->trigger_event(OnCollisionStartedEvent(nc.objects[0]->GetOwner(), nc.objects[1]->GetOwner(), &nc.contactData));
			else //If found in previous contacts -> CollisionPersisted
			{
				EventDisp->trigger_event(OnCollisionPersistedEvent(nc.objects[0]->GetOwner(), nc.objects[1]->GetOwner(), &nc.contactData));
				mPrevContacts.erase(it);
			}
		}

		//If the remaining contacts haven't been updated in current frame -> CollisionEnded
		for (auto& pc : mPrevContacts)
			EventDisp->trigger_event(OnCollisionEndedEvent(pc.objects[0]->GetOwner(), pc.objects[1]->GetOwner(), &pc.contactData));

		mPrevContacts.clear();
	}

	// ------------------------------------------------------------------------
	/*! CheckLists
	*
	*   Checks if an object is in the correct list
	*/ //----------------------------------------------------------------------
	void CollisionSystem::CheckLists()
	{
		//Check in Dyn. list if there is a Static object
		for (auto it = mBodies.begin(); it != mBodies.end(); ) {
			
			RigidBody* body = (*it)->GetOwner()->GetComponentByType<RigidBody>();
			if (body && body->m_inv_mass == 0) {
				mStaticBodies.push_back((*it));
				auto copy = (*it);
				it++;
				mBodies.remove(copy);

			}
			else it++;
		}

		//Check in Sta. list if there is a Dynamic object
		for (auto it = mStaticBodies.begin(); it != mStaticBodies.end(); ) {
			
			RigidBody* body = (*it)->GetOwner()->GetComponentByType<RigidBody>();
			if (body && body->m_inv_mass > 0) {
				mBodies.push_back((*it));
				auto copy = (*it);
				it++;
				mStaticBodies.remove(copy);

			}
			else it++;
		}
	}
}