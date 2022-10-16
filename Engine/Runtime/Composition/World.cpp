#include <Shared.h>
#include "World.h"

namespace Engine {
	Scene* World::GetScene() {
		return mScene;
	}
	void World::SetScene(Scene* scene) {
		delete mScene;
		GObjectMgr->RemoveAllDispatches();
		mScene = scene;
	}
	void World::UpdateScene() {
#ifdef _EDITOR_
		if (!GEditor->IsPlaying() || GEditor->IsPaused()) {
			int count = mScene->GetObjectCount();
			for (int i = 0; i < count; i++) {
				auto ptr = mScene->GetObjectByID(i);
				auto temp = ptr->GetComponentCount();
				//ptr->EditorTick();
				for (int j = 0; j < temp; j++) {
					ptr->GetComponent(j)->EditorTick();
				}
				EditTickChildren(ptr);
			}

			return;
		}
#endif

		int count = mScene->GetObjectCount();
		for (int i = 0; i < count; i++) {
			auto ptr = mScene->GetObjectByID(i);
			ptr->Tick();
		}
	}

	World& World::Instance() noexcept {
		static World world;
		return world;
	}

	void World::ChangeLevel(const char* filename)
	{
		GStateManager->SetNewLevel(filename);
	}
	void World::ReloadLevel()
	{
		GStateManager->ReloadLevel();
	}
	void World::EditTickChildren(GameObject* obj)
	{
		size_t size = obj->GetChildCount();
		for (size_t i = 0; i < size; i++)
		{
			auto child = obj->GetChild(i);
			size_t comp_size = child->GetComponentCount();
			for (size_t j = 0; j < comp_size; j++)
				child->GetComponent(j)->EditorTick();
			EditTickChildren(child);
		}
	}

	void World::ChangeCurrentCam(Camera* cam, bool interpolate, float time, float speed)
	{
		if (!cam)
			return;

		GGfxPipeline->SetCurrentCamera(cam, interpolate, time, speed);
	}

}
