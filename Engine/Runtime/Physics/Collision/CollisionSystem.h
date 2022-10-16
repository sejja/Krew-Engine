#ifndef GCollisionSystem
#define GCollisionSystem
//
//  CollisionSystem.h
//  Astro Engine
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine
{
	//Collision shapes indexes
	enum ECollisionShape
	{
		CSHAPE_POINT = 1,
		CSHAPE_AABB = 2,
		CSHAPE_CIRCLE = 4,
		CSHAPE_OBB = 8,
		CSHAPE_INDEX_MAX = (CSHAPE_OBB | CSHAPE_CIRCLE) + 1
	};

	//Collision groups indexes
	enum ECollisionGroup
	{
		ENVIRONMENT = 1,
		ZONE = 2,
		PLAYER = 4,
		ENEMY = 8,
		PLAYER_BULLET = 16,
		ENEMY_BULLET = 32,
		GROUP_INDEX_MAX = (PLAYER_BULLET | ENEMY_BULLET) + 1
	};

	class Collider;

	//Functions to handle each collision type
	bool CollideCircles(Collider* body1, Collider* body2, Contact* c);
	bool CollideAABBs(Collider* body1, Collider* body2, Contact * c);
	bool CollideOBBs(Collider* body1, Collider* body2, Contact * c);
	bool CollideAABBToCircle(Collider* body1, Collider* body2, Contact * c);
	bool CollideOBBToCircle(Collider* body1, Collider* body2, Contact * c);

	// typedef for function pointer CollisionFn
	typedef bool(*CollisionFn)(Collider*, Collider*, Contact*);

	struct ENGINE_API CollisionSystem
	{

		//Struct to store the contact between two objects
		struct BodyContact
		{
			Collider* objects[2];
			Contact contactData;

			bool operator==(const BodyContact& rhs) const
			{
				return (rhs.objects[0] == this->objects[0] && rhs.objects[1] == this->objects[1]) ||
					(rhs.objects[0] == this->objects[1] && rhs.objects[1] == this->objects[0]);
			}
		};

		//Constructor and destructor
		CollisionSystem() { Init(); }
		~CollisionSystem();

		void Init();
		void Update();
		void Shutdown();

		//Initalizes collision group table
		void InitCollisionGroup();

		//Add Collider to collision check lists
		void AddCollider(Collider* obj, bool is_dynamic);
		//Remove Collider from collision check lists
		void RemoveCollider(Collider* obj);
		//Clears the collision check lists
		void ClearBodies();

		//Finding which function to use
		CollisionFn GetCollisionFn(Collider* b1, Collider* b2);
		//Finding which Collider Group to check
		bool GetColliderGroup(Collider* b1, Collider* b2);

		//Performs collisions against all objects (First Dyn-Dyn, then Dyn-Sta)
		void CollideAllBodies();
		//Checks if collision occurs and solves it.
		void CollideAndResolve(Collider* obj0, Collider* obj1);
		//Checks if objects are in their respective lists (Sta. object in Sta. list and viceversa)
		void CheckLists();
		//Triggers collision events respect to collision state
		void BroadcastCollisions();

		//Collider lists
		std::list<Collider*> mBodies;
		std::list<Collider*> mStaticBodies;

		//Collision iterations
		unsigned mCurrentCollisionIteration = 0u;
		unsigned mCollisionPrecision = 5;

		//Collision Tests - They are added to the collision system at initialize.
		CollisionFn mCollisionTests[CSHAPE_INDEX_MAX];
		//Collision Groups - They are set in the collision system initialization
		bool mColliderGroup[GROUP_INDEX_MAX];

		//Contacts from previous frame
		std::list<BodyContact> mPrevContacts;
		//Contacts from current frame
		std::list<BodyContact> mNewContacts;

		//Space Partitioning
		PhysicsPartitioning* mPhySpacePart = nullptr;
		bool mUsingSpacePart = false; // to use or not space partitioning
	};

	extern CollisionSystem* gCollisionSystem;
}


#endif