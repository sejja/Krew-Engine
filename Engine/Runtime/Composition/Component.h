//
//	Component.h
//	Good Neighbours
//
//	Created by Diego Revilla on 11/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _COMPONENT__H_
#define _COMPONENT__H_

namespace Engine {
	struct ENGINE_API Component : public IBase, public Serializable {
#pragma region // Declarations & Properties
		friend class GameObject;
#pragma endregion

#pragma region // Constructors & Destructors
		Component() noexcept;
#pragma endregion

#pragma region // Functions
		virtual void inline OnEditor();
		virtual void inline EditorTick();
		DONTDISCARD GameObject inline* GetOwner() const noexcept;
#pragma endregion

#pragma region // Members
	private:
		GameObject* mOwner;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! On Editor
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	void inline Component::OnEditor() {}

	// ------------------------------------------------------------------------
	/*! Editor Tick
	*
	*   EMPTY FUNCTION
	*/ // ---------------------------------------------------------------------
	void inline Component::EditorTick() {}

	// ------------------------------------------------------------------------
	/*! Get Owner
	*
	*   Returns the Owner of the Component
	*/ // ----------------------------------------------------------------------
	GameObject inline* Component::GetOwner() const noexcept {
		return mOwner;
	}
}

#endif