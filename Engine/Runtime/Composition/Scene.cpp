//
//	Scene.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Destructor
	*
	*   Destroys the scene to ensure no memory leaking
	*/ // ---------------------------------------------------------------------
	Scene::~Scene() {
		Clear();
	}

	// ------------------------------------------------------------------------
	/*! Spawn Object
	*
	*   Spawns an Object in the Scene with the given name
	*/ // --------------------------------------------------------------------
	Object* Scene::SpawnObject(const char* name) {
		PageAllocator<Object> objalloc;

		Object* obj_ = objalloc.allocate();
		objalloc.construct(obj_);
#ifdef _EDITOR_
		obj_->SetName(name ? name : __Texts::Editor::PaletteWindow::StaticText::NoName);
#else
		obj_->SetName(name ? name : "No Name");
#endif
		mObjectList.emplace_back(obj_);

		return obj_;
	}


	Object* Scene::InstantiatePrefab(const std::string& prefabPath) {
		GameObject* newGO = SpawnObject();

		newGO->LoadFromPrefab(prefabPath);

		//if (prefabPath.compare("EngineContent/Prefabs/Zombie.prefab") == 0) {
		//	//newGO->GetComponentByType<Graphics::Model3D>()->mModelData = GGfxPipeline->mLoadedModels.find("Enemy/Zombie")->second;
		//}
		//else if (prefabPath.compare("EngineContent/Prefabs/TurretMob.prefab") == 0) {
		//	//return nullptr;
		//}
		//// ...

		return newGO;
	}

	// ------------------------------------------------------------------------
	/*! Get Object By Name
	*
	*   Retrieves an Object by it's name
	*/ // ---------------------------------------------------------------------
	Array<Object*> Scene::GetObjectsByTag(const char* tag) const {
		Array<Object*> r;
		FForEach(mObjectList.begin(), mObjectList.end(), [&r, tag](Object* const x) {
			if (x->HasTag(tag))
				r.push_back(x);
			});
		return r;
	}

	// ------------------------------------------------------------------------
	/*! Remove Object
	*
	*   Removes an Object From the Scene
	*/ // ---------------------------------------------------------------------
	void Scene::RemoveObject(Object* const torem) const {
		GCollector::Instance().AddObjectToRemove(torem);
	}

	// ------------------------------------------------------------------------
	/*! From JSON
	*
	*   Retrieves a Scene from a Json Files
	*/ // --------------------------------------------------------------------
	void Scene::FromJson(const json& val) {
		Clear();
		FForEach(val.begin(), val.end(), [this](const json& a) {
				SpawnObject()->FromJson(a);
			});
	}

	// ------------------------------------------------------------------------
	/*! To JSON
	*
	*   Saves the scene onto a Json File
	*/ // --------------------------------------------------------------------
	void Scene::ToJson(json& val) const {
		size_t count = 0;
		FForEach(mObjectList.begin(), mObjectList.end(), [&val, &count](Object* const x) {
			x->ToJson(val[std::to_string(count++)]);
			});
	}

	// ------------------------------------------------------------------------
	/*! Clear
	*
	*   Clears a Level from it's objects
	*/ // --------------------------------------------------------------------
	void Scene::Clear() {
		PageAllocator<Object> objalloc;

		FForEach(mObjectList.begin(), mObjectList.end(), [&objalloc](Engine::Object* const obj) {
			obj->Destroy();
			objalloc.destroy(obj);
			objalloc.deallocate(obj);
			});
		mObjectList.clear();
		mAllDoors.clear();
		mAllSpawners.clear();
		mAllMobs.clear();
	}

	// ------------------------------------------------------------------------
	/*! Get Object By Name
	*
	*   Retrieves an Object by it's name
	*/ // ---------------------------------------------------------------------
	GameObject* Scene::GetObjectByName(const char* name) const {
		assertThis(name, "No object has nullptr name");

		for (auto& x : mObjectList)
		{
			if (!strcmp(x->GetName(), name)) 
				return x;
			unsigned childCount = x->GetChildCount();
			for (unsigned i = 0; i < childCount; i++)
			{
				GameObject* obj = x->GetChild(i);
				if (!strcmp(obj->GetName(), name))
					return obj;
			}
		}

		return nullptr;
	}
}