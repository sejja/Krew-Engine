//
//	ObjMgr.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _OBJ_MGR__H_
#define _OBJ_MGR__H_

namespace Engine {
	namespace _private_ {
		class ObjectManager : public Singleton<ObjectManager> {
#pragma region // Declarations
			friend class GameObject;
			friend class ImGuiManager;
#pragma endregion

		public:

#pragma region // Functions
			void Update() const;
			void InitializeDispatch();
			void BeginPlayAll();
#pragma endregion

#pragma region // Members
			std::set<IBase*> beginqueue;
#pragma endregion

			void RemoveAllDispatches() {
				beginqueue.clear();
			}
		};
	}
}

#define GObjectMgr (&Engine::_private_::ObjectManager::Instance())

#endif