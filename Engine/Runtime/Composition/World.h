#pragma once

namespace Engine {

	struct Camera;
	class ENGINE_API World {
	public:
		Scene* GetScene();
		void SetScene(Scene* scene);
		void UpdateScene();
		static World& Instance() noexcept;
		void ChangeLevel(const char* filename);
		void ReloadLevel();
		void EditTickChildren(GameObject* obj);
		void ChangeCurrentCam(Camera* cam, bool interpolate, float time = 0.0f, float speed = 1.0f);

	private:
		Scene* mScene = nullptr;
	};
}

#define GWorld (&Engine::World::Instance())