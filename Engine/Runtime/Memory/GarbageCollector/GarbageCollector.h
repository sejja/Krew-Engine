//
//	GarbageCollector.h
//	Good Neighbours
//
//	Created by Diego Revilla on 10/12/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _GARBAGE_COLLECTOR__H_
#define _GARBAGE_COLLECTOR__H_

namespace Engine {
	class GameObject;
	class Component;

	class ENGINE_API GCollector : public Singleton<GCollector> {
#pragma region // Functions
	public:
		void GOGarbageCollection();
		void AddComponentToRemove(Component* comp);
		void AddChildToRemove(GameObject* child);
		void AddObjectToRemove(GameObject* obj);

#pragma endregion

#pragma region // Members
	private:
		std::set<Component*> mToRemoveComps;
		std::set<GameObject*> mToRemoveChilds;
		std::set<GameObject*> mToRemoveObjs;
#pragma endregion
	};
}

#endif
