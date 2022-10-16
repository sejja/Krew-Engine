//
//	PropertiesGUI.h
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _PROPERTIES_GUI__H_
#define _PROPERTIES_GUI__H_

namespace Editor {
	class PropertiesWindow : public IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override;
#pragma endregion
	};

	class SceneWindow : public IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override;
	private:
		void _present_children(Engine::GameObject* const parent) noexcept;
		void _Move(const size_t i, DiffPtr<Engine::GameObject*>& move, DiffPtr<Engine::GameObject> y);
#pragma endregion
	};

	class PostProcessorGUI : public IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override;
#pragma endregion
	};
}

#endif
#endif