#ifndef GCollider
#define GCollider

//
//  Collider.h
//
//  Created by Asier Azpiri on 17/9/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

namespace Engine
{
	//Forward declarations
	struct RigidBody; 
	struct AABB;
	struct Contact;

	struct ENGINE_API Collider : public Component, public events::Listener
	{
		public:

			//Constructor and destructor
			Collider();
			~Collider();
			
			//Initializer functions
			void Construct() override;
			void BeginPlay() override;

			//Editor modifier
			void OnEditor() override;

			//Removes a collider
			void Destroy() override;

			//Update
			void Tick() override;
			void EditorTick() override;

			//Serialization
			void FromJson(const nlohmann::json& val);
			void ToJson(nlohmann::json& val) const;

			//Local transform of the collider
			Math::Transform mTransform;
			//Shape of the collider
			unsigned mColliderShape;
			//Group of the collider
			unsigned mColliderGroup;

			RigidBody* mRigidBody; 

			//Space Partitioning Data
			AABB m_bounding_vol; // this doesnt have to be aabb, need to check mcollidershape!!
			Octree<Collider>::node* m_octree_node = nullptr;
			Collider* m_octree_next_obj = nullptr;
			Collider* m_octree_prev_obj = nullptr;
			int m_ID = -1;

		private:
			//Shape selector for the editor
			int mSelectShape;
			//Group selector for the editor
			int mSelectGroup;
			//Checks if it has already a collider
			bool mHasCollider;
	};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////COLLISION EVENTS//////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct OnCollisionStartedEvent : public events::Event
	{
		GameObject* otherObject, * affectedObject;
		Contact* conct;
		OnCollisionStartedEvent(GameObject* otherObj = nullptr, GameObject* affectedObj = nullptr, Contact* con = nullptr)
		{
			otherObject = otherObj;
			affectedObject = affectedObj;
			conct = con;
		}
	};

	struct OnCollisionPersistedEvent : public events::Event
	{
		GameObject* otherObject, * affectedObject;
		Contact* conct;

		OnCollisionPersistedEvent(GameObject* otherObj = nullptr, GameObject* affectedObj = nullptr, Contact* con = nullptr)
		{
			otherObject = otherObj;
			affectedObject = affectedObj;
			conct = con;
		}
	};

	struct OnCollisionEndedEvent : public events::Event
	{
		GameObject* otherObject, * affectedObject;
		Contact* conct;

		OnCollisionEndedEvent(GameObject* otherObj = nullptr, GameObject* affectedObj = nullptr, Contact* con = nullptr)
		{
			otherObject = otherObj;
			affectedObject = affectedObj;
			conct = con;
		}
	};
}

#endif