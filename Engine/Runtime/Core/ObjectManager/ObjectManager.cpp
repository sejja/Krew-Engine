//
//	ObjMgr.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>


//HELPER FOR THE MOMENT
void BeginObj(Engine::GameObject* obj)
{
	size_t counter = obj->GetChildCount(), i = 0;

	//While there are children to update
	while (i < counter)
		BeginObj(obj->GetChild(i++));;

	//While there are components to update
	unsigned compCount = obj->GetComponentCount();
	for (i = 0, counter = compCount; i < counter;)
		obj->GetComponent(i++)->BeginPlay();
}


namespace Engine {
	namespace _private_ {
		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Entire Scene
		*/ //----------------------------------------------------------------------
		void ObjectManager::Update() const 
		{
			auto& temp_ = GWorld->GetScene()->mObjectList;

			std::for_each(temp_.begin(), temp_.end(), [](Engine::GameObject* obj) {
				size_t counter = obj->GetChildCount(), i = 0;

				//While there are children to update
				while (i < counter)
					obj->GetChild(i++)->Tick();

				//While there are components to update
				for (i = 0, counter = obj->GetComponentCount(); i < counter;)
					obj->GetComponent(i++)->Tick();
				});
		}

		// ------------------------------------------------------------------------
		/*! Initialize Dispatch
		*
		*   Calls Initialize on unitilized objects
		*/ //----------------------------------------------------------------------
		void ObjectManager::InitializeDispatch() 
		{
			if (!beginqueue.empty()) 
			{
				for (auto& x : beginqueue)
					x->BeginPlay();

				beginqueue.clear();
			}
		}
		void ObjectManager::BeginPlayAll() {
			auto& temp_ = GWorld->GetScene()->mObjectList;

			//NEEDS REDESIGN: USING SUSCRIPT OPERATOR BECAUSE ++ OF ARRAY DOESNT WORK WELL
			for (unsigned i = 0; i < temp_.size(); i++)
				BeginObj(temp_[i]);
		}
	}
}