//
//	Scene.h
//	Good Neighbour
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

namespace Engine {
	namespace _private_ {
		class ObjectManager;
	}

	struct ENGINE_API Scene : public Serializable {
#pragma region // Defines
		friend class GCollector;
		friend class _private_::ObjectManager;
#pragma endregion

#pragma region // Constructors & Destructors
		~Scene();
		Scene& operator=(const Scene&) = delete;
#pragma endregion

#pragma region // Functions
		void FromJson(const json& val);
		void ToJson(json& val) const;
		DONTDISCARD Object* SpawnObject(const char* name = nullptr);
		void inline SpawnObject(Object* const go);

		DONTDISCARD Object* InstantiatePrefab(const std::string& prefabpath);

		DONTDISCARD Object* GetObjectByName(const char* name) const;
		DONTDISCARD Array<Object*> GetObjectsByTag(const char* tag) const;
		void RemoveObject(Object* const torem) const;
		void inline RemoveObjectOwnership(Object* const torem);
		DONTDISCARD Object inline* GetObjectByID(const size_t idx);
		DONTDISCARD size_t inline GetObjectCount() const noexcept;
		void Clear();
		Array<Object*> inline& get_all_objects() noexcept;

#pragma endregion

#pragma region // Members
		std::vector<GameObject*> mAllDoors;
		std::vector<GameObject*> mAllSpawners;
		std::vector<GameObject*> mAllMobs;
	private:
		Array<Object*> mObjectList;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Spawn Object
	*
	*   Spawns an already created object onto the scene
	*/ // ---------------------------------------------------------------------
	void inline Scene::SpawnObject(Object* const go) {
		mObjectList.emplace_back(go);
	}

	// ------------------------------------------------------------------------
	/*! Remove Object
	*
	*   Removes an Object From the Scene
	*/ // ---------------------------------------------------------------------
	void inline Scene::RemoveObjectOwnership(Object* const torem) {
		assertThis(torem, "Can't t remove ownership from a nullptr object");
		mObjectList.remove(torem);
	}

	// ------------------------------------------------------------------------
	/*! Get Object Count
	*
	*   Get the Root Objects placed on the Scene
	*/ // --------------------------------------------------------------------
	size_t inline Scene::GetObjectCount() const noexcept {
		return mObjectList.size();
	}

	// ------------------------------------------------------------------------
	/*! Get Object By ID
	*
	*   Gets an Object based on it's ID
	*/ // ----------------------------------------------------------------------
	Object inline* Scene::GetObjectByID(const size_t idx) {
		return mObjectList.at(idx);
	}

	// ------------------------------------------------------------------------
	/*! Get All Objects
	*
	*   Returns a reference to the inner container
	*/ // ----------------------------------------------------------------------
	Array<Object*> inline& Scene::get_all_objects() noexcept { 
		return mObjectList; 
	}
}

#endif