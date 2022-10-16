//
//	Gizmo.h
//	Good Neighbours
//
//	Created by Jon Ibarra on 17/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _GIZMO__H_
#define _GIZMO__H_

namespace Engine {
	class GameObject;
}

namespace Editor {
	struct Component;

	class Gizmo {
	public:
#pragma region // Constructors & Destructors
		Gizmo() noexcept;
#pragma endregion

#pragma region // Functions
		bool EditTransform(const std::list<Engine::GameObject*>& object);
#pragma endregion

#pragma region // Members
	private:
		ImGuizmo::OPERATION mOperation;
		ImGuizmo::MODE			mMode;
		bool mShowGrid;
		int mGridSize;
#pragma endregion
	};
}

#endif
#endif